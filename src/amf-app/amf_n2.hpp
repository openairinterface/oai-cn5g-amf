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

#ifndef _AMF_N2_H_
#define _AMF_N2_H_

#include "amf.hpp"
#include "itti_msg_n2.hpp"
#include "ngap_app.hpp"
#include "ue_ngap_context.hpp"
#include "Struct.hpp"
#include "NgapIeType_anyOf.h"
#include "N2InformationNotification.h"

namespace amf_application {

class amf_n2 : public oai::ngap::ngap_app {
 public:
  amf_n2(const std::string& address, const uint16_t port_num);
  ~amf_n2();

  /*
   * Handle ITTI message (New SCTP Association)
   * @param [std::shared_ptr<itti_new_sctp_association>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_new_sctp_association>& new_assoc);

  /*
   * Handle ITTI message (NG Setup Request)
   * @param [std::shared_ptr<itti_ng_setup_request>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_ng_setup_request>& ng_setup_req);

  /*
   * Handle ITTI message (NG Reset)
   * @param [std::shared_ptr<itti_downlink_nas_transfer>&]: ITTI message
   * @return void
   */
  void handle_itti_message(std::shared_ptr<itti_ng_reset>&);

  /*
   * Handle ITTI message (SCTP Shutdown)
   * @param [std::shared_ptr<itti_ng_shutdown>&]: ITTI message
   * @return void
   */
  void handle_itti_message(std::shared_ptr<itti_ng_shutdown>&);

  /*
   * Handle ITTI message (InitialUEMessage)
   * @param [std::shared_ptr<itti_initial_ue_message>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_initial_ue_message>& init_ue_msg);

  /*
   * Handle ITTI message (UplinkNASTransport)
   * @param [std::shared_ptr<itti_ul_nas_transport>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_ul_nas_transport>& ul_nas_transport);

  /*
   * Handle ITTI message (DLNASTransport)
   * @param [std::shared_ptr<itti_dl_nas_transport>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_dl_nas_transport>& dl_nas_transport);

  /*
   * Handle ITTI message (InitialContextSetupRequest)
   * @param [std::shared_ptr<itti_initial_context_setup_request>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_initial_context_setup_request>& itti_msg);

  /*
   * Handle ITTI message (PDUSessionResourceSetupRequest)
   * @param [std::shared_ptr<itti_pdu_session_resource_setup_request>&]: ITTI
   * message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_pdu_session_resource_setup_request>& itti_msg);

  /*
   * Handle ITTI message (PDUSessionResourceModifyRequest)
   * @param [std::shared_ptr<itti_pdu_session_resource_modify_request>&]: ITTI
   * message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_pdu_session_resource_modify_request>& itti_msg);

  /*
   * Handle ITTI message (PDUSessionResourceReleaseCommand)
   * @param [std::shared_ptr<itti_pdu_session_resource_release_command>&]: ITTI
   * message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_pdu_session_resource_release_command>& itti_msg);

  /*
   * Handle ITTI message (PDUSessionResourceReleaseRequest)
   * @param [std::shared_ptr<itti_ue_context_release_request>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_ue_context_release_request>& itti_msg);

  /*
   * Handle ITTI message (PDUSessionResourceReleaseComplete)
   * @param [std::shared_ptr<itti_ue_context_release_complete>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_ue_context_release_complete>& itti_msg);

  /*
   * Handle ITTI message (UEContextReleaseCommand)
   * @param [std::shared_ptr<itti_ue_context_release_command>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_ue_context_release_command>& itti_msg);

  /*
   * Handle ITTI message (UECapabilityIndication)
   * @param [std::shared_ptr<itti_ue_radio_capability_indication>&]: ITTI
   * message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_ue_radio_capability_indication>& itti_msg);

  /*
   * Handle ITTI message (HandoverRequired)
   * @param [std::shared_ptr<itti_handover_required>&]: ITTI message
   * @return void
   */
  bool handle_itti_message(std::shared_ptr<itti_handover_required>& itti_msg);

  /*
   * Handle ITTI message (HandoverRequestAck)
   * @param [std::shared_ptr<itti_handover_request_ack>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_handover_request_ack>& itti_msg);

  /*
   * Handle ITTI message (HandoverNotify)
   * @param [std::shared_ptr<itti_handover_notify>&]: ITTI message
   * @return void
   */
  void handle_itti_message(std::shared_ptr<itti_handover_notify>& itti_msg);

  /*
   * Handle ITTI message (UplinkRANStatusTransfer)
   * @param [std::shared_ptr<itti_uplink_ran_status_transfer>&]: ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_uplink_ran_status_transfer>& itti_msg);

  /*
   * Handle ITTI message (RerouteNAS)
   * @param [std::shared_ptr<itti_rereoute_nas>&]: ITTI message
   * @return void
   */
  void handle_itti_message(std::shared_ptr<itti_rereoute_nas>& itti_msg);

