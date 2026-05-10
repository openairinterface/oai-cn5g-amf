#!/usr/bin/env python3

import argparse
from pathlib import Path
import re
import sys


def read_text(repo_root: Path, rel_path: str) -> str:
    return (repo_root / rel_path).read_text(encoding="utf-8")


def extract_function_body(source: str, signature: str) -> str:
    start = source.find(signature)
    if start == -1:
        raise AssertionError(f"Missing function signature: {signature}")

    brace_start = source.find("{", start)
    if brace_start == -1:
        raise AssertionError(f"Missing opening brace for: {signature}")

    depth = 0
    for index in range(brace_start, len(source)):
        char = source[index]
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return source[brace_start + 1 : index]

    raise AssertionError(f"Missing closing brace for: {signature}")


def require(text: str, needle: str, description: str) -> None:
    if needle not in text:
        raise AssertionError(f"Missing {description}: {needle}")


def require_regex(text: str, pattern: str, description: str) -> None:
    if re.search(pattern, text, flags=re.MULTILINE | re.DOTALL) is None:
        raise AssertionError(f"Missing {description}: {pattern}")


def extract_assignment_block(source: str, start_marker: str, end_marker: str) -> str:
    start = source.find(start_marker)
    end = source.find(end_marker, start)
    if start == -1 or end == -1:
        raise AssertionError(
            f"Could not isolate assignment block between {start_marker!r} and {end_marker!r}"
        )
    return source[start : end + len(end_marker)]


def assigned_fields(block: str) -> set[str]:
    return set(re.findall(r"itti_msg->([A-Za-z0-9_]+)\s*=", block))


