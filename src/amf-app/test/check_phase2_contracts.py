#!/usr/bin/env python3

import argparse
from pathlib import Path
import re


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


def check_validated_service_request_teardown(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    for signature in (
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas,\n    bool paging_response_integrity_checked, uint8_t& cause)",
        "bool amf_n1::service_request_handle(\n    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,\n    const uint64_t amf_ue_ngap_id, bstring nas, uint8_t ulCount,\n    bool paging_response_integrity_checked, uint8_t& cause)",
    ):
        body = extract_function_body(source, signature)
        decode_pos = body.find("service_request->Decode")
        complete_pos = body.find("complete_paging_response_transition(")
        reject_pos = body.find("if (!nc or !uc or !nc->security_ctx")
        if decode_pos == -1 or complete_pos == -1 or reject_pos == -1:
            raise AssertionError(f"Could not locate decode/validation/teardown ordering in {signature}")
        if not (decode_pos < reject_pos < complete_pos):
            raise AssertionError("Service Request paging teardown no longer appears to be gated on decode/validation")

    require(
        source,
        "if (!amf_n2_inst->has_paging_targets(\n          amf_ue_ngap_id, nc->ran_ue_ngap_id, true))",
        "T3513 no-target retransmit guard",
    )
    require(source, "handle_t3513_final_expiry(nc, amf_ue_ngap_id);", "T3513 final expiry on no-target")


def check_t3513_no_target_timer_cleanup(repo_root: Path) -> None:
    n1_source = read_text(repo_root, "src/amf-app/amf_n1.cpp")
    timer_source = read_text(repo_root, "src/amf-app/nas_timer_manager.cpp")

    expiry_body = extract_function_body(
        n1_source,
        "void amf_n1::handle_t3513_expiry(\n    timer_id_t timer_id, std::string amf_ue_ngap_id_str)",
    )
    require_regex(
        expiry_body,
        r"if \(!amf_n2_inst->has_paging_targets\(\s*amf_ue_ngap_id, nc->ran_ue_ngap_id, true\)\) \{\s*Logger::amf_n1\(\)\.warn\([\s\S]*?handle_t3513_final_expiry\(\s*nc,\s*amf_ue_ngap_id,\s*paging::paging_outcome::NO_TARGET,\s*kNoPagingTargetTransferCause\s*\);\s*return;\s*\}",
        "no-target expiry terminal branch",
    )

    final_expiry_body = extract_function_body(
        n1_source,
        "void amf_n1::handle_t3513_final_expiry(\n    std::shared_ptr<nas_context>& nc, uint64_t amf_ue_ngap_id,",
    )
    require(
        final_expiry_body,
        "nas_timer_manager_.stop_timer(nas_timer_type_e::T3513, nc);",
        "T3513 final-expiry timer cleanup",
    )

    stop_timer_body = extract_function_body(
        timer_source,
        "void nas_timer_manager::stop_timer(\n    nas_timer_type_e type, std::shared_ptr<nas_context>& nc)",
    )
    require(
        stop_timer_body,
        "nc->nas_timers[idx].itti_timer_id == ITTI_INVALID_TIMER_ID",
        "idempotent stop_timer stale-id guard",
    )
    require(
        stop_timer_body,
        "nc->nas_timers[idx].itti_timer_id        = ITTI_INVALID_TIMER_ID;",
        "stop_timer clears stale timer id",
    )
    require(
        stop_timer_body,
        "nc->nas_timers[idx].is_running           = false;",
        "stop_timer clears running state",
    )


