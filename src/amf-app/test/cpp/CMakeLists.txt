# SPDX-License-Identifier: LicenseRef-CSSL-1.0
#
# Track H2 — deferred gtest stubs for AMF paging unit tests.
#
# This directory is a placeholder for gtest-based unit tests that require a
# running build environment and cannot be expressed as regex/AST contract
# checks against source files.  Two tests are deferred here:
#
#   paging_controller_concurrent_queue_safety_test
#     Verifies that every push/pop/clear of nas_context::pending_paging_messages,
#     awaiting_registration_messages, and temporarily_unreachable_messages is
#     executed under std::lock_guard<std::mutex>(nc->paging_queues_mutex).
#     Requires linking against AMF_APP, CONTEXTS, and GoogleTest.  Needs a
#     mock itti_inst and a minimal amf_cfg to instantiate paging_controller.
#     Until the gtest binary is added, the G1 mutex contract is verified by the
#     phase-3 contract test temporary_unreachable_timer_stopped_when_queue_drains
#     (which confirms the lock-guard pattern in stop_temporary_unreachable_timer
#     and schedule_temporary_unreachable_expiry).
#
#   paging_message_encoder_roundtrip_test
#     Covers all 6 encoded IEs of PagingMsg (UE Paging Identity, TAI List for
#     Paging, Paging DRX, Paging Priority, Paging Origin, UE Radio Capability
#     for Paging) and the ASN_STRUCT_FREE leak-fix paths added in Track D for
#     setUePagingIdentity, setPagingDrx, setPagingPriority, setPagingOrigin,
#     setUeRadioCapabilityForPaging, and setTaiListForPaging (Track G9).
#     Requires linking against the NGAP library and libasn1c.
#     Until the gtest binary is added, the D5/G9 leak fixes are verified by
#     code review (ASN_STRUCT_FREE replaces raw free() at every failure path).
#
# To activate: add real test sources alongside this file, then uncomment
# add_subdirectory(cpp) in src/amf-app/test/CMakeLists.txt and flesh out the
# target below.
#
# find_package(GTest REQUIRED)
#
# add_executable(paging_unit_tests
#   # paging_controller_concurrent_queue_safety_test.cpp   # TODO
#   # paging_message_encoder_roundtrip_test.cpp            # TODO
# )
# target_link_libraries(paging_unit_tests PRIVATE
#   AMF_APP CONTEXTS NGAP GTest::gtest GTest::gtest_main
# )
# include(GoogleTest)
# gtest_discover_tests(paging_unit_tests)
