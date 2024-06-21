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

#ifndef _AMF_APP_H_
#define _AMF_APP_H_

#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <map>
#include <shared_mutex>
#include <unordered_set>

#include "N1MessageClass_anyOf.h"
#include "N2InformationClass_anyOf.h"
#include "NsiInformation.h"
#include "ProblemDetails.h"
#include "UeN1N2InfoSubscriptionCreateData.h"
#include "amf_config.hpp"
#include "amf_msg.hpp"
#include "amf_profile.hpp"
#include "amf_subscription.hpp"
#include "itti.hpp"
#include "itti_msg_amf_app.hpp"
#include "itti_msg_sbi.hpp"
#include "ue_context.hpp"
#include "uint_generator.hpp"

using namespace oai::config;
using namespace oai::model::amf;

namespace amf_application {

#define TASK_AMF_APP_PERIODIC_STATISTICS (0)
#define TASK_AMF_MOBILE_REACHABLE_TIMER_EXPIRE (1)
#define TASK_AMF_IMPLICIT_DEREGISTRATION_TIMER_EXPIRE (2)
#define TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT (3)
#define TASK_AMF_APP_TIMEOUT_NRF_DEREGISTRATION (4)
#define TASK_AMF_APP_TIMEOUT_NRF_REGISTRATION (5)

class amf_app {
 private:
  inline static uint32_t amf_app_ue_ngap_id_generator = 1;
  amf_profile nf_instance_profile;
  std::string amf_instance_id;
  // timer_id_t timer_nrf_heartbeat;
  std::map<std::string, timer_id_t> timer_nrfs_heartbeat;

  oai::utils::uint_generator<uint32_t> evsub_id_generator;
  std::map<
      std::pair<evsub_id_t, amf_event_type_t>,
      std::shared_ptr<amf_subscription>>
      amf_event_subscriptions;
  mutable std::shared_mutex m_amf_event_subscriptions;

  oai::utils::uint_generator<uint32_t> tmsi_generator;

  std::map<uint64_t, std::shared_ptr<ue_context>> amf_ue_ngap_id2ue_ctx;
  mutable std::shared_mutex m_amf_ue_ngap_id2ue_ctx;
  std::map<std::string, std::shared_ptr<ue_context>> ue_ctx_key;
  mutable std::shared_mutex m_ue_ctx_key;

  std::map<std::string, std::shared_ptr<ue_context>> supi2ue_ctx;
  mutable std::shared_mutex m_supi2ue_ctx;

  mutable std::shared_mutex m_curl_handle_responses_sbi;
  std::map<uint32_t, boost::shared_ptr<boost::promise<nlohmann::json>>>
      curl_handle_responses_sbi;

  oai::utils::uint_generator<uint32_t> n1n2sub_id_generator;
  std::map<
      std::pair<std::string, uint32_t>,
      std::shared_ptr<oai::model::amf::UeN1N2InfoSubscriptionCreateData>>
      n1n2_message_subscribe;
  mutable std::shared_mutex m_n1n2_message_subscribe;

  std::map<
      n1n2sub_id_t,
      std::shared_ptr<oai::model::amf::NonUeN2InfoSubscriptionCreateData>>
      non_ue_n2_info_subscribe;
  mutable std::shared_mutex m_non_ue_n2_info_subscribe;

  std::unordered_set<std::string> registered_nrfs;

 public:
  explicit amf_app(const amf_config& amf_cfg);
  amf_app(amf_app const&) = delete;
  ~amf_app();
  void operator=(amf_app const&) = delete;

  void start();
  /**
   * Stop all the ongoing processes and send NF deregistration towards NRF
   */
  void stop();

  /*
   * Generate AMF UE NGAP ID
   * @return generated ID
   */
  uint64_t generate_amf_ue_ngap_id();

