#!/bin/sh
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

## Sample values for configuration
export INSTANCE=0
export PID_DIRECTORY=/var/run
export MCC=208
export MNC=95
export REGION_ID=128
export AMF_SET_ID=1
export SERVED_GUAMI_MCC_0=208
export SERVED_GUAMI_MNC_0=95
export SERVED_GUAMI_REGION_ID_0=128
export SERVED_GUAMI_AMF_SET_ID_0=1
export PLMN_SUPPORT_MCC=208
export PLMN_SUPPORT_MNC=95
export PLMN_SUPPORT_TAC=0xa000
export SST_0=1
export SD_0=0xFFFFFF
export SST_1=1
export SD_1=1
export AMF_INTERFACE_NAME_FOR_NGAP=eth0
export AMF_INTERFACE_NAME_FOR_N11=eth0
export SMF_INSTANCE_ID_0=1
export SMF_FQDN_0=localhost
export SMF_IPV4_ADDR_0=127.0.0.1
export SMF_HTTP_VERSION_0=v1
export SELECTED_0=true
export MYSQL_SERVER=127.0.0.1
export MYSQL_USER=root
export MYSQL_PASS=linux
export MYSQL_DB=oai_db
export NRF_IPV4_ADDRESS=127.0.0.1
export NRF_PORT=80
export EXTERNAL_NRF=no
export NF_REGISTRATION=yes
export SMF_SELECTION=yes
export USE_FQDN_DNS=yes
export EXTERNAL_AUSF=yes
export EXTERNAL_UDM=no
export EXTERNAL_NSSF=no
export USE_HTTP2=no
export NRF_API_VERSION=v1
export NRF_FQDN=localhost
export AUSF_IPV4_ADDRESS=127.0.0.1
export AUSF_PORT=80
export AUSF_API_VERSION=v1
export AUSF_FQDN=localhost
export UDM_IPV4_ADDRESS=127.0.0.1
export UDM_PORT=80
export UDM_API_VERSION=v2
export UDM_FQDN=localhost

../../scripts/entrypoint.py