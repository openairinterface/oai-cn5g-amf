# SPDX-License-Identifier: LicenseRef-CSSL-1.0

file(GLOB AMF_NGAP_src_files
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_msgs/*.cpp
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_ies/*.cpp
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/utils/*.cpp
)
add_library (AMF_NGAP STATIC
  ${AMF_NGAP_src_files}
)

set(NGAP_DIR "${SRC_TOP_DIR}/ngap")

target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/common)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/common/unicode)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/itti)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/itti/msgs)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/sctp)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/utils)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap_ies)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_ies)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_msgs)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/utils)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf.cmake)
