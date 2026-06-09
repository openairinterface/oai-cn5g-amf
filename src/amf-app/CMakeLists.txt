# SPDX-License-Identifier: LicenseRef-CSSL-1.0

add_library (AMF_APP STATIC
  amf_app.cpp
  amf_config.cpp
  amf_n1.cpp
  amf_n2.cpp
  amf_sbi.cpp
  amf_event.cpp
  amf_profile.cpp
  amf_statistics.cpp
  amf_subscription.cpp
  authentication.cpp
  failure_notify_client.cpp
  ${SRC_TOP_DIR}/nas/nas_procedure_manager.cpp
  ${SRC_TOP_DIR}/nas/nas_state_machine.cpp
  ${SRC_TOP_DIR}/nas/nas_timer_manager.cpp
  paging/paging_controller.cpp
  ${SRC_TOP_DIR}/ngap/ngap_app.cpp
)

target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/amf-app)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/amf-app/paging)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/common/unicode)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/contexts)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/itti)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/itti/msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/nas)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/ngap)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi/api)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi/impl)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sctp)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/secu_algorithms/5gaka)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/secu_algorithms/nas_enc_int)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/config)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/http)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/5gmm-msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap_ies)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_ies)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf.cmake)
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/utils.cmake)