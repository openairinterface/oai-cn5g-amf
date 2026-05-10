#!/usr/bin/env python3

import argparse
from pathlib import Path
import re
import sys


def read_text(repo_root: Path, rel_path: str) -> str:
    return (repo_root / rel_path).read_text(encoding="utf-8")


def require(text: str, needle: str, description: str) -> None:
    if needle not in text:
        raise AssertionError(f"Missing {description}: {needle}")


def require_regex(text: str, pattern: str, description: str) -> None:
    if re.search(pattern, text, flags=re.MULTILINE | re.DOTALL) is None:
        raise AssertionError(f"Missing {description}: {pattern}")


def require_absent(text: str, needle: str, description: str) -> None:
    if needle in text:
        raise AssertionError(f"Unexpected {description}: {needle}")


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


def extract_between(source: str, start_marker: str, end_marker: str) -> str:
    start = source.find(start_marker)
    if start == -1:
        raise AssertionError(f"Missing start marker: {start_marker}")
    end = source.find(end_marker, start)
    if end == -1:
        raise AssertionError(f"Missing end marker: {end_marker}")
    return source[start:end]


def require_in_order(text: str, needles: list[str], description: str) -> None:
    position = -1
    for needle in needles:
        next_position = text.find(needle, position + 1)
        if next_position == -1:
            raise AssertionError(f"Missing {description}: {needle}")
        position = next_position


