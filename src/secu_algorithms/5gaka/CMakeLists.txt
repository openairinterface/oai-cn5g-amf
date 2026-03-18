# SPDX-License-Identifier: LicenseRef-CSSL-1.0

include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
include_directories(${SRC_TOP_DIR}/nas/ies)
include_directories(${SRC_TOP_DIR}/nas/common)
include_directories(${SRC_TOP_DIR}/secu_algorithms/5gaka)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

add_library (AMF_SECU_5GAKA STATIC
  authentication_algorithms_with_5gaka.cpp
  rijndael.cpp
  sha256.cpp
 )

