################################################################################
# Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The OpenAirInterface Software Alliance licenses this file to You under
# the OAI Public License, Version 1.1  (the "License"); you may not use this file
# except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.openairinterface.org/?page_id=698
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#-------------------------------------------------------------------------------
# For more information about the OpenAirInterface (OAI) Software Alliance:
#      contact@openairinterface.org
################################################################################

include_directories(${SRC_TOP_DIR}/amf-app)
include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
include_directories(${SRC_TOP_DIR}/itti)
include_directories(${SRC_TOP_DIR}/itti/msgs)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/common.cmake)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf/amf.cmake)

add_library (AMF_UTILS STATIC
 ${CMAKE_CURRENT_SOURCE_DIR}/mime_parser.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/3gpp_conversions.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/amf_conversions.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/thread_sched.cpp
 )