def check_temp_unreachable_admission(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/paging_controller.cpp")
    body = extract_function_body(
        source,
        "paging::admission_result paging_controller::admit_transfer(",
    )

    require(body, "if (nc->is_mico_mode) {", "temporary-unreachable branch")
    require(
        body,
        "transaction.ext_buf_support.value_or(false)",
        "temporary-unreachable extBufSupport admission gate",
    )
    require(
        body,
        "paging::admission_decision::DEFER_TEMPORARY_UNREACHABLE",
        "temporary-unreachable defer decision",
    )
    require(
        body,
        "WAITING_FOR_ASYNCHRONOUS_TRANSFER",
        "temporary-unreachable accepted cause",
    )
    require(body, "result.max_waiting_time =", "temporary-unreachable maxWaitingTime")
    require(
        body,
        "REJECTION_DUE_TO_PAGING_RESTRICTION",
        "temporary-unreachable rejection cause",
    )
    require(
        source,
        "nc->temporarily_unreachable_messages.size() >= max_transactions_per_ue_",
        "temporary-unreachable queue cap",
    )


def check_area_and_access_forwarding_gate(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/paging_controller.cpp")
    require(
        source,
        "bool paging_controller::can_forward_n2_sm_over_3gpp_access(",
        "shared N2 forwarding gate helper",
    )
    require(
        source,
        "AccessType::eAccessType::NON_3GPP_ACCESS",
        "targetAccess non-3GPP gating",
    )
    require(
        source,
        "is_pdu_session_allowed_on_3gpp_access(",
        "Allowed PDU Session Status gating helper",
    )
    require(
        source,
        "matches_area_of_validity(",
        "areaOfValidity matching helper",
    )
    require(
        source,
        "outside the areaOfValidity for the N2 SM ",
        "areaOfValidity rejection reason",
    )

    app_source = read_text(repo_root, "src/amf-app/amf_app.cpp")
    require(
        app_source,
        "make_n2_forwarding_blocked_result(",
        "direct-delivery N2 forwarding rejection helper",
    )
    require(
        app_source,
        "Skipping N2 SM forwarding for SUPI %s while still delivering N1 ",
        "N1-only fallback when N2 is blocked",
    )


def check_waiting_async_wire_shape(repo_root: Path) -> None:
    http1 = read_text(repo_root, "src/sbi/impl/N1N2MessageCollectionDocumentApiImpl.cpp")
    http2 = read_text(repo_root, "src/sbi/amf_http2_server.cpp")

    http1_body = extract_function_body(http1, "nlohmann::json make_success_body(")
    http2_body = extract_function_body(http2, "nlohmann::json make_n1n2_success_body(")

    for body, label in ((http1_body, "HTTP/1"), (http2_body, "HTTP/2")):
        require(
            body,
            "WAITING_FOR_ASYNCHRONOUS_TRANSFER",
            f"{label} deferred success cause gate",
        )
        require(
            body,
            'body["maxWaitingTime"] = result.max_waiting_time.value();',
            f"{label} success-body maxWaitingTime field",
        )


def check_page_response_session_semantics(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    first_service_request = extract_function_body(
        source,
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas,\n    bool paging_response_integrity_checked, uint8_t& cause) {",
    )
    second_service_request = extract_function_body(
        source,
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas, uint8_t ulCount,\n    bool paging_response_integrity_checked, uint8_t& cause) {",
    )
    follow_up = extract_function_body(
        source,
        "void amf_n1::complete_reconnect_follow_up(",
    )
    deliver_pending = extract_function_body(
        source,
        "void amf_n1::deliver_pending_paging_messages(",
    )
    wake_temp = extract_function_body(
        source,
        "void amf_n1::wake_temporary_unreachable_messages(",
    )

    for body, label in (
        (first_service_request, "plain Service Request"),
        (second_service_request, "protected Service Request"),
    ):
        require(
            body,
            "GetAllowedPduSessionStatus()",
            f"{label} Allowed PDU Session Status parsing",
        )
        require_regex(
            body,
            r"page_reconnect_allowed_pdu_session_status\s*=\s*allowed_pdu_session_status_opt;",
            f"{label} reconnect status retention",
        )
        require_regex(
            body,
            r"complete_reconnect_follow_up\(\s*nc,\s*ran_ue_ngap_id,\s*amf_ue_ngap_id,\s*page_triggered_reconnect\);",
            f"{label} reconnect follow-up helper use",
        )
        require(
            body,
            "paging_response_integrity_checked",
            f"{label} terminal T3513 integrity gate",
        )

    require_in_order(
        follow_up,
        [
            "maybe_send_configuration_update_with_new_guti(",
            "deliver_pending_paging_messages(",
            "wake_temporary_unreachable_messages(",
            "nc->page_reconnect_allowed_pdu_session_status.reset();",
        ],
        "reconnect follow-up ordering",
    )
    require(
        source,
        "Sent Configuration Update Command with new 5G-GUTI",
        "new 5G-GUTI post-page action log",
    )
    require_absent(
        deliver_pending,
        "page_reconnect_allowed_pdu_session_status.reset()",
        "early Allowed PDU Session Status reset in pending drain",
    )
    require_absent(
        wake_temp,
        "page_reconnect_allowed_pdu_session_status.reset()",
        "early Allowed PDU Session Status reset in temporary-unreachable wake-up",
    )


def check_signalling_only_service_request_acceptance(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")
    first_service_request = extract_function_body(
        source,
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas,\n    bool paging_response_integrity_checked, uint8_t& cause) {",
    )
    second_service_request = extract_function_body(
        source,
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas, uint8_t ulCount,\n    bool paging_response_integrity_checked, uint8_t& cause) {",
    )

    for body, label in (
        (first_service_request, "plain Service Request"),
        (second_service_request, "protected Service Request"),
    ):
        no_pdu_branch = extract_between(
            body,
            "if (pdu_session_to_be_activated.size() == 0) {",
            "} else {",
        )
        require(
            no_pdu_branch,
            "Service Accept",
            f"{label} signalling-only Service Accept path",
        )
        require_absent(
            no_pdu_branch,
            "k5gmmCauseInsufficientUpResourcesForThePduSession",
            f"{label} signalling-only hard reject",
        )


def check_registration_paging_response_exceptions(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")
    registration_request = extract_function_body(
        source,
        "bool amf_n1::registration_request_handle(",
    )
    registration_complete = extract_function_body(
        source,
        "bool amf_n1::registration_complete_handle(",
    )

    require(
        registration_request,
        "active_paging_targets_non_3gpp_access(nc)",
        "non-3GPP scoped Allowed PDU Session Status branch",
    )
    require(
        registration_request,
        "GetAllowedPduSessionStatus()",
        "Registration Request Allowed PDU Session Status parsing",
    )
    require(
        registration_request,
        "registration_security_exception",
        "mobility/periodic registration security exception",
    )
    require(
        registration_request,
        "paging::paging_response_class::\n                                     REGISTRATION_REQUEST",
        "Registration Request terminal gate",
    )
    require(
        registration_complete,
        "page_triggered_registration_complete",
        "Registration Complete paging-response detection",
    )
    require(
        registration_complete,
        "paging::paging_response_class::REGISTRATION_COMPLETE",
        "Registration Complete terminal gate",
    )
    require(
        registration_complete,
        "nc->page_reconnect_guti_refresh_pending",
        "staged post-registration GUTI refresh marker",
    )
    require(
        registration_complete,
        "page_reconnect_guti_refresh",
        "post-registration GUTI refresh trigger",
    )


def check_control_plane_service_request_follow_up(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")
    cp_case = extract_between(
        source,
        "case kControlPlaneServiceRequest: {",
        "case kRegistrationRequest: {",
    )

    require_regex(
        cp_case,
        r"const bool page_triggered_reconnect\s*=\s*nc->is_paging_ongoing && !nc->paging_completed;",
        "CP Service Request reconnect detection",
    )
    require_regex(
        cp_case,
        r'"Control Plane Service Request",\s*false',
        "CP Service Request transition without inline queue drain",
    )
    require_regex(
        cp_case,
        r"complete_reconnect_follow_up\(\s*nc,\s*ran_ue_ngap_id,\s*amf_ue_ngap_id,\s*true\);",
        "CP Service Request reconnect follow-up parity",
    )


def check_temp_unreachable_expiry_contract(repo_root: Path) -> None:
    header = read_text(repo_root, "src/amf-app/amf_app.hpp")
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    require(
        header,
        "TASK_AMF_TEMPORARY_UNREACHABLE_DEFER_TIMER_EXPIRE",
        "temporary-unreachable timer task id",
    )
    require(
        source,
        "void amf_n1::schedule_temporary_unreachable_expiry(",
        "temporary-unreachable timer scheduler",
    )
    require(
        source,
        "void amf_n1::handle_temporary_unreachable_expiry(",
        "temporary-unreachable expiry handler",
    )
    require(
        source,
        "REJECTION_DUE_TO_PAGING_RESTRICTION",
        "temporary-unreachable expiry terminal paging restriction outcome",
    )
    require(
        source,
        "UE_NOT_REACHABLE_FOR_SESSION",
        "temporary-unreachable expiry terminal unreachable outcome",
    )
    # Track G3: deferred_expiry_set removed; check uses .has_value() on the
    # optional deferred_expiry_at field.
    require_regex(
        source,
        r"deferred_expiry_at\.has_value\(\)",
        "temporary-unreachable expiry draining loop (G3 optional check)",
    )


def check_registration_area_tai_list_seeded_from_registration_accept(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    # D2: registration_area_tai_list must be populated from the same local
    # tai_list variable used in SetTaiList(tai_list) on Registration Accept,
    # not from amf_cfg->plmn_list directly.

    # Require SetTaiList(tai_list) — proves the local var drives Registration Accept.
    require(
        source,
        "registration_accept->SetTaiList(tai_list);",
        "SetTaiList called with the local tai_list variable",
    )

    # Require that registration_area_tai_list is populated in the same region.
    require(
        source,
        "unc_reg->registration_area_tai_list.clear();",
        "registration_area_tai_list cleared before repopulation",
    )
    require(
        source,
        "unc_reg->registration_area_tai_list.push_back(",
        "registration_area_tai_list populated from tai_list loop",
    )

    # Confirm the comment attributing the source to tai_list (D2 track marker).
    require(
        source,
        "populate registration_area_tai_list in UE NGAP context for paging fan-out.",
        "D2 attribution comment for registration_area_tai_list population",
    )


def check_service_request_initial_context_setup_request_sets_cm_connected(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    # A4: After successful service_request_handle on the non-paging branch,
    # set_5gcm_state(nc, CM_CONNECTED) must be called.
    # Both overloads must contain the call guarded by the CM state check.
    for signature in (
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas,\n    bool paging_response_integrity_checked, uint8_t& cause)",
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas, uint8_t ulCount,\n    bool paging_response_integrity_checked, uint8_t& cause)",
    ):
        body = extract_function_body(source, signature)
        require(
            body,
            "set_5gcm_state(nc, CM_CONNECTED);",
            f"set_5gcm_state(CM_CONNECTED) in {signature[:60]}",
        )
        require(
            body,
            "if (nc->nas_status != CM_CONNECTED) {",
            f"CM_CONNECTED guard before set_5gcm_state in {signature[:60]}",
        )


def check_temporary_unreachable_timer_stopped_when_queue_drains(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    # G2: stop_temporary_unreachable_timer must exist and respect the
    # temp_unreachable_timer_running flag.
    stop_body = extract_function_body(
        source,
        "void amf_n1::stop_temporary_unreachable_timer(",
    )
    require(
        stop_body,
        "nc->temp_unreachable_timer_running",
        "temp_unreachable_timer_running checked in stop helper",
    )

    # G2: schedule_temporary_unreachable_expiry must stop the timer when the
    # queue is empty (queue drains → timer stopped).
    schedule_body = extract_function_body(
        source,
        "void amf_n1::schedule_temporary_unreachable_expiry(",
    )
    require(
        schedule_body,
        "if (nc->temporarily_unreachable_messages.empty()) {",
        "empty-queue guard in schedule_temporary_unreachable_expiry",
    )
    require(
        schedule_body,
        "stop_temporary_unreachable_timer(nc);",
        "stop_temporary_unreachable_timer called when queue empties",
    )

    # G2: set flag to true when timer is armed.
    require(
        source,
        "nc->temp_unreachable_timer_running = true;",
        "temp_unreachable_timer_running set true on arm",
    )
    # G2: clear flag to false on expiry.
    require(
        source,
        "nc->temp_unreachable_timer_running = false;",
        "temp_unreachable_timer_running cleared on expiry",
    )


CHECKS = {
    "area_and_access_forwarding_gate": check_area_and_access_forwarding_gate,
    "control_plane_service_request_follow_up": check_control_plane_service_request_follow_up,
    "page_response_session_semantics": check_page_response_session_semantics,
    "registration_paging_response_exceptions": check_registration_paging_response_exceptions,
    "signalling_only_service_request_acceptance": check_signalling_only_service_request_acceptance,
    "temp_unreachable_admission": check_temp_unreachable_admission,
    "temp_unreachable_expiry_contract": check_temp_unreachable_expiry_contract,
    "waiting_async_wire_shape": check_waiting_async_wire_shape,
    "registration_area_tai_list_seeded_from_registration_accept": check_registration_area_tai_list_seeded_from_registration_accept,
    "service_request_initial_context_setup_request_sets_cm_connected": check_service_request_initial_context_setup_request_sets_cm_connected,
    "temporary_unreachable_timer_stopped_when_queue_drains": check_temporary_unreachable_timer_stopped_when_queue_drains,
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
