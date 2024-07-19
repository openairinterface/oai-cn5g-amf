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

#ifndef _AMF_CONFIG_H_
#define _AMF_CONFIG_H_

#include <arpa/inet.h>
#include <netinet/in.h>

#include <nlohmann/json.hpp>

#include "3gpp_24.501.hpp"
#include "amf.hpp"
#include "common_defs.h"
#include "if.hpp"
#include "pdu_session_context.hpp"
#include "sbi_helper.hpp"
#include "thread_sched.hpp"

constexpr auto AMF_CONFIG_OPTION_YES_STR = "Yes";
constexpr auto AMF_CONFIG_OPTION_NO_STR  = "No";

using namespace oai::common::sbi;

namespace oai::config {

typedef struct support_features_s {
  bool enable_nf_registration;
  bool enable_smf_selection;
  bool enable_external_ausf_udm;
  bool enable_nssf;
  bool enable_lmf;
  uint8_t http_version;
  nlohmann::json to_json() const {
    nlohmann::json json_data              = {};
    json_data["enable_nf_registration"]   = this->enable_nf_registration;
    json_data["enable_smf_selection"]     = this->enable_smf_selection;
    json_data["enable_external_ausf_udm"] = this->enable_external_ausf_udm;
    json_data["enable_nssf"]              = this->enable_nssf;
    json_data["enable_lmf"]               = this->enable_lmf;
    json_data["http_version"]             = this->http_version;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    try {
      if (json_data.find("enable_nf_registration") != json_data.end()) {
        this->enable_nf_registration =
            json_data["enable_nf_registration"].get<bool>();
      }
      if (json_data.find("enable_smf_selection") != json_data.end()) {
        this->enable_smf_selection =
            json_data["enable_smf_selection"].get<bool>();
      }
      if (json_data.find("enable_external_ausf_udm") != json_data.end()) {
        this->enable_external_ausf_udm =
            json_data["enable_external_ausf_udm"].get<bool>();
      }
      if (json_data.find("enable_nssf") != json_data.end()) {
        this->enable_nssf = json_data["enable_nssf"].get<bool>();
      }
      if (json_data.find("http_version") != json_data.end()) {
        this->http_version = json_data["http_version"].get<int>();
      }
      if (json_data.find("enable_lmf") != json_data.end()) {
        this->enable_lmf = json_data["enable_lmf"].get<bool>();
      }
    } catch (std::exception& e) {
      Logger::amf_app().error("%s", e.what());
    }
  }

} support_features_t;

class amf_config {
 public:
  amf_config();
  ~amf_config();

  /*
   * Get the URI of AMF N1N2MessageSubscribe
   * @param [const std::string&] ue_cxt_id: UE Context Id
   * @return URI in string format
   */
  std::string get_amf_n1n2_message_subscribe_uri(
      const std::string& ue_cxt_id, const std::string& subscription_id);

  /*
   * Get the URI of AMF NonUEN2InfoSubscribe
   * @param [const std::string&] subscription_id: Subscription Id
   * @return URI in string format
   */
  std::string get_non_ue_n2_info_subscribe_uri(
      const std::string& subscription_id);

  /*
   * Get the URI of UDM Slice Selection Subscription Data Retrieval Service
   * @param [const std::string&] supi: UE SUPI
   * @return URI in string format
   */
  std::string get_udm_slice_selection_subscription_data_retrieval_uri(
      const std::string& supi);

  /*
   * Get the URI of NSSF Network Slice Selection Information Service
   * @param void
   * @return URI in string format
   */
  std::string get_nssf_network_slice_selection_information_uri();

  /*
   * Get the URI of AUSF UE Authentication Service
   * @param void
   * @return URI in string format
   */
  std::string get_ausf_ue_authentications_uri();

  /*
   * Get the URI of LMF Determine Location Service
   * @param void
   * @return URI in string format
   */
  std::string get_lmf_determine_location_uri();

  /*
   * Get the URI of SMF PDU Session Service
   * @param [const std::shared_ptr<pdu_session_context>&] psc: pointer to the
   * PDU Session Context
   * @param [std::string&] smf_uri: based URI of Nsmf_PDUSession Services
   * @return true if can get the URI. otherwise false
   */
  bool get_smf_pdu_session_context_uri(
      const std::shared_ptr<pdu_session_context>& psc, std::string& smf_uri);

  /*
   * Get the URI of SMF Services
   * @param [const std::string&] smf_uri_root: in form SMF's Addr:Port
   * @param [const std::string&] smf_api_version: SMF's API version in String
   * representation
   * @return URI in string format
   */
  std::string get_smf_pdu_session_base_uri(
      const std::string& smf_uri_root, const std::string& smf_api_version);

  /*
   * Display the AMF configuration parameters
   * @param void
   * @return void
   */
  void display();

  /*
   * Represent AMF's config as json object
   * @param [nlohmann::json &] json_data: Json data
   * @return void
   */
  void to_json(nlohmann::json& json_data) const;

  /*
   * Update AMF's config from Json
   * @param [nlohmann::json &] json_data: Updated configuration in json format
   * @return true if success otherwise return false
   */
  bool from_json(nlohmann::json& json_data);

  unsigned int instance;
  std::string pid_dir;
  spdlog::level::level_enum log_level;
  interface_cfg_t n2;
  interface_cfg_t sbi;
  itti_cfg_t itti;

  uint32_t http_request_timeout;
  unsigned int statistics_interval;
  std::string amf_name;
  guami_full_format_t guami;
  std::vector<guami_full_format_t> guami_list;
  unsigned int relative_amf_capacity;
  std::vector<plmn_item_t> plmn_list;
  bool is_emergency_support;
  auth_conf_t auth_para;
  nas_conf_t nas_cfg;
  support_features_t support_features;
  nf_addr_t smf_addr;
  nf_addr_t nrf_addr;
  nf_addr_t ausf_addr;
  nf_addr_t udm_addr;
  nf_addr_t nssf_addr;
  nf_addr_t lmf_addr;
};

}  // namespace oai::config

#endif
