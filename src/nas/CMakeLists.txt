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
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/5gmm-msgs/*.cpp
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies/*.cpp
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils/*.c
  ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils/*.cpp
)

add_library (AMF_NAS STATIC ${AMF_NAS_src_files})

target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/ies)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/common)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/5gmm-msgs)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/nas/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/logger)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/common)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/model/common_model)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils)
target_include_directories(AMF_NAS PUBLIC ${SRC_TOP_DIR}/${MOUNTED_COMMON}/utils/bstr)
