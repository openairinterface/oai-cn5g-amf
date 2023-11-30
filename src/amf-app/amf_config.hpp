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
#include <sys/socket.h>

#include <boost/algorithm/string.hpp>
#include <libconfig.h++>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "amf.hpp"
#include "common_defs.h"
#include "3gpp_24.501.h"
#include "if.hpp"
#include "pdu_session_context.hpp"
#include "string.hpp"
#include "thread_sched.hpp"

// TODO: common for both YAML/Libconfig
constexpr auto AMF_CONFIG_OPTION_YES_STR = "Yes";
constexpr auto AMF_CONFIG_OPTION_NO_STR  = "No";

using namespace libconfig;

namespace oai::config {

typedef struct auth_conf_s {
  std::string mysql_server;
  std::string mysql_user;
  std::string mysql_pass;
  std::string mysql_db;
  bool random;

  nlohmann::json to_json() const {
    nlohmann::json json_data  = {};
    json_data["mysql_server"] = this->mysql_server;
    json_data["mysql_user"]   = this->mysql_user;
    json_data["mysql_pass"]   = this->mysql_pass;
    json_data["mysql_db"]     = this->mysql_db;
    json_data["random"]       = this->random;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->mysql_server = json_data["mysql_server"].get<std::string>();
    this->mysql_user   = json_data["mysql_user"].get<std::string>();
    this->mysql_pass   = json_data["mysql_pass"].get<std::string>();
    this->mysql_db     = json_data["mysql_db"].get<std::string>();
    this->random       = json_data["random"].get<bool>();
  }
} auth_conf_t;

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int mtu;
  unsigned int port;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["if_name"]     = this->if_name;
    json_data["addr4"]       = inet_ntoa(this->addr4);
    json_data["network4"]    = inet_ntoa(this->network4);
    char str_addr6[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &this->addr6, str_addr6, sizeof(str_addr6));
    json_data["addr6"] = str_addr6;
    json_data["mtu"]   = this->mtu;
    json_data["port"]  = this->port;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->if_name         = json_data["if_name"].get<std::string>();
    std::string addr4_str = {};
    addr4_str             = json_data["addr4"].get<std::string>();

    if (boost::iequals(addr4_str, "read")) {
      if (get_inet_addr_infos_from_iface(
              this->if_name, this->addr4, this->network4, this->mtu)) {
        Logger::amf_app().error(
            "Could not read %s network interface configuration", this->if_name);
        return;
      }
    } else {
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(addr4_str).c_str(), this->addr4,
          "BAD IPv4 ADDRESS FORMAT FOR INTERFACE !");

      std::string network4_str = json_data["network4"].get<std::string>();
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(network4_str).c_str(), this->network4,
          "BAD IPv4 ADDRESS FORMAT FOR INTERFACE !");
      // TODO: addr6
      this->mtu  = json_data["mtu"].get<int>();
      this->port = json_data["port"].get<int>();
    }
  }

} interface_cfg_t;

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params sx_sched_params;
  util::thread_sched_params s5s8_sched_params;
  util::thread_sched_params pgw_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

typedef struct guami_s {
  std::string mcc;
  std::string mnc;
  uint8_t region_id;
  uint16_t amf_set_id;
  uint8_t amf_pointer;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["mcc"]         = this->mcc;
    json_data["mnc"]         = this->mnc;
    json_data["region_id"]   = this->region_id;
    json_data["amf_set_id"]  = this->amf_set_id;
    json_data["amf_pointer"] = this->amf_pointer;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->mcc         = json_data["mcc"].get<std::string>();
    this->mnc         = json_data["mnc"].get<std::string>();
    this->region_id   = json_data["region_id"].get<int>();
    this->amf_set_id  = json_data["amf_set_id"].get<int>();
    this->amf_pointer = json_data["amf_pointer"].get<int>();
  }
} guami_t;

typedef struct slice_s {
  uint8_t sst;
  uint32_t sd;

  bool operator==(const struct slice_s& s) const {
    if ((s.sst == this->sst) && (s.sd == this->sd)) {
      return true;
    } else {
      return false;
    }
  }

  bool operator>(const struct slice_s& s) const {
    if (this->sst > s.sst) return true;
    if (this->sst == s.sst) {
      return (this->sd > s.sd);
    }
    return false;
  }

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["sst"]         = this->sst;
    if (this->sd != SD_NO_VALUE) json_data["sd"] = this->sd;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->sst = json_data["sst"].get<int>();
    if (json_data.find("sd") != json_data.end()) {
      this->sd = json_data["sd"].get<int>();
    }
  }
} slice_t;