  /*
   * Handle ITTI message (Paging)
   * @param [std::shared_ptr<itti_paging>&]: ITTI message
   * @return void
   */
  void handle_itti_message(std::shared_ptr<itti_paging>& itti_msg);

  /*
   * Handle ITTI message (DownlinkUEAssociatedNRPPaTransport)
   * @param [std::shared_ptr<itti_downlink_ue_associated_nrppa_transport>&]:
   * ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_downlink_ue_associated_nrppa_transport>& itti_msg);

  /*
   * Handle ITTI message (DownlinkNonUEAssociatedNRPPaTransport)
   * @param [std::shared_ptr<itti_downlink_non_ue_associated_nrppa_transport>&]:
   * ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_downlink_non_ue_associated_nrppa_transport>&
          itti_msg);

  /*
   * Handle ITTI message (itti_uplink_ue_associated_nrppa_transport)
   * @param [std::shared_ptr<itti_uplink_ue_associated_nrppa_transport>&]:
   * ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_uplink_ue_associated_nrppa_transport>& itti_msg);

  /*
   * Handle ITTI message (itti_uplink_non_ue_associated_nrppa_transport)
   * @param [std::shared_ptr<itti_uplink_non_ue_associated_nrppa_transport>&]:
   * ITTI message
   * @return void
   */
  void handle_itti_message(
      std::shared_ptr<itti_uplink_non_ue_associated_nrppa_transport>& itti_msg);

  /*
   * Send Handover Preparaton Failure message
   * @param [const uint64_t] amf_ue_ngap_id: AMF UE NGAP ID
   * @param [const uint32_t] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [const sctp_assoc_id_t&] gnb_assoc_id: gNB Association ID
   * @return void
   */
  void send_handover_preparation_failure(
      const uint64_t amf_ue_ngap_id, const uint32_t ran_ue_ngap_id,
      const sctp_assoc_id_t& gnb_assoc_id);

  /*
   * Send NG Setup Failure
   * @param [const T&] cause: Cause
   * @param [const e_Ngap_TimeToWait&] time_to_wait: Time to Wait IE
   * @param [const uint32_t] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [const sctp_assoc_id_t&] assoc_id: gNB Association ID
   * @param [const sctp_stream_id_t&] stream: Stream ID
   * @return void
   */
  template<typename T>
  void send_ng_setup_failure(
      const T& cause, const e_Ngap_TimeToWait& time_to_wait,
      const sctp_assoc_id_t& assoc_id, const sctp_stream_id_t& stream);

  /*
   * Get list of common PLMN between AMF and gNB
   * @param [const std::vector<SupportedTaItem_t>&] list: Supported TA list from
   * gNB
   * @param [std::vector<SupportedTaItem_t>&] result: list of common TA
   * @return true if there's at least 1 common TA, otherwise return false
   */
  bool get_common_plmn(
      const std::vector<SupportedTaItem_t>& list,
      std::vector<SupportedTaItem_t>& result);

  /*
   * Get list of common S-NSSAIs between AMF and gNB to be used by UE
   * @param [const uint32_t&] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [std::vector<oai::nas::SNSSAI_t>&] common_nssai: list of common
   * S-NSSAIs
   * @return void
   */
  bool get_common_NSSAI(
      const uint32_t& ran_ue_ngap_id, uint32_t gnb_id,
      std::vector<oai::nas::SNSSAI_t>& common_nssai);

  /*
   * Get the UE NGAP context associated with a RAN UE NGAP ID if it exists and
   * not null
   * @param [const uint32_t&] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [const uint32_t&] gnb_id: gNB ID
   * @param [std::shared_ptr<ue_ngap_context>&] unc: shared pointer to the
   * existing UE NGAP context
   * @return true if the context exists and is not null, otherwise return false
   */
  bool ran_ue_id_2_ue_ngap_context(
      uint32_t ran_ue_ngap_id, uint32_t gnb_id,
      std::shared_ptr<ue_ngap_context>& unc) const;

  /*
   * Get the UE NGAP context associated with a RAN UE NGAP ID if it exists and
   * not null
   * @param [uint32_t&] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [const std::string&] ue_context_key: UE context key from AMF UE NGAP
   * ID and RAN UE NGAP ID
   * @param [const std::shared_ptr<ue_ngap_context>&] unc: shared pointer to the
   * existing UE NGAP context
   * @return true if the context exists and is not null, otherwise return false
   */
  bool ran_ue_id_2_ue_ngap_context(
      uint32_t ran_ue_ngap_id, const std::string& ue_context_key,
      std::shared_ptr<ue_ngap_context>& unc) const;

