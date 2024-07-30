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

#ifndef _AMF_SBI_H_
#define _AMF_SBI_H_

#include "3gpp_29.500.h"
#include "AuthenticationInfo.h"
#include "UEAuthenticationCtx.h"
#include "itti_msg_sbi.hpp"
#include "pdu_session_context.hpp"

namespace amf_application {

class amf_sbi {
 public:
  amf_sbi();
  ~amf_sbi();

  /*
   * Handle ITTI message (Nsmf_PDUSessionCreateSMContext) to create a new PDU
   * Session SM context
   * @param [itti_nsmf_pdusession_create_sm_context&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_nsmf_pdusession_create_sm_context&);

  /*
   * Handle ITTI message (Nsmf_PDUSessionUpdateSMContext) to update a PDU
   * Session SM context
   * @param [itti_nsmf_pdusession_update_sm_context&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_nsmf_pdusession_update_sm_context& itti_msg);

  /*
   * Handle ITTI message (Nsmf_PDUSessionReleaseSMContext) to release a PDU
   * Session
   * @param [itti_nsmf_pdusession_release_sm_context&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_nsmf_pdusession_release_sm_context& itti_msg);

  /*
   * Handle ITTI message (receiving PDU Session Resource Setup Response)
   * @param [itti_pdu_session_resource_setup_response&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_pdu_session_resource_setup_response& itti_msg);

  /*
   * Handle ITTI message to send Event Notification to the subscribed NFs
   * @param [itti_sbi_notify_subscribed_event&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_notify_subscribed_event& itti_msg);

  /*
   * Handle ITTI message to get the Slice Selection Subscription Data from UDM
   * @param [itti_sbi_slice_selection_subscription_data&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      itti_sbi_slice_selection_subscription_data& itti_msg);

  /*
   * Handle ITTI message to get the Network Slice Selection Information from
   * NSSF
   * @param [itti_sbi_network_slice_selection_information&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      itti_sbi_network_slice_selection_information& itti_msg);

  /*
   * Handle ITTI message to get the Network Slice Selection Discovery from
   * NSSF
   * @param [itti_sbi_network_slice_selection_discovery&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      itti_sbi_network_slice_selection_discovery& itti_msg);

  /*
   * Handle ITTI message to reroute N1 message to the targer AMF
   * @param [itti_sbi_n1_message_notify&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_n1_message_notify& itti_msg);

  /*
   * Handle ITTI message to send N2 Info Notify to the subscribed NFs
   * @param [itti_sbi_n2_info_notify&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_n2_info_notify& itti_msg);

  /*
   * Handle ITTI message to discover NF instance information from NRF
   * @param [itti_sbi_nf_instance_discovery&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_nf_instance_discovery& itti_msg);

  /*
   * Handle ITTI message to register to NRF
   * @param [itti_sbi_register_nf_instance_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_register_nf_instance_request& itti_msg);

  /*
   * Handle ITTI message to update to NRF
   * @param [itti_sbi_update_nf_instance_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_update_nf_instance_request& itti_msg);

  /*
   * Handle ITTI message to deregister to NRF
   * @param [itti_sbi_deregister_nf_instance_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_deregister_nf_instance_request& itti_msg);

  /*
   * Handle ITTI message to trigger Determine Location Request procedure towards
   * LMF
   * @param [itti_sbi_determine_location_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_determine_location_request& itti_msg);

  /*
   * Handle ITTI message to trigger UE Authentication request towards AUSF
   * @param [itti_sbi_ue_authentication_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_ue_authentication_request& itti_msg);

  /*
   * Handle ITTI message to trigger UE Authentication Confirmation towards AUSF
   * @param [itti_sbi_ue_authentication_confirmation&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_ue_authentication_confirmation& itti_msg);

  /*
   * Handle request to create a new PDU Session
   * @param [const std::string&] supi: SUPI
   * @param [std::shared_ptr<pdu_session_context>&] psc: Pointer to the PDU
   * Session Context
   * @param [const std::string&] smf_uri_root: SMF's Address:Port
   * @param [const std::string&] smf_api_version: SMF's API version
   * @param [bstring] sm_msg: SM message
   * @param [const std::string&] dnn: DNN
   * @return void
   */
  void handle_pdu_session_initial_request(
      const std::string& supi, std::shared_ptr<pdu_session_context>& psc,
      const std::string& smf_uri_root, const std::string& smf_api_version,
      bstring sm_msg, const std::string& dnn);

  /*
   * Send SM Context response error to AMF
   * @param [const long] code: HTTP response code
   * @param [const std::string&] cause: Response cause
   * @param [bstring] n1sm: N1 SM message
   * @param [const std::string&] supi: SUPI
   * @param [uint8_t] pdu_session_id: PDU Session ID
   * @return void
   */
  void handle_post_sm_context_response_error(
      const long code, const std::string& cause, bstring n1sm,
      const std::string& supi, uint8_t pdu_session_id);

