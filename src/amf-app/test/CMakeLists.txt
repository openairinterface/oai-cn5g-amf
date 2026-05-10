# SPDX-License-Identifier: LicenseRef-CSSL-1.0

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(AMF_PAGING_PHASE1_CHECK ${CMAKE_CURRENT_SOURCE_DIR}/check_phase1_contracts.py)
set(AMF_PAGING_PHASE2_CHECK ${CMAKE_CURRENT_SOURCE_DIR}/check_phase2_contracts.py)
set(AMF_PAGING_PHASE3_CHECK ${CMAKE_CURRENT_SOURCE_DIR}/check_phase3_contracts.py)
set(AMF_PAGING_PHASE4_CHECK ${CMAKE_CURRENT_SOURCE_DIR}/check_phase4_contracts.py)

function(add_amf_paging_phase1_check test_name case_name)
  add_test(
    NAME ${test_name}
    COMMAND ${Python3_EXECUTABLE} ${AMF_PAGING_PHASE1_CHECK}
            --repo-root ${OPENAIRCN_DIR}
            --case ${case_name}
  )
  set_tests_properties(
    ${test_name}
    PROPERTIES WORKING_DIRECTORY ${OPENAIRCN_DIR}
  )
endfunction()

function(add_amf_paging_phase2_check test_name case_name)
  add_test(
    NAME ${test_name}
    COMMAND ${Python3_EXECUTABLE} ${AMF_PAGING_PHASE2_CHECK}
            --repo-root ${OPENAIRCN_DIR}
            --case ${case_name}
  )
  set_tests_properties(
    ${test_name}
    PROPERTIES WORKING_DIRECTORY ${OPENAIRCN_DIR}
  )
endfunction()

function(add_amf_paging_phase3_check test_name case_name)
  add_test(
    NAME ${test_name}
    COMMAND ${Python3_EXECUTABLE} ${AMF_PAGING_PHASE3_CHECK}
            --repo-root ${OPENAIRCN_DIR}
            --case ${case_name}
  )
  set_tests_properties(
    ${test_name}
    PROPERTIES WORKING_DIRECTORY ${OPENAIRCN_DIR}
  )
endfunction()

function(add_amf_paging_phase4_check test_name case_name)
  add_test(
    NAME ${test_name}
    COMMAND ${Python3_EXECUTABLE} ${AMF_PAGING_PHASE4_CHECK}
            --repo-root ${OPENAIRCN_DIR}
            --case ${case_name}
  )
  set_tests_properties(
    ${test_name}
    PROPERTIES WORKING_DIRECTORY ${OPENAIRCN_DIR}
  )
endfunction()

add_amf_paging_phase1_check(
  amf_paging_no_ppi_admission
  no_ppi_paging_admission
)
add_amf_paging_phase1_check(
  amf_paging_cm_connected_direct_delivery
  cm_connected_direct_delivery
)
add_amf_paging_phase1_check(
  amf_paging_deferred_registration
  deferred_registration
)
add_amf_paging_phase1_check(
  amf_paging_queue_full_rejection
  queue_full_rejection
)
add_amf_paging_phase1_check(
  amf_paging_http_metadata_parity
  http_metadata_parity
)

add_amf_paging_phase2_check(
  amf_paging_validated_service_request_teardown
  validated_service_request_teardown
)
add_amf_paging_phase2_check(
  amf_paging_t3513_no_target_timer_cleanup
  t3513_no_target_timer_cleanup
)
add_amf_paging_phase2_check(
  amf_paging_response_classification
  response_classification
)
add_amf_paging_phase2_check(
  amf_paging_t3513_terminal_gate_contract
  t3513_terminal_gate_contract
)
add_amf_paging_phase2_check(
  amf_paging_ngap_resume_terminal_contract
  ngap_resume_terminal_contract
)
add_amf_paging_phase2_check(
  amf_paging_priority_retrigger
  priority_retrigger
)
add_amf_paging_phase2_check(
  amf_paging_no_target_rejection
  no_target_rejection
)
add_amf_paging_phase2_check(
  amf_paging_full_tai_fanout
  full_tai_fanout
)