def check_no_ppi_paging_admission(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/paging_controller.cpp")
    body = extract_function_body(
        source,
        "paging::admission_result paging_controller::admit_transfer(",
    )

    require(
        body,
        "if (!nc->ppf_3gpp || nc->is_mobile_reachable_timer_timeout)",
        "CM-IDLE reachability gate",
    )
    require(
        body,
        "result.decision         = paging::admission_decision::PAGING;",
        "paging decision",
    )
    require(
        body,
        "result.http_status_code = 202;",
        "paging admission HTTP status",
    )
    require(
        body,
        "eN1N2MessageTransferCause_anyOf::ATTEMPTING_TO_REACH_UE",
        "paging admission cause",
    )
    require(body, "result.trigger_paging = true;", "initial paging trigger")

    paging_gate_prefix = body.split("if (!enqueue_for_paging", 1)[0]
    if "transaction.ppi" in paging_gate_prefix or "is_ppi_set" in paging_gate_prefix:
        raise AssertionError("Paging admission still appears to be gated on PPI metadata")


def check_cm_connected_direct_delivery(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/paging_controller.cpp")
    require_regex(
        source,
        r"if \(nc->nas_status == CM_CONNECTED\)\s*\{\s*paging::admission_result result;\s*"
        r"result\.decision\s*=\s*paging::admission_decision::DIRECT_DELIVERY;\s*"
        r"result\.http_status_code\s*=\s*200;\s*"
        r".*N1_N2_TRANSFER_INITIATED",
        "CM-CONNECTED direct-delivery branch",
    )


def check_deferred_registration(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/paging_controller.cpp")
    body = extract_function_body(
        source,
        "paging::admission_result paging_controller::admit_transfer(",
    )

    require(body, "if (nc->_5gmm_state != _5GMM_REGISTERED)", "registration-state branch")
    require(
        body,
        "if (!is_registration_in_progress(*nc))",
        "registration reject branch",
    )
    require(
        body,
        "TEMPORARY_REJECT_REGISTRATION_ONGOING",
        "queue-full deferred-registration reject cause",
    )
    require(body, "transaction.deferred_expiry_set = true;", "deferred expiry flag")
    require(body, "transaction.deferred_expiry_at", "deferred expiry timestamp")
    require(body, "if (!enqueue_for_registration", "registration queue admission")
    require_regex(
        body,
        r"result\.decision\s*=\s*paging::admission_decision::DEFER_AWAITING_REGISTRATION;",
        "deferred-registration decision",
    )
    require(
        body,
        "eN1N2MessageTransferCause_anyOf::WAITING_FOR_ASYNCHRONOUS_TRANSFER",
        "deferred-registration cause",
    )
    require(body, "result.max_waiting_time =", "deferred-registration wait time")


def check_queue_full_rejection(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/paging_controller.cpp")

    require(
        source,
        "nc->pending_paging_messages.size() >= max_transactions_per_ue_",
        "paging queue cap",
    )
    require(
        source,
        "nc->awaiting_registration_messages.size() >= max_transactions_per_ue_",
        "awaiting-registration queue cap",
    )
    require_regex(
        source,
        r"if \(!enqueue_for_registration\(nc, std::move\(transaction\)\)\) \{\s*return make_reject_result\(\s*503,\s*"
        r".*\"queue-full\"",
        "explicit deferred-registration queue-full rejection",
    )
    require_regex(
        source,
        r"if \(!enqueue_for_paging\(nc, std::move\(transaction\)\)\) \{\s*return make_reject_result\(\s*503,\s*"
        r".*\"queue-full\"",
        "explicit paging queue-full rejection",
    )


def check_http_metadata_parity(repo_root: Path) -> None:
    http1 = read_text(repo_root, "src/sbi/impl/N1N2MessageCollectionDocumentApiImpl.cpp")
    http2 = read_text(repo_root, "src/sbi/amf_http2_server.cpp")

    start_marker = "itti_msg->is_lcs_correlation_id_set ="
    end_marker = "itti_msg->nf_id = n1N2MessageTransferReqData.getNfId();"
    http1_block = extract_assignment_block(http1, start_marker, end_marker)
    http2_block = extract_assignment_block(http2, start_marker, end_marker)

    http1_fields = assigned_fields(http1_block)
    http2_fields = assigned_fields(http2_block)
    if http1_fields != http2_fields:
        raise AssertionError(
            "HTTP/1 and HTTP/2 N1N2 ingress metadata assignments diverged:\n"
            f"HTTP/1 only: {sorted(http1_fields - http2_fields)}\n"
            f"HTTP/2 only: {sorted(http2_fields - http1_fields)}"
        )

    required_fields = {
        "n1n2_failure_txf_notif_uri",
        "is_area_of_validity_set",
        "area_of_validity",
        "is_supported_features_set",
        "supported_features",
        "is_target_access_set",
        "target_access",
        "is_nf_id_set",
        "nf_id",
        "is_arp_set",
        "arp",
        "is_r5qi_set",
        "r5qi",
    }
    missing = required_fields - http1_fields
    if missing:
        raise AssertionError(f"Missing expected parity fields: {sorted(missing)}")

    require(http1, "result.max_waiting_time.has_value()", "HTTP/1 maxWaitingTime shaping")
    require(http2, "result.max_waiting_time.has_value()", "HTTP/2 maxWaitingTime shaping")


def check_service_type_branches_emergency_skips_admission_gates(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    # A1: GetServiceType() must be called in both service_request_handle overloads.
    require(
        source,
        "service_request->GetServiceType(service_type);",
        "GetServiceType call in service_request_handle",
    )

    # A1: kServiceTypeSignalling must guard PDU session activation in both overloads.
    require(
        source,
        "if (service_type != kServiceTypeSignalling) {",
        "Signalling-only path skips PDU activation",
    )

    # A1: is_priority_service_type helper must distinguish emergency service types.
    require(
        source,
        "bool amf_n1::is_priority_service_type(uint8_t service_type) const {",
        "is_priority_service_type helper implemented",
    )
    require(
        source,
        "service_type == kServiceTypeEmergency",
        "Emergency service type in is_priority_service_type",
    )


def check_n1n2_transfer_rejects_unknown_pdu_session_id(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_app.cpp")

    # B2: When SM container is present, pdu_session_id == 0 must be rejected.
    require(
        source,
        "if (itti_msg.is_n1sm_set || itti_msg.is_n2sm_set) {",
        "SM container presence check before pdu_session_id validation",
    )
    require(
        source,
        "if (itti_msg.pdu_session_id == 0) {",
        "pdu_session_id zero guard",
    )
    require(
        source,
        'bad_req.problem_cause  = "MANDATORY_IE_MISSING";',
        "MANDATORY_IE_MISSING cause for missing pdu_session_id",
    )


CHECKS = {
    "no_ppi_paging_admission": check_no_ppi_paging_admission,
    "cm_connected_direct_delivery": check_cm_connected_direct_delivery,
    "deferred_registration": check_deferred_registration,
    "queue_full_rejection": check_queue_full_rejection,
    "http_metadata_parity": check_http_metadata_parity,
    "service_type_branches_emergency_skips_admission_gates": check_service_type_branches_emergency_skips_admission_gates,
    "n1n2_transfer_rejects_unknown_pdu_session_id": check_n1n2_transfer_rejects_unknown_pdu_session_id,
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo-root", required=True)
    parser.add_argument("--case", required=True, choices=sorted(CHECKS))
    args = parser.parse_args()

    repo_root = Path(args.repo_root).resolve()
    CHECKS[args.case](repo_root)
    print(f"{args.case}: PASS")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as exc:
        print(f"CHECK FAILED: {exc}", file=sys.stderr)
        sys.exit(1)
