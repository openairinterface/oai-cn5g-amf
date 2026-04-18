# SPDX-License-Identifier: LicenseRef-CSSL-1.0

cmake_minimum_required (VERSION 3.16.3)
project(OpenAirInterface)

# ---------- ccache (speeds up incremental rebuilds) ----------
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_C_COMPILER_LAUNCHER   "${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
  message(STATUS "ccache found: ${CCACHE_PROGRAM}")
endif()
# -------------------------------------------------------------

# Unit tests for AMF are NOT registered via CTest in this build tree.
# They live in a separate build rooted at src/common-src/build/common/ and
# are built and run independently:
#   cd src/common-src/build/common && cmake . && cmake --build . && cd build && ctest --output-on-failure
# (CTest registers 47 tests in src/common-src/build/common/build, not the parent dir.)
# Running `ctest` in build/amf/build will therefore always report
# "No tests were found" — this is expected and not a regression.

# Override options for AMF
set (  PACKAGE_NAME                    "AMF" )
set (  STATIC_LINKING                   False )
#############################################
# Base directories, compatible with legacy OAI building
################################################
set (OPENAIRCN_DIR   $ENV{OPENAIRCN_DIR})
set (BUILD_TOP_DIR   ${OPENAIRCN_DIR}/build)
set (SRC_TOP_DIR     $ENV{OPENAIRCN_DIR}/src)

################################################
# For common-src usage
################################################
set (  NF_TARGET                        amf )
set (  MOUNTED_COMMON                   common-src)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../src/oai-amf/CMakeLists.txt)
