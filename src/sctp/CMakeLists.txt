# SPDX-License-Identifier: LicenseRef-CSSL-1.0

include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
include_directories(${SRC_TOP_DIR}/nas/common)
include_directories(${SRC_TOP_DIR}/sctp)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

add_library (AMF_SCTP STATIC
${CMAKE_CURRENT_SOURCE_DIR}/sctp_server.cpp
 )

