/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_SBI_H_
#define _AMF_SBI_H_

#include "3gpp_29.500.h"
#include "http_definitions.hpp"
#include "itti_msg_sbi.hpp"
#include "pdu_session_context.hpp"
#include "ue_context.hpp"

namespace amf_application {

class amf_sbi {
 public:
  amf_sbi();
  virtual ~amf_sbi();

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
   * Handle ITTI message to reroute N1 message to the target AMF
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
   * Handle ITTI message to trigger AMF Registration for 3GPP Access towards UDM
   * @param [itti_sbi_register_with_udm&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_register_with_udm& itti_msg);

  /*
   * Handle ITTI message to Retrieve a UE's Access and Mobility Subscription
   * Data from UDM
   * @param [itti_sbi_retrieve_am_data&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_retrieve_am_data& itti_msg);

  /*
   * Handle ITTI message to retrieve a UE's SMF Selection Subscription Data from
   * UDM
   * @param [itti_sbi_retrieve_smf_selection_subscription_data&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      itti_sbi_retrieve_smf_selection_subscription_data& itti_msg);

  /*
   * Handle ITTI message to discover PCF's info from the corresponding NRF
   * @param [itti_sbi_pcf_discovery&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_pcf_discovery& itti_msg);

  /*
   * Handle ITTI message to perform AM Policy Association with PCF
   * @param [itti_sbi_am_policy_association&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_am_policy_association& itti_msg);

  /*
   * Handle ITTI message to perform AM Policy Association Termination with PCF
   * @param [itti_sbi_am_policy_association_termination&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      itti_sbi_am_policy_association_termination& itti_msg);

  /*
   * Handle ITTI message to update the AM Policy Association with PCF
   * @param [itti_sbi_am_policy_association_update&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_am_policy_association_update& itti_msg);

  /*
   * Handle ITTI message to retrieve the AM Policy Association with PCF
   * @param [itti_sbi_am_policy_association_update&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_am_policy_association_retrieval& itti_msg);

  /*
   * Handle ITTI message to retrieve a UE's UE Context In SMF Data
   * @param [itti_sbi_ue_context_in_smf_data_retrieval&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_ue_context_in_smf_data_retrieval& itti_msg);

  /*
   * Handle ITTI message to subscribe for UDM SDM notifications
   * (TS 23.502 §4.2.2.2.2 step 14b, TS 29.503 §5.2.3.3.3)
   * @param [itti_sbi_sdm_subscribe&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_sdm_subscribe& itti_msg);

  /*
   * Handle Nudm_SDM_Unsubscribe (TS 29.503 §5.2.3.3.4)
   * Send a DELETE to UDM to remove the SDM subscription.
   * @param [itti_sbi_sdm_unsubscribe&] itti_msg: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_sdm_unsubscribe& itti_msg);

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
      bstring sm_msg, const std::string& dnn,
      const std::shared_ptr<ue_context>& uc);

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
   * @return true if an SMF found successful, otherwise return false
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
  /*
   * Get Network Slice Information
   * @param [const snssai_t&] snssai: SNSSAI
   * @param [const plmn_t&] plmn: PLMN
   * @param [const std::optional<std::string>&] dnn: DNN
   * @param [const std::string&] amf_instance_id: AMF Instance ID
   * @param [nlohmann::json&] response_data: Response data in JSON format
   * @param [uint32_t&] response_code: HTTP Response code
   * @return void
   */
  void get_network_slice_information(
      const snssai_t& snssai, const plmn_t& plmn,
      const std::optional<std::string>& dnn, const std::string& amf_instance_id,
      nlohmann::json& response_data, uint32_t& response_code);

  /*
   * Create multipart content for HTTP request
   * @param [const std::string&] json_data: Json data (msg body)
   * @param [const std::string&] n1sm_msg: N1 SM message
   * @param [const std::string&] n2sm_msg: N2 SM message
   * @param [bool] is_multipart: Flag indicating if multipart content is
   * required
   * @param [std::string&] body: Output body content
   * @return void
   */
  void create_multipart_content(
      const std::string& json_data, const std::string& n1sm_msg,
      const std::string& n2sm_msg, bool is_multipart, std::string& body);
  /*
   * Send a HTTP request to the HTTP server
   * @param [const std::string&] remote_uri: Server's Address
   * @param [const std::string&] json_data: Json data (msg body)
   * @param [std::string&] n1sm_msg: N1 SM message
   * @param [ std::string&] n2sm_msg: N2 SM message
   * @param [ const std::string&] supi: SUPI
   * @param [const std::string&] pdu_session_id: PDU Session ID
   * @param [uint8_t] http_version: HTTP version
   * @param [uint32_t] promise_id: Promise ID
   * @return true if successful, otherwise return false
   */
  bool send_http_request(
      const std::string& remote_uri, const std::string& json_data,
      const std::string& n1sm_msg, const std::string& n2sm_msg,
      const std::string& supi, uint8_t pdu_session_id, uint8_t http_version = 1,
      const uint32_t& promise_id = 0);

  /*
   * Send a HTTP request to the HTTP server
   * @param [const std::string&] remote_uri: Server's Address
   * @param [oai::common::sbi::method_e] method: HTTP method
   * @param [const std::string&] msg_body: Msg body
   * @param [std::string&] response_json: Respone in Json format
   * @param [uint32_t&] response_code: HTTP Response code
   * @param [uint8_t] http_version: HTTP version
   * @return true if successful, otherwise return false
   */
  bool send_http_request(
      const std::string& remote_uri, oai::common::sbi::method_e method,
      const std::string& msg_body, nlohmann::json& response_json,
      uint32_t& response_code, uint8_t http_version = 1);

  /*
   * Send a HTTP request to the HTTP server
   * @param [const std::string&] remote_uri: Server's Address
   * @param [std::string&] json_data: Msg body
   * @param [std::string&] n1sm_msg: N1 SM message
   * @param [std::string&] n2sm_msg: N2 SM message
   * @param [uint8_t] http_version: HTTP version
   * @param [uint32_t&] response_code: HTTP Response code
   * @param [uint32_t] promise_id: Promise ID
   * @return true if successful, otherwise return false
   */
  bool send_http_request(
      const std::string& remote_uri, std::string& json_data,
      std::string& n1sm_msg, std::string& n2sm_msg, uint8_t http_version,
      uint32_t& response_code, const uint32_t& promise_id = 0);

  /*
   * Send a HTTP request to the HTTP server
   * @param [const std::string&] remote_uri: Server's Address
   * @param [oai::common::sbi::method_e] method: HTTP method
   * @param [const std::string&] msg_body: Msg body
   * @param [uint32_t&] http_response: HTTP Response code
   * @return true if successful, otherwise return false
   */
  bool send_http_request(
      const std::string& remote_uri, const oai::common::sbi::method_e method,
      const std::string& msg_body, oai::http::response& http_response);
};

}  // namespace amf_application

#endif
