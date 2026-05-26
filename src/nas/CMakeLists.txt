# SPDX-License-Identifier: LicenseRef-CSSL-1.0

file(GLOB AMF_NAS_src_files
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/5gmm-msgs/*.cpp
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies/*.cpp
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils/*.c
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils/*.cpp
)

add_library (AMF_NAS STATIC ${AMF_NAS_src_files})

target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/5gmm-msgs)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf.cmake)
