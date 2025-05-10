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

#include "amf_sbi_helper.hpp"

#include <fmt/format.h>

#include <boost/algorithm/string.hpp>
#include <regex>
#include <vector>

#include "ProblemDetails.h"
#include "logger.hpp"

namespace oai::amf::api {
//------------------------------------------------------------------------------
void amf_sbi_helper::set_problem_details(
    nlohmann::json& json_data, const std::string& detail) {
  Logger::amf_app().error("%s", detail);
  oai::model::common::ProblemDetails problem_details;
  problem_details.setDetail(detail);
  to_json(json_data, problem_details);
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_amf_n1n2_message_subscribe_uri(
    const interface_cfg_t& sbi, const std::string& ue_cxt_id,
    const std::string& subscription_id) {
  std::string fmr_format_str = {};
  get_fmt_format_form(
      AmfCommPathUeContextContextIdN1N2MessageSubscriptionsSubscriptionId,
      fmr_format_str);
  return sbi.get_ipv4_root() + AmfCommunicationServiceBase() +
         fmt::format(fmr_format_str, ue_cxt_id, subscription_id);
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_non_ue_n2_info_subscribe_uri(
    const interface_cfg_t& sbi, const std::string& subscription_id) {
  std::string fmr_format_str = {};
  get_fmt_format_form(
      AmfCommPathNonUeN1N2MessageSubscriptionsn2NotifySubscriptionId,
      fmr_format_str);
  return sbi.get_ipv4_root() + AmfCommunicationServiceBase() +
         fmt::format(fmr_format_str, subscription_id);
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_nssf_network_slice_selection_information_uri(
    const nf_addr_t& nssf_addr) {
  return nssf_addr.uri_root + NssfNsSelectionBase + nssf_addr.api_version +
         NssfNsSelectionPathNetworSliceInformation;
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_ausf_ue_authentications_uri(
    const nf_addr_t& ausf_addr) {
  return ausf_addr.uri_root + AusfAuthBase + ausf_addr.api_version +
         AusfAuthPathUeAuthentications;
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_lmf_determine_location_uri(
    const nf_addr_t& lmf_addr) {
  return lmf_addr.uri_root + AmfLocationServiceBase() +
         AmflocPathDetermineLocation;
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_smf_pdu_session_base_uri(
    const std::string& smf_uri_root, const std::string& smf_api_version) {
  return smf_uri_root + SmfPduSessionBase + smf_api_version +
         SmfPduSessionPathSmContexts;
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_sm_context_status_notification_uri(
    const interface_cfg_t& sbi, const std::string& supi,
    uint8_t pdu_session_id) {
  std::string fmr_format_str = {};
  get_fmt_format_form(
      AmfStatusNotifPathPduSessionReleasePduSessionId, fmr_format_str);
  std::string http_str = "http://";

  if (amf_cfg->enable_tls()) http_str = "https://";

  return http_str + sbi.get_ipv4_root() + AmfStatusNotifyServiceBase() +
         fmt::format(fmr_format_str, supi, std::to_string(pdu_session_id));
}

//------------------------------------------------------------------------------
bool amf_sbi_helper::get_smf_pdu_session_context_uri(
    const std::shared_ptr<pdu_session_context>& psc, std::string& smf_uri) {
  if (!psc) return false;

  if (!psc->smf_info.info_available) {
    Logger::amf_sbi().error("No SMF is available for this PDU session");
    return false;
  }

  if (psc->smf_info.context_location.size() == 0) return false;

  Logger::amf_sbi().debug(
      "smf_info, context location %s", psc->smf_info.context_location);
  std::size_t found = psc->smf_info.context_location.find("/");
  if (found != 0)
    smf_uri = psc->smf_info.context_location;
  else
    smf_uri = psc->smf_info.uri_root + psc->smf_info.context_location;
  return true;
}

//------------------------------------------------------------------------------
std::string
amf_sbi_helper::get_udm_slice_selection_subscription_data_retrieval_uri(
    const nf_addr_t& udm_addr, const std::string& supi) {
  std::string fmr_format_str = {};
  get_fmt_format_form(UdmSdmPathSupiNssai, fmr_format_str);
  return udm_addr.uri_root + UdmSdmBase + udm_addr.api_version +
         fmt::format(fmr_format_str, supi);
}

//------------------------------------------------------------------------------
std::string amf_sbi_helper::get_udm_amf_3gpp_access_registration_uri(
    const nf_addr_t& udm_addr, const std::string& supi) {
  std::string fmr_format_str = {};
  get_fmt_format_form(UdmUeCmPath3gppRegistrations, fmr_format_str);
  return udm_addr.uri_root + UdmUeCmBase + udm_addr.api_version +
         fmt::format(fmr_format_str, supi);
}

}  // namespace oai::amf::api
