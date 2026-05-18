# SPDX-License-Identifier: LicenseRef-CSSL-1.0
# Allow target_link_libraries to targets built in subdirectories
cmake_policy(SET CMP0079 NEW)

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
#target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/5gmm-msgs)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf.cmake)

# NAS unit tests — use pre-built GTest from NEF build tree if system GTest is absent
enable_testing()
set(_NAS_TEST_DIR "${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/test")
if (EXISTS "${_NAS_TEST_DIR}/CMakeLists.txt")
  set(NAS_DIR     "${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas")
  set(NAS_TEST_DIR "${_NAS_TEST_DIR}")
  set(3GPP_DIR    "${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp")
  set(COMMON_DIR  "${SRC_TOP_DIR}/${MOUNTED_COMMON}/common")
  set(LOGGER_DIR  "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger")
  set(MODEL_DIR   "${SRC_TOP_DIR}/${MOUNTED_COMMON}/model")
  set(UTILS_DIR   "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils")
  # The test CMakeLists references cmake targets NAS/LOGGER/COMMON.
  # In the AMF build these are embedded into AMF_NAS/AMF_UTILS; create shims.
  if(NOT TARGET NAS)
    # ALIAS so AMF_NAS appears as a DIRECT dep (comes first in link order)
    add_library(NAS ALIAS AMF_NAS)
  endif()
  if(NOT TARGET LOGGER)
    add_library(NAS_TEST_LOGGER STATIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/lttng_logger_base.cpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/spd_logger_base.cpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/logger_base.cpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger/nf-tp.cpp"
    )
    target_include_directories(NAS_TEST_LOGGER PUBLIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger")
    target_link_libraries(NAS_TEST_LOGGER PUBLIC -lfmt -lspdlog)
    add_library(LOGGER ALIAS NAS_TEST_LOGGER)
  endif()
  if(NOT TARGET COMMON)
    # COMMON bundles: bstrlib (for TLVDecoder) + minimal stubs for
    # oai::utils::conv / oai::utils::output_wrapper / oai::utils::utils.
    # Those symbols are referenced by AMF_NAS but not exercised by our tests.
    add_library(NAS_TEST_BSTR STATIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr/bstrlib.c"
      "${CMAKE_CURRENT_SOURCE_DIR}/nas_test_stubs.cpp"
    )
    target_include_directories(NAS_TEST_BSTR PUBLIC
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/3gpp"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/common"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger"
      "${SRC_TOP_DIR}/${MOUNTED_COMMON}/model"
    )
    target_link_libraries(NAS_TEST_BSTR PUBLIC -lfmt -lspdlog)
    add_library(COMMON ALIAS NAS_TEST_BSTR)
  endif()
  add_subdirectory("${_NAS_TEST_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/nas_test")
  # cmake deduplicates keeping the last occurrence, so appending here ensures
  # the correct link order: AMF_NAS → AMF_UTILS → NAS_TEST_LOGGER → NAS_TEST_BSTR
  target_link_libraries(test_nas_msg PRIVATE
    AMF_UTILS NAS_TEST_LOGGER NAS_TEST_BSTR)
endif()
