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
file(GLOB AMF_NAS_src_files
  msgs/*.cpp
  ies/*.cpp
  utils/*.c
)

add_library (AMF_NAS STATIC ${AMF_NAS_src_files})

set(NAS_DIR "${SRC_TOP_DIR}/nas")
target_include_directories(AMF_NAS PUBLIC ${NAS_DIR}/ies)
target_include_directories(AMF_NAS PUBLIC ${NAS_DIR}/common)
target_include_directories(AMF_NAS PUBLIC ${NAS_DIR}/msgs)
target_include_directories(AMF_NAS PUBLIC ${NAS_DIR}/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/ngap/libngap)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/common)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/common/unicode)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/sbi/model)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/sctp)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/secu_algorithms/5gaka)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/secu_algorithms/nas_enc_int)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/utils/bstr)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
