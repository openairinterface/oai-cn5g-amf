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
#target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap_ies)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_ies)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_msgs)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/utils)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
target_include_directories(AMF_NGAP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf.cmake)

# NGAP unit tests — use pre-built GTest from NEF build tree if system GTest is absent
cmake_policy(SET CMP0079 NEW)
enable_testing()
set(_NGAP_TEST_DIR "${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/test")
if (EXISTS "${_NGAP_TEST_DIR}/CMakeLists.txt")
  set(NGAP_DIR     "${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap")
  set(NGAP_TEST_DIR "${_NGAP_TEST_DIR}")
  set(3GPP_DIR    "${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp")
  set(COMMON_DIR  "${SRC_TOP_DIR}/${MOUNTED_COMMON}/common")
  set(LOGGER_DIR  "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger")
  set(MODEL_DIR   "${SRC_TOP_DIR}/${MOUNTED_COMMON}/model")
  set(UTILS_DIR   "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils")
  # Create alias NGAP -> AMF_NGAP for test linking
  if(NOT TARGET NGAP)
    add_library(NGAP ALIAS AMF_NGAP)
  endif()
  if(NOT TARGET LOGGER)
    add_library(NGAP_TEST_LOGGER STATIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/lttng_logger_base.cpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/spd_logger_base.cpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/logger_base.cpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/nf-tp.cpp"
    )
    target_include_directories(NGAP_TEST_LOGGER PUBLIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger")
    target_link_libraries(NGAP_TEST_LOGGER PUBLIC -lfmt -lspdlog)
    add_library(LOGGER ALIAS NGAP_TEST_LOGGER)
  endif()
  if(NOT TARGET COMMON)
    add_library(NGAP_TEST_BSTR STATIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr/bstrlib.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/ngap_test_stubs.cpp"
    )
    target_include_directories(NGAP_TEST_BSTR PUBLIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/common"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/model"
    )
    target_link_libraries(NGAP_TEST_BSTR PUBLIC -lfmt -lspdlog)
    add_library(COMMON ALIAS NGAP_TEST_BSTR)
  endif()
  add_subdirectory("${_NGAP_TEST_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/ngap_test")
  if(TARGET test_ngap_msg)
    target_link_libraries(test_ngap_msg PRIVATE NGAP_LIB_C NGAP_LIB_C_IES AMF_UTILS)
    if(TARGET NGAP_TEST_LOGGER)
      target_link_libraries(test_ngap_msg PRIVATE NGAP_TEST_LOGGER)
    endif()
    if(TARGET NGAP_TEST_BSTR)
      target_link_libraries(test_ngap_msg PRIVATE NGAP_TEST_BSTR)
    endif()
  endif()
endif()
