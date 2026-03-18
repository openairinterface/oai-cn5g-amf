# SPDX-License-Identifier: LicenseRef-CSSL-1.0

include_directories(${SRC_TOP_DIR}/amf-app)
include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
include_directories(${SRC_TOP_DIR}/itti)
include_directories(${SRC_TOP_DIR}/itti/msgs)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common.cmake)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf.cmake)

add_library (AMF_UTILS STATIC
 ${CMAKE_CURRENT_SOURCE_DIR}/3gpp_conversions.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/amf_conversions.cpp
 )