  /*
   * Store UE NGAP context associated with a RAN UE NGAP ID
   * @param [uint32_t] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [uint32_t] gnb_id: gNB ID
   * @param [const std::shared_ptr<ue_ngap_context>&] unc: pointer to UE NGAP
   * context
   * @return void
   */
  void set_ran_ue_ngap_id_2_ue_ngap_context(
      uint32_t ran_ue_ngap_id, uint32_t gnb_id,
      const std::shared_ptr<ue_ngap_context>& unc);

  /*
   * Remove UE NGAP context associated with a RAN UE NGAP ID
   * @param [uint32_t] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [uint32_t] gnb_id: gNB ID
   * @return void
   */
  void remove_ran_ue_ngap_id_2_ngap_context(
      uint32_t ran_ue_ngap_id, uint32_t gnb_id);

  /*
   * Remove UE Context associated with a RAN UE NGAP ID
   * @param [uint32_t] ran_ue_ngap_id: RAN UE NGAP ID
   * @param [uint32_t] gnb_id: gNB ID
   * @return void
   */
  void remove_ue_context_with_ran_ue_ngap_id(
      uint32_t ran_ue_ngap_id, uint32_t gnb_id);

  /*
   * Get UE NGAP context associated with a AMF UE NGAP ID if the context exists
   * and is not null
   * @param [const uint64_t&] amf_ue_ngap_id: AMF UE NGAP ID
   * @param [std::shared_ptr<ue_ngap_context>&] unc: store the pointer to UE
   * NGAP context
   * @return true if the context exists and is not null, otherwise return false
   */
  bool amf_ue_id_2_ue_ngap_context(
      const uint64_t& amf_ue_ngap_id,
      std::shared_ptr<ue_ngap_context>& unc) const;

  /*
   * Store UE NGAP context associated with a AMF UE NGAP ID
   * @param [const uint64_t&] amf_ue_ngap_id: AMF UE NGAP ID
   * @param [const std::shared_ptr<ue_ngap_context>&] unc: pointer to UE NGAP
   * context
   * @return void
   */
  void set_amf_ue_ngap_id_2_ue_ngap_context(
      const uint64_t& amf_ue_ngap_id, std::shared_ptr<ue_ngap_context> unc);

  /*
   * Remove UE NGAP context associated with a AMF UE NGAP ID
   * @param [const uint64_t&] amf_ue_ngap_id: AMF UE NGAP ID
   * @return void
   */
  void remove_amf_ue_ngap_id_2_ue_ngap_context(const uint64_t& amf_ue_ngap_id);

  /*
   * Remove UE Context associated with a AMF UE NGAP ID
   * @param [const unsigned uint64_t&] amf_ue_ngap_id: AMF UE NGAP ID
   * @return void
   */
  void remove_ue_context_with_amf_ue_ngap_id(const uint64_t& amf_ue_ngap_id);

  /*
   * Get list of UE Context associated with a gNB
   * @param [const sctp_assoc_id_t&] gnb_assoc_id: gNB Association ID
   * @param [std::vector<std::shared_ptr<ue_ngap_context>>&] ue_contexts: vector
   * of UE Context
   * @return void
   */
  void get_ue_ngap_contexts(
      const sctp_assoc_id_t& gnb_assoc_id,
      std::vector<std::shared_ptr<ue_ngap_context>>& ue_contexts);

  /*
   * Fill the content of N2InformationNotification with the corresponding info
   * @param [const std::string&] subscription_id: Subscription ID
   * @param [const oai::model::amf::NgapIeType_anyOf::eNgapIeType_anyOf&]
   * ngap_ie_type: Type of NGAP Ie
   * @param [oai::model::amf::N2InformationNotification&] n2_info_notification:
   * N2InformationNotification
   * @return void
   */
  void fill_n2_information_notification(
      const std::string& subscription_id,
      const oai::model::amf::NgapIeType_anyOf::eNgapIeType_anyOf& ngap_ie_type,
      oai::model::amf::N2InformationNotification& n2_info_notification);

 private:
  // <RAN UE NGAP ID, gNB ID> <-> UE Context
  std::map<std::pair<uint32_t, uint32_t>, std::shared_ptr<ue_ngap_context>>
      ranid2uecontext;
  mutable std::shared_mutex m_ranid2uecontext;

  // AMF UE ID <-> UE Context
  std::map<unsigned long, std::shared_ptr<ue_ngap_context>> amfueid2uecontext;
  mutable std::shared_mutex m_amfueid2uecontext;
};

}  // namespace amf_application

#endif