  /*
   * Send the request to update PDU session context at SMF
   * @param [const std::string&] supi: SUPI
   * @param [std::shared_ptr<pdu_session_context>&] psc: Pointer to the PDU
   * Session Context
   * @param [bstring] sm_msg: SM message
   * @param [const std::string&] dnn: DNN
   * @return void
   */
  void send_pdu_session_update_sm_context_request(
      const std::string& supi, std::shared_ptr<pdu_session_context>& psc,
      bstring sm_msg, const std::string& dnn);

  /*
   * Select SMF from the configuration file
   * @param [std::string&] smf_uri_root: in form SMF's Address:Port
   * @param [std::string&] smf_api_version: SMF's API version
   * @return true if successful, otherwise return false
   */
  bool smf_selection_from_configuration(
      std::string& smf_uri_root, std::string& smf_api_version);

  /*
   * Find suitable SMF from NRF (based on snssai, plmn and dnn)
   * @param [std::string&] smf_uri_root: in the form of ADDR:PORT
   * @param [std::string&] smf_api_version: SMF's API version
   * @param [const snssai_t&] snssai: SNSSAI
   * @param [const plmn_t&] plmn: PLMN
   * @param [const std::string&] dnn: DNN
   * @param [const std::string&] nrf_uri: NRF's NF Discovery Service URI
   * @return true if successful, otherwise return false
   */
  bool discover_smf(
      std::string& smf_uri_root, std::string& smf_api_version,
      const snssai_t& snssai, const plmn_t& plmn, const std::string& dnn,
      const std::string& nrf_uri = {});

  /*
   * Get NRF's URI from NSSF/configuration file
   * @param [const snssai_t&] snssai: SNSSAI
   * @param [const plmn_t&] plmn: PLMN
   * @param [const std::string&] dnn: DNN
   * @param [std::string&] nrf_uri: NRF's NF Discovery Service URI
   * @return true if successful, otherwise return false
   */
  bool get_nrf_uri(
      const snssai_t& snssai, const plmn_t& plmn, const std::string& dnn,
      std::string& nrf_uri);

  void get_network_slice_information(
      const snssai_t& snssai, const plmn_t& plmn,
      const std::optional<std::string>& dnn, const std::string& amf_instance_id,
      nlohmann::json& response_data, uint32_t& response_code);

  void create_multipart_content(
      const std::string& json_data, const std::string& n1sm_msg,
      const std::string& n2sm_msg, bool is_multipart, std::string& body);
  /*
   * CURL client to send request to the HTTP server
   * @param [const std::string&] remote_uri: Server's Address
   * @param [const std::string&] json_data: Json data (msg body)
   * @param [const std::string&] n1sm_msg: N1 SM message
   * @param [ const std::string&] n2sm_msg: N2 SM message
   * @param [ const std::string&] supi: SUPI
   * @param [const std::string&] pdu_session_id: PDU Session ID
   * @param [uint8_t] http_version: HTTP versioin
   * @param [uint32_t] promise_id: Promise ID
   * @return void
   */
  bool curl_http_client(
      const std::string& remote_uri, const std::string& json_data,
      const std::string& n1sm_msg, const std::string& n2sm_msg,
      const std::string& supi, uint8_t pdu_session_id, uint8_t http_version = 1,
      const uint32_t& promise_id = 0);

  /*
   * CURL client to send request to the HTTP server
   * @param [const std::string&] remote_uri: Server's Address
   * @param [oai::common::sbi::method_e] method: HTTP method
   * @param [const std::string&] msg_body: Msg body
   * @param [std::string&] response_json: Respone in Json format
   * @param [uint32_t&] response_code: HTTP Response code
   * @param [uint8_t] http_version: HTTP versioin
   * @return void
   */
  void curl_http_client(
      const std::string& remote_uri, oai::common::sbi::method_e method,
      const std::string& msg_body, nlohmann::json& response_json,
      uint32_t& response_code, uint8_t http_version = 1);

  /*
   * CURL client to send request to the HTTP server
   * @param [const std::string&] remote_uri: Server's Address
   * @param [std::string&] json_data: Msg body
   * @param [std::string&] n1sm_msg: N1 SM message
   * @param [std::string&] n2sm_msg: N2 SM message
   * @param [uint8_t] http_version: HTTP versioin
   * @param [uint32_t&] response_code: HTTP Response code
   * @param [uint32_t] promise_id: Promise ID
   * @return void
   */
  void curl_http_client(
      const std::string& remote_uri, std::string& json_data,
      std::string& n1sm_msg, std::string& n2sm_msg, uint8_t http_version,
      uint32_t& response_code, const uint32_t& promise_id = 0);
};

}  // namespace amf_application

#endif