  /*
   * Handle ITTI message (NAS Signalling Establishment Request: Registration
   * Request, Service Request )
   * @param [itti_nas_signalling_establishment_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_nas_signalling_establishment_request& itti_msg);

  /*
   * Handle ITTI message (N1N2MessageTransferRequest)
   * @param [itti_n1n2_message_transfer_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_n1n2_message_transfer_request& itti_msg);

  /*
   * Handle ITTI message (NonUeN2MessageTransferRequest)
   * @param [itti_non_ue_n2_message_transfer_request&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_non_ue_n2_message_transfer_request& itti_msg);

  /*
   * Handle ITTI message (SBI N1 Message Notification)
   * @param [itti_sbi_n1_message_notification&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_n1_message_notification& itti_msg);

  /*
   * Handle ITTI message (SBI N1N2 Message Subscribe)
   * @param [itti_sbi_n1n2_subscribe_message&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_n1n2_message_subscribe& itti_msg);

  /*
   * Handle ITTI message (SBI N1N2 Message UnSubscribe)
   * @param [itti_sbi_n1n2_unsubscribe_message&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_n1n2_message_unsubscribe& itti_msg);

  /*
   * Handle ITTI message (SBI NON UE N2 Info Subscribe)
   * @param [itti_sbi_non_ue_n2_info_subscribe&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_non_ue_n2_info_subscribe& itti_msg);

  /*
   * Handle ITTI message (SBI NON UE N2 Info Unsubscribe)
   * @param [itti_sbi_non_ue_n2_info_unsubscribe&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_non_ue_n2_info_unsubscribe& itti_msg);

  /*
   * Handle ITTI message (SBI PDU Session Release Notification)
   * @param [itti_sbi_pdu_session_release_notif&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_pdu_session_release_notif& itti_msg);

  /*
   * Handle ITTI message (SBI AMF configuration)
   * @param [itti_sbi_amf_configuration&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_amf_configuration& itti_msg);

  /*
   * Handle ITTI message (Update AMF configuration)
   * @param [itti_sbi_update_amf_configuration&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_update_amf_configuration& itti_msg);

  /*
   * Handle ITTI message (Register NF Instance Response)
   * @param [itti_sbi_register_nf_instance_response&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_register_nf_instance_response& r);

  /*
   * Handle ITTI message (Update NF Instance Response)
   * @param [itti_sbi_update_nf_instance_response&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_update_nf_instance_response& r);

  /*
   * Handle ITTI message (Deregister NF Instance Response)
   * @param [itti_sbi_deregister_nf_instance_response&]: ITTI message
   * @return void
   */
  void handle_itti_message(itti_sbi_deregister_nf_instance_response& r);

  /*
   * Get the current AMF's configuration
   * @param [nlohmann::json&]: json_data: Store AMF configuration
   * @return true if success, otherwise return false
   */
  bool read_amf_configuration(nlohmann::json& json_data);

  /*
   * Update AMF configuration
   * @param [nlohmann::json&]: json_data: New AMF configuration
   * @return true if success, otherwise return false
   */
  bool update_amf_configuration(nlohmann::json& json_data);

  /*
   * Get number of registered UEs to this AMF
   * @param [uint32_t&]: num_ues: Store the number of registered UEs
   * @return void
   */
  void get_number_registered_ues(uint32_t& num_ues) const;

  /*
   * Get number of registered UEs to this AMF
   * @param void
   * @return: number of registered UEs
   */
  uint32_t get_number_registered_ues() const;

  /*
   * Get UE context associated with an UE Context Key and verify if this pointer
   * is nullptr
   * @param [const std::string&] ue_context_key: UE Context Key
   * @param [std::shared_ptr<ue_context>&] uc: pointer to UE context if exist
   * @return true if UE Context exist and not null
   */
  bool ran_amf_id_2_ue_context(
      const std::string& ue_context_key, std::shared_ptr<ue_context>& uc) const;

  /*
   * Store an UE context associated with UE Context Key
   * @param [const std::string&] ue_context_key: UE Context Key
   * @param [std::shared_ptr<ue_context>&] uc: pointer to UE context
   * @return void
   */
  void set_ran_amf_id_2_ue_context(
      const std::string& ue_context_key, const std::shared_ptr<ue_context>& uc);

  /*
   * Get UE context associated with a SUPI
   * @param [const std::string&] supi: SUPI
   * @param [std::shared_ptr<ue_context>&] uc: pointer to UE context if exist
   * @return true if UE Context exist and not null
   */
  bool supi_2_ue_context(
      const std::string& supi, std::shared_ptr<ue_context>& uc) const;

  /*
   * Store an UE context associated with a SUPI
   * @param [const std::string&] supi: UE SUPI
   * @param [const std::shared_ptr<ue_context>&] uc: pointer to UE context
   * @return void
   */
  void set_supi_2_ue_context(
      const std::string& supi, const std::shared_ptr<ue_context>& uc);

  /*
   * Find a PDU Session Context associated with a SUPI and a PDU Session ID
   * @param [const std::string&] supi: UE SUPI
   * @param [ const std::uint8_t] pdu_session_id: PDU Session ID
   * @param [const std::shared_ptr<pdu_session_context>&] psc: pointer to the
   * PDU Session Context
   * @return true if found, otherwise false
   */
  bool find_pdu_session_context(
      const std::string& supi, const std::uint8_t pdu_session_id,
      std::shared_ptr<pdu_session_context>& psc);

