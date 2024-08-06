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

#include "amf_n2.hpp"

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/duration.hpp>

#include "3gpp_24.501.hpp"
#include "DefaultPagingDrx.hpp"
#include "DownlinkNasTransport.hpp"
#include "DownlinkNonUeAssociatedNrppaTransport.hpp"
#include "DownlinkRanStatusTransfer.hpp"
#include "DownlinkUeAssociatedNrppaTransport.hpp"
#include "HandoverCommandMsg.hpp"
#include "HandoverPreparationFailure.hpp"
#include "HandoverRequest.hpp"
#include "InitialContextSetupRequest.hpp"
#include "NgResetAck.hpp"
#include "NgSetupFailure.hpp"
#include "NgSetupResponse.hpp"
#include "Ngap_Cause.h"
#include "Ngap_CauseNas.h"
#include "Ngap_CauseRadioNetwork.h"
#include "Ngap_TimeToWait.h"
#include "Paging.hpp"
#include "PduSessionResourceHandoverCommandTransfer.hpp"
#include "PduSessionResourceModifyRequest.hpp"
#include "PduSessionResourceReleaseCommand.hpp"
#include "PduSessionResourceSetupRequest.hpp"
#include "RerouteNasRequest.hpp"
#include "UeContextReleaseCommand.hpp"
#include "amf_app.hpp"
#include "amf_config.hpp"
#include "amf_conversions.hpp"
#include "amf_n1.hpp"
#include "amf_sbi.hpp"
#include "amf_statistics.hpp"
#include "itti.hpp"
#include "itti_msg_amf_app.hpp"
#include "logger.hpp"
#include "ngap_utils.hpp"
#include "output_wrapper.hpp"
#include "sctp_server.hpp"
#include "utils.hpp"

using namespace amf_application;
using namespace oai::config;
using namespace oai::ngap;
using namespace oai::model::common;
extern itti_mw* itti_inst;
extern amf_n2* amf_n2_inst;
extern amf_n1* amf_n1_inst;
extern amf_config amf_cfg;
extern amf_app* amf_app_inst;
extern statistics stacs;

void amf_n2_task(void*);

