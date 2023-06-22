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
set(NGAP_DIR "${SRC_TOP_DIR}/ngap")
include_directories(${NGAP_DIR}/libngap)
include_directories(${NGAP_DIR}/ngapIEs)
include_directories(${NGAP_DIR}/ngapMsgs)
include_directories(${SRC_TOP_DIR}/itti)
include_directories(${SRC_TOP_DIR}/itti/msgs)
include_directories(${SRC_TOP_DIR}/common)
include_directories(${SRC_TOP_DIR}/common/unicode)
include_directories(${SRC_TOP_DIR}/sctp)
include_directories(${SRC_TOP_DIR}/utils)
include_directories(${SRC_TOP_DIR}/utils/bstr)
include_directories(${SRC_TOP_DIR}/sbi/amf_server/model)
include_directories(${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)

file(GLOB AMF_NGAP_src_files
  ${SRC_TOP_DIR}/ngap/libngap/*.c
  ${SRC_TOP_DIR}/ngap/ngapMsgs/*.cpp
  ${SRC_TOP_DIR}/ngap/ngapIEs/*.cpp
)

add_library (AMF_NGAP STATIC
${AMF_NGAP_src_files}
)