def check_response_classification(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")
    types = read_text(repo_root, "src/amf-app/paging_types.hpp")
    for response_class in (
        "SERVICE_REQUEST",
        "CONTROL_PLANE_SERVICE_REQUEST",
        "REGISTRATION_REQUEST",
        "REGISTRATION_COMPLETE",
        "NGAP_RESUME",
        "NOTIFICATION",
        "NOTIFICATION_RESPONSE",
    ):
        require(types, f'paging_response_class::{response_class}', f"{response_class} gate")

    require(
        source,
        "case kControlPlaneServiceRequest: {",
        "CP Service Request branch",
    )
    require_regex(
        source,
        r"Malformed Control Plane Service Request received while paging\s*\"\s*"
        r"\"is active for UE %lu - keeping paging active",
        "non-terminal malformed CP Service Request logging",
    )
    require_regex(
        source,
        r"\"Control Plane Service Request\",\s*false\);",
        "terminal CP Service Request paging transition",
    )
    require(
        source,
        "complete_reconnect_follow_up(",
        "CP Service Request reconnect follow-up helper",
    )
    require_regex(
        source,
        r"Registration Request classified as non-terminal for active\s*\"\s*"
        r"\"paging on UE %lu",
        "registration request non-terminal classification",
    )
    require(
        source,
        "Notification classified as non-terminal for active paging on ",
        "notification non-terminal classification",
    )
    require(
        source,
        "Notification Response classified as non-terminal for active ",
        "notification response non-terminal classification",
    )
    require(
        source,
        "case kRegistrationComplete:",
        "Registration Complete response classification",
    )


def check_t3513_terminal_gate_contract(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")
    types = read_text(repo_root, "src/amf-app/paging_types.hpp")

    require(types, "struct paging_response_gate", "explicit paging response gate type")
    require(types, "requires_integrity_checked_nas", "integrity gate field")
    require(types, "allows_registration_security_success", "registration exception gate")
    require(types, "lower_layer_terminal", "NGAP resume lower-layer gate")

    gate_body = extract_function_body(
        source,
        "bool amf_n1::can_complete_paging_response(",
    )
    require(
        gate_body,
        "gate.requires_integrity_checked_nas && !integrity_checked_nas_response",
        "integrity-checked T3513 stop guard",
    )
    require(
        gate_body,
        "registration_security_exception",
        "qualifying registration/security exception",
    )
    require(
        gate_body,
        "active_paging_targets_non_3gpp_access(nc)",
        "non-3GPP Allowed PDU Session Status exception scope",
    )
    require(
        source,
        "Control Plane Service Request for UE %lu is non-terminal for ",
        "non-integrity CP Service Request leaves paging active",
    )


def check_ngap_resume_terminal_contract(repo_root: Path) -> None:
    header = read_text(repo_root, "src/amf-app/amf_n1.hpp")
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    require(
        header,
        "void complete_ngap_resume_paging_response(",
        "NGAP Resume terminal hook declaration",
    )
    body = extract_function_body(
        source,
        "void amf_n1::complete_ngap_resume_paging_response(",
    )
    require(
        body,
        "paging::paging_response_class::NGAP_RESUME",
        "NGAP Resume terminal gate use",
    )
    require(
        body,
        '"NGAP Resume"',
        "NGAP Resume T3513 stop transition",
    )
    require(
        body,
        "complete_reconnect_follow_up(nc, ran_ue_ngap_id, amf_ue_ngap_id, true);",
        "post-resume GUTI refresh follow-up",
    )


def check_priority_retrigger(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/paging_controller.cpp")
    body = extract_function_body(
        source,
        "paging::admission_result paging_controller::admit_transfer(",
    )
    ongoing_pos = body.find("if (nc->is_paging_ongoing)")
    if ongoing_pos == -1:
        raise AssertionError("Missing priority-triggered immediate repage: active-paging branch")
    active_paging_branch = body[ongoing_pos:]
    require_regex(
        active_paging_branch,
        r"const auto& queued\s*=\s*nc->pending_paging_messages\.back\(\);",
        "priority-triggered immediate repage queued transfer lookup",
    )
    require_regex(
        active_paging_branch,
        r"queued\.ppi\.has_value\(\)\s*&&\s*\(!nc->paging_priority_present\s*\|\|\s*"
        r"queued\.ppi\.value\(\)\s*<\s*nc->paging_effective_ppi\)",
        "priority-triggered immediate repage priority comparison",
    )
    require_regex(
        active_paging_branch,
        r"nc->paging_effective_ppi\s*=\s*queued\.ppi\.value\(\);",
        "priority-triggered immediate repage effective priority update",
    )
    require_regex(
        active_paging_branch,
        r"nc->paging_priority_present\s*=\s*true;",
        "priority-triggered immediate repage priority-present flag",
    )
    require_regex(
        active_paging_branch,
        r"result\.trigger_paging\s*=\s*true;",
        "priority-triggered immediate repage trigger flag",
    )


def check_no_target_rejection(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_app.cpp")
    require(source, "make_no_paging_target_result()", "no-target result helper")
    require(source, "eN1N2MessageTransferCause_anyOf::AN_NOT_RESPONDING", "AN_NOT_RESPONDING result cause")
    require(
        source,
        "!amf_n2_inst->has_paging_targets(\n          amf_ue_ngap_id, ran_ue_ngap_id, paging_was_ongoing)",
        "pre-admission paging target resolution",
    )


def check_full_tai_fanout(repo_root: Path) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n2.cpp")
    require(source, "plmn.getMcc() == tai.mcc && plmn.getMnc() == tai.mnc", "full TAI PLMN+TAC match")
    require(source, "if (matched_assoc_ids.empty()) {", "empty target-set guard")
    require(source, "if (paging_sent == 0) {", "no-send timer guard")
    require_regex(
        source,
        r"if \(paging_sent == 0\) \{\s*Logger::amf_n2\(\)\.warn\(\s*\"Paging fan-out resolved targets but did not send any NGAP paging",
        "resolved-target no-send warning",
    )


def check_service_request_ngksi_mismatch_yields_service_reject(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    # A2: GetNgKsi must be called in the second service_request_handle overload.
    require(
        source,
        "service_request->GetNgKsi(sr_ngksi);",
        "GetNgKsi call in service_request_handle",
    )
    # A2: Mismatch against nc->ngksi triggers send_service_reject.
    require_regex(
        source,
        r"sr_ngksi != kNasKeySetIdentifierNotAvailable &&\s*"
        r"sr_ngksi != old_nc->ngksi",
        "ngKSI mismatch comparison against stored nc->ngksi",
    )
    # A2: cause 74 is used and service reject is sent on mismatch.
    require(
        source,
        "cause = k5gmmCauseTemporarilyNotAuthorizedForThisSnpn;",
        "cause #74 assigned on ngKSI mismatch",
    )
    require_regex(
        source,
        r"send_service_reject\(nc, cause\);[\s\S]{0,200}start_authentication_procedure\(",
        "send_service_reject followed by start_authentication_procedure on mismatch",
    )


def check_service_reject_cause_31_is_integrity_protected(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    # A3: send_service_reject must branch on cause #31 for integrity protection.
    body = extract_function_body(source, "void amf_n1::send_service_reject(")

    require(
        body,
        "cause == k5gmmCauseRedirectionToEpcRequired",
        "cause #31 in integrity-protection branch",
    )
    require(
        body,
        "nc->security_ctx.has_value();",
        "security context gate for integrity-protected SERVICE REJECT",
    )
    require(
        body,
        "encode_nas_message_protected(",
        "encode_nas_message_protected called for integrity-protected SERVICE REJECT",
    )


def check_service_reject_cause_76_is_integrity_protected(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    # A3: send_service_reject must branch on cause #76 for integrity protection.
    body = extract_function_body(source, "void amf_n1::send_service_reject(")

    require(
        body,
        "cause == k5gmmCauseNotAuthorizedForThisCagOrAuthorizedForCagCellsOnly",
        "cause #76 in integrity-protection branch",
    )
    require(
        body,
        "nc->security_ctx.has_value();",
        "security context gate for integrity-protected SERVICE REJECT",
    )
    require(
        body,
        "encode_nas_message_protected(",
        "encode_nas_message_protected called for integrity-protected SERVICE REJECT",
    )


def check_t3513_retransmit_only_restarts_on_send_success(
    repo_root: Path,
) -> None:
    source = read_text(repo_root, "src/amf-app/amf_n1.cpp")

    expiry_body = extract_function_body(
        source,
        "void amf_n1::handle_t3513_expiry(\n    timer_id_t timer_id, std::string amf_ue_ngap_id_str) {",
    )

    # E2: Timer restart must follow ITTI send success (not precede it).
    # Verify: itti_inst->send_msg appears BEFORE start_timer in the retx branch.
    send_pos = expiry_body.find("itti_inst->send_msg(paging_msg)")
    restart_pos = expiry_body.find(
        "nas_timer_manager_.start_timer(nas_timer_type_e::T3513, nc, amf_ue_ngap_id);"
    )
    if send_pos == -1:
        raise AssertionError(
            "Missing itti_inst->send_msg(paging_msg) in handle_t3513_expiry"
        )
    if restart_pos == -1:
        raise AssertionError(
            "Missing nas_timer_manager_.start_timer(T3513) in handle_t3513_expiry"
        )
    if not (send_pos < restart_pos):
        raise AssertionError(
            "T3513 timer restart appears BEFORE the ITTI send — should be after"
        )

    # E2: On send failure the final-expiry path is taken (not a restart).
    require_regex(
        expiry_body,
        r"if \(ret != RETURNok\) \{[\s\S]{0,400}handle_t3513_final_expiry\(\s*nc,\s*amf_ue_ngap_id\);",
        "ITTI send failure triggers final expiry (not timer restart)",
    )


CHECKS = {
    "validated_service_request_teardown": check_validated_service_request_teardown,
    "t3513_no_target_timer_cleanup": check_t3513_no_target_timer_cleanup,
    "response_classification": check_response_classification,
    "t3513_terminal_gate_contract": check_t3513_terminal_gate_contract,
    "ngap_resume_terminal_contract": check_ngap_resume_terminal_contract,
    "priority_retrigger": check_priority_retrigger,
    "no_target_rejection": check_no_target_rejection,
    "full_tai_fanout": check_full_tai_fanout,
    "service_request_ngksi_mismatch_yields_service_reject": check_service_request_ngksi_mismatch_yields_service_reject,
    "service_reject_cause_31_is_integrity_protected": check_service_reject_cause_31_is_integrity_protected,
    "service_reject_cause_76_is_integrity_protected": check_service_reject_cause_76_is_integrity_protected,
    "t3513_retransmit_only_restarts_on_send_success": check_t3513_retransmit_only_restarts_on_send_success,
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo-root", required=True)
    parser.add_argument("--case", required=True, choices=sorted(CHECKS))
    args = parser.parse_args()

    repo_root = Path(args.repo_root)
    CHECKS[args.case](repo_root)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