  /*
   * Get a list of PDU Session Contexts associated with a SUPI
   * @param [const std::string&] supi: UE SUPI
   * @param [std::vector<std::shared_ptr<pdu_session_context>>&] sessions_ctx:
   * vector of contexts
   * @return true if found, otherwise false
   */
  bool get_pdu_sessions_context(
      const std::string& supi,
      std::vector<std::shared_ptr<pdu_session_context>>& sessions_ctx);

  /*
   * Update PDU Session Context status
   * @param [const std::string&] supi: UE SUPI
   * @param [uint8_t] pdu_session_id: PDU Session ID
   * @param [const oai::model::amf::SmContextStatusNotification&]
   * statusNotification: Notification information received from SMF
   * @return true if success, otherwise false
   */
  bool update_pdu_sessions_context(
      const std::string& supi, uint8_t pdu_session_id,
      const oai::model::amf::SmContextStatusNotification& statusNotification);

  /*
   * Generate a TMSI value for UE
   * @param void
   * @return generated value in uint32_t
   */
  uint32_t generate_tmsi();

  /*
   * Create a 5G GUTI from PLMN/TMSI
   * @param [const uint32_t] ranid: RAN UE NGAP ID
   * @param [const long] amfid: AMF UE NGAP ID
   * @param [std::string&] MCC: Mobile Country Code
   * @param [std::string&] MNC: Mobile Network Code
   * @param [uint32_t&] tmsi: Generated TMSI
   * @return true if generated successfully, otherwise return false
   */
  bool generate_5g_guti(
      const uint32_t ranid, const long amfid, std::string& mcc,
      std::string& mnc, uint32_t& tmsi);

  /*
   * Generate an Event Exposure Subscription ID
   * @param [void]
   * @return the generated reference
   */
  evsub_id_t generate_ev_subscription_id();

  /*
   * Generate an N1N2MessageSubscribe ID
   * @param [void]
   * @return the generated reference
   */
  n1n2sub_id_t generate_n1n2_message_subscription_id();

  /*
   * Add an N1N2MessageSubscribe subscription to the list
   * @param [const std::string&] ue_ctx_id: UE Context ID
   * @param [const n1n2sub_id_t&] sub_id: Subscription ID
   * @param [std::shared_ptr<oai::model::amf::UeN1N2InfoSubscriptionCreateData>]
   * oai::model::amf::UeN1N2InfoSubscriptionCreateData: a shared pointer stored
   * information of the subscription
   * @return void
   */
  void add_n1n2_message_subscription(
      const std::string& ue_ctx_id, const n1n2sub_id_t& sub_id,
      std::shared_ptr<oai::model::amf::UeN1N2InfoSubscriptionCreateData>&
          subscription);

  /*
   * Remove an N1N2MessageSubscribe subscription from the list
   * @param [const std::string&] ue_ctx_id: UE Context ID
   * @param [const std::string&] sub_id: Subscription ID
   * @return true if the subscription is deleted successfully, otherwise return
   * false
   */
  bool remove_n1n2_message_subscription(
      const std::string& ue_ctx_id, const std::string& sub_id);

  /*
   * Find the subscriptions matched with certain conditions
   * @param [const std::string&] ue_ctx_id: UE Context ID
   * @param
   * [std::optional<oai::model::amf::N1MessageClass_anyOf::eN1MessageClass_anyOf>&]
   * n1_message_class: Type of N1 Message
   * @param
   * [std::optional<oai::model::amf::N2InformationClass_anyOf::eN2InformationClass_anyOf>&]
   * n2_info_class: Type of N2 Message
   * @param [std::map<n1n2sub_id_t,
   * std::shared_ptr<oai::model::amf::UeN1N2InfoSubscriptionCreateData>>&]
   * subscriptions: list of subscriptions matched
   * @return void
   */
  void find_n1n2_info_subscriptions(
      const std::string& ue_ctx_id,
      std::optional<
          oai::model::amf::N1MessageClass_anyOf::eN1MessageClass_anyOf>&
          n1_message_class,
      std::optional<
          oai::model::amf::N2InformationClass_anyOf::eN2InformationClass_anyOf>&
          n2_info_class,
      std::map<
          n1n2sub_id_t,
          std::shared_ptr<oai::model::amf::UeN1N2InfoSubscriptionCreateData>>&
          subscriptions);

