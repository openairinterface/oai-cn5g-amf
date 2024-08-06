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

#ifndef FILE_AMF_HPP_SEEN
#define FILE_AMF_HPP_SEEN

#include "3gpp_23.003.h"

#include <nlohmann/json.hpp>
#include <string>

#include "3gpp_24.501.hpp"
#include "string.hpp"
#include "thread_sched.hpp"

// for CURL
constexpr auto CURL_MIME_BOUNDARY = "----Boundary";

#define BUFFER_SIZE_8192 8192
#define BUFFER_SIZE_4096 4096
#define BUFFER_SIZE_2048 2048
#define BUFFER_SIZE_1024 1024
#define BUFFER_SIZE_512 512
#define BUFFER_SIZE_256 256

#define GNB_ID_FMT "%" PRIu32
#define GNB_UE_NGAP_ID_FMT "%" PRIu32
#define AMF_UE_NGAP_ID_FMT "%" PRIu64

constexpr uint64_t INVALID_AMF_UE_NGAP_ID = 0x010000000000;  // 2^40

// Event Subscription IDs)
typedef uint32_t evsub_id_t;
#define EVSUB_ID_FMT "0x%" PRIx32
#define EVSUB_ID_SCAN_FMT SCNx32
#define INVALID_EVSUB_ID ((evsub_id_t) 0x00000000)
#define UNASSIGNED_EVSUB_ID ((evsub_id_t) 0x00000000)

typedef uint32_t n1n2sub_id_t;

constexpr uint64_t SECONDS_SINCE_FIRST_EPOCH = 2208988800;

#define UE_AGGREGATE_MAXIMUM_BIT_RATE_DL 1000000000
#define UE_AGGREGATE_MAXIMUM_BIT_RATE_UL 1000000000

#define NAMF_EVENT_EXPOSURE_BASE "/namf-evts/"

#define NAS_MESSAGE_DOWNLINK 1
#define NAS_MESSAGE_UPLINK 0

#define JSON_CONTENT_ID_MIME "root"
#define N1_SM_CONTENT_ID "n1SmMsg"
#define N2_SM_CONTENT_ID "n2msg"
#define N2_NRPPa_CONTENT_ID "n2NrppaMsg"

typedef enum {
  PlainNasMsg                                              = 0x0,
  IntegrityProtected                                       = 0x1,
  IntegrityProtectedAndCiphered                            = 0x2,
  IntegrityProtectedWithNew5GNASSecurityContext            = 0x3,
  IntegrityProtectedAndCipheredWithNew5GNASSecurityContext = 0x4,
} SecurityHeaderType_t;

constexpr uint32_t DEFAULT_HTTP1_PORT  = 80;
constexpr uint32_t DEFAULT_HTTP2_PORT  = 8080;
constexpr auto DEFAULT_SBI_API_VERSION = "v1";
constexpr auto DEFAULT_SUPI_TYPE =
    "imsi";  // Set to "imsi" when supporting both IMSI and NAI as SUPI

constexpr auto DEFAULT_SST = 1;
constexpr auto DEFAULT_DNN = "default";

constexpr auto kSbiResponseJsonData         = "jsonData";
constexpr auto kSbiResponseHttpResponseCode = "httpResponseCode";

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
    try {
      if (json_data.find("mysql_server") != json_data.end()) {
        this->mysql_server = json_data["mysql_server"].get<std::string>();
      }
      if (json_data.find("mysql_user") != json_data.end()) {
        this->mysql_user = json_data["mysql_user"].get<std::string>();
      }
      if (json_data.find("mysql_pass") != json_data.end()) {
        this->mysql_pass = json_data["mysql_pass"].get<std::string>();
      }
      if (json_data.find("mysql_db") != json_data.end()) {
        this->mysql_db = json_data["mysql_db"].get<std::string>();
      }
      if (json_data.find("random") != json_data.end()) {
        this->random = json_data["random"].get<bool>();
      }
    } catch (std::exception& e) {
      Logger::amf_app().error("%s", e.what());
    }
  }
} auth_conf_t;

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params sx_sched_params;
  util::thread_sched_params s5s8_sched_params;
  util::thread_sched_params pgw_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

typedef struct plmn_support_item_s {
  std::string mcc;
  std::string mnc;
  uint32_t tac;
  std::vector<snssai_t> slice_list;

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
    try {
      if (json_data.find("mcc") != json_data.end()) {
        this->mcc = json_data["mcc"].get<std::string>();
      }
      if (json_data.find("mnc") != json_data.end()) {
        this->mnc = json_data["mnc"].get<std::string>();
      }
      if (json_data.find("tac") != json_data.end()) {
        this->tac = json_data["tac"].get<int>();
      }

      if (json_data.find("slice_list") != json_data.end()) {
        for (auto s : json_data["slice_list"]) {
          snssai_t sl = {};
          sl.from_json(s);
          slice_list.push_back(sl);
        }
      }
    } catch (std::exception& e) {
      Logger::amf_app().error("%s", e.what());
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
    try {
      if (json_data.find("prefered_integrity_algorithm") != json_data.end()) {
        for (auto s : json_data["prefered_integrity_algorithm"]) {
          std::string integ_alg = s.get<std::string>();
          prefered_integrity_algorithm.push_back(get_5g_ia(integ_alg));
        }
      }

      if (json_data.find("prefered_ciphering_algorithm") != json_data.end()) {
        for (auto s : json_data["prefered_ciphering_algorithm"]) {
          std::string cipher_alg = s.get<std::string>();
          prefered_ciphering_algorithm.push_back(get_5g_ea(cipher_alg));
        }
      }
    } catch (std::exception& e) {
      Logger::amf_app().error("%s", e.what());
    }
  }
} nas_conf_t;

#endif