//------------------------------------------------------------------------------
void amf_n2_task(void* args_p) {
  const task_id_t task_id = TASK_AMF_N2;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case NEW_SCTP_ASSOCIATION: {
        Logger::amf_n2().info(
            "Received new SCTP Association message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_new_sctp_association>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case NG_SETUP_REQ: {
        Logger::amf_n2().info("Received NGSetupRequest message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_ng_setup_request>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case NG_RESET: {
        Logger::amf_n2().info("Received NGReset message, handling");
        auto msg_ptr = std::dynamic_pointer_cast<itti_ng_reset>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case NG_SHUTDOWN: {
        Logger::amf_n2().info("Received SCTP Shutdown Event, handling");
        auto msg_ptr = std::dynamic_pointer_cast<itti_ng_shutdown>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case INITIAL_UE_MSG: {
        Logger::amf_n2().info("Received Initial UE Message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_initial_ue_message>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case ITTI_UL_NAS_TRANSPORT: {
        Logger::amf_n2().info(
            "Received Uplink NAS Transport message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_ul_nas_transport>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case ITTI_DL_NAS_TRANSPORT: {
        Logger::amf_n2().info(
            "Received Downlink NAS Transport message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_dl_nas_transport>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case PDU_SESSION_RESOURCE_SETUP_REQUEST: {
        Logger::amf_n2().info(
            "Received PDU Session Resource Setup Request message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_pdu_session_resource_setup_request>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case PDU_SESSION_RESOURCE_MODIFY_REQUEST: {
        Logger::amf_n2().info(
            "Received PDU Session Resource Modify Request message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_pdu_session_resource_modify_request>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case INITIAL_CONTEXT_SETUP_REQUEST: {
        Logger::amf_n2().info(
            "Received Initial Context Setup Request message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_initial_context_setup_request>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case UE_CONTEXT_RELEASE_REQUEST: {
        Logger::amf_n2().info(
            "Received UE Context Release Request message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_ue_context_release_request>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case UE_CONTEXT_RELEASE_COMMAND: {
        Logger::amf_n2().info(
            "Received UE Context Release Command message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_ue_context_release_command>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case UE_CONTEXT_RELEASE_COMPLETE: {
        Logger::amf_n2().info(
            "Received UE Context Release Complete message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_ue_context_release_complete>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case PDU_SESSION_RESOURCE_RELEASE_COMMAND: {
        Logger::amf_n2().info(
            "Received PDU Session Resource Release Command message, handling");
        auto msg_ptr = std::dynamic_pointer_cast<
            itti_pdu_session_resource_release_command>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case UE_RADIO_CAP_IND: {
        Logger::amf_n2().info(
            "Received UE Radio Capability Indication message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_ue_radio_capability_indication>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case HANDOVER_REQUIRED_MSG: {
        Logger::amf_n2().info("Received Handover Required message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_handover_required>(shared_msg);
        if (!amf_n2_inst->handle_itti_message(msg_ptr))
          amf_n2_inst->send_handover_preparation_failure(
              msg_ptr->handoverReq->getAmfUeNgapId(),
              msg_ptr->handoverReq->getRanUeNgapId(), msg_ptr->assoc_id);
      } break;

      case HANDOVER_REQUEST_ACK: {
        Logger::amf_n2().info(
            "Received Handover Request Ack message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_handover_request_ack>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case HANDOVER_NOTIFY: {
        Logger::amf_n2().info("Received Handover Notify message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_handover_notify>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case UPLINK_RAN_STATUS_TRANSFER: {
        Logger::amf_n2().info(
            "Received Uplink RAN Status Transfer message, handling");
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_uplink_ran_status_transfer>(
                shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case PAGING: {
        Logger::amf_n2().info("Received Paging message, handling");
        auto msg_ptr = std::dynamic_pointer_cast<itti_paging>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case REROUTE_NAS_REQ: {
        Logger::amf_n2().info("Received Reroute NAS Request message, handling");
        auto msg_ptr = std::dynamic_pointer_cast<itti_rereoute_nas>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT: {
        Logger::amf_n2().info(
            "Received Downlink UE Associated NRPPA Transport message, "
            "handling");
        auto msg_ptr = std::dynamic_pointer_cast<
            itti_downlink_ue_associated_nrppa_transport>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT: {
        Logger::amf_n2().info(
            "Received Downlink Non UE Associated NRPPA Transport message, "
            "handling");
        auto msg_ptr = std::dynamic_pointer_cast<
            itti_downlink_non_ue_associated_nrppa_transport>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT: {
        Logger::amf_n2().info(
            "Received Uplink UE Associated NRPPA Transport message, "
            "handling");
        auto msg_ptr = std::dynamic_pointer_cast<
            itti_uplink_ue_associated_nrppa_transport>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT: {
        Logger::amf_n2().info(
            "Received Uplink Non UE Associated NRPPA Transport message, "
            "handling");
        auto msg_ptr = std::dynamic_pointer_cast<
            itti_uplink_non_ue_associated_nrppa_transport>(shared_msg);
        amf_n2_inst->handle_itti_message(msg_ptr);
      } break;

      case TERMINATE: {
        auto msg_ptr =
            std::dynamic_pointer_cast<itti_msg_terminate>(shared_msg);
        Logger::amf_n2().info("Received Terminate message");
        return;
      } break;

      default:
        Logger::amf_n2().info("No handler for message type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
amf_n2::amf_n2(const std::string& address, const uint16_t port_num)
    : ngap_app(address, port_num), m_ranid2uecontext(), m_amfueid2uecontext() {
  if (itti_inst->create_task(TASK_AMF_N2, amf_n2_task, nullptr)) {
    Logger::amf_n2().error("Cannot create task TASK_AMF_N2");
    throw std::runtime_error("Cannot create task TASK_AMF_N2");
  }
  ranid2uecontext   = {};
  amfueid2uecontext = {};
  Logger::amf_n2().startup("amf_n2 started");
}

//------------------------------------------------------------------------------
amf_n2::~amf_n2() {}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(std::shared_ptr<itti_paging>& itti_msg) {
  Logger::amf_n2().debug("Handle Paging message...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  if (unc->amf_ue_ngap_id != itti_msg->amf_ue_ngap_id) {
    Logger::amf_n2().error(
        "The requested UE (amf_ue_ngap_id: " AMF_UE_NGAP_ID_FMT
        ") is not valid, existed UE "
        "which's amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
        itti_msg->amf_ue_ngap_id, unc->amf_ue_ngap_id);
  }

  // TODO: check UE reachability status

  // get NAS context
  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(itti_msg->amf_ue_ngap_id, nc)) {
    Logger::amf_n2().warn(
        "No existed nas_context with amf_ue_ngap_id(" AMF_UE_NGAP_ID_FMT ")",
        itti_msg->amf_ue_ngap_id);
  }
  // Network stops sending paging messages since the mobile reachable timer
  // expires
  if (amf_n1_inst->get_mobile_reachable_timer_timeout(nc)) {
    return;
  }

  PagingMsg paging_msg = {};
  Logger::amf_n2().debug(
      " UE NGAP Context, s_setid (%d), s_pointer (%d), s_tmsi (%d)",
      unc->s_setid, unc->s_pointer, unc->s_tmsi);
  paging_msg.setUePagingIdentity(unc->s_setid, unc->s_pointer, unc->s_tmsi);

  std ::vector<struct Tai_s> list;
  Tai_t tai = {};
  tai.mcc   = unc->tai.mcc;
  tai.mnc   = unc->tai.mnc;
  tai.tac   = unc->tai.tac;

  list.push_back(tai);
  paging_msg.setTaiListForPaging(list);

  uint8_t buffer[BUFFER_SIZE_512];
  int encoded_size = paging_msg.Encode(buffer, BUFFER_SIZE_512);
  bstring b        = blk2bstr(buffer, encoded_size);

  amf_n2_inst->sctp_s_38412.sctp_send_msg(
      unc->gnb_assoc_id, unc->sctp_stream_send, &b);

  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_new_sctp_association>& new_assoc) {
  // TODO:
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_ng_setup_request>& itti_msg) {
  Logger::amf_n2().debug(
      "[gNB Assoc ID %d] Handle NG Setup Request...", itti_msg->assoc_id);
  Logger::amf_n2().debug(
      "Parameters: assoc_id %d, stream %d", itti_msg->assoc_id,
      itti_msg->stream);

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "No gNB context with assoc_id (%d)", itti_msg->assoc_id);
    send_ng_setup_failure(
        Ngap_CauseProtocol_message_not_compatible_with_receiver_state,
        Ngap_TimeToWait_v5s, itti_msg->assoc_id, itti_msg->stream);
    return;
  }

  if (gc->ng_state == NGAP_RESETING || gc->ng_state == NGAP_SHUTDOWN) {
    Logger::amf_n2().warn(
        "[gNB Assoc ID %d] Received a new association request on an "
        "association that is being "
        "%s, "
        "ignoring",
        itti_msg->assoc_id, ng_gnb_state_str[gc->ng_state]);
  } else {
    Logger::amf_n2().debug(
        "Update gNB context with assoc id (%d)", itti_msg->assoc_id);
  }

  // Get IE Global RAN Node ID
  uint32_t gnb_id     = {};
  std::string gnb_mcc = {};
  std::string gnb_mnc = {};
  if (!itti_msg->ngSetupReq->getGlobalGnbId(gnb_id, gnb_mcc, gnb_mnc)) {
    Logger::amf_n2().error(
        "[gNB Assoc ID %d] Missing Mandatory IE Global RAN Node ID",
        itti_msg->assoc_id);
    send_ng_setup_failure(
        Ngap_CauseProtocol_abstract_syntax_error_falsely_constructed_message,
        Ngap_TimeToWait_v5s, itti_msg->assoc_id, itti_msg->stream);
    return;
  }
  Logger::amf_n2().debug(
      "RAN Node Info, Global RAN Node ID: 0x%x, MCC %s, MNC %s", gnb_id,
      gnb_mcc.c_str(), gnb_mnc.c_str());

  // Store GNB info in the gNB context
  gc->gnb_id   = gnb_id;
  gc->plmn.mcc = gnb_mcc;
  gc->plmn.mnc = gnb_mnc;

  std::string gnb_name = {};
  if (!itti_msg->ngSetupReq->getRanNodeName(gnb_name)) {
    Logger::amf_n2().warn("Missing IE RanNodeName");
  } else {
    gc->gnb_name = gnb_name;
    Logger::amf_n2().debug("IE RanNodeName: %s", gnb_name.c_str());
  }

  // Store Paging DRX in gNB context
  gc->default_paging_drx = itti_msg->ngSetupReq->getDefaultPagingDrx();
  Logger::amf_n2().debug("IE DefaultPagingDRX: %d", gc->default_paging_drx);

  // Get supported TA List
  std::vector<SupportedTaItem_t> supported_ta_list;
  if (!itti_msg->ngSetupReq->getSupportedTaList(supported_ta_list)) {
    Logger::amf_n2().error("Missing Mandatory IE Supported TA List");
    send_ng_setup_failure(
        Ngap_CauseProtocol_abstract_syntax_error_falsely_constructed_message,
        Ngap_TimeToWait_v5s, itti_msg->assoc_id, itti_msg->stream);
    return;
  }
  // Store UE Retention Information if available
  std::optional<UeRetentionInformation> ue_retention_info = std::nullopt;
  itti_msg->ngSetupReq->getUeRetentionInformation(ue_retention_info);
  gc->ue_retention_info = ue_retention_info;

  // Verify PLMN Identity and TAC with configuration and store supportedTAList
  // in gNB context
  if (!get_common_plmn(supported_ta_list, gc->supported_ta_list)) {
    // If there's no common PLMN between AMF and GNB, send NG SETUP FAILURE
    // MESSAGE with cause "Unknown PLMN"(Section 9.3.1.2, 3GPP TS 38.413)
    Logger::amf_n2().error(
        "[gNB ID %d] No common PLMN between gNB and AMF, encoding "
        "NG_SETUP_FAILURE with cause (Unknown PLMN)",
        gc->gnb_id);
    send_ng_setup_failure(
        Ngap_CauseMisc_unknown_PLMN_or_SNPN, Ngap_TimeToWait_v5s,
        itti_msg->assoc_id, itti_msg->stream);
    return;
  }

  set_gnb_id_2_gnb_context(gnb_id, gc);

  // Re-initialises the NGAP UE-related contexts (except if AMF agree on
  // retaining the UE contexts)
  bool ue_retention_option = false;
  if (gc->ue_retention_info.has_value()) {
    e_Ngap_UERetentionInformation ue_retention_info_e =
        gc->ue_retention_info.value().get();
    if (ue_retention_info_e == Ngap_UERetentionInformation_ues_retained) {
      ue_retention_option = true;
    }
  }
  if (!ue_retention_option) {
    // Release all the resources related to this association
    std::vector<std::shared_ptr<ue_ngap_context>> ue_contexts;
    get_ue_ngap_contexts(itti_msg->assoc_id, ue_contexts);

    for (auto ue_context : ue_contexts) {
      remove_ue_context_with_amf_ue_ngap_id(ue_context->amf_ue_ngap_id);
      remove_ue_context_with_ran_ue_ngap_id(
          ue_context->ran_ue_ngap_id, gc->gnb_id);
    }
  }

  // Send NG SETUP RESPONSE message
  Logger::amf_n2().debug("Encoding NG_SETUP_RESPONSE ...");
  auto buffer = new (std::nothrow) uint8_t[BUFFER_SIZE_1024]();
  if (buffer == nullptr) {
    Logger::amf_n2().error("Error when allocating buffer!");
    return;
  }

  NgSetupResponseMsg ngSetupResp = {};
  ngSetupResp.setAmfName(amf_cfg.amf_name);
  std::vector<GuamiItem_t> guami_list;
  for (int i = 0; i < amf_cfg.guami_list.size(); i++) {
    GuamiItem_t tmp = {};
    tmp.mcc         = amf_cfg.guami_list[i].mcc;
    tmp.mnc         = amf_cfg.guami_list[i].mnc;
    tmp.regionId    = amf_cfg.guami_list[i].region_id;
    tmp.amfSetId    = amf_cfg.guami_list[i].amf_set_id;
    tmp.amfPointer  = amf_cfg.guami_list[i].amf_pointer;
    guami_list.push_back(tmp);
  }
  ngSetupResp.setGuamiList(guami_list);

  ngSetupResp.setRelativeAmfCapacity(amf_cfg.relative_amf_capacity);
  std::vector<PlmnSliceSupport_t> plmn_list;
  for (int i = 0; i < amf_cfg.plmn_list.size(); i++) {
    PlmnSliceSupport_t tmp = {};
    tmp.mcc                = amf_cfg.plmn_list[i].mcc;
    tmp.mnc                = amf_cfg.plmn_list[i].mnc;
    for (int j = 0; j < amf_cfg.plmn_list[i].slice_list.size(); j++) {
      S_Nssai s_tmp = {};
      s_tmp.sst     = std::to_string(amf_cfg.plmn_list[i].slice_list[j].sst);
      s_tmp.sd      = amf_cfg.plmn_list[i].slice_list[j].sd;
      tmp.sliceList.push_back(s_tmp);
    }
    plmn_list.push_back(tmp);
  }

  ngSetupResp.setPlmnSupportList(plmn_list);
  if (ue_retention_info.has_value() and ue_retention_option)
    ngSetupResp.setUeRetentionInformation(ue_retention_info.value());

  int encoded = ngSetupResp.Encode(buffer, BUFFER_SIZE_1024);

  if (encoded < 1) {
    Logger::amf_n2().error("Encode NG Setup Response message error!");
    send_ng_setup_failure(
        Ngap_CauseMisc_unspecified, Ngap_TimeToWait_v5s, itti_msg->assoc_id,
        itti_msg->stream);
    return;
  }

  bstring b = blk2bstr(buffer, encoded);
  sctp_s_38412.sctp_send_msg(itti_msg->assoc_id, itti_msg->stream, &b);
  Logger::amf_n2().debug("Sending NG_SETUP_RESPONSE Ok");
  gc->ng_state = NGAP_READY;
  Logger::amf_n2().debug(
      "gNB with gNB_id 0x%x, assoc_id %d has been attached to AMF", gc->gnb_id,
      itti_msg->assoc_id);

  // Store gNB info for statistic purpose
  stacs.add_gnb(gc);

  // TODO: Do we need to store gNB context in UDSF (if available)?

  // delete[] buffer;//Free in destructor of NgapMessage
  oai::utils::utils::bdestroy_wrapper(&b);
  return;
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(std::shared_ptr<itti_ng_reset>& itti_msg) {
  Logger::amf_n2().debug("Handle NG Reset...");
  Logger::amf_n2().debug(
      "Parameters: assoc_id %d, stream %d", itti_msg->assoc_id,
      itti_msg->stream);

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "No gNB context with assoc_id (%d)", itti_msg->assoc_id);
    return;
  }

  gc->ng_state = NGAP_RESETING;
  // TODO: (8.7.4.2.2, NG Reset initiated by the NG-RAN node @3GPP TS 38.413
  // V16.0.0) the AMF shall release all allocated resources on NG related to the
  // UE association(s) indicated explicitly or implicitly in the NG RESET
  // message and remove the NGAP ID for the indicated UE associations.
  ResetType reset_type = {};
  std::vector<UeAssociatedLogicalNgConnectionItem>
      ueAssociatedLogicalNGConnectionList;
  itti_msg->ngReset->getResetType(reset_type);
  if (reset_type.getResetType() == Ngap_ResetType_PR_nG_Interface) {
    // Reset all
    // release all the resources related to this interface

    std::vector<std::shared_ptr<ue_ngap_context>> ue_contexts;
    get_ue_ngap_contexts(itti_msg->assoc_id, ue_contexts);

    for (auto ue_context : ue_contexts) {
      remove_ue_context_with_amf_ue_ngap_id(ue_context->amf_ue_ngap_id);
      remove_ue_context_with_ran_ue_ngap_id(
          ue_context->ran_ue_ngap_id, gc->gnb_id);
    }

    stacs.display();
  } else if (
      reset_type.getResetType() == Ngap_ResetType_PR_partOfNG_Interface) {
    // TODO:
    reset_type.getUeAssociatedLogicalNgConnectionList(
        ueAssociatedLogicalNGConnectionList);
    for (auto ue : ueAssociatedLogicalNGConnectionList) {
      uint64_t amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
      uint32_t ran_ue_ngap_id = 0;
      if (ue.getAmfUeNgapId(amf_ue_ngap_id)) {
        remove_ue_context_with_amf_ue_ngap_id(amf_ue_ngap_id);
      } else if (ue.getRanUeNgapId(ran_ue_ngap_id)) {
        remove_ue_context_with_ran_ue_ngap_id(ran_ue_ngap_id, gc->gnb_id);
      }
    }
  }

  // Create NGResetAck and reply to gNB
  std::unique_ptr<NgResetAckMsg> ng_reset_ack =
      std::make_unique<NgResetAckMsg>();
  // UeAssociatedLogicalNgConnectionList
  if (ueAssociatedLogicalNGConnectionList.size() > 0) {
    ng_reset_ack->setUeAssociatedLogicalNgConnectionList(
        ueAssociatedLogicalNGConnectionList);
  }

  uint8_t buffer[BUFFER_SIZE_512];
  int encoded_size = ng_reset_ack->Encode(buffer, BUFFER_SIZE_512);

  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, itti_msg->stream, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
  return;
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(std::shared_ptr<itti_ng_shutdown>& itti_msg) {
  Logger::amf_n2().debug("Handle NG Shutdown ...");
  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "No gNB context with assoc_id (%d)", itti_msg->assoc_id);
    return;
  }

  gc->ng_state = NGAP_SHUTDOWN;

  // Release all the resources related to this interface
  std::vector<std::shared_ptr<ue_ngap_context>> ue_contexts;
  get_ue_ngap_contexts(itti_msg->assoc_id, ue_contexts);

  // Release all PDUs session for the associated UEs
  for (auto context : ue_contexts) {
    // Get UE Context
    std::string ue_context_key = amf_conv::get_ue_context_key(
        context->ran_ue_ngap_id, context->amf_ue_ngap_id);
    std::shared_ptr<ue_context> uc = {};

    if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) {
      continue;
    }
    amf_app_inst->trigger_pdu_session_up_deactivation(uc);
  }

  // Remove UEs' contexts
  for (auto ue_context : ue_contexts) {
    remove_ue_context_with_amf_ue_ngap_id(ue_context->amf_ue_ngap_id);
    remove_ue_context_with_ran_ue_ngap_id(
        ue_context->ran_ue_ngap_id, gc->gnb_id);
  }

  // Delete gNB context and update statistic
  remove_gnb_context(itti_msg->assoc_id);
  stacs.update_gnb(gc, kStatisticGnbStatusDisconnected);

  Logger::amf_n2().debug(
      "Remove gNB with association id %d, gnb_id 0x%x", itti_msg->assoc_id,
      gc->gnb_id);
  stacs.display();
  return;
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_initial_ue_message>& init_ue_msg) {
  Logger::amf_n2().debug("Handle Initial UE Message...");

  // Create NGAP-UE context and store in gNB context to store UE information in
  // gNB, for example, here RAN UE NGAP ID and location information and RRC
  // Establishment Cause send NAS-PDU to NAS layer Get INITIAL_UE_MESSAGE IEs

  // Check the gNB context on which  this UE is attached with assoc_id
  std::shared_ptr<itti_nas_signalling_establishment_request> itti_msg =
      std::make_shared<itti_nas_signalling_establishment_request>(
          TASK_AMF_N2, TASK_AMF_APP);

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(init_ue_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", init_ue_msg->assoc_id);
    return;
  }

  if (gc->ng_state == NGAP_RESETING || gc->ng_state == NGAP_SHUTDOWN) {
    Logger::amf_n2().warn(
        "Received new association request on an association that is being %s, "
        "ignoring",
        ng_gnb_state_str[gc->ng_state]);
  } else if (gc->ng_state != NGAP_READY) {
    Logger::amf_n2().debug(
        "gNB with assoc_id (%d) is illegal", init_ue_msg->assoc_id);
    return;
  }

  if (!init_ue_msg->initUeMsg) return;

  // UE NGAP Context
  uint32_t ran_ue_ngap_id = 0;
  if (!init_ue_msg->initUeMsg->getRanUENgapID(ran_ue_ngap_id)) {
    Logger::amf_n2().error("Missing Mandatory IE (RanUeNgapId)");
    return;
  }

  std::shared_ptr<ue_ngap_context> unc = {};
  if (!ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id, gc->gnb_id, unc)) {
    Logger::amf_n2().debug(
        "Create a new UE NGAP context with ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT,
        ran_ue_ngap_id);
    unc = std::shared_ptr<ue_ngap_context>(new ue_ngap_context());
    unc.get()->sctp_stream_recv = init_ue_msg->stream;
    unc.get()->sctp_stream_send = init_ue_msg->stream;
    set_ran_ue_ngap_id_2_ue_ngap_context(ran_ue_ngap_id, gc->gnb_id, unc);
  }

  // Store related information into UE NGAP context
  unc->ran_ue_ngap_id   = ran_ue_ngap_id;
  unc->sctp_stream_recv = init_ue_msg->stream;
  unc->sctp_stream_send == gc->next_sctp_stream;
  gc->next_sctp_stream += 1;
  if (gc->next_sctp_stream >= gc->instreams) gc->next_sctp_stream = 1;
  unc->gnb_assoc_id = init_ue_msg->assoc_id;

  // User Location Info NR (Mandatory)
  NrCgi_t cgi = {};
  Tai_t tai   = {};
  if (init_ue_msg->initUeMsg->getUserLocationInfoNr(cgi, tai)) {
    itti_msg->cgi = cgi;
    itti_msg->tai = tai;
    unc->tai      = tai;
  } else {
    Logger::amf_n2().error("Missing Mandatory IE UserLocationInfoNR");
    return;
  }

  // RCC Establishment Cause (Mandatory)
  itti_msg->rrc_cause = init_ue_msg->initUeMsg->getRrcEstablishmentCause();

  // UE Context Request (Optional)
  // TODO: use std::optional
  if (init_ue_msg->initUeMsg->getUeContextRequest() == -1) {
    Logger::amf_n2().warn("IE UeContextRequest not present");
    itti_msg->ueCtxReq = -1;  // not present, TODO with optional
  } else {
    itti_msg->ueCtxReq = init_ue_msg->initUeMsg->getUeContextRequest();
  }

  // 5G-S-TMSI (Optional)
  std::string _5g_s_tmsi = {};
  if (!init_ue_msg->initUeMsg->get5GSTmsi(_5g_s_tmsi)) {
    itti_msg->is_5g_s_tmsi_present = false;
    Logger::amf_n2().debug("5g_s_tmsi not present");
  } else {
    itti_msg->is_5g_s_tmsi_present = true;
    itti_msg->_5g_s_tmsi           = _5g_s_tmsi;
    Logger::amf_n2().debug("5g_s_tmsi present: %s", _5g_s_tmsi);
    init_ue_msg->initUeMsg->get5GSTmsi(
        unc->s_setid, unc->s_pointer, unc->s_tmsi);
  }

  // NAS PDU (Mandatory)
  if (!init_ue_msg->initUeMsg->getNasPdu(itti_msg->nas_buf)) {
    Logger::amf_n2().error("Missing mandatory IE NAS-PDU");
    return;
  }

  // Store InitialUEMessage for Rereoute NAS later
  if (unc->initial_ue_msg.buf) {
    Logger::amf_n2().debug(
        "Store InitialUEMessage for Reroute NAS (if necessary)");
    auto initial_ue_msg_buf = new (std::nothrow) uint8_t[BUFFER_SIZE_1024]();
    if (initial_ue_msg_buf == nullptr) {
      Logger::amf_n2().error("Error when allocating buffer!");
      return;
    }

    int encoded_size = 0;
    init_ue_msg->initUeMsg->Encode(initial_ue_msg_buf, encoded_size);

    if (encoded_size > 0) {
      Logger::amf_n2().debug("Encoded InitialUEMessage size %d", encoded_size);
      memcpy(
          (void*) unc->initial_ue_msg.buf, (void*) initial_ue_msg_buf,
          encoded_size);
      oai::utils::output_wrapper::print_buffer(
          "ngap", "InitialUEMessage", unc->initial_ue_msg.buf, encoded_size);
      unc->initial_ue_msg.size = encoded_size;
    }
    // delete[] initial_ue_msg_buf;//Free in destructor of NgapMessage
  }

  itti_msg->gnb_id         = gc->gnb_id;
  itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n2().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_ul_nas_transport>& ul_nas_transport) {
  Logger::amf_n2().debug("Handle Uplink NAS Transport...");

  uint64_t amf_ue_ngap_id         = ul_nas_transport->ulNas->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id         = ul_nas_transport->ulNas->getRanUeNgapId();
  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(ul_nas_transport->assoc_id, gc)) {
    Logger::amf_n2().error(
        "gNB with assoc_id (%d) is illegal", ul_nas_transport->assoc_id);
    return;
  }

  std::shared_ptr<ue_ngap_context> unc = {};
  if (!ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id, gc->gnb_id, unc)) return;

  if (unc->amf_ue_ngap_id != amf_ue_ngap_id) {
    Logger::amf_n2().error(
        "The requested UE (amf_ue_ngap_id: " AMF_UE_NGAP_ID_FMT
        ") is not valid, existed UE "
        "which's amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
        amf_ue_ngap_id, unc->amf_ue_ngap_id);
  }
  /*
     //TODO: check with a correct NGAP state
     if (unc->ng_ue_state != NGAP_UE_CONNECTED) {
      Logger::amf_n2().error(
          "Received NGAP UPLINK_NAS_TRANSPORT while UE in state != "
          "NGAP_UE_CONNECTED");
      // return;
    }
    */

  auto itti_msg =
      std::make_shared<itti_uplink_nas_data_ind>(TASK_AMF_N2, TASK_AMF_N1);

  itti_msg->is_nas_signalling_estab_req = false;
  itti_msg->amf_ue_ngap_id              = amf_ue_ngap_id;
  itti_msg->ran_ue_ngap_id              = ran_ue_ngap_id;
  itti_msg->is_guti_valid               = false;

  if (!ul_nas_transport->ulNas->getNasPdu(itti_msg->nas_msg)) {
    Logger::amf_n2().error("Missing IE NAS-PDU");
    return;
  }

  // UserLocation
  NrCgi_t cgi = {};
  Tai_t tai   = {};
  if (ul_nas_transport->ulNas->getUserLocationInfoNr(cgi, tai)) {
    itti_msg->mcc = cgi.mcc;
    itti_msg->mnc = cgi.mnc;
  } else {
    Logger::amf_n2().debug("Missing IE UserLocationInformationNR");
  }

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n2().error(
        "Could not send ITTI message %s to task TASK_AMF_N1",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_dl_nas_transport>& dl_nas_transport) {
  Logger::amf_n2().debug("Handle DL NAS Transport ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      dl_nas_transport->ran_ue_ngap_id, dl_nas_transport->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          dl_nas_transport->ran_ue_ngap_id, ue_context_key, unc))
    return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  unc->amf_ue_ngap_id = dl_nas_transport->amf_ue_ngap_id;
  set_amf_ue_ngap_id_2_ue_ngap_context(unc->amf_ue_ngap_id, unc);
  unc->ng_ue_state = NGAP_UE_CONNECTED;
  auto ngap_msg    = std::make_unique<DownLinkNasTransportMsg>();
  ngap_msg->setAmfUeNgapId(dl_nas_transport->amf_ue_ngap_id);
  ngap_msg->setRanUeNgapId(dl_nas_transport->ran_ue_ngap_id);
  ngap_msg->setNasPdu(dl_nas_transport->nas);

  uint8_t buffer[BUFFER_SIZE_1024];
  int encoded_size = ngap_msg->Encode(buffer, BUFFER_SIZE_1024);
  bstring b        = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, unc->sctp_stream_send, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_initial_context_setup_request>& itti_msg) {
  Logger::amf_n2().debug("Handle Initial Context Setup Request ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  unc->ncc                        = 1;
  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(itti_msg->amf_ue_ngap_id, nc))
    return;

  auto msg = std::make_unique<InitialContextSetupRequestMsg>();
  msg->setAmfUeNgapId(itti_msg->amf_ue_ngap_id);
  msg->setRanUeNgapId(itti_msg->ran_ue_ngap_id);
  guami_full_format_t guami = {};
  guami.mcc                 = amf_cfg.guami.mcc;
  guami.mnc                 = amf_cfg.guami.mnc;
  guami.region_id           = amf_cfg.guami.region_id;
  guami.amf_set_id          = amf_cfg.guami.amf_set_id;
  guami.amf_pointer         = amf_cfg.guami.amf_pointer;
  msg->setGuami(guami);
  // Get the list allowed NSSAI from the common PLMN between gNB and AMF
  std::vector<S_Nssai> list;
  for (auto s : gc->supported_ta_list) {
    for (auto p : s.plmnSliceSupportList) {
      for (auto s : p.sliceList) {
        S_Nssai item;
        item.sst = s.sst;
        item.sd  = s.sd;
        list.push_back(item);
      }
    }
  }
  msg->setAllowedNssai(list);

  // Set UE Security Capability from NAS context
  uint8_t eea_value = 0x0000;
  nc->ue_security_capability.GetEea(eea_value);
  uint8_t eia_value = 0x0000;
  nc->ue_security_capability.GetEia(eia_value);
  msg->setUeSecurityCapability(
      (nc->ue_security_capability.GetEa() << 9),
      (nc->ue_security_capability.GetIa() << 9), (eea_value << 9),
      (eia_value << 9));

  // Security Key
  msg->setSecurityKey(
      (uint8_t*) bdata(itti_msg->kgnb), AUTH_VECTOR_LENGTH_OCTETS);

  // Mobility Restriction List
  oai::ngap::PlmnId plmn_id = {};
  plmn_id.set(amf_cfg.guami.mcc, amf_cfg.guami.mnc);
  msg->setMobilityRestrictionList(plmn_id);

  // IMEISV
  if (nc->imeisv.has_value()) {
    Logger::nas_mm().debug(
        "Set IMEISV InitialContextSetupRequestMsg: %s",
        nc->imeisv.value().identity.c_str());
    msg->setMaskedImeisv(nc->imeisv.value().identity);
  } else {
    Logger::nas_mm().debug("No IMEISV info available");
  }

  msg->setNasPdu(itti_msg->nas);

  if (itti_msg->is_sr or !itti_msg->pdu_sessions.empty()) {
    // Set UE Radio Capability if available
    if (unc->ue_radio_cap_ind) {
      // TODO: Disable this for the moment
      // msg->setUeRadioCapability(bstrcpy(unc->ue_radio_cap_ind));
    }

    if (itti_msg->is_sr)
      Logger::amf_n2().debug("Encoding parameters for Service Accept");
    else
      Logger::amf_n2().debug(
          "Encoding parameters for Initial Context Setup Request");

    if (!itti_msg->pdu_sessions.empty()) {
      std::vector<PDUSessionResourceSetupRequestItem_t> list;
      for (auto& p : itti_msg->pdu_sessions) {
        PDUSessionResourceSetupRequestItem_t item = {};
        item.pduSessionId                         = p.first;

        // Get NSSAI from PDU Session Context
        std::string supi = amf_conv::imsi_to_supi(nc->imsi);
        Logger::amf_n2().debug("SUPI %s", supi.c_str());

        // Get S_NSSAI from PDU Session Context
        std::shared_ptr<pdu_session_context> psc = {};

        if (!amf_app_inst->find_pdu_session_context(supi, p.first, psc)) {
          item.sNssai.sst = std::to_string(DEFAULT_SST);
          item.sNssai.sd  = std::to_string(SD_NO_VALUE);
        } else {
          item.sNssai.sst = std::to_string(psc->snssai.sst);
          item.sNssai.sd  = psc->snssai.sd;
        }

        Logger::amf_n2().debug(
            "S_NSSAI (SST, SD) %s, %s", item.sNssai.sst.c_str(),
            item.sNssai.sd.c_str());

        // TODO: NAS PDU
        if (p.second.is_n2sm_available) {
          if (blength(p.second.n2sm) != 0) {
            ngap_utils::bstring_2_octet_string(
                p.second.n2sm, item.pduSessionResourceSetupRequestTransfer);
            list.push_back(item);
          } else {
            Logger::amf_n2().error("n2sm empty!");
          }
        }
      }

      if (list.size() > 0) msg->setPduSessionResourceSetupRequestList(list);

      // UEAggregateMaximumBitRate
      msg->setUeAggregateMaxBitRate(
          UE_AGGREGATE_MAXIMUM_BIT_RATE_DL, UE_AGGREGATE_MAXIMUM_BIT_RATE_UL);

      // TODO: Mobility RestrictionList
    }
  }

  uint8_t buffer[BUFFER_SIZE_2048];
  int encoded_size = msg->Encode(buffer, BUFFER_SIZE_2048);
  bstring b        = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, unc->sctp_stream_send, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_pdu_session_resource_setup_request>& itti_msg) {
  Logger::amf_n2().debug("Handle PDU Session Resource Setup Request ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(itti_msg->amf_ue_ngap_id, nc)) {
    Logger::amf_n2().error(
        "No existing NAS context with amf_ue_ngap_id(" AMF_UE_NGAP_ID_FMT ")",
        itti_msg->amf_ue_ngap_id);
    return;
  }
  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  Logger::amf_n2().debug("SUPI (%s)", supi.c_str());

  auto psrsr = std::make_unique<PduSessionResourceSetupRequestMsg>();
  psrsr->setAmfUeNgapId(itti_msg->amf_ue_ngap_id);
  psrsr->setRanUeNgapId(itti_msg->ran_ue_ngap_id);

  if (!itti_msg->pdu_sessions.empty()) {
    std::vector<PDUSessionResourceSetupRequestItem_t> list;
    for (auto& p : itti_msg->pdu_sessions) {
      PDUSessionResourceSetupRequestItem_t item = {};
      item.pduSessionId                         = p.first;
      item.nasPdu                               = bstrcpy(itti_msg->nas);

      // Get SNSSAI info from PDU Session Context
      item.sNssai.sd                           = {};
      std::shared_ptr<pdu_session_context> psc = {};
      if (!amf_app_inst->find_pdu_session_context(supi, p.first, psc)) {
        // TODO: get from N1N2msgTranferMsg
        Logger::amf_n2().debug(
            "Using default value for S_NSSAI (SST, SD) %s, %s",
            item.sNssai.sst.c_str(), item.sNssai.sd.c_str());
        item.sNssai.sst = std::to_string(DEFAULT_SST);
        item.sNssai.sd  = std::to_string(SD_NO_VALUE);
      } else {
        item.sNssai.sst = std::to_string(psc->snssai.sst);
        item.sNssai.sd  = psc->snssai.sd;
        Logger::amf_n2().debug(
            "S_NSSAI (SST, SD) %s, %s", item.sNssai.sst.c_str(),
            item.sNssai.sd.c_str());
      }

      if (p.second.is_n2sm_available) {
        if (blength(p.second.n2sm) != 0) {
          ngap_utils::bstring_2_octet_string(
              p.second.n2sm, item.pduSessionResourceSetupRequestTransfer);
          list.push_back(item);
        } else {
          Logger::amf_n2().error("n2sm empty!");
        }
      }
    }

    if (list.size() > 0) psrsr->setPduSessionResourceSetupRequestList(list);
  }

  psrsr->setUeAggregateMaxBitRate(
      UE_AGGREGATE_MAXIMUM_BIT_RATE_DL, UE_AGGREGATE_MAXIMUM_BIT_RATE_UL);

  auto buffer = new (std::nothrow) uint8_t[BUFFER_SIZE_4096]();
  if (buffer == nullptr) {
    Logger::amf_n2().error("Error when allocating buffer!");
    return;
  }

  int encoded_size = 0;
  psrsr->encode2NewBuffer(buffer, encoded_size);
  oai::utils::output_wrapper::print_buffer(
      "amf_n2", "N2 SM buffer data:", buffer, encoded_size);
  Logger::amf_n2().debug(" (%d bytes) \n", encoded_size);

  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, unc->sctp_stream_send, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
  // delete[] buffer;//Free in destructor of NgapMessage
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_pdu_session_resource_modify_request>& itti_msg) {
  Logger::amf_n2().debug("Handle PDU Session Resource Modify Request ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  auto modify_request_msg =
      std::make_unique<PduSessionResourceModifyRequestMsg>();

  modify_request_msg->setAmfUeNgapId(itti_msg->amf_ue_ngap_id);
  modify_request_msg->setRanUeNgapId(itti_msg->ran_ue_ngap_id);

  std::vector<PDUSessionResourceModifyRequestItem_t> list;
  PDUSessionResourceModifyRequestItem_t item = {};
  item.pduSessionId                          = itti_msg->pdu_session_id;

  ngap_utils::bstring_2_octet_string(
      itti_msg->n2sm, item.pduSessionResourceModifyRequestTransfer);
  S_Nssai tmp = {};
  tmp.sd      = itti_msg->s_NSSAI.getSd();
  tmp.sst     = itti_msg->s_NSSAI.getSst();
  item.sNssai = std::optional<S_Nssai>(tmp);

  item.nasPdu = bstrcpy(itti_msg->nas);
  list.push_back(item);

  modify_request_msg->setPduSessionResourceModifyRequestList(list);

  auto buffer = new (std::nothrow) uint8_t[BUFFER_SIZE_4096]();
  if (buffer == nullptr) {
    Logger::amf_n2().error("Error when allocating buffer!");
    return;
  }

  int encoded_size = 0;
  modify_request_msg->encode2NewBuffer(buffer, encoded_size);
  oai::utils::output_wrapper::print_buffer(
      "amf_n2", "N2 SM buffer data:", buffer, encoded_size);
  Logger::amf_n2().debug(" (%d bytes) \n", encoded_size);

  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, unc->sctp_stream_send, &b);
  // free memory
  oai::utils::utils::bdestroy_wrapper(&b);
  // delete[] buffer;//Free in destructor of NgapMessage
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_pdu_session_resource_release_command>& itti_msg) {
  Logger::amf_n2().debug("Handle PDU Session Resource Release Command ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  auto release_cmd_msg =
      std::make_unique<PduSessionResourceReleaseCommandMsg>();

  release_cmd_msg->setAmfUeNgapId(itti_msg->amf_ue_ngap_id);
  release_cmd_msg->setRanUeNgapId(itti_msg->ran_ue_ngap_id);
  release_cmd_msg->setNasPdu(itti_msg->nas);

  std::vector<PDUSessionResourceToReleaseItem_t> list;
  PDUSessionResourceToReleaseItem_t item = {};
  item.pduSessionId                      = itti_msg->pdu_session_id;

  ngap_utils::bstring_2_octet_string(
      itti_msg->n2sm, item.pduSessionResourceReleaseCommandTransfer);
  list.push_back(item);
  release_cmd_msg->setPduSessionResourceToReleaseList(list);

  auto buffer = new (std::nothrow) uint8_t[BUFFER_SIZE_4096]();
  if (buffer == nullptr) {
    Logger::amf_n2().error("Error when allocating buffer!");
    return;
  }

  int encoded_size = 0;
  release_cmd_msg->encode2NewBuffer(buffer, encoded_size);
  oai::utils::output_wrapper::print_buffer(
      "amf_n2", "N2 SM buffer data:", buffer, encoded_size);
  Logger::amf_n2().debug(" (%d bytes) \n", encoded_size);

  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, unc->sctp_stream_send, &b);
  // free memory
  oai::utils::utils::bdestroy_wrapper(&b);
  // delete[] buffer;//Free in destructor of NgapMessage
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_ue_context_release_request>& itti_msg) {
  Logger::amf_n2().debug("Handle UE Context Release Request ...");

  uint64_t amf_ue_ngap_id = itti_msg->ueCtxRel->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id = itti_msg->ueCtxRel->getRanUeNgapId();

  // Store the list of PDU Session ID to be released/deactivated
  std::shared_ptr<ue_ngap_context> unc = {};
  if (amf_ue_id_2_ue_ngap_context(amf_ue_ngap_id, unc)) {
    unc->pdu_sessions_to_be_released.clear();
    PduSessionResourceListCxtRelReq pdu_session_resource_list_cxt_rel_req = {};
    if (itti_msg->ueCtxRel->getPduSessionResourceList(
            pdu_session_resource_list_cxt_rel_req)) {
      std::vector<PduSessionResourceItemCxtRelReq> ctx_rel_req_list;
      pdu_session_resource_list_cxt_rel_req.get(ctx_rel_req_list);
      if (ctx_rel_req_list.size() > 0) {
        for (const auto& cxt_rel_req : ctx_rel_req_list) {
          OCTET_STRING_t pdu_session_resource_transfer = {};
          unc->pdu_sessions_to_be_released.emplace(
              cxt_rel_req.get().get(), pdu_session_resource_transfer);
        }
      }
    }
  }

  e_Ngap_CauseRadioNetwork cause = {};
  itti_msg->ueCtxRel->getCauseRadioNetwork(cause);
  auto ueCtxRelCmd = std::make_unique<UeContextReleaseCommandMsg>();
  ueCtxRelCmd->setUeNgapIdPair(amf_ue_ngap_id, ran_ue_ngap_id);
  ueCtxRelCmd->setCauseRadioNetwork(cause);

  uint8_t buffer[BUFFER_SIZE_512];
  int encoded_size = ueCtxRelCmd->Encode(buffer, BUFFER_SIZE_512);
  bstring b        = blk2bstr(buffer, encoded_size);

  sctp_s_38412.sctp_send_msg(itti_msg->assoc_id, itti_msg->stream, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_ue_context_release_command>& itti_msg) {
  Logger::amf_n2().debug("Handling UE Context Release Command ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  auto ueCtxRelCmd = std::make_unique<UeContextReleaseCommandMsg>();
  ueCtxRelCmd->setUeNgapIdPair(
      itti_msg->amf_ue_ngap_id, itti_msg->ran_ue_ngap_id);

  if (itti_msg->cause.getChoiceOfCause() == Ngap_Cause_PR_nas) {
    ueCtxRelCmd->setCauseNas((e_Ngap_CauseNas) itti_msg->cause.get());
  }

  if (itti_msg->cause.getChoiceOfCause() == Ngap_Cause_PR_radioNetwork) {
    ueCtxRelCmd->setCauseRadioNetwork(
        (e_Ngap_CauseRadioNetwork) itti_msg->cause.get());
  }

  uint8_t buffer[BUFFER_SIZE_256];
  int encoded_size = ueCtxRelCmd->Encode(buffer, BUFFER_SIZE_256);

  bstring b = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, unc->sctp_stream_send, &b);
  oai::utils::utils::bdestroy_wrapper(&b);

  // Send ITTI to N11 SBI, notify CommunicationFailure Report, RAN Cause
  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(itti_msg->amf_ue_ngap_id, nc)) {
    Logger::amf_n2().warn(
        "Could not notify RAN caused CommunicationFailure."
        "No existing nas_context with amf_ue_ngap_id(" AMF_UE_NGAP_ID_FMT ")",
        itti_msg->amf_ue_ngap_id);
    return;
  }
  std::string supi = amf_conv::imsi_to_supi(nc->imsi);

  Logger::amf_n2().debug(
      "Send request to SBI to trigger UE Communication Failure Report (SUPI "
      "%s )",
      supi.c_str());
  std::vector<std::shared_ptr<amf_subscription>> subscriptions = {};
  amf_app_inst->get_ee_subscriptions(
      amf_event_type_t::COMMUNICATION_FAILURE_REPORT, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed event
    Logger::amf_n2().debug(
        "Send ITTI msg to AMF SBI to trigger the event notification");

    auto itti_msg_ev = std::make_shared<itti_sbi_notify_subscribed_event>(
        TASK_AMF_N2, TASK_AMF_SBI);

    itti_msg_ev->http_version = amf_cfg.support_features.http_version;

    for (auto i : subscriptions) {
      // Avoid repeated notifications
      // TODO: use the anyUE field from the subscription request
      if (i->supi_is_set && std::strcmp(i->supi.c_str(), supi.c_str()))
        continue;

      event_notification ev_notif = {};
      ev_notif.set_notify_correlation_id(i->notify_correlation_id);
      ev_notif.set_notify_uri(i->notify_uri);  // Direct subscription
      // ev_notif.set_subs_change_notify_correlation_id(i->notify_uri);

      oai::model::amf::AmfEventReport event_report = {};
      oai::model::amf::AmfEventType amf_event_type = {};
      amf_event_type.set_value("COMMUNICATION_FAILURE_REPORT");
      event_report.setType(amf_event_type);

      oai::model::amf::AmfEventState amf_event_state = {};
      amf_event_state.setActive(true);
      event_report.setState(amf_event_state);

      oai::model::amf::CommunicationFailure comm_failure = {};

      NgApCause ngap_cause = {};
      ngap_cause.setGroup(itti_msg->cause.getChoiceOfCause());
      ngap_cause.setValue(itti_msg->cause.get());
      comm_failure.setRanReleaseCode(ngap_cause);

      event_report.setCommFailure(comm_failure);

      event_report.setSupi(supi);
      ev_notif.add_report(event_report);

      itti_msg_ev->event_notifs.push_back(ev_notif);
    }

    int ret = itti_inst->send_msg(itti_msg_ev);
    if (0 != ret) {
      Logger::amf_n2().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg_ev->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_ue_context_release_complete>& itti_msg) {
  Logger::amf_n2().debug("Handle UE Context Release Complete ...");
  uint64_t amf_ue_ngap_id = itti_msg->ueCtxRelCmpl->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id = itti_msg->ueCtxRelCmpl->getRanUeNgapId();

  Logger::amf_n2().debug(
      "UE Context Release Complete ran_ue_ngap_id (" GNB_UE_NGAP_ID_FMT
      ") amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
      ran_ue_ngap_id, amf_ue_ngap_id);

  // Get UE Context
  std::string ue_context_key =
      amf_conv::get_ue_context_key(ran_ue_ngap_id, amf_ue_ngap_id);
  std::shared_ptr<ue_context> uc = {};

  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) return;

  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_ue_id_2_ue_ngap_context(amf_ue_ngap_id, unc)) return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "gNB with assoc_id (%d) is illegal", itti_msg->assoc_id);
    return;
  }

  // verify release cause -> if HandoverSuccessful no further operations
  // required
  Logger::amf_n2().debug(
      "Release cause %d No UE NGAP context with gnb_assoc_id " GNB_ID_FMT
      ", Release gnb_assoc_id " GNB_ID_FMT "",
      unc->release_cause, itti_msg->assoc_id, unc->release_gnb);

  if (unc->release_cause == Ngap_CauseRadioNetwork_successful_handover &&
      itti_msg->assoc_id == unc->release_gnb) {
    remove_ran_ue_ngap_id_2_ngap_context(ran_ue_ngap_id, gc->gnb_id);
    unc->release_cause = 0;
    return;
  }

  Logger::amf_n2().debug("Continue with UE Context Release Complete procedure");

  // Change UE status from CM-CONNECTED to CM-IDLE
  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
    Logger::amf_n2().warn(
        "No existed nas_context with amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
        amf_ue_ngap_id);
  }

  if (nc != nullptr) {
    // Get SUPI
    std::string supi = amf_conv::imsi_to_supi(nc->imsi);
    // Get the current AMF UE NGAP ID and compare with the one from
    // UEContextReleaseComplete
    uint64_t current_amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    amf_n1_inst->supi_2_amf_id(supi, current_amf_ue_ngap_id);
    if (current_amf_ue_ngap_id != amf_ue_ngap_id) {
      // Remove UE NGAP context
      Logger::amf_n2().debug("UE Context Release Complete for the old context");
      remove_amf_ue_ngap_id_2_ue_ngap_context(amf_ue_ngap_id);
      remove_ran_ue_ngap_id_2_ngap_context(ran_ue_ngap_id, gc->gnb_id);
      return;
    } else {
      amf_n1_inst->set_5gcm_state(nc, CM_IDLE);
      // Start/reset the Mobile Reachable Timer
      timer_id_t tid = itti_inst->timer_setup(
          kMobileReachableTimerNoEmergencyServicesMin * 60, 0, TASK_AMF_N1,
          TASK_AMF_MOBILE_REACHABLE_TIMER_EXPIRE,
          std::to_string(amf_ue_ngap_id));
      Logger::amf_n2().startup("Started mobile reachable timer (tid %d)", tid);

      amf_n1_inst->set_mobile_reachable_timer(nc, tid);
      amf_n1_inst->set_mobile_reachable_timer_timeout(nc, false);
    }
  } else {
    return;
  }

  // TODO: User Location Information IE
  // TODO: Information on Recommended Cells & RAN Nodes for Paging IE

  // TODO: Process Secondary RAT Usage Information IE if available

  // Get PDU Sessions in UE Context Release Complete and send to SMF
  // Note: According to 3GPP, this steps should be executed after receiving UE
  // Context Release Request. If this is the case, we won't have PDU Session
  // Resource Release Response Transfer to be forwarded to SMF. That's why we do
  // it here!
  std::vector<PDUSessionResourceCxtRelCplItem_t> pdu_sessions_to_be_released;
  itti_msg->ueCtxRelCmpl->getPduSessionResourceCxtRelCplList(
      pdu_sessions_to_be_released);

  // Get info from UE Context Release Request if neccessary
  if (pdu_sessions_to_be_released.size() == 0) {
    PDUSessionResourceCxtRelCplItem_t item = {};
    for (const auto& p : unc->pdu_sessions_to_be_released) {
      item.pduSessionId = p.first;
      pdu_sessions_to_be_released.push_back(item);
    }
  }

  // TODO: may consider releasing all exisiting PDU sessions
  /*
  if (pdu_sessions_to_be_released.size() == 0) {
      string supi = amf_conv::imsi_to_supi(nc->imsi);
    std::vector<std::shared_ptr<pdu_session_context>> sessions_ctx;
    if (!amf_app_inst->get_pdu_sessions_context(supi, sessions_ctx)) {
      Logger::amf_n2().debug("No PDU Session Context found");
      return;
    } else {
      for (auto pdu_session : sessions_ctx) {
        PDUSessionResourceCxtRelCplItem_t item = {};
        item.pduSessionId = pdu_session->pdu_session_id;
        pdu_sessions_to_be_released.push_back(item);
      }
    }
  }
*/
  // Send PDUSessionUpdateSMContextRequest to SMF for each PDU session
  std::map<uint32_t, boost::shared_future<nlohmann::json>> curl_responses;

  for (auto pdu_session : pdu_sessions_to_be_released) {
    Logger::amf_n2().debug(
        "Trigger PDU Ssesion Deactivation, PDU Session ID %d",
        pdu_session.pduSessionId);
    // Generate a promise and associate this promise to the curl handle
    uint32_t promise_id = amf_app_inst->generate_promise_id();
    Logger::amf_n2().debug("Promise ID generated %d", promise_id);

    boost::shared_ptr<boost::promise<nlohmann::json>> p =
        boost::make_shared<boost::promise<nlohmann::json>>();
    boost::shared_future<nlohmann::json> f = p->get_future();

    // Store the future to be processed later
    amf_app_inst->add_promise(promise_id, p);
    curl_responses.emplace(pdu_session.pduSessionId, f);

    Logger::amf_n2().debug(
        "Sending ITTI to trigger PDUSessionUpdateSMContextRequest to SMF to "
        "task TASK_AMF_SBI");

    auto itti_n11_msg =
        std::make_shared<itti_nsmf_pdusession_update_sm_context>(
            TASK_NGAP, TASK_AMF_SBI);
    itti_n11_msg->pdu_session_id = pdu_session.pduSessionId;
    itti_n11_msg->is_n2sm_set =
        false;  // Enable to include PDU Session Resource Release Response
                // Transfer to SMF
    itti_n11_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_n11_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_n11_msg->promise_id     = promise_id;
    itti_n11_msg->up_cnx_state   = "DEACTIVATED";

    int ret = itti_inst->send_msg(itti_n11_msg);
    if (0 != ret) {
      Logger::ngap().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_n11_msg->get_msg_name());
    }
  }

  bool is_up_activated = true;
  while (!curl_responses.empty()) {
    // Wait for the result available and process accordingly
    std::optional<nlohmann::json> result_opt = std::nullopt;
    oai::utils::utils::wait_for_result(
        curl_responses.begin()->second, result_opt);

    if (result_opt.has_value()) {
      nlohmann::json result_json = result_opt.value();
      Logger::amf_server().debug(
          "Got result for PDU Session Id %d", curl_responses.begin()->first);
      uint32_t http_response_code = 0;
      if (result_json.find(kSbiResponseHttpResponseCode) != result_json.end()) {
        http_response_code =
            result_json[kSbiResponseHttpResponseCode].get<int>();
        if ((http_response_code == 200) or (http_response_code == 204)) {
          uc->set_up_cnx_state(
              curl_responses.begin()->first,
              up_cnx_state_e::UPCNX_STATE_DEACTIVATED);
        }
      } else {
        is_up_activated = false;
        Logger::ngap().warn("Couldn't get the HTTP response code");
      }
    } else {
      is_up_activated = false;
      Logger::ngap().warn("Couldn't get the HTTP response");
    }

    curl_responses.erase(curl_responses.begin());
  }

  // Remove UE NGAP context
  remove_amf_ue_ngap_id_2_ue_ngap_context(amf_ue_ngap_id);
  remove_ran_ue_ngap_id_2_ngap_context(ran_ue_ngap_id, gc->gnb_id);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_ue_radio_capability_indication>& itti_msg) {
  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "No gNB context with assoc_id (%d)", itti_msg->assoc_id);
    return;
  }

  uint64_t amf_ue_ngap_id = itti_msg->ueRadioCap->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id = itti_msg->ueRadioCap->getRanUeNgapId();
  OCTET_STRING_t ue_radio_cap;
  itti_msg->ueRadioCap->getUeRadioCapability(ue_radio_cap);

  // Store UE Radio Capability in UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  if (!ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id, gc->gnb_id, unc)) return;

  unc->ue_radio_cap_ind = blk2bstr(ue_radio_cap.buf, ue_radio_cap.size);
}

//------------------------------------------------------------------------------
bool amf_n2::handle_itti_message(
    std::shared_ptr<itti_handover_required>& itti_msg) {
  Logger::amf_n2().debug("Handling Handover Required ...");
  uint64_t amf_ue_ngap_id = itti_msg->handoverReq->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id = itti_msg->handoverReq->getRanUeNgapId();

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "gNB with assoc_id (%d) is illegal", itti_msg->assoc_id);
    return false;
  }

  Logger::amf_n2().debug(
      "Handover Required, gNB info (gNB Name %s, gNB ID 0x%x)",
      gc->gnb_name.c_str(), gc->gnb_id);

  std::shared_ptr<ue_ngap_context> unc = {};
  if (!ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id, gc->gnb_id, unc))
    return false;

  if (unc->amf_ue_ngap_id != amf_ue_ngap_id) {
    Logger::amf_n2().error(
        "The requested UE (amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT
        ") is not valid, existed UE "
        "with  amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
        amf_ue_ngap_id, unc->amf_ue_ngap_id);
    return false;
  }

  if (itti_msg->handoverReq->getHandoverType() != Ngap_HandoverType_intra5gs) {
    Logger::amf_n2().error("Handover Type is not supported");
    return false;
  }

  Logger::amf_n2().debug(
      "Handover Required, Choice of Cause %d, Cause %ld",
      (int) itti_msg->handoverReq->getChoiceOfCause(),
      itti_msg->handoverReq->getCauseValue());
  long direct_forward_path_availability = {};
  if (itti_msg->handoverReq->getDirectForwardingPathAvailability(
          direct_forward_path_availability))
    Logger::amf_n2().debug(
        "Handover Required, DirectForwardingPathAvailability %ld",
        direct_forward_path_availability);

  unc->gnb_assoc_id = itti_msg->assoc_id;
  unc->ng_ue_state  = NGAP_UE_HANDOVER;

  GlobalGnbId target_global_gnb_id = {};
  oai::ngap::Tai tai               = {};
  itti_msg->handoverReq->getTargetId(target_global_gnb_id, tai);
  oai::ngap::PlmnId plmn = {};
  GnbId gnbid            = {};
  target_global_gnb_id.get(plmn, gnbid);
  std::string mcc = {};
  std::string mnc = {};
  plmn.getMcc(mcc);
  plmn.getMnc(mnc);
  uint32_t gnb_id_value = {0};
  gnbid.get(gnb_id_value);

  Logger::amf_n2().debug(
      "Handover Required, Target ID (gNB ID 0x%x), PLMN (MCC %s, MNC %s)",
      gnb_id_value, mcc.c_str(), mnc.c_str());

  std::string mcc_select_tai = {};
  std::string mnc_select_tai = {};
  uint32_t tac               = {};

  tai.get(mcc_select_tai, mnc_select_tai, tac);
  Logger::amf_n2().debug(
      "Handover Required: Target ID selected TAI PLMN (MCC %s, MNC %s, TAC %x)",
      mcc_select_tai.c_str(), mnc_select_tai.c_str(), tac);

  OCTET_STRING_t source_to_target =
      itti_msg->handoverReq->getSourceToTargetTransparentContainer();

  // TODO: T-AMF selection, for now use the same AMF

  // Create HandoverRequest message to be sent to target gNB
  auto handover_request = std::make_unique<HandoverRequest>();
  handover_request->setAmfUeNgapId(amf_ue_ngap_id);
  handover_request->setHandoverType(0);
  handover_request->setCause(
      Ngap_Cause_PR_radioNetwork,
      Ngap_CauseRadioNetwork_handover_desirable_for_radio_reason);
  handover_request->setUeAggregateMaximumBitRate(
      UE_AGGREGATE_MAXIMUM_BIT_RATE_DL, UE_AGGREGATE_MAXIMUM_BIT_RATE_UL);
  handover_request->setUeSecurityCapabilities(
      0xe000, 0xe000, 0xe000, 0xe000);  // TODO: remove hardcoded values

  handover_request->setSourceToTargetTransparentContainer(source_to_target);

  // Allowed NSSAI
  std::vector<oai::ngap::SNssai> allowed_nssai;
  for (int i = 0; i < amf_cfg.plmn_list.size(); i++) {
    for (int j = 0; j < amf_cfg.plmn_list[i].slice_list.size(); j++) {
      S_Nssai s_tmp;
      oai::ngap::SNssai s_nssai = {};
      s_nssai.setSst(amf_cfg.plmn_list[i].slice_list[j].sst);
      s_nssai.setSd(amf_cfg.plmn_list[i].slice_list[j].sd);
      allowed_nssai.push_back(s_nssai);
    }
  }
  handover_request->setAllowedNssai(allowed_nssai);

  // GUAMI, PLMN
  oai::ngap::PlmnId plmn_id = {};
  plmn_id.set(amf_cfg.guami.mcc, amf_cfg.guami.mnc);

  handover_request->setMobilityRestrictionList(plmn_id);
  handover_request->setGuami(
      amf_cfg.guami.mcc, amf_cfg.guami.mnc, amf_cfg.guami.region_id,
      amf_cfg.guami.amf_set_id, amf_cfg.guami.amf_pointer);

  // Security context
  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return false;

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n2().error("No Security Context found");
    return false;
  }

  unc->ncc = nc->security_ctx.value().ul_count.seq_num & 0x07;

  uint8_t kamf[AUTH_VECTOR_LENGTH_OCTETS];
  uint8_t kgnb[AUTH_VECTOR_LENGTH_OCTETS];
  if (!nc->get_kamf(nc->security_ctx.value().vector_pointer, kamf)) {
    Logger::amf_n2().warn("No Kamf found");
    return false;
  }
  uint32_t ulcount = nc->security_ctx.value().ul_count.seq_num |
                     (nc->security_ctx.value().ul_count.overflow << 8);
  Logger::amf_n2().debug(
      "Handover Required, Uplink count (%d)",
      nc->security_ctx.value().ul_count.seq_num);
  uint8_t knh[AUTH_VECTOR_LENGTH_OCTETS];
  Authentication_5gaka::handover_ncc_derive_knh(
      ulcount, 0x01, kamf, kgnb, knh,
      unc->ncc);  // TODO: remove hardcoded value
  bstring knh_bs = blk2bstr(knh, AUTH_VECTOR_LENGTH_OCTETS);
  handover_request->setSecurityContext(unc->ncc /*NCC count*/, knh_bs);

  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  Logger::amf_n2().debug(
      "Received Handover Required for UE (SUPI %s)", supi.c_str());

  PduSessionResourceListHandoverRqd pDUSessionResourceListHORqd = {};
  std::vector<PDUSessionResourceItem_t> pdu_session_resource_list;
  if (!itti_msg->handoverReq->getPduSessionResourceList(
          pDUSessionResourceListHORqd)) {
    Logger::ngap().warn(
        "Decoding PDU Session Resource List IE error or IE missing");
  }

  std::vector<PduSessionResourceItem> item_ho_required_list;
  pDUSessionResourceListHORqd.get(item_ho_required_list);

  std::map<uint8_t, boost::shared_future<nlohmann::json>> curl_responses;

  // Send PDUSessionUpdateSMContextRequest to SMF for all PDU sessions included
  // in HO Required message
  for (auto& item : item_ho_required_list) {
    PduSessionId pdu_session_id               = {};
    OCTET_STRING_t handover_required_transfer = {};
    item.get(pdu_session_id, handover_required_transfer);
    uint8_t pdu_session_id_value = 0;
    pdu_session_id.get(pdu_session_id_value);

    Logger::ngap().debug("PDU Session ID %d", pdu_session_id_value);
    std::shared_ptr<pdu_session_context> psc = {};
    if (amf_app_inst->find_pdu_session_context(
            supi, pdu_session_id_value, psc)) {
      // Generate a promise and associate this promise to the curl handle
      uint32_t promise_id = amf_app_inst->generate_promise_id();
      Logger::amf_n2().debug("Promise ID generated %d", promise_id);

      boost::shared_ptr<boost::promise<nlohmann::json>> p =
          boost::make_shared<boost::promise<nlohmann::json>>();
      boost::shared_future<nlohmann::json> f = p->get_future();

      // Store the future to be processed later
      curl_responses.emplace(psc->pdu_session_id, f);
      amf_app_inst->add_promise(promise_id, p);

      Logger::amf_n2().debug(
          "Sending ITTI to trigger PDUSessionUpdateSMContextRequest to SMF to "
          "task TASK_AMF_SBI");
      auto itti_msg = std::make_shared<itti_nsmf_pdusession_update_sm_context>(
          TASK_NGAP, TASK_AMF_SBI);

      itti_msg->pdu_session_id = pdu_session_id_value;
      itti_msg->n2sm           = blk2bstr(
          handover_required_transfer.buf, handover_required_transfer.size);
      itti_msg->is_n2sm_set    = true;
      itti_msg->n2sm_info_type = "HANDOVER_REQUIRED";
      itti_msg->ho_state       = "PREPARING";
      itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
      itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
      itti_msg->promise_id     = promise_id;

      int ret = itti_inst->send_msg(itti_msg);
      if (0 != ret) {
        Logger::ngap().error(
            "Could not send ITTI message %s to task TASK_AMF_SBI",
            itti_msg->get_msg_name());
      }
    }
  }

  // TODO: Handover Response supervision
  // Wait until receiving all responses from SMFs before sending Handover
  std::vector<PDUSessionResourceSetupRequestItem_t> list;

  // Wait for the response available and process accordingly
  bool result = true;
  while (!curl_responses.empty()) {
    std::optional<nlohmann::json> result_json_opt = std::nullopt;
    oai::utils::utils::wait_for_result(
        curl_responses.begin()->second, result_json_opt);
    if (result_json_opt.has_value()) {
      nlohmann::json result_json = result_json_opt.value();

      if (result_json.find("n2sm") != result_json.end()) {
        std::string n2_sm = {};
        n2_sm             = result_json["n2sm"].get<std::string>();
        Logger::ngap().debug(
            "Got result for PDU Session ID %d", curl_responses.begin()->first);
        result = result && true;

        std::shared_ptr<pdu_session_context> psc = {};
        if (amf_app_inst->find_pdu_session_context(
                supi, curl_responses.begin()->first, psc)) {
          PDUSessionResourceSetupRequestItem_t item = {};
          item.pduSessionId                         = psc->pdu_session_id;
          item.sNssai.sst = std::to_string(psc->snssai.sst);
          item.sNssai.sd  = psc->snssai.sd;
          // item.pduSessionNAS_PDU = nullptr;
          unsigned int data_len = n2_sm.length();
          item.pduSessionResourceSetupRequestTransfer.buf =
              (unsigned char*) malloc(data_len + 1);
          memcpy(
              (void*) item.pduSessionResourceSetupRequestTransfer.buf,
              (void*) n2_sm.c_str(), data_len);
          item.pduSessionResourceSetupRequestTransfer.size = data_len;
          list.push_back(item);
        }
      } else {
        result = false;
      }
    } else {
      result = false;
    }

    curl_responses.erase(curl_responses.begin());
  }
  // TODO: process result

  // Request to Target RAN
  handover_request->setPduSessionResourceSetupList(list);

  uint8_t buffer[BUFFER_SIZE_8192];
  int encoded_size = handover_request->Encode(buffer, BUFFER_SIZE_8192);
  bstring b        = blk2bstr(buffer, encoded_size);
  std::shared_ptr<gnb_context> gc_target = {};
  if (!gnb_id_2_gnb_context(gnb_id_value, gc_target)) {
    Logger::amf_n2().warn("Could not find Target's gNB context!");
    return false;
  }

  unc->target_gnb_assoc_id = gc_target->sctp_assoc_id;
  sctp_s_38412.sctp_send_msg(gc_target->sctp_assoc_id, 0, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
  return true;
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_handover_request_ack>& itti_msg) {
  Logger::amf_n2().debug("Handling Handover Request Ack ...");
  uint64_t amf_ue_ngap_id = itti_msg->handoverRequestAck->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id = itti_msg->handoverRequestAck->getRanUeNgapId();
  Logger::amf_n2().debug(
      "Handover Request Ack ran_ue_ngap_id (" GNB_UE_NGAP_ID_FMT
      ") amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
      ran_ue_ngap_id, amf_ue_ngap_id);

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "gNB with assoc_id (%d) is illegal", itti_msg->assoc_id);
    return;
  }

  Logger::amf_n2().debug(
      "Handover Request Ack, gNB info (gNB Name %s, gNB ID 0x%x)",
      gc->gnb_name.c_str(), gc->gnb_id);

  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_ue_id_2_ue_ngap_context(amf_ue_ngap_id, unc)) return;

  unc->target_ran_ue_ngap_id = ran_ue_ngap_id;  // store target RAN ID

  std::vector<PDUSessionResourceAdmittedItem_t> list;
  if (!itti_msg->handoverRequestAck->getPduSessionResourceAdmittedList(list)) {
    Logger::ngap().error(
        "Decoding HandoverRequestACK getPduSessionResourceList IE error");
    return;
  }

  OCTET_STRING_t targetTosource =
      itti_msg->handoverRequestAck->getTargetToSourceTransparentContainer();

  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  std::string supi = amf_conv::imsi_to_supi(nc->imsi);

  // Send PDUSessionUpdateSMContextRequest to SMF for all associated PDU
  // sessions
  std::map<uint8_t, boost::shared_future<nlohmann::json>> curl_responses;

  for (auto pdu_session_resource : list) {
    // Generate a promise and associate this promise to the curl handle
    uint32_t promise_id = amf_app_inst->generate_promise_id();
    Logger::amf_n2().debug("Promise ID generated %d", promise_id);

    auto p = boost::make_shared<boost::promise<nlohmann::json>>();
    boost::shared_future<nlohmann::json> f = p->get_future();

    // Store the future to be processed later
    amf_app_inst->add_promise(promise_id, p);
    curl_responses.emplace(pdu_session_resource.pduSessionId, f);

    Logger::amf_n2().debug(
        "Sending ITTI to trigger PDUSessionUpdateSMContextRequest to SMF to "
        "task TASK_AMF_SBI");
    auto itti_msg = std::make_shared<itti_nsmf_pdusession_update_sm_context>(
        TASK_NGAP, TASK_AMF_SBI);

    itti_msg->pdu_session_id = pdu_session_resource.pduSessionId;
    itti_msg->n2sm           = blk2bstr(
        pdu_session_resource.handoverRequestAcknowledgeTransfer.buf,
        pdu_session_resource.handoverRequestAcknowledgeTransfer.size);
    itti_msg->is_n2sm_set    = true;
    itti_msg->n2sm_info_type = "HANDOVER_REQ_ACK";
    itti_msg->ho_state       = "PREPARED";
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->ran_ue_ngap_id = unc.get()->ran_ue_ngap_id;
    itti_msg->promise_id     = promise_id;

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::ngap().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }

  // Send HandoverCommandMsg to Source gNB
  auto handovercommand = std::make_unique<HandoverCommandMsg>();
  handovercommand->setAmfUeNgapId(amf_ue_ngap_id);
  handovercommand->setRanUeNgapId(unc->ran_ue_ngap_id);
  handovercommand->setHandoverType(Ngap_HandoverType_intra5gs);

  PduSessionResourceHandoverList handoverList = {};
  std::vector<PduSessionResourceItem> handoverItemList;
  PduSessionResourceItem handoverItem = {};

  // TODO: wait for response from SMF and transfer T-RAN N3 information/ or
  // T-UPF to the source gNB
  bool result = true;
  while (!curl_responses.empty()) {
    std::optional<nlohmann::json> result_json_opt = std::nullopt;
    oai::utils::utils::wait_for_result(
        curl_responses.begin()->second, result_json_opt);
    if (result_json_opt.has_value()) {
      nlohmann::json result_json = result_json_opt.value();
      if (result_json.find("n2sm") != result_json.end()) {
        std::string n2_sm = {};
        n2_sm             = result_json["n2sm"].get<std::string>();
        Logger::ngap().debug(
            "Got result for PDU Session ID %d", curl_responses.begin()->first);

        result                                 = result && true;
        uint8_t pdu_session_id_value           = curl_responses.begin()->first;
        unsigned int data_len                  = n2_sm.length();
        PduSessionId pdu_session_id            = {};
        OCTET_STRING_t handoverCommandTransfer = {};
        pdu_session_id.set(pdu_session_id_value);
        OCTET_STRING_fromBuf(
            &handoverCommandTransfer, n2_sm.c_str(), n2_sm.length());
        handoverItem.set(pdu_session_id, handoverCommandTransfer);
        handoverItemList.push_back(handoverItem);
        handoverList.set(handoverItemList);

        std::shared_ptr<pdu_session_context> psc = {};
        if (amf_app_inst->find_pdu_session_context(
                supi, pdu_session_id_value, psc)) {
          psc->is_ho_accepted = true;
        }
      } else {
        result = false;
      }
    }

    curl_responses.erase(curl_responses.begin());
  }

  handovercommand->setPduSessionResourceHandoverList(handoverList);
  handovercommand->setTargetToSourceTransparentContainer(targetTosource);

  uint8_t buffer[BUFFER_SIZE_1024];
  int encoded_size = handovercommand->Encode(buffer, BUFFER_SIZE_1024);
  bstring b        = blk2bstr(buffer, encoded_size);
  sctp_s_38412.sctp_send_msg(unc->gnb_assoc_id, 0, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_handover_notify>& itti_msg) {
  Logger::amf_n2().info("Handle Handover Notify ...");
  uint64_t amf_ue_ngap_id = itti_msg->handoverNotify->getAmfUeNgapId();
  uint32_t ran_ue_ngap_id = itti_msg->handoverNotify->getRanUeNgapId();
  Logger::amf_n2().debug(
      "Handover Notify ran_ue_ngap_id (" GNB_UE_NGAP_ID_FMT
      ") amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
      ran_ue_ngap_id, amf_ue_ngap_id);

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(itti_msg->assoc_id, gc)) {
    Logger::amf_n2().error(
        "gNB with assoc_id (%d) is illegal", itti_msg->assoc_id);
    return;
  }

  Logger::amf_n2().debug(
      "Handover Notify, gNB info (gNB Name: %s, gNB ID 0x%x)",
      gc->gnb_name.c_str(), gc->gnb_id);

  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_ue_id_2_ue_ngap_context(amf_ue_ngap_id, unc)) return;

  NrCgi_t NR_CGI = {};
  Tai_t tai      = {};
  if (!itti_msg->handoverNotify->getUserLocationInfoNr(NR_CGI, tai)) {
    Logger::amf_n2().debug("Missing IE UserLocationInformationNR");
    return;
  }

  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  uint32_t old_ran_ue_ngap_id    = unc->ran_ue_ngap_id;
  std::shared_ptr<ue_context> uc = {};
  std::string ue_context_key =
      amf_conv::get_ue_context_key(old_ran_ue_ngap_id, amf_ue_ngap_id);
  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) {
    Logger::amf_app().debug(
        "No existing UE Context, with %s", ue_context_key.c_str());
  }

  std::string supi = amf_conv::imsi_to_supi(nc->imsi);

  // Get PDU Session Context
  std::vector<std::shared_ptr<pdu_session_context>> sessions_ctx;
  if (!amf_app_inst->get_pdu_sessions_context(supi, sessions_ctx)) {
    Logger::amf_n2().debug("No PDU Session Context found");
  }

  // Send PDUSessionUpdateSMContextRequest to SMF for accepted PDU sessions
  std::map<uint8_t, boost::shared_future<nlohmann::json>> curl_responses;

  for (auto pdu_session : sessions_ctx) {
    if (pdu_session->is_ho_accepted) {
      // Generate a promise and associate this promise to the curl handle
      uint32_t promise_id = amf_app_inst->generate_promise_id();
      Logger::amf_n2().debug("Promise ID generated %d", promise_id);

      auto p = boost::make_shared<boost::promise<nlohmann::json>>();
      boost::shared_future<nlohmann::json> f = p->get_future();

      // Store the future to be processed later
      amf_app_inst->add_promise(promise_id, p);
      curl_responses.emplace(pdu_session->pdu_session_id, f);

      Logger::amf_n2().debug(
          "Sending ITTI to trigger PDUSessionUpdateSMContextRequest to SMF to "
          "task TASK_AMF_SBI");

      auto itti_n11_msg =
          std::make_shared<itti_nsmf_pdusession_update_sm_context>(
              TASK_NGAP, TASK_AMF_SBI);

      itti_n11_msg->pdu_session_id = pdu_session->pdu_session_id;

      // TODO: Secondary RAT Usage
      // itti_n11_msg->n2sm           = blk2bstr("Secondary RAT Usage", 19);
      // itti_n11_msg->n2sm_info_type = "SECONDARY_RAT_USAGE";
      itti_n11_msg->is_n2sm_set = false;
      itti_n11_msg->ho_state    = "COMPLETED";

      itti_n11_msg->amf_ue_ngap_id = amf_ue_ngap_id;
      itti_n11_msg->ran_ue_ngap_id = old_ran_ue_ngap_id;
      itti_n11_msg->promise_id     = promise_id;

      int ret = itti_inst->send_msg(itti_n11_msg);
      if (0 != ret) {
        Logger::ngap().error(
            "Could not send ITTI message %s to task TASK_AMF_SBI",
            itti_n11_msg->get_msg_name());
      }
    }
  }

  // Wait for the response available and process accordingly
  bool result = true;
  while (!curl_responses.empty()) {
    // Wait for the result available and process accordingly
    std::optional<nlohmann::json> result_json_opt = std::nullopt;
    oai::utils::utils::wait_for_result(
        curl_responses.begin()->second, result_json_opt);

    if (result_json_opt.has_value()) {
      nlohmann::json result_json = result_json_opt.value();
      Logger::amf_server().debug(
          "Got result for promise ID %d", curl_responses.begin()->first);
      uint32_t pdu_session_id = 0;
      if (result_json.find("pduSessionId") != result_json.end()) {
        result         = result && true;
        pdu_session_id = result_json["pduSessionId"].get<int>();
      } else {
        result = false;
      }
    } else {
      result = false;
    }

    curl_responses.erase(curl_responses.begin());
  }

  // Send UE Release Command to Source gNB
  Logger::ngap().info("Send UE Release Command to source gNB");
  auto ueContextReleaseCommand = std::make_unique<UeContextReleaseCommandMsg>();
  ueContextReleaseCommand->setUeNgapIdPair(amf_ue_ngap_id, old_ran_ue_ngap_id);
  ueContextReleaseCommand->setCauseRadioNetwork(
      Ngap_CauseRadioNetwork_successful_handover);

  uint8_t buffer[BUFFER_SIZE_1024];
  int encoded_size = ueContextReleaseCommand->Encode(buffer, BUFFER_SIZE_1024);
  bstring b        = blk2bstr(buffer, encoded_size);

  sctp_s_38412.sctp_send_msg(unc->gnb_assoc_id, 0, &b);
  oai::utils::utils::bdestroy_wrapper(&b);

  // update the NGAP Context
  unc->release_cause         = Ngap_CauseRadioNetwork_successful_handover;
  unc->release_gnb           = unc->gnb_assoc_id;
  unc->ran_ue_ngap_id        = ran_ue_ngap_id;  // store new RAN ID
  unc->target_ran_ue_ngap_id = 0;               // Clear target RAN ID
  unc->ng_ue_state           = NGAP_UE_CONNECTED;
  unc->gnb_assoc_id          = itti_msg->assoc_id;  // update serving gNB
  set_ran_ue_ngap_id_2_ue_ngap_context(ran_ue_ngap_id, gc->gnb_id, unc);

  // update NAS Context
  nc->ran_ue_ngap_id = ran_ue_ngap_id;

  // update User Context
  uc->ran_ue_ngap_id = ran_ue_ngap_id;
  uc->gnb_id         = gc->gnb_id;

  std::string new_ue_context_key =
      amf_conv::get_ue_context_key(ran_ue_ngap_id, amf_ue_ngap_id);
  amf_app_inst->set_ran_amf_id_2_ue_context(new_ue_context_key, uc);

  // Retrieve new location from the UE and notify generate location change
  // signal
  UserLocation user_location = {};
  NrLocation nr_location     = {};

  Ncgi ncgi;
  oai::model::common::Tai tai_model;
  oai::model::common::PlmnId plmnId;
  oai::model::common::GlobalRanNodeId ranid;
  GNbId gnbid;

  plmnId.setMcc(NR_CGI.mcc);
  plmnId.setMnc(NR_CGI.mnc);

  gnbid.setBitLength(32);
  gnbid.setGNBValue(std::to_string(gc->gnb_id));
  ranid.setGNbId(gnbid);
  ranid.setPlmnId(plmnId);

  ncgi.setNid("");
  ncgi.setNrCellId(std::to_string(NR_CGI.nrCellId));
  ncgi.setPlmnId(plmnId);
  tai_model.setPlmnId(plmnId);
  tai_model.setTac(std::to_string(tai.tac));
  nr_location.setNcgi(ncgi);
  nr_location.setTai(tai_model);
  nr_location.setGlobalGnbId(ranid);

  user_location.setNrLocation(nr_location);
  amf_n1_inst->event_sub.ue_location_report(
      supi, user_location, amf_cfg.support_features.http_version);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_uplink_ran_status_transfer>& itti_msg) {
  Logger::amf_n2().debug("Handling Uplink RAN Status Transfer ...");
  uint64_t amf_ue_ngap_id = itti_msg->uplinkRanTransfer->getAmfUeNgapId();
  Logger::amf_n2().debug(
      "Uplink RAN Status Transfer amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
      amf_ue_ngap_id);

  if (!is_assoc_id_2_gnb_context(itti_msg->assoc_id)) {
    Logger::amf_n2().error(
        "gNB with assoc_id (%d) is illegal", itti_msg->assoc_id);
    return;
  }

  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_ue_id_2_ue_ngap_context(amf_ue_ngap_id, unc)) return;

  RanStatusTransferTransparentContainer ran_status_transfer = {};
  itti_msg->uplinkRanTransfer->getRanStatusTransferTransparentContainer(
      ran_status_transfer);
  DrbSubjectToStatusTransferList amf_m_list = {};
  ran_status_transfer.getDrbSubjectList(amf_m_list);
  std::vector<DrbSubjectToStatusTransferItem> drb_subject_item_list;
  amf_m_list.get(drb_subject_item_list);
  // TODO: check size

  DrbStatusDl status_dl = {};
  DrbStatusUl status_ul = {};
  Ngap_DRB_ID_t drb_id  = {};
  drb_subject_item_list[0].get(drb_id, status_ul, status_dl);
  // TODO:dRBStatusUL18 or dRBStatusUL12
  std::optional<DrbStatusUl18> status_ul_18 = {};
  // TODO: DRBStatusDL18 or DRBStatusDL18
  std::optional<DrbStatusDl18> status_dl_18 = std::nullopt;
  status_dl.getDrbStatusDl18(status_dl_18);
  status_ul.getDrbStatusUl(status_ul_18);
  CountValueForPdcpSn18 count_value_ul = {};
  CountValueForPdcpSn18 count_value_dl = {};
  if (status_ul_18.has_value()) status_ul_18.value().get(count_value_ul);
  if (status_dl_18.has_value()) status_dl_18.value().get(count_value_dl);
  long amf_ul_pdcp     = {0};
  long amf_hfn_ul_pdcp = {0};
  count_value_ul.get(amf_ul_pdcp, amf_hfn_ul_pdcp);
  long amf_dl_pdcp     = {0};
  long amf_hfn_dl_pdcp = {0};
  count_value_dl.get(amf_dl_pdcp, amf_hfn_dl_pdcp);
  long amf_drb_id = {0};
  amf_drb_id      = drb_id;

  auto dl_ran_status_transfer = std::make_unique<DownlinkRanStatusTransfer>();
  dl_ran_status_transfer->setAmfUeNgapId(amf_ue_ngap_id);
  dl_ran_status_transfer->setRanUeNgapId(unc->target_ran_ue_ngap_id);
  dl_ran_status_transfer->setRanStatusTransferTransparentContainer(
      amf_drb_id, amf_ul_pdcp, amf_hfn_ul_pdcp, amf_dl_pdcp, amf_hfn_dl_pdcp);
  uint8_t buffer[BUFFER_SIZE_1024];
  int encode_size = dl_ran_status_transfer->Encode(buffer, BUFFER_SIZE_1024);
  bstring b       = blk2bstr(buffer, encode_size);
  sctp_s_38412.sctp_send_msg(unc->target_gnb_assoc_id, 0, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(std::shared_ptr<itti_rereoute_nas>& itti_msg) {
  Logger::amf_n2().debug("Handle Reroute NAS Request message...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  if (unc->amf_ue_ngap_id != itti_msg->amf_ue_ngap_id) {
    Logger::amf_n2().error(
        "AMF UE NGAP ID does not match the expected value (requested "
        "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT
        ", existed amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT ")",
        itti_msg->amf_ue_ngap_id, unc->amf_ue_ngap_id);
    return;
  }

  RerouteNasRequest reroute_nas_request = {};
  reroute_nas_request.setRanUeNgapId(itti_msg->ran_ue_ngap_id);
  reroute_nas_request.setAmfUeNgapId(itti_msg->amf_ue_ngap_id);
  if (!reroute_nas_request.setAmfSetId(itti_msg->amf_set_id)) return;
  if (unc->initial_ue_msg.size > 0)
    reroute_nas_request.setNgapMessage(
        unc->initial_ue_msg);  // Include InitialUEMessage

  // TODO: AllowedNSSAI (Optional)

  uint8_t buffer[BUFFER_SIZE_2048];
  int encoded_size = reroute_nas_request.Encode(buffer, BUFFER_SIZE_2048);
  bstring b        = blk2bstr(buffer, encoded_size);

  amf_n2_inst->sctp_s_38412.sctp_send_msg(
      unc->gnb_assoc_id, unc->sctp_stream_send, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_downlink_ue_associated_nrppa_transport>& itti_msg) {
  Logger::amf_n2().debug("Handle Downlink UE Associated NRPPa Transport ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  DownlinkUeAssociatedNrppaTransportMsg duant = {};
  duant.setAmfUeNgapId(itti_msg->amf_ue_ngap_id);
  duant.setRanUeNgapId(itti_msg->ran_ue_ngap_id);

  duant.setNrppaPdu(itti_msg->nrppa_pdu);
  duant.setRoutingId(itti_msg->routing_id);

  uint8_t buffer[BUFFER_SIZE_4096];
  int encoded_size = duant.Encode(buffer, BUFFER_SIZE_1024);
  if (encoded_size > 0) {
    bstring b = blk2bstr(buffer, encoded_size);
    sctp_s_38412.sctp_send_msg(gc->sctp_assoc_id, unc->sctp_stream_send, &b);
    oai::utils::utils::bdestroy_wrapper(&b);
  }
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_downlink_non_ue_associated_nrppa_transport>&
        itti_msg) {
  Logger::amf_n2().debug(
      "Handle Downlink Non UE Associated NRPPa Transport ...");

  DownlinkNonUeAssociatedNrppaTransportMsg dnuant = {};
  dnuant.setNrppaPdu(itti_msg->nrppa_pdu);
  dnuant.setRoutingId(itti_msg->routing_id);

  uint8_t buffer[BUFFER_SIZE_4096];
  int encoded_size = dnuant.Encode(buffer, BUFFER_SIZE_1024);
  if (encoded_size > 0) {
    bstring b = blk2bstr(buffer, encoded_size);
    // TODO: Should be verified
    std::vector<sctp::sctp_assoc_id_t> assoc_ids = get_all_assoc_ids();
    for (auto& assoc_id : assoc_ids) {
      sctp_s_38412.sctp_send_msg(assoc_id, 0, &b);
    }
    oai::utils::utils::bdestroy_wrapper(&b);
  }
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_uplink_ue_associated_nrppa_transport>& itti_msg) {
  Logger::amf_n2().debug("Handle Uplink UE Associated NRPPa Transport ...");

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  std::string ue_context_key           = amf_conv::get_ue_context_key(
      itti_msg->ran_ue_ngap_id, itti_msg->amf_ue_ngap_id);

  if (!ran_ue_id_2_ue_ngap_context(
          itti_msg->ran_ue_ngap_id, ue_context_key, unc))
    return;

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No existing gNG context with assoc_id (%d)", unc->gnb_assoc_id);
    return;
  }

  // Find subscribed LMFs
  std::shared_ptr<ue_context> uc = {};
  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) return;

  std::optional<oai::model::amf::N1MessageClass_anyOf::eN1MessageClass_anyOf>
      n1_message_class = std::nullopt;
  std::optional<
      oai::model::amf::N2InformationClass_anyOf::eN2InformationClass_anyOf>
      n2_info_class = std::make_optional<
          oai::model::amf::N2InformationClass_anyOf::eN2InformationClass_anyOf>(
          oai::model::amf::N2InformationClass_anyOf::eN2InformationClass_anyOf::
              NRPPA);
  std::map<
      n1n2sub_id_t,
      std::shared_ptr<oai::model::amf::UeN1N2InfoSubscriptionCreateData>>
      subscriptions;

  amf_app_inst->find_n1n2_info_subscriptions(
      uc->supi, n1_message_class, n2_info_class, subscriptions);

  // Create message to send to subscribed NFs
  if (subscriptions.size() > 0) {
    // TODO:
    // Send request to SBI to trigger the notification to the subscribed NFs (N2
    // Info Notify)
    Logger::amf_n2().debug(
        "Send ITTI msg to AMF SBI to trigger the notification");

    auto itti_msg_notification =
        std::make_shared<itti_sbi_n2_info_notify>(TASK_AMF_N2, TASK_AMF_SBI);

    for (auto sub : subscriptions) {
      oai::model::amf::N2InformationNotification n2_info_notification = {};
      fill_n2_information_notification(
          std::to_string(sub.first),
          oai::model::amf::NgapIeType_anyOf::eNgapIeType_anyOf::NRPPA_PDU,
          n2_info_notification);

      itti_msg_notification->n2_info_notification = n2_info_notification;
      itti_msg_notification->n2_info =
          std::make_optional<bstring>(bstrcpy(itti_msg->nrppa_pdu));
      itti_msg_notification->nf_uri =
          sub.second.get()->getN2NotifyCallbackUri();
    }

    int ret = itti_inst->send_msg(itti_msg_notification);
    if (0 != ret) {
      Logger::amf_n2().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg_notification->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n2::handle_itti_message(
    std::shared_ptr<itti_uplink_non_ue_associated_nrppa_transport>& itti_msg) {
  Logger::amf_n2().debug("Handle Uplink Non UE Associated NRPPa Transport ...");

  // Find subscribed LMF based on Routing ID
  std::string lmf_id = {};
  amf_conv::bstring_2_string(itti_msg->routing_id, lmf_id);

  auto n2_info_class = oai::model::amf::N2InformationClass_anyOf::
      eN2InformationClass_anyOf::NRPPA;

  std::map<
      n1n2sub_id_t,
      std::shared_ptr<oai::model::amf::NonUeN2InfoSubscriptionCreateData>>
      subscriptions;

  amf_app_inst->find_non_ue_n2_info_subscriptions(
      lmf_id, n2_info_class, subscriptions);

  // Create message to send to subscribed NFs
  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed NFs (N2
    // Info Notify)
    Logger::amf_n2().debug(
        "Send ITTI msg to AMF SBI to trigger the notification");

    auto itti_msg_notification =
        std::make_shared<itti_sbi_n2_info_notify>(TASK_AMF_N2, TASK_AMF_SBI);

    for (auto sub : subscriptions) {
      oai::model::amf::N2InformationNotification n2_info_notification = {};
      fill_n2_information_notification(
          std::to_string(sub.first),
          oai::model::amf::NgapIeType_anyOf::eNgapIeType_anyOf::NRPPA_PDU,
          n2_info_notification);

      itti_msg_notification->n2_info_notification = n2_info_notification;
      itti_msg_notification->n2_info =
          std::make_optional<bstring>(bstrcpy(itti_msg->nrppa_pdu));
      itti_msg_notification->nf_uri =
          sub.second.get()->getN2NotifyCallbackUri();
    }

    int ret = itti_inst->send_msg(itti_msg_notification);
    if (0 != ret) {
      Logger::amf_n2().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg_notification->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n2::send_handover_preparation_failure(
    const uint64_t amf_ue_ngap_id, const uint32_t ran_ue_ngap_id,
    const sctp_assoc_id_t& gnb_assoc_id) {
  // Create HandoverPreparationFailure message to be sent to target gNB
  auto ho_preparation_failure_msg =
      std::make_unique<HandoverPreparationFailure>();
  ho_preparation_failure_msg->setAmfUeNgapId(amf_ue_ngap_id);
  ho_preparation_failure_msg->setRanUeNgapId(amf_ue_ngap_id);
  ho_preparation_failure_msg->setCause(Ngap_Cause_PR_NOTHING);

  uint8_t buffer[BUFFER_SIZE_1024];
  int encoded_size =
      ho_preparation_failure_msg->Encode(buffer, BUFFER_SIZE_1024);
  bstring b = blk2bstr(buffer, encoded_size);

  sctp_s_38412.sctp_send_msg(gnb_assoc_id, 0, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
}

//------------------------------------------------------------------------------
template<typename T>
void amf_n2::send_ng_setup_failure(
    const T& cause, const e_Ngap_TimeToWait& time_to_wait,
    const sctp_assoc_id_t& assoc_id, const sctp_stream_id_t& stream_id) {
  auto buffer = new (std::nothrow) uint8_t[BUFFER_SIZE_1024]();
  if (buffer == nullptr) {
    Logger::amf_n2().error("Error when allocating buffer!");
    return;
  }

  NgSetupFailureMsg ngSetupFailure = {};
  ngSetupFailure.set(cause, time_to_wait);
  int encoded = ngSetupFailure.Encode((uint8_t*) buffer, BUFFER_SIZE_1024);

  if (encoded < 1) {
    Logger::amf_n2().error("Encode NG Setup Failure message error!");
    return;
  }

  bstring b = blk2bstr(buffer, encoded);
  sctp_s_38412.sctp_send_msg(assoc_id, stream_id, &b);
  oai::utils::utils::bdestroy_wrapper(&b);
  // delete[] buffer;//Free in destructor of NgapMessage
}

//------------------------------------------------------------------------------
bool amf_n2::ran_ue_id_2_ue_ngap_context(
    uint32_t ran_ue_ngap_id, uint32_t gnb_id,
    std::shared_ptr<ue_ngap_context>& unc) const {
  auto ue_id = std::make_pair(ran_ue_ngap_id, gnb_id);
  std::shared_lock lock(m_ranid2uecontext);
  if (ranid2uecontext.count(ue_id) > 0) {
    if (ranid2uecontext.at(ue_id) != nullptr) {
      unc = ranid2uecontext.at(ue_id);
      return true;
    }
  }
  Logger::amf_n2().warn(
      "No UE NGAP context with ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT
      ", gnb_id " GNB_ID_FMT "",
      ran_ue_ngap_id, gnb_id);
  return false;
}

//------------------------------------------------------------------------------
bool amf_n2::ran_ue_id_2_ue_ngap_context(
    uint32_t ran_ue_ngap_id, const std::string& ue_context_key,
    std::shared_ptr<ue_ngap_context>& unc) const {
  // Get UE Context
  std::shared_ptr<ue_context> uc = {};
  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) return false;

  auto ue_id = std::make_pair(ran_ue_ngap_id, uc->gnb_id);
  std::shared_lock lock(m_ranid2uecontext);
  if (ranid2uecontext.count(ue_id) > 0) {
    if (ranid2uecontext.at(ue_id) != nullptr) {
      unc = ranid2uecontext.at(ue_id);
      return true;
    }
  }
  Logger::amf_n2().warn(
      "No UE NGAP context with ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT
      ", ue_context_key %s",
      ran_ue_ngap_id, ue_context_key.c_str());
  return false;
}

//------------------------------------------------------------------------------
void amf_n2::set_ran_ue_ngap_id_2_ue_ngap_context(
    uint32_t ran_ue_ngap_id, uint32_t gnb_id,
    const std::shared_ptr<ue_ngap_context>& unc) {
  auto ue_id = std::make_pair(ran_ue_ngap_id, gnb_id);
  std::unique_lock lock(m_ranid2uecontext);
  ranid2uecontext[ue_id] = unc;
}

//------------------------------------------------------------------------------
void amf_n2::remove_ran_ue_ngap_id_2_ngap_context(
    uint32_t ran_ue_ngap_id, uint32_t gnb_id) {
  auto ue_id = std::make_pair(ran_ue_ngap_id, gnb_id);
  std::unique_lock lock(m_ranid2uecontext);
  if (ranid2uecontext.count(ue_id) > 0) {
    ranid2uecontext.erase(ue_id);
    Logger::amf_n2().debug(
        "Removed UE NGAP context with ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT
        ", gnb_id " GNB_ID_FMT "",
        ran_ue_ngap_id, gnb_id);
  }
}

//------------------------------------------------------------------------------
void amf_n2::remove_ue_context_with_ran_ue_ngap_id(
    uint32_t ran_ue_ngap_id, uint32_t gnb_id) {
  // Remove NAS context if still available
  std::shared_ptr<ue_ngap_context> unc = {};

  if (!ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id, gnb_id, unc)) return;

  // Remove all NAS context if still exist
  std::shared_ptr<nas_context> nc = {};
  if (amf_n1_inst->amf_ue_id_2_nas_context(unc->amf_ue_ngap_id, nc)) {
    // TODO: Verify where it's current context
    // Remove all NAS context
    std::string supi = amf_conv::imsi_to_supi(nc->imsi);
    stacs.update_5gmm_state(nc, _5GMM_DEREGISTERED);

    // Trigger UE Loss of Connectivity Status Notify
    Logger::amf_n2().debug(
        "Signal the UE Loss of Connectivity Event notification for SUPI %s",
        supi.c_str());
    amf_n1_inst->event_sub.ue_loss_of_connectivity(
        supi, DEREGISTERED, amf_cfg.support_features.http_version,
        ran_ue_ngap_id, unc->amf_ue_ngap_id);

    amf_n1_inst->remove_supi_2_nas_context(supi);
    // TODO:  remove_guti_2_nas_context(guti);
    amf_n1_inst->remove_amf_ue_ngap_id_2_nas_context(unc->amf_ue_ngap_id);
    // Update UE status

  } else {
    Logger::amf_n2().warn(
        "No existed nas_context with amf_ue_ngap_id(" AMF_UE_NGAP_ID_FMT ")",
        unc->amf_ue_ngap_id);
  }

  // Remove NGAP context
  remove_amf_ue_ngap_id_2_ue_ngap_context(unc->amf_ue_ngap_id);
  remove_ran_ue_ngap_id_2_ngap_context(ran_ue_ngap_id, gnb_id);
}

//------------------------------------------------------------------------------
void amf_n2::get_ue_ngap_contexts(
    const sctp_assoc_id_t& gnb_assoc_id,
    std::vector<std::shared_ptr<ue_ngap_context>>& ue_contexts) {
  std::shared_lock lock(m_ranid2uecontext);
  for (auto ue : ranid2uecontext) {
    if (ue.second->gnb_assoc_id == gnb_assoc_id)
      ue_contexts.push_back(ue.second);
  }
}

//------------------------------------------------------------------------------
bool amf_n2::amf_ue_id_2_ue_ngap_context(
    const uint64_t& amf_ue_ngap_id,
    std::shared_ptr<ue_ngap_context>& unc) const {
  std::shared_lock lock(m_amfueid2uecontext);
  if (amfueid2uecontext.count(amf_ue_ngap_id) > 0) {
    unc = amfueid2uecontext.at(amf_ue_ngap_id);
    if (unc != nullptr) {
      return true;
    }
  }
  Logger::amf_n2().error(
      "No UE NGAP context with amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT "",
      amf_ue_ngap_id);
  return false;
}

//------------------------------------------------------------------------------
void amf_n2::set_amf_ue_ngap_id_2_ue_ngap_context(
    const uint64_t& amf_ue_ngap_id, std::shared_ptr<ue_ngap_context> unc) {
  std::unique_lock lock(m_amfueid2uecontext);
  amfueid2uecontext[amf_ue_ngap_id] = unc;
}

//------------------------------------------------------------------------------
void amf_n2::remove_amf_ue_ngap_id_2_ue_ngap_context(
    const uint64_t& amf_ue_ngap_id) {
  std::unique_lock lock(m_amfueid2uecontext);
  if (amfueid2uecontext.count(amf_ue_ngap_id) > 0) {
    amfueid2uecontext.erase(amf_ue_ngap_id);
    Logger::amf_n2().debug(
        "Removed UE NGAP context with amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT "",
        amf_ue_ngap_id);
  }
}

//------------------------------------------------------------------------------
void amf_n2::remove_ue_context_with_amf_ue_ngap_id(
    const uint64_t& amf_ue_ngap_id) {
  // Remove all NAS context if still exist
  std::shared_ptr<nas_context> nc = {};
  if (amf_n1_inst->amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
    // Remove all NAS context
    std::string supi = amf_conv::imsi_to_supi(nc->imsi);
    // Update UE status
    stacs.update_5gmm_state(nc, _5GMM_DEREGISTERED);

    // Trigger UE Loss of Connectivity Status Notify
    Logger::amf_n2().debug(
        "Signal the UE Loss of Connectivity Event notification for SUPI %s",
        supi.c_str());
    amf_n1_inst->event_sub.ue_loss_of_connectivity(
        supi, DEREGISTERED, amf_cfg.support_features.http_version,
        nc->ran_ue_ngap_id, amf_ue_ngap_id);

    amf_n1_inst->remove_supi_2_nas_context(supi);
    // TODO:  remove_guti_2_nas_context(guti);
    amf_n1_inst->remove_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id);
    // Remove NGAP context related to RAN UE NGAP ID
    // Get UE Context
    std::string ue_context_key =
        amf_conv::get_ue_context_key(nc->ran_ue_ngap_id, amf_ue_ngap_id);
    std::shared_ptr<ue_context> uc = {};

    if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) return;
    remove_ran_ue_ngap_id_2_ngap_context(nc->ran_ue_ngap_id, uc->gnb_id);

  } else {
    Logger::amf_n2().warn(
        "No existed nas_context with amf_ue_ngap_id(" AMF_UE_NGAP_ID_FMT ")",
        amf_ue_ngap_id);
  }
  // Remove NGAP context
  remove_amf_ue_ngap_id_2_ue_ngap_context(amf_ue_ngap_id);
}

//------------------------------------------------------------------------------
bool amf_n2::get_common_plmn(
    const std::vector<SupportedTaItem_t>& list,
    std::vector<SupportedTaItem_t>& result) {
  std::vector<SupportedTaItem_t> plmn_list = {};
  bool found_common_plmn                   = false;
  for (int i = 0; i < amf_cfg.plmn_list.size(); i++) {
    for (int j = 0; j < list.size(); j++) {
      Logger::amf_n2().debug(
          "TAC configured %d, TAC received %d", amf_cfg.plmn_list[i].tac,
          list[j].tac);
      if (amf_cfg.plmn_list[i].tac != list[j].tac) {
        continue;
      }
      for (int k = 0; k < list[j].plmnSliceSupportList.size(); k++) {
        if (!(list[j].plmnSliceSupportList[k].mcc.compare(
                amf_cfg.plmn_list[i].mcc)) &&
            !(list[j].plmnSliceSupportList[k].mnc.compare(
                amf_cfg.plmn_list[i].mnc))) {
          Logger::amf_n2().debug(
              "Common PLMN MCC %s, MNC %s", amf_cfg.plmn_list[i].mcc.c_str(),
              amf_cfg.plmn_list[i].mnc.c_str());
          // Get the common S-NSSAI
          SupportedTaItem_t item                     = {};
          PlmnSliceSupport_t plmn_slice_support_item = {};
          item.tac                                   = list[j].tac;
          plmn_slice_support_item.mcc = list[j].plmnSliceSupportList[k].mcc;
          plmn_slice_support_item.mnc = list[j].plmnSliceSupportList[k].mnc;

          for (const auto& s1 : list[j].plmnSliceSupportList[k].sliceList) {
            snssai_t s1_snssai;
            try {
              s1_snssai.sst = std::stoi(s1.sst);
              s1_snssai.sd  = s1.sd;

              Logger::amf_n2().debug(
                  "S-NSSAI from gNB (SST %s, SD %s)", s1.sst.c_str(),
                  s1.sd.c_str());
              for (const auto& s2 : amf_cfg.plmn_list[i].slice_list) {
                Logger::amf_n2().debug(
                    "S-NSSAI from AMF (SST %d, SD %s)", s2.sst, s2.sd);
                if (s1_snssai.sst == s2.sst &&
                    s1_snssai.get_sd_int() == s2.get_sd_int()) {
                  Logger::amf_n2().debug(
                      "Common S-NSSAI (SST %s, SD %s)", s1.sst.c_str(),
                      s1.sd.c_str());
                  plmn_slice_support_item.sliceList.push_back(s1);
                  found_common_plmn = true;
                }
              }
            } catch (std::invalid_argument&) {
              Logger::amf_n2().error(
                  "Could not convert SST %s to int, wrong NGAP values", s1.sst);
            }
          }

          item.plmnSliceSupportList.push_back(plmn_slice_support_item);
          result.push_back(item);
        }
      }
    }
  }
  return found_common_plmn;
}

//------------------------------------------------------------------------------
bool amf_n2::get_common_NSSAI(
    const uint32_t& ran_ue_ngap_id, uint32_t gnb_id,
    std::vector<oai::nas::SNSSAI_t>& common_nssai) {
  Logger::amf_n2().debug("Getting common S-NSSAIs between gNB and AMF");

  bool found = false;
  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};

  if (!ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id, gnb_id, unc)) return false;

  // Get gNB Context
  std::shared_ptr<gnb_context> gc = {};
  if (!amf_n2_inst->assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n2().error(
        "No gNB context with assoc_id (%d)", unc->gnb_assoc_id);
    return false;
  }

  for (const auto& ta : gc->supported_ta_list) {
    for (const auto& plmn : ta.plmnSliceSupportList) {
      for (const auto& slice : plmn.sliceList) {
        oai::nas::SNSSAI_t snssai = {};
        try {
          snssai_t slice_parsed(std::stoi(slice.sst), slice.sd);
          snssai.sst = slice_parsed.sst;
          snssai.sd  = slice_parsed.get_sd_int();
        } catch (const std::exception& err) {
          Logger::amf_app().error("Invalid SST/SD");
          break;
        }
        common_nssai.push_back(snssai);
        found = true;
      }
    }
  }
  Logger::amf_n2().debug("Getting common S-NSSAIs between gNB and AMF");

  for (auto s : common_nssai) {
    Logger::amf_n2().debug(
        "Common S-NSSAI (SST %s, SD %s)", std::to_string(s.sst).c_str(),
        std::to_string(s.sd).c_str());
  }

  return found;
}

//------------------------------------------------------------------------------
void amf_n2::fill_n2_information_notification(
    const std::string& subscription_id,
    const oai::model::amf::NgapIeType_anyOf::eNgapIeType_anyOf& ngap_ie_type_e,
    oai::model::amf::N2InformationNotification& n2_info_notification) {
  n2_info_notification.setN2NotifySubscriptionId(subscription_id);
  oai::model::amf::N2InfoContainer n2_info_container = {};

  // N2 Information Class (only for NRPPA for now)
  oai::model::amf::N2InformationClass n2_information_class = {};
  n2_information_class.setEnumValue(oai::model::amf::N2InformationClass_anyOf::
                                        eN2InformationClass_anyOf::NRPPA);
  n2_info_container.setN2InformationClass(n2_information_class);

  oai::model::amf::N2InfoContent n2_info_content         = {};
  oai::model::amf::NgapIeType ngap_ie_type               = {};
  oai::model::common::RefToBinaryData ref_to_binary_data = {};
  ngap_ie_type.setEnumValue(ngap_ie_type_e);
  // TODO: for another types
  if (ngap_ie_type_e ==
      oai::model::amf::NgapIeType_anyOf::eNgapIeType_anyOf::NRPPA_PDU) {
    ref_to_binary_data.setContentId(N2_NRPPa_CONTENT_ID);
  }
  n2_info_content.setNgapIeType(ngap_ie_type);
  // n2_info_content.setNgapMessageType(value)
  n2_info_content.setNgapData(ref_to_binary_data);

  if (ngap_ie_type_e ==
      oai::model::amf::NgapIeType_anyOf::eNgapIeType_anyOf::NRPPA_PDU) {
    // NRPPA Info
    oai::model::amf::NrppaInformation nrppa_information = {};
    nrppa_information.setNrppaPdu(n2_info_content);
    n2_info_container.setNrppaInfo(nrppa_information);
  }

  n2_info_notification.setN2InfoContainer(n2_info_container);
}