  /*
   * Add an NonUEN2InfoSubscribe subscription to the list
   * @param [const n1n2sub_id_t&] sub_id: Subscription ID
   * @param
   * [std::shared_ptr<oai::model::amf::NonUeN2InfoSubscriptionCreateData>]
   * subscription_data: a shared pointer stored information of the subscription
   * @return void
   */
  void add_non_ue_n2_info_subscription(
      const n1n2sub_id_t& sub_id,
      std::shared_ptr<oai::model::amf::NonUeN2InfoSubscriptionCreateData>&
          subscription_data);

  /*
   * Remove an NonUEN2InfoSubscribe subscription from the list
   * @param [const std::string&] sub_id: Subscription ID
   * @return true if the subscription is deleted successfully, otherwise return
   * false
   */
  bool remove_non_ue_n2_info_subscription(const std::string& sub_id);

  /*
   * Find the subscriptions matched with certain conditions
   * @param [const std::string&] nf_id: NF Id
   * @param
   * [std::optional<oai::model::amf::N2InformationClass_anyOf::eN2InformationClass_anyOf>&]
   * n2_info_class: Type of N2 Message
   * @param [std::map<n1n2sub_id_t,
   * std::shared_ptr<oai::model::amf::NonUeN2InfoSubscriptionCreateData>>&]
   * subscriptions: list of subscriptions matched
   * @return void
   */
  void find_non_ue_n2_info_subscriptions(
      const std::string& nf_id,
      const oai::model::amf::N2InformationClass_anyOf::
          eN2InformationClass_anyOf& n2_info_class,
      std::map<
          n1n2sub_id_t,
          std::shared_ptr<oai::model::amf::NonUeN2InfoSubscriptionCreateData>>&
          subscriptions);

  /*
   * Trigger NF instance registration to NRF
   * @param [void]
   * @return void
   */
  void register_to_nrf();

  /*
   * Trigger NF instance registration to a NRF
   * @param [const std::string&] nrf_uri: NRF's URI
   * @return void
   */
  void register_to_nrf(const std::string& nrf_uri) const;

  /*
   * Get a list of suitable NRFs from a NSSF
   * @param [std::unordered_set<std::string>&] nrfs: store the list of NRF's URI
   * @return void
   */
  void get_nrfs(std::unordered_set<std::string>& nrfs);

  /*
   * Handle Event Exposure Msg from NF
   * @param [std::shared_ptr<itti_sbi_event_exposure_request>&] Request message
   * @return [evsub_id_t] ID of the created subscription
   */
  evsub_id_t handle_event_exposure_subscription(
      std::shared_ptr<itti_sbi_event_exposure_request> msg);

  /*
   * Handle Unsubscribe Request from an NF
   * @param [const std::string&] subscription_id: Subscription ID
   * @return true if the subscription is unsubscribed successfully, otherwise
   * return false
   */
  bool handle_event_exposure_delete(const std::string& subscription_id);

  /*
   * Handle NF status notification (e.g., when an UPF becomes available)
   * @param [std::shared_ptr<itti_sbi_notification_data>& ] msg: message
   * @param [oai::model::amf::ProblemDetails& ] problem_details
   * @param [uint32_t&] http_code
   * @return true if handle successfully, otherwise return false
   */
  bool handle_nf_status_notification(
      std::shared_ptr<itti_sbi_notification_data>& msg,
      oai::model::common::ProblemDetails& problem_details, uint32_t& http_code);

  /*
   * Handle a request to determine location from LMF
   * @param void
   * return void
   */
  void handle_determine_location_request();

  /*
   * Generate a random UUID for AMF instance
   * @param [void]
   * @return void
   */
  void generate_uuid();

  /*
   * Add an Event Subscription to the list
   * @param [const evsub_id_t&] sub_id: Subscription ID
   * @param [const amf_event_t&] ev: Event type
   * @param [std::shared_ptr<amf_subscription>] ss: a shared pointer stored
   * information of the subscription
   * @return void
   */
  void add_event_subscription(
      const evsub_id_t& sub_id, const amf_event_type_t& ev,
      std::shared_ptr<amf_subscription> ss);

  /*
   * Remove an Event Subscription from the list
   * @param [const evsub_id_t&] sub_id: Subscription ID
   * @return bool
   */
  bool remove_event_subscription(const evsub_id_t& sub_id);

