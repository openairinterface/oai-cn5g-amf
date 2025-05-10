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

#ifndef _AMF_SBI_HELPER_HPP
#define _AMF_SBI_HELPER_HPP

#include <nlohmann/json.hpp>

#include "amf_config.hpp"
#include "sbi_helper.hpp"
#include "pdu_session_context.hpp"

using namespace oai::config;
using namespace oai::common::sbi;

extern std::unique_ptr<oai::config::amf_config> amf_cfg;

namespace oai::amf::api {

class amf_sbi_helper : public sbi_helper {
 public:
  static inline const std::string AmfCommPathN1MessageNotify =
      "n1-message-notify";
  static inline const std::string AmfCommPathN1N2Messages = "n1-n2-messages";

  static std::string AmfCommunicationServiceBase() {
    return sbi_helper::AmfCommBase +
           amf_cfg->sbi.api_version.value_or(kDefaultSbiApiVersion);
  }

  static std::string AmfEventExposureServiceBase() {
    return sbi_helper::AmfEvtsBase +
           amf_cfg->sbi.api_version.value_or(kDefaultSbiApiVersion);
  }

  static std::string AmfStatusNotifyServiceBase() {
    return sbi_helper::AmfStatusNotifBase +
           amf_cfg->sbi.api_version.value_or(kDefaultSbiApiVersion);
  }

  static std::string AmfConfigurationServiceBase() {
    return sbi_helper::AmfConfBase +
           amf_cfg->sbi.api_version.value_or(kDefaultSbiApiVersion);
  }

  static std::string AmfLocationServiceBase() {
    return sbi_helper::AmflocBase +
           amf_cfg->sbi.api_version.value_or(kDefaultSbiApiVersion);
  }

  static std::string AmfCallbackBase() {
    return sbi_helper::AmfCallbackBase +
           amf_cfg->sbi.api_version.value_or(kDefaultSbiApiVersion);
  }

  static void set_problem_details(
      nlohmann::json& json_data, const std::string& detail);

  /*
   * Get the URI of AMF N1N2MessageSubscribe
   * @param [const interface_cfg_t&] sbi: SBI interface
   * @param [const std::string&] ue_cxt_id: UE Context Id
   * @return URI in string format
   */
  static std::string get_amf_n1n2_message_subscribe_uri(
      const interface_cfg_t& sbi, const std::string& ue_cxt_id,
      const std::string& subscription_id);

  /*
   * Get the URI of AMF NonUEN2InfoSubscribe
   * @param [const interface_cfg_t&] sbi: SBI interface
   * @param [const std::string&] subscription_id: Subscription Id
   * @return URI in string format
   */
  static std::string get_non_ue_n2_info_subscribe_uri(
      const interface_cfg_t& sbi, const std::string& subscription_id);

  /*
   * Get the URI of NSSF Network Slice Selection Information Service
   * @param [const nf_addr_t&] nssf_addr: NSSF's Address
   * @param void
   * @return URI in string format
   */
  static std::string get_nssf_network_slice_selection_information_uri(
      const nf_addr_t& nssf_addr);

  /*
   * Get the URI of AUSF UE Authentication Service
   * @param [const nf_addr_t&] ausf_addr: AUSF's Address
   * @param void
   * @return URI in string format
   */
  static std::string get_ausf_ue_authentications_uri(
      const nf_addr_t& ausf_addr);

  /*
   * Get the URI of LMF Determine Location Service
   * @param [const nf_addr_t&] lmf_addr: LMF's Address
   * @param void
   * @return URI in string format
   */
  static std::string get_lmf_determine_location_uri(const nf_addr_t& lmf_addr);

  /*
   * Get the URI of SMF PDU Session Service
   * @param [const std::shared_ptr<pdu_session_context>&] psc: pointer to the
   * PDU Session Context
   * @param [std::string&] smf_uri: based URI of Nsmf_PDUSession Services
   * @return true if can get the URI. otherwise false
   */
  static bool get_smf_pdu_session_context_uri(
      const std::shared_ptr<pdu_session_context>& psc, std::string& smf_uri);

  /*
   * Get the URI of SMF Services
   * @param [const std::string&] smf_uri_root: in form SMF's Addr:Port
   * @param [const std::string&] smf_api_version: SMF's API version in String
   * representation
   * @return URI in string format
   */
  static std::string get_smf_pdu_session_base_uri(
      const std::string& smf_uri_root, const std::string& smf_api_version);

  /*
   * Get the URI of SM context  status notification
   * @param [const interface_cfg_t&] sbi: SBI interface
   * @param [const std::string&] supi: UE SUPI
   * @param [uint8_t] pdu_session_id: PDU Session ID
   * @return URI in string format
   */
  static std::string get_sm_context_status_notification_uri(
      const interface_cfg_t& sbi, const std::string& supi,
      uint8_t pdu_session_id);

  /*
   * Get the URI of UDM Slice Selection Subscription Data Retrieval Service
   * @param [const nf_addr_t&] udm_addr: UDM's Address
   * @param [const std::string&] supi: UE SUPI
   * @return URI in string format
   */
  static std::string get_udm_slice_selection_subscription_data_retrieval_uri(
      const nf_addr_t& udm_addr, const std::string& supi);

  /*
   * Get the URI of UDM AMF Registration for 3GPP access
   * @param [const nf_addr_t&] udm_addr: UDM's Address
   * @param [const std::string&] supi: UE SUPI
   * @return URI in string format
   */
  static std::string get_udm_amf_3gpp_access_registration_uri(
      const nf_addr_t& udm_addr, const std::string& supi);
};

}  // namespace oai::amf::api

#endif
