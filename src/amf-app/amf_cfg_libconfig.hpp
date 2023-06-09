/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#ifndef _AMF_CFG_LIBCONFIG_H_
#define _AMF_CFG_LIBCONFIG_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "amf.hpp"
#include "amf_config.hpp"
#include "common_defs.h"
#include "if.hpp"
#include "pdu_session_context.hpp"

#define AMF_CONFIG_STRING_AMF_CONFIG "AMF"
#define AMF_CONFIG_STRING_PID_DIRECTORY "PID_DIRECTORY"
#define AMF_CONFIG_STRING_INSTANCE_ID "INSTANCE_ID"
#define AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL "STATISTICS_TIMER_INTERVAL"
#define AMF_CONFIG_STRING_INTERFACES "INTERFACES"
#define AMF_CONFIG_STRING_INTERFACE_NGAP_AMF "NGAP_AMF"

#define AMF_CONFIG_STRING_INTERFACE_NAME "INTERFACE_NAME"
#define AMF_CONFIG_STRING_IPV4_ADDRESS "IPV4_ADDRESS"
#define AMF_CONFIG_STRING_PORT "PORT"
#define AMF_CONFIG_STRING_SBI_HTTP2_PORT "HTTP2_PORT"
#define AMF_CONFIG_STRING_PPID "PPID"

#define AMF_CONFIG_STRING_INTERFACE_SBI "SBI"
#define AMF_CONFIG_STRING_SMF "SMF"
#define AMF_CONFIG_STRING_NRF "NRF"
#define AMF_CONFIG_STRING_AUSF "AUSF"
#define AMF_CONFIG_STRING_UDM "UDM"
#define AMF_CONFIG_STRING_NSSF "NSSF"
#define AMF_CONFIG_STRING_LMF "LMF"

// TO BE REMOVED
#define AMF_CONFIG_STRING_SMF_INSTANCES_POOL "SMF_INSTANCES_POOL"
#define AMF_CONFIG_STRING_SMF_INSTANCE_ID "SMF_INSTANCE_ID"
#define AMF_CONFIG_STRING_SMF_INSTANCE_PORT "PORT"
#define AMF_CONFIG_STRING_SMF_INSTANCE_VERSION "VERSION"
#define AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED "SELECTED"

#define AMF_CONFIG_STRING_API_VERSION "API_VERSION"
#define AMF_CONFIG_STRING_FQDN_DNS "FQDN"

#define AMF_CONFIG_STRING_SCHED_PARAMS "SCHED_PARAMS"
#define AMF_CONFIG_STRING_THREAD_RD_CPU_ID "CPU_ID"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_POLICY "SCHED_POLICY"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY "SCHED_PRIORITY"

#define AMF_CONFIG_STRING_AMF_NAME "AMF_NAME"
#define AMF_CONFIG_STRING_GUAMI "GUAMI"
#define AMF_CONFIG_STRING_SERVED_GUAMI_LIST "SERVED_GUAMI_LIST"
#define AMF_CONFIG_STRING_TAC "TAC"
#define AMF_CONFIG_STRING_MCC "MCC"
#define AMF_CONFIG_STRING_MNC "MNC"
#define AMF_CONFIG_STRING_REGION_ID "RegionID"
#define AMF_CONFIG_STRING_AMF_SET_ID "AMFSetID"
#define AMF_CONFIG_STRING_AMF_POINTER "AMFPointer"
#define AMF_CONFIG_STRING_RELATIVE_AMF_CAPACITY "RELATIVE_CAPACITY"
#define AMF_CONFIG_STRING_PLMN_SUPPORT_LIST "PLMN_SUPPORT_LIST"
#define AMF_CONFIG_STRING_SLICE_SUPPORT_LIST "SLICE_SUPPORT_LIST"
#define AMF_CONFIG_STRING_SST "SST"
#define AMF_CONFIG_STRING_SD "SD"

#define AMF_CONFIG_STRING_CORE_CONFIGURATION "CORE_CONFIGURATION"
#define AMF_CONFIG_STRING_EMERGENCY_SUPPORT "EMERGENCY_SUPPORT"
#define AMF_CONFIG_STRING_AUTHENTICATION "AUTHENTICATION"

#define AMF_CONFIG_STRING_AUTH_MYSQL_SERVER "MYSQL_server"
#define AMF_CONFIG_STRING_AUTH_MYSQL_USER "MYSQL_user"
#define AMF_CONFIG_STRING_AUTH_MYSQL_PASS "MYSQL_pass"
#define AMF_CONFIG_STRING_AUTH_MYSQL_DB "MYSQL_db"
#define AMF_CONFIG_STRING_AUTH_RANDOM "RANDOM"

#define AMF_CONFIG_STRING_NAS "NAS"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_INTEGRITY_ALGORITHM_LIST               \
  "ORDERED_SUPPORTED_INTEGRITY_ALGORITHM_LIST"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST               \
  "ORDERED_SUPPORTED_CIPHERING_ALGORITHM_LIST"

#define AMF_CONFIG_STRING_SUPPORT_FEATURES "SUPPORT_FEATURES"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_NF_REGISTRATION "NF_REGISTRATION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_NRF_SELECTION "NRF_SELECTION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_SMF_SELECTION "SMF_SELECTION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_AUSF "EXTERNAL_AUSF"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_UDM "EXTERNAL_UDM"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_ENABLE_NSSF "EXTERNAL_NSSF"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_FQDN_DNS "USE_FQDN_DNS"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_USE_HTTP2 "USE_HTTP2"

#define AMF_CONFIG_STRING_FQDN_DNS "FQDN"
#define AMF_CONFIG_STRING_LOG_LEVEL "LOG_LEVEL"

using namespace libconfig;

namespace oai::config {

class amf_cfg_libconfig {
 public:
  amf_cfg_libconfig();
  ~amf_cfg_libconfig();

  /*
   * Load AMF configuration file
   * @param [const std::string&] config_file: Configuration file
   * @return RETURNclear/RETURNerror/RETURNok
   */
  int static load(
      const std::string& config_file, oai::config::amf_config& amf_cfg);

  /*
   * Read the network interface configuration
   * @param [const Setting&] if_cfg: Configuration for the network interface
   * @param [interface_cfg_t&] cfg: Interface config
   * @return RETURNclear/RETURNerror/RETURNok
   */
  int static load_interface(const Setting& if_cfg, interface_cfg_t& cfg);

  bool static resolve_fqdn(const std::string& fqdn, struct in_addr& ipv4_addr);
};

}  // namespace oai::config

#endif
