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

add_library (AMF_APP STATIC
  amf_app.cpp
  amf_config.cpp
  amf_config_yaml.cpp
  amf_n1.cpp
  amf_n2.cpp
  amf_sbi.cpp
  amf_event.cpp
  amf_profile.cpp
  amf_statistics.cpp
  amf_statistics.cpp
  amf_subscription.cpp
  authentication.cpp
  ${SRC_TOP_DIR}/ngap/ngap_app.cpp
)

target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/amf-app)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/itti)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/itti/msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/common/unicode)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/contexts)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/ngap)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/libngap_ies)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_ies)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/ngap_msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/ngap/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi/api)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi/impl)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sctp)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/secu_algorithms/5gaka)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/secu_algorithms/nas_enc_int)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/config)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/http)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/5gmm-msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils)

include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/amf/amf.cmake)
include(${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/utils.cmake)