typedef struct plmn_support_item_s {
  std::string mcc;
  std::string mnc;
  uint32_t tac;
  std::vector<slice_t> slice_list;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["mcc"]         = this->mcc;
    json_data["mnc"]         = this->mnc;
    json_data["tac"]         = this->tac;
    json_data["slice_list"]  = nlohmann::json::array();
    for (auto s : slice_list) {
      json_data["slice_list"].push_back(s.to_json());
    }
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->mcc = json_data["mcc"].get<std::string>();
    this->mnc = json_data["mnc"].get<std::string>();
    this->tac = json_data["tac"].get<int>();

    for (auto s : json_data["slice_list"]) {
      slice_t sl = {};
      sl.from_json(s);
      slice_list.push_back(sl);
    }
  }
} plmn_item_t;

typedef struct {
  std::vector<_5g_ia_e> prefered_integrity_algorithm;
  std::vector<_5g_ea_e> prefered_ciphering_algorithm;

  nlohmann::json to_json() const {
    nlohmann::json json_data                  = {};
    json_data["prefered_integrity_algorithm"] = nlohmann::json::array();
    json_data["prefered_ciphering_algorithm"] = nlohmann::json::array();
    for (auto s : this->prefered_integrity_algorithm) {
      json_data["prefered_integrity_algorithm"].push_back(get_5g_ia_str(s));
    }
    for (auto s : this->prefered_ciphering_algorithm) {
      json_data["prefered_ciphering_algorithm"].push_back(get_5g_ea_str(s));
    }
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    for (auto s : json_data["prefered_integrity_algorithm"]) {
      std::string integ_alg = s.get<std::string>();
      prefered_integrity_algorithm.push_back(get_5g_ia(integ_alg));
    }

    for (auto s : json_data["prefered_ciphering_algorithm"]) {
      std::string cipher_alg = s.get<std::string>();
      prefered_ciphering_algorithm.push_back(get_5g_ea(cipher_alg));
    }
  }
} nas_conf_t;

typedef struct nf_addr_s {
  struct in_addr ipv4_addr;
  unsigned int port;
  std::string api_version;
  std::string fqdn;
  std::string uri_root;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["uri_root"]    = this->uri_root;
    json_data["api_version"] = this->api_version;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->uri_root    = json_data["uri_root"].get<std::string>();
    this->api_version = json_data["api_version"].get<std::string>();
  }
} nf_addr_t;

typedef struct support_features_s {
  bool enable_nf_registration;
  bool enable_smf_selection;
  bool enable_external_ausf;
  bool enable_external_udm;
  bool enable_nssf;
  bool enable_external_nrf;
  bool enable_lmf;
  bool use_fqdn_dns;
  bool use_http2;
  nlohmann::json to_json() const {
    nlohmann::json json_data            = {};
    json_data["enable_nf_registration"] = this->enable_nf_registration;
    json_data["enable_smf_selection"]   = this->enable_smf_selection;
    json_data["enable_external_ausf"]   = this->enable_external_ausf;
    json_data["enable_external_udm"]    = this->enable_external_udm;
    json_data["enable_nssf"]            = this->enable_nssf;
    json_data["enable_lmf"]             = this->enable_lmf;
    json_data["use_fqdn_dns"]           = this->use_fqdn_dns;
    json_data["use_http2"]              = this->use_http2;
    return json_data;
  }

  void from_json(nlohmann::json& json_data) {
    this->enable_nf_registration =
        json_data["enable_nf_registration"].get<bool>();
    this->enable_smf_selection = json_data["enable_smf_selection"].get<bool>();
    this->enable_external_ausf = json_data["enable_external_ausf"].get<bool>();
    this->enable_external_udm  = json_data["enable_external_udm"].get<bool>();
    this->enable_nssf          = json_data["enable_nssf"].get<bool>();
    this->use_fqdn_dns         = json_data["use_fqdn_dns"].get<bool>();
    this->use_http2            = json_data["use_http2"].get<bool>();
    this->enable_lmf           = json_data["enable_lmf"].get<bool>();
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
  std::string get_amf_n1n2_message_subscribe_uri(const std::string& ue_cxt_id);

  /*
   * Get the URI of NRF NF Discovery Service
   * @param void
   * @return URI in string format
   */
  std::string get_nrf_nf_discovery_service_uri();

  /*
   * Get the URI of NRF NF Registration Service
   * @param [const std::string&] nf_instance_id: NF instance ID
   * @return URI in string format
   */
  std::string get_nrf_nf_registration_uri(const std::string& nf_instance_id);

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
  std::string sbi_api_version;
  unsigned int sbi_http2_port;

  unsigned int statistics_interval;
  std::string amf_name;
  guami_t guami;
  std::vector<guami_t> guami_list;
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