add_amf_paging_phase3_check(
  amf_paging_temp_unreachable_admission
  temp_unreachable_admission
)
add_amf_paging_phase3_check(
  amf_paging_area_and_access_forwarding_gate
  area_and_access_forwarding_gate
)
add_amf_paging_phase3_check(
  amf_paging_waiting_async_wire_shape
  waiting_async_wire_shape
)
add_amf_paging_phase3_check(
  amf_paging_page_response_session_semantics
  page_response_session_semantics
)
add_amf_paging_phase3_check(
  amf_paging_signalling_only_service_request_acceptance
  signalling_only_service_request_acceptance
)
add_amf_paging_phase3_check(
  amf_paging_registration_paging_response_exceptions
  registration_paging_response_exceptions
)
add_amf_paging_phase3_check(
  amf_paging_control_plane_service_request_follow_up
  control_plane_service_request_follow_up
)
add_amf_paging_phase3_check(
  amf_paging_temp_unreachable_expiry_contract
  temp_unreachable_expiry_contract
)

add_amf_paging_phase4_check(
  amf_paging_extended_ngap_ies_gate
  extended_ngap_ies_gate
)
add_amf_paging_phase4_check(
  amf_paging_subscription_notification_gate
  subscription_notification_gate
)
add_amf_paging_phase4_check(
  amf_paging_queue_saturation_metrics
  queue_saturation_metrics
)
add_amf_paging_phase4_check(
  amf_paging_config_backed_controller_construction
  config_backed_controller_construction
)
add_amf_paging_phase4_check(
  amf_paging_defer_admission_schedules_registration_expiry
  defer_admission_schedules_registration_expiry
)
add_amf_paging_phase4_check(
  amf_paging_failure_callback_contract_and_no_target_consistency
  failure_callback_contract_and_no_target_consistency
)

# Track H2 — new contract tests for gaps closed by Tracks A-G

add_amf_paging_phase1_check(
  amf_paging_service_type_branches_emergency_skips_admission_gates
  service_type_branches_emergency_skips_admission_gates
)
add_amf_paging_phase1_check(
  amf_paging_n1n2_transfer_rejects_unknown_pdu_session_id
  n1n2_transfer_rejects_unknown_pdu_session_id
)

add_amf_paging_phase2_check(
  amf_paging_service_request_ngksi_mismatch_yields_service_reject
  service_request_ngksi_mismatch_yields_service_reject
)
add_amf_paging_phase2_check(
  amf_paging_service_reject_cause_31_is_integrity_protected
  service_reject_cause_31_is_integrity_protected
)
add_amf_paging_phase2_check(
  amf_paging_service_reject_cause_76_is_integrity_protected
  service_reject_cause_76_is_integrity_protected
)
add_amf_paging_phase2_check(
  amf_paging_t3513_retransmit_only_restarts_on_send_success
  t3513_retransmit_only_restarts_on_send_success
)

add_amf_paging_phase3_check(
  amf_paging_registration_area_tai_list_seeded_from_registration_accept
  registration_area_tai_list_seeded_from_registration_accept
)
add_amf_paging_phase3_check(
  amf_paging_service_request_initial_context_setup_request_sets_cm_connected
  service_request_initial_context_setup_request_sets_cm_connected
)
add_amf_paging_phase3_check(
  amf_paging_temporary_unreachable_timer_stopped_when_queue_drains
  temporary_unreachable_timer_stopped_when_queue_drains
)

add_amf_paging_phase4_check(
  amf_paging_n1n2_transfer_failure_notification_body_uses_29518_schema
  n1n2_transfer_failure_notification_body_uses_29518_schema
)
add_amf_paging_phase4_check(
  amf_paging_t3513_final_expiry_uses_ue_not_responding_cause
  t3513_final_expiry_uses_ue_not_responding_cause
)
add_amf_paging_phase4_check(
  amf_paging_paging_priority_only_when_arp_indicates_priority
  paging_priority_only_when_arp_indicates_priority
)
add_amf_paging_phase4_check(
  amf_paging_http1_and_http2_n1n2_dispatch_share_helper
  http1_and_http2_n1n2_dispatch_share_helper
)
add_amf_paging_phase4_check(
  amf_paging_paging_attempt_information_encoded_when_extended_ngap_ies_enabled
  paging_attempt_information_encoded_when_extended_ngap_ies_enabled
)

# Track H2 — gtest TODO stubs (deferred; see src/amf-app/test/cpp/)
# paging_controller_concurrent_queue_safety_test  — deferred gtest
# paging_message_encoder_roundtrip_test            — deferred gtest
# add_subdirectory(cpp)  # uncomment when paging_unit_tests binary is added
