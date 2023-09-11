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
  amf_cfg_libconfig.cpp
  amf_config_yaml.cpp
  amf_n1.cpp
  amf_n2.cpp
  amf_sbi.cpp
  amf_event.cpp
  amf_profile.cpp
  amf_statistics.cpp
  amf_statistics.cpp
  mysql_db.cpp
  amf_subscription.cpp
  ${SRC_TOP_DIR}/ngap/ngap_app/ngap_app.cpp
)

target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/amf-app)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/itti)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/itti/msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/common/unicode)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/contexts)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/nas/ies)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/nas/common)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/nas/msgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/nas/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/ngap/libngap)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/ngap/ngap_app)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/ngap/ngapIEs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/ngap/ngapMsgs)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi/api)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi/impl)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sbi/model)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/sctp)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/secu_algorithms/5gaka)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/secu_algorithms/nas_enc_int)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/utils)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/utils/bstr)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
target_include_directories(AMF_APP PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/config)

include(${BUILD_TOP_DIR}/amf/used_models.cmake)