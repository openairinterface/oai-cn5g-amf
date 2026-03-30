# SPDX-License-Identifier: LicenseRef-CSSL-1.0

set(AMF_API_SERVER_DIR "${SRC_TOP_DIR}/sbi")

include_directories(${SRC_TOP_DIR}/amf-app)
include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/contexts)
include_directories(${SRC_TOP_DIR}/itti)
include_directories(${SRC_TOP_DIR}/itti/msgs)
include_directories(${SRC_TOP_DIR}/sbi)
include_directories(${SRC_TOP_DIR}/sbi/api)
include_directories(${SRC_TOP_DIR}/sbi/impl)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/config)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/http)
#include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap_ies)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_ies)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf.cmake)

file(GLOB AMF_API_SERVER_src_files
    ${AMF_API_SERVER_DIR}/amf_http1_server.cpp
    ${AMF_API_SERVER_DIR}/amf_http2_server.cpp
    ${AMF_API_SERVER_DIR}/api/*.cpp
    ${AMF_API_SERVER_DIR}/impl/*.cpp
)

add_library(AMF_SBI_SERVER STATIC
           ${AMF_API_SERVER_src_files})