  /*
   * Get a list of subscription associated with a particular event
   * @param [const amf_event_t] ev: Event type
   * @param [std::vector<std::shared_ptr<amf_subscription>>&] subscriptions:
   * store the list of the subscription associated with this event type
   * @return void
   */
  void get_ee_subscriptions(
      const amf_event_type_t& ev,
      std::vector<std::shared_ptr<amf_subscription>>& subscriptions);

  /*
   * Get a list of subscription associated with a particular event
   * @param [const evsub_id_t&] sub_id: Subscription ID
   * @param [std::vector<std::shared_ptr<amf_subscription>>&] subscriptions:
   * store the list of the subscription associated with this event type
   * @return void
   */
  void get_ee_subscriptions(
      const evsub_id_t& sub_id,
      std::vector<std::shared_ptr<amf_subscription>>& subscriptions);

  /*
   * Get a list of subscription associated with a particular event
   * @param [const amf_event_t] ev: Event type
   * @param [std::string&] supi: SUPI
   * @param [std::vector<std::shared_ptr<amf_subscription>>&] subscriptions:
   * store the list of the subscription associated with this event type
   * @return void
   */
  void get_ee_subscriptions(
      const amf_event_type_t& ev, std::string& supi,
      std::vector<std::shared_ptr<amf_subscription>>& subscriptions);

  /*
   * Generate a AMF profile for this instance
   * @param [void]
   * @return void
   */
  void generate_amf_profile();

  /*
   * Send request to SBI task to trigger NF instance deregistration to NRF
   * @param [void]
   * @return void
   */
  void deregister_to_nrf() const;

  /*
   * Handle Heartbeat timeout
   * @param [timer_id_t] timer_id
   * @param [uint64_t] arg2_user
   * @return void
   */
  void timer_nrf_heartbeat_timeout(timer_id_t timer_id, std::string arg2_user);

  /*
   * Handle NF registration timeout
   * @param [timer_id_t] timer_id
   * @param [uint64_t] arg2_user
   * @return void
   */
  void timer_nrf_registration_timeout(
      timer_id_t timer_id, std::string arg2_user);

  /*
   * Store the promise
   * @param [const uint32_t] pid: promise id
   * @param [const boost::shared_ptr<boost::promise<nlohmann::json>>&] p:
   * promise
   * @return void
   */
  void add_promise(
      const uint32_t pid,
      const boost::shared_ptr<boost::promise<nlohmann::json>>& p);

  /*
   * Remove the promise
   * @param [uint32_t] pid: promise id
   * @return void
   */
  void remove_promise(const uint32_t id);

  /*
   * Generate an unique value for promise id
   * @param void
   * @return generated promise id
   */
  static uint64_t generate_promise_id() {
    return oai::utils::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }

  /*
   * Trigger the response from API server
   * @param [const uint32_t] pid: promise id
   * @param [const nlohmann::json&] json_data: result for the corresponding
   * promise
   * @return void
   */
  void trigger_process_response(
      const uint32_t pid, const nlohmann::json& json_data);

  /*
   * Send request to SBI task to trigger PDU session release to SMF
   * @param [const std::shared_ptr<ue_context>&]uc: UE Context
   * @return void
   */
  void trigger_pdu_session_release(const std::shared_ptr<ue_context>& uc) const;

  /*
   * Send request to SBI task to trigger UP Deactivation of a PDU session
   * towards SMF
   * @param [const std::shared_ptr<ue_context>&]uc: UE Context
   * @return void
   */
  void trigger_pdu_session_up_deactivation(
      const std::shared_ptr<ue_context>& uc) const;

  /*
   * Send request to SBI task to trigger UP Activation of all PDU sessions
   * associated with an UE context towards SMF
   * @param [const std::shared_ptr<ue_context>&]uc: UE Context
   * @return true if activation success, otherwise return false
   */
  bool trigger_pdu_session_up_activation(
      const std::shared_ptr<ue_context>& uc) const;

  /*
   * Send request to SBI task to trigger UP Activation of a PDU session towards
   * SMF
   * @param [uint8_t]pdu_session_id: PDU Session ID
   * @param [const std::shared_ptr<ue_context>&]uc: UE Context
   * @return true if activation success, otherwise return false
   */
  bool trigger_pdu_session_up_activation(
      uint8_t pdu_session_id, const std::shared_ptr<ue_context>& uc) const;

  /*
   * Get the AMF's NF instance
   * @return NF instance in string format
   */
  std::string get_nf_instance() const;
};

}  // namespace amf_application

#endif
