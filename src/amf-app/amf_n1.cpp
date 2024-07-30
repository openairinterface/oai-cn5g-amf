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

#include "amf_n1.hpp"

#include <bitset>

#include "3gpp_24.501.hpp"
#include "AmfEventReport.h"
#include "AmfEventType.h"
#include "AuthenticationFailure.hpp"
#include "AuthenticationInfo.h"
#include "AuthenticationRequest.hpp"
#include "AuthenticationResponse.hpp"
#include "ConfigurationUpdateCommand.hpp"
#include "ConfirmationData.h"
#include "ConfirmationDataResponse.h"
#include "DeregistrationAccept.hpp"
#include "DeregistrationRequest.hpp"
#include "IdentityRequest.hpp"
#include "IdentityResponse.hpp"
#include "RegistrationAccept.hpp"
#include "RegistrationComplete.hpp"
#include "RegistrationReject.hpp"
#include "RegistrationRequest.hpp"
#include "RejectedSNssai.hpp"
#include "SecurityModeCommand.hpp"
#include "SecurityModeComplete.hpp"
#include "ServiceAccept.hpp"
#include "ServiceReject.hpp"
#include "ServiceRequest.hpp"
#include "UEAuthenticationCtx.h"
#include "UlNasTransport.hpp"
#include "amf_app.hpp"
#include "amf_config.hpp"
#include "amf_conversions.hpp"
#include "amf_n2.hpp"
#include "amf_sbi.hpp"
#include "amf_sbi_helper.hpp"
#include "authentication.hpp"
#include "bstrlib.h"
#include "itti.hpp"
#include "itti_msg_n2.hpp"
#include "itti_msg_sbi.hpp"
#include "logger.hpp"
#include "nas_algorithms.hpp"
#include "ngap_utils.hpp"
#include "output_wrapper.hpp"
#include "sha256.hpp"
#include "utils.hpp"

using namespace amf_application;
using namespace boost::placeholders;
using namespace oai::model::amf;
using namespace oai::amf::api;
using namespace oai::config;
using namespace oai::model::common;
using namespace oai::nas;

extern itti_mw* itti_inst;
extern amf_n1* amf_n1_inst;
extern amf_sbi* amf_sbi_inst;
extern amf_config amf_cfg;
extern amf_app* amf_app_inst;
extern amf_n2* amf_n2_inst;
extern statistics stacs;

// Static variables
std::map<std::string, std::string> amf_n1::rand_record = {};

void amf_n1_task(void*);

//------------------------------------------------------------------------------
void amf_n1_task(void*) {
  const task_id_t task_id = TASK_AMF_N1;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();

    switch (msg->msg_type) {
      case UL_NAS_DATA_IND: {
        Logger::amf_n1().info("Received UL_NAS_DATA_IND");
        itti_uplink_nas_data_ind* m =
            dynamic_cast<itti_uplink_nas_data_ind*>(msg);
        amf_n1_inst->handle_itti_message(std::ref(*m));
      } break;

      case DOWNLINK_NAS_TRANSFER: {
        Logger::amf_n1().info("Received DOWNLINK_NAS_TRANSFER");
        itti_downlink_nas_transfer* m =
            dynamic_cast<itti_downlink_nas_transfer*>(msg);
        amf_n1_inst->handle_itti_message(std::ref(*m));
      } break;

      case TIME_OUT: {
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          switch (to->arg1_user) {
            case TASK_AMF_MOBILE_REACHABLE_TIMER_EXPIRE:
              amf_n1_inst->mobile_reachable_timer_timeout(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_AMF_IMPLICIT_DEREGISTRATION_TIMER_EXPIRE:
              amf_n1_inst->implicit_deregistration_timer_timeout(
                  to->timer_id, to->arg2_user);
              break;
            default:
              Logger::amf_n1().info(
                  "No handler for timer(%d) with arg1_user(%d) ", to->timer_id,
                  to->arg1_user);
          }
        }
      } break;

      case TERMINATE: {
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::amf_n1().info("Received terminate message");
          return;
        }
      } break;

      default:
        Logger::amf_n1().error("No handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
amf_n1::amf_n1()
    : m_amfueid2nas_context(), m_nas_context(), m_guti2nas_context() {
  if (itti_inst->create_task(TASK_AMF_N1, amf_n1_task, nullptr)) {
    Logger::amf_n1().error("Cannot create task TASK_AMF_N1");
    throw std::runtime_error("Cannot create task TASK_AMF_N1");
  }
  amfueid2nas_context = {};
  supi2nas_context    = {};
  supi2amfId          = {};
  supi2ranId          = {};
  guti2nas_context    = {};

  // EventExposure: subscribe to UE Location Report
  ee_ue_location_report_connection = event_sub.subscribe_ue_location_report(
      boost::bind(&amf_n1::handle_ue_location_change, this, _1, _2, _3));

  // EventExposure: subscribe to UE Reachability Status change
  ee_ue_reachability_status_connection =
      event_sub.subscribe_ue_reachability_status(boost::bind(
          &amf_n1::handle_ue_reachability_status_change, this, _1, _2, _3));

  // EventExposure: subscribe to UE Registration State change
  ee_ue_registration_state_connection =
      event_sub.subscribe_ue_registration_state(boost::bind(
          &amf_n1::handle_ue_registration_state_change, this, _1, _2, _3, _4,
          _5));

  // EventExposure: subscribe to UE Connectivity State change
  ee_ue_connectivity_state_connection =
      event_sub.subscribe_ue_connectivity_state(boost::bind(
          &amf_n1::handle_ue_connectivity_state_change, this, _1, _2, _3));

  // EventExposure: subscribe to UE Loss of Connectivity change
  ee_ue_loss_of_connectivity_connection =
      event_sub.subscribe_ue_loss_of_connectivity(boost::bind(
          &amf_n1::handle_ue_loss_of_connectivity_change, this, _1, _2, _3, _4,
          _5));
  // EventExposure: subscribe to UE Communication Failure Report
  ee_ue_communication_failure_connection =
      event_sub.subscribe_ue_communication_failure(boost::bind(
          &amf_n1::handle_ue_communication_failure_change, this, _1, _2, _3));

  Logger::amf_n1().startup("amf_n1 started");
}

//------------------------------------------------------------------------------
amf_n1::~amf_n1() {
  // Disconnect the boost connection
  if (ee_ue_location_report_connection.connected())
    ee_ue_location_report_connection.disconnect();
  if (ee_ue_reachability_status_connection.connected())
    ee_ue_reachability_status_connection.disconnect();
  if (ee_ue_registration_state_connection.connected())
    ee_ue_registration_state_connection.disconnect();
  if (ee_ue_connectivity_state_connection.connected())
    ee_ue_connectivity_state_connection.disconnect();
  if (ee_ue_loss_of_connectivity_connection.connected())
    ee_ue_loss_of_connectivity_connection.disconnect();
  if (ee_ue_communication_failure_connection.connected())
    ee_ue_communication_failure_connection.disconnect();
}

//------------------------------------------------------------------------------
void amf_n1::handle_itti_message(itti_downlink_nas_transfer& itti_msg) {
  uint64_t amf_ue_ngap_id         = itti_msg.amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id         = itti_msg.ran_ue_ngap_id;
  std::shared_ptr<nas_context> nc = {};
  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    return;
  }

  bstring protected_nas = nullptr;
  encode_nas_message_protected(
      nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
      NAS_MESSAGE_DOWNLINK, (uint8_t*) bdata(itti_msg.dl_nas),
      blength(itti_msg.dl_nas), protected_nas);

  if (itti_msg.is_n2sm_set) {
    // PDU Session Resource Release Command
    if (itti_msg.n2sm_info_type.compare("PDU_RES_REL_CMD") == 0) {
      auto release_command =
          std::make_shared<itti_pdu_session_resource_release_command>(
              TASK_AMF_N1, TASK_AMF_N2);
      release_command->nas            = bstrcpy(protected_nas);
      release_command->n2sm           = bstrcpy(itti_msg.n2sm);
      release_command->amf_ue_ngap_id = amf_ue_ngap_id;
      release_command->ran_ue_ngap_id = ran_ue_ngap_id;
      release_command->pdu_session_id = itti_msg.pdu_session_id;

      int ret = itti_inst->send_msg(release_command);
      if (0 != ret) {
        Logger::amf_n1().error(
            "Could not send ITTI message %s to task TASK_AMF_N2",
            release_command->get_msg_name());
      }
      // PDU Session Resource Modify Request
    } else if (itti_msg.n2sm_info_type.compare("PDU_RES_MOD_REQ") == 0) {
      auto itti_modify_request_msg =
          std::make_shared<itti_pdu_session_resource_modify_request>(
              TASK_AMF_N1, TASK_AMF_N2);
      itti_modify_request_msg->nas            = bstrcpy(protected_nas);
      itti_modify_request_msg->n2sm           = bstrcpy(itti_msg.n2sm);
      itti_modify_request_msg->amf_ue_ngap_id = amf_ue_ngap_id;
      itti_modify_request_msg->ran_ue_ngap_id = ran_ue_ngap_id;
      itti_modify_request_msg->pdu_session_id = itti_msg.pdu_session_id;

      // Get NSSAI
      std::shared_ptr<nas_context> nc = {};
      if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

      std::shared_ptr<pdu_session_context> psc = {};
      if (!amf_app_inst->find_pdu_session_context(
              nc->imsi, itti_msg.pdu_session_id, psc))
        return;

      itti_modify_request_msg->s_NSSAI.setSd(psc->snssai.sd);
      itti_modify_request_msg->s_NSSAI.setSst(psc->snssai.sst);

      int ret = itti_inst->send_msg(itti_modify_request_msg);
      if (0 != ret) {
        Logger::amf_n1().error(
            "Could not send ITTI message %s to task TASK_AMF_N2",
            itti_modify_request_msg->get_msg_name());
      }

    } else {
      std::shared_ptr<ue_context> uc = {};
      if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) return;

      if (uc->is_ue_context_request) {
        // PDU SESSION RESOURCE SETUP_REQUEST
        auto psrsr = std::make_shared<itti_pdu_session_resource_setup_request>(
            TASK_AMF_N1, TASK_AMF_N2);
        psrsr->nas            = bstrcpy(protected_nas);
        psrsr->amf_ue_ngap_id = amf_ue_ngap_id;
        psrsr->ran_ue_ngap_id = ran_ue_ngap_id;

        pdu_session_info_t item = {};
        item.n2sm               = bstrcpy(itti_msg.n2sm);
        item.is_n2sm_available  = true;
        psrsr->pdu_sessions.insert(std::pair<uint8_t, pdu_session_info_t>(
            itti_msg.pdu_session_id, item));

        int ret = itti_inst->send_msg(psrsr);
        if (0 != ret) {
          Logger::amf_n1().error(
              "Could not send ITTI message %s to task TASK_AMF_N2",
              psrsr->get_msg_name());
        }
      } else {
        // send using InitialContextSetupRequest
        uint8_t kamf[AUTH_VECTOR_LENGTH_OCTETS];
        uint8_t kgnb[AUTH_VECTOR_LENGTH_OCTETS];
        if (!nc->get_kamf(nc->security_ctx.value().vector_pointer, kamf)) {
          Logger::amf_n1().warn("No Kamf found");
          return;
        }
        uint32_t ulcount = nc->security_ctx.value().ul_count.seq_num |
                           (nc->security_ctx.value().ul_count.overflow << 8);
        Authentication_5gaka::derive_kgnb(
            ulcount, KAccessType3gppAccess, kamf, kgnb);
        oai::utils::output_wrapper::print_buffer(
            "amf_n1", "Kamf", kamf, AUTH_VECTOR_LENGTH_OCTETS);

        auto csr = std::make_shared<itti_initial_context_setup_request>(
            TASK_AMF_N1, TASK_AMF_N2);
        csr->ran_ue_ngap_id     = ran_ue_ngap_id;
        csr->amf_ue_ngap_id     = amf_ue_ngap_id;
        csr->kgnb               = blk2bstr(kgnb, AUTH_VECTOR_LENGTH_OCTETS);
        csr->nas                = bstrcpy(protected_nas);
        pdu_session_info_t item = {};
        item.n2sm               = bstrcpy(itti_msg.n2sm);
        item.is_n2sm_available  = true;
        csr->pdu_sessions.insert(std::pair<uint8_t, pdu_session_info_t>(
            itti_msg.pdu_session_id, item));
        csr->is_sr = false;  // TODO: for Service Request procedure

        int ret = itti_inst->send_msg(csr);
        if (0 != ret) {
          Logger::amf_n1().error(
              "Could not send ITTI message %s to task TASK_AMF_N2",
              csr->get_msg_name());
        }
      }
    }
  } else {
    auto dnt =
        std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
    dnt->nas            = bstrcpy(protected_nas);
    dnt->amf_ue_ngap_id = amf_ue_ngap_id;
    dnt->ran_ue_ngap_id = ran_ue_ngap_id;

    int ret = itti_inst->send_msg(dnt);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          dnt->get_msg_name());
    }
  }

  oai::utils::utils::bdestroy_wrapper(&protected_nas);
}

//------------------------------------------------------------------------------
void amf_n1::handle_itti_message(itti_uplink_nas_data_ind& nas_data_ind) {
  uint64_t amf_ue_ngap_id = nas_data_ind.amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id = nas_data_ind.ran_ue_ngap_id;

  std::string nas_context_key = amf_conv::get_ue_context_key(
      ran_ue_ngap_id, amf_ue_ngap_id);  // key for nas_context, option 1

  std::string snn =
      amf_conv::get_serving_network_name(nas_data_ind.mnc, nas_data_ind.mcc);
  Logger::amf_n1().debug("Serving network name %s", snn.c_str());

  plmn_t plmn = {};
  plmn.mnc    = nas_data_ind.mnc;
  plmn.mcc    = nas_data_ind.mcc;

  bstring received_nas_msg  = bstrcpy(nas_data_ind.nas_msg);
  bstring decoded_plain_msg = nullptr;

  std::shared_ptr<nas_context> nc = {};
  if (nas_data_ind.is_guti_valid) {
    std::string guti = nas_data_ind.guti;
    Logger::amf_n1().debug("GUTI valid %s", guti.c_str());
    if (guti_2_nas_context(guti, nc)) {
      Logger::amf_n1().debug(
          "Existing nas_context with GUTI %s: Store GUTI and update "
          "amf_ue_ngap_id/ran_ue_ngap_id",
          guti.c_str());

      nc->guti               = std::make_optional<std::string>(guti);
      nc->old_amf_ue_ngap_id = nc->amf_ue_ngap_id;
      nc->old_ran_ue_ngap_id = nc->ran_ue_ngap_id;
      nc->amf_ue_ngap_id     = nas_data_ind.amf_ue_ngap_id;
      nc->ran_ue_ngap_id     = nas_data_ind.ran_ue_ngap_id;

      Logger::amf_n1().debug(
          "Old AMF UE NGAP ID "
          "(" AMF_UE_NGAP_ID_FMT
          "), RAN UE NGAP ID "
          "(" GNB_UE_NGAP_ID_FMT ")",
          nc->old_amf_ue_ngap_id, nc->old_ran_ue_ngap_id);

      Logger::amf_n1().debug(
          "New AMF UE NGAP ID "
          "(" AMF_UE_NGAP_ID_FMT
          "), RAN UE NGAP ID "
          "(" GNB_UE_NGAP_ID_FMT ")",
          nc->amf_ue_ngap_id, nc->ran_ue_ngap_id);

      set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
      std::string supi = amf_conv::imsi_to_supi(nc->imsi);
      set_supi_2_amf_id(supi, amf_ue_ngap_id);
      set_supi_2_ran_id(supi, ran_ue_ngap_id);
      set_supi_2_nas_context(supi, nc);
    } else {
      Logger::amf_n1().error(
          "No existing nas_context with GUTI %s", nas_data_ind.guti.c_str());
      // TODO:
      // return;
    }
  } else {
    if (amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
      Logger::amf_n1().debug(
          "Existing nas_context with amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT,
          amf_ue_ngap_id);
    } else
      Logger::amf_n1().warn(
          "No existing nas_context with amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT,
          amf_ue_ngap_id);
  }

  SecurityHeaderType_t type = {};
  if (!check_security_header_type(
          type, (uint8_t*) bdata(received_nas_msg),
          blength(received_nas_msg))) {
    Logger::amf_n1().error("Not 5GS MOBILITY MANAGEMENT message");
    return;
  }

  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Received Uplink NAS Message",
      (uint8_t*) bdata(received_nas_msg), blength(received_nas_msg));

  uint8_t ulCount = 0;

  switch (type) {
    case kPlain5gsMessage: {
      Logger::amf_n1().debug("Received plain NAS message");
      decoded_plain_msg = bstrcpy(received_nas_msg);
    } break;

    case IntegrityProtected: {
      Logger::amf_n1().debug("Received integrity protected NAS message");
      ulCount =
          *((uint8_t*) bdata(received_nas_msg) +
            kSecurityProtected5gsNasMessageSequenceNumberOctet);
      Logger::amf_n1().info(
          "Integrity protected message: ulCount(%d)", ulCount);
      decoded_plain_msg = blk2bstr(
          (uint8_t*) bdata(received_nas_msg) +
              kSecurityProtected5gsNasMessageHeaderLength,
          blength(received_nas_msg) -
              kSecurityProtected5gsNasMessageHeaderLength);
    } break;

    case IntegrityProtectedAndCiphered: {
      Logger::amf_n1().debug(
          "Received integrity protected and ciphered NAS message");
    }
    case IntegrityProtectedWithNew5GNASSecurityContext: {
      Logger::amf_n1().debug(
          "Received integrity protected with new security context NAS message");
    }
    case IntegrityProtectedAndCipheredWithNew5GNASSecurityContext: {
      Logger::amf_n1().debug(
          "Received integrity protected and ciphered with new security context "
          "NAS message");
      if (nc == nullptr) {
        Logger::amf_n1().debug(
            "Abnormal condition: NAS context does not exist ...");
        return;
      }
      if (!nc->security_ctx.has_value()) {
        Logger::amf_n1().error("No Security Context found");
        return;
      }

      uint32_t mac32 = 0;
      if (!nas_message_integrity_protected(
              nc->security_ctx.value(), NAS_MESSAGE_UPLINK,
              (uint8_t*) bdata(received_nas_msg) +
                  kSecurityProtected5gsNasMessageSequenceNumberOctet,
              blength(received_nas_msg) -
                  kSecurityProtected5gsNasMessageSequenceNumberOctet,
              mac32)) {
        Logger::amf_n1().debug("IA0_5G");
      } else {
        bool isMatched      = false;
        uint8_t* buf        = (uint8_t*) bdata(received_nas_msg);
        int buf_len         = blength(received_nas_msg);
        uint32_t mac32_recv = ntohl((((uint32_t*) (buf + 2))[0]));
        Logger::amf_n1().debug(
            "Received mac32 (0x%x) from the message", mac32_recv);
        if (mac32 == mac32_recv) {
          isMatched = true;
          Logger::amf_n1().debug("Integrity matched");
          // nc->security_ctx.value().ul_count.seq_num ++;
        }
        if (!isMatched) {
          Logger::amf_n1().error("Received message not integrity matched");
          return;
        }
      }

      bstring ciphered = blk2bstr(
          (uint8_t*) bdata(received_nas_msg) +
              kSecurityProtected5gsNasMessageHeaderLength,
          blength(received_nas_msg) -
              kSecurityProtected5gsNasMessageHeaderLength);
      if (!nas_message_cipher_protected(
              nc->security_ctx.value(), NAS_MESSAGE_UPLINK, ciphered,
              decoded_plain_msg)) {
        Logger::amf_n1().error("Decrypt NAS message failure");
        oai::utils::utils::bdestroy_wrapper(&ciphered);
        return;
      }
      oai::utils::utils::bdestroy_wrapper(&ciphered);
    } break;
    default: {
      Logger::amf_n1().error("Unknown NAS Message Type");
      return;
    }
  }

  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Decoded Plain Message", (uint8_t*) bdata(decoded_plain_msg),
      blength(decoded_plain_msg));

  if (nas_data_ind.is_nas_signalling_estab_req) {
    Logger::amf_n1().debug("Received NAS Signalling Establishment request...");
    nas_signalling_establishment_request_handle(
        type, nc, nas_data_ind.ran_ue_ngap_id, nas_data_ind.amf_ue_ngap_id,
        decoded_plain_msg, snn, ulCount);
  } else {
    Logger::amf_n1().debug("Received Uplink NAS message...");
    uplink_nas_msg_handle(
        nas_data_ind.ran_ue_ngap_id, nas_data_ind.amf_ue_ngap_id,
        decoded_plain_msg, plmn);
  }
}

//------------------------------------------------------------------------------
void amf_n1::nas_signalling_establishment_request_handle(
    SecurityHeaderType_t type, std::shared_ptr<nas_context> nc,
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id, bstring plain_msg,
    std::string snn, uint8_t ulCount) {
  // Create NAS Context, or Update if existed
  if (!nc) {
    Logger::amf_n1().debug(
        "No existing nas_context with amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT
        " --> Create a new one",
        amf_ue_ngap_id);
    nc = std::shared_ptr<nas_context>(new nas_context);
    if (!nc) {
      Logger::amf_n1().error(
          "Cannot allocate memory for new nas_context, exit...");
      return;
    }
    set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
    nc->ctx_avaliability_ind = false;
    // change UE connection status CM-IDLE -> CM-CONNECTED
    nc->nas_status      = CM_CONNECTED;
    nc->amf_ue_ngap_id  = amf_ue_ngap_id;
    nc->ran_ue_ngap_id  = ran_ue_ngap_id;
    nc->serving_network = snn;
    // Stop Mobile Reachable Timer/Implicit Deregistration Timer
    itti_inst->timer_remove(nc->mobile_reachable_timer);
    itti_inst->timer_remove(nc->implicit_deregistration_timer);

    // Trigger UE Reachability Status Notify
    if (!nc->imsi.empty()) {
      std::string supi = amf_conv::imsi_to_supi(nc->imsi);
      Logger::amf_n1().debug(
          "Signal the UE Reachability Status Event notification for SUPI %s",
          supi.c_str());
      event_sub.ue_reachability_status(
          supi, CM_CONNECTED, amf_cfg.support_features.http_version);
    }
  } else {
    Logger::amf_n1().debug(
        "Existing nas_context with amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
        amf_ue_ngap_id);
    set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
  }

  uint8_t message_type =
      get_nas_message_type((uint8_t*) bdata(plain_msg), blength(plain_msg));
  Logger::amf_n1().debug("NAS message type 0x%x", message_type);

  switch (message_type) {
    case kRegistrationRequest: {
      Logger::amf_n1().debug(
          "Received Registration Request message, handling...");
      uint8_t cause = k5gmmCauseProtocolErrorUnspecified;
      if (!registration_request_handle(
              nc, ran_ue_ngap_id, amf_ue_ngap_id, snn, plain_msg, cause)) {
        // Send Registration Reject with appropriate cause
        send_registration_reject_msg(ran_ue_ngap_id, amf_ue_ngap_id, cause);
      }
    } break;

    case kServiceRequest: {
      Logger::amf_n1().debug(
          "Received Service Request message (InitialUeMessage), handling...");
      if (!nc) {
        Logger::amf_n1().error("No NAS Context found");
        return;
      }
      /*  if (!nc->security_ctx.has_value()) {
          Logger::amf_n1().error("No Security Context found");
          return;
        }
         */
      if (nc && nc->security_ctx.has_value())
        nc->security_ctx.value().ul_count.seq_num = ulCount;
      uint8_t service_reject_cause = k5gmmCauseProtocolErrorUnspecified;
      if (!service_request_handle(
              nc, ran_ue_ngap_id, amf_ue_ngap_id, plain_msg, ulCount,
              service_reject_cause)) {
        // Send Service Reject with appropriate cause
        send_service_reject(nc, service_reject_cause);
      }
    } break;

    case kDeregistrationRequestUeOriginating: {
      Logger::amf_n1().debug(
          "Received InitialUeMessage De-registration Request message, "
          "handling...");
    } break;

    default:
      Logger::amf_n1().error("No handler for NAS message 0x%x", message_type);
  }
}

//------------------------------------------------------------------------------
void amf_n1::uplink_nas_msg_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    bstring plain_msg, const plmn_t& plmn) {
  uint8_t message_type =
      get_nas_message_type((uint8_t*) bdata(plain_msg), blength(plain_msg));

  switch (message_type) {
    case kAuthenticationResponse: {
      Logger::amf_n1().debug(
          "Received Authentication Response message, handling...");
      authentication_response_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    } break;

    case kAuthenticationFailure: {
      Logger::amf_n1().debug(
          "Received Authentication Failure message, handling...");
      authentication_failure_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    } break;

    case kSecurityModeComplete: {
      Logger::amf_n1().debug(
          "Received Security Mode Complete message, handling...");
      security_mode_complete_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    } break;

    case kSecurityModeReject: {
      Logger::amf_n1().debug(
          "Received Security Mode Reject message, handling...");
      security_mode_reject_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    } break;

    case kUlNasTransport: {
      Logger::amf_n1().debug("Received UL NAS Transport message, handling...");
      ul_nas_transport_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg, plmn);
    } break;

    case kDeregistrationRequestUeOriginating: {
      Logger::amf_n1().debug(
          "Received De-registration Request message, handling...");
      ue_initiate_de_registration_handle(
          ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    } break;

    case kIdentityResponse: {
      Logger::amf_n1().debug("Received Identity Response message, handling...");
      identity_response_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    } break;

    case kRegistrationComplete: {
      Logger::amf_n1().debug(
          "Received Registration Complete message, handling...");
      registration_complete_handle(ran_ue_ngap_id, amf_ue_ngap_id, plain_msg);
    } break;

    case kServiceRequest: {
      Logger::amf_n1().debug(
          "Received Service Request message (UplinkNasTransport), handling...");
      std::shared_ptr<nas_context> nc = {};
      if (amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
        uint8_t service_reject_cause = k5gmmCauseProtocolErrorUnspecified;
        if (!service_request_handle(
                nc, ran_ue_ngap_id, amf_ue_ngap_id, plain_msg,
                service_reject_cause)) {
          // Send Service Reject with appropriate cause
          send_service_reject(nc, service_reject_cause);
        }

      } else {
        Logger::amf_n1().debug("No NAS context available");
      }
    } break;

    case kRegistrationRequest: {
      Logger::amf_n1().debug("Received Registration Request, handling...");
      std::string snn = amf_conv::get_serving_network_name(plmn.mnc, plmn.mcc);
      Logger::amf_n1().debug("Serving network name %s", snn.c_str());
      std::shared_ptr<nas_context> nc = {};
      if (amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
        uint8_t cause = k5gmmCauseProtocolErrorUnspecified;
        if (!registration_request_handle(
                nc, ran_ue_ngap_id, amf_ue_ngap_id, snn, plain_msg, cause)) {
          // Send Registration Reject with appropriate cause
          send_registration_reject_msg(ran_ue_ngap_id, amf_ue_ngap_id, cause);
        }

      } else {
        Logger::amf_n1().debug("No NAS context available");
      }
    } break;

    default: {
      Logger::amf_n1().debug(
          "Received Unknown message type 0x%x, ignoring...", message_type);
    }
  }
}

//------------------------------------------------------------------------------
bool amf_n1::check_security_header_type(
    SecurityHeaderType_t& type, const uint8_t* buffer, const uint32_t length) {
  if (length < kNasMessageMinLength) {
    return false;
  }
  uint8_t octet        = 0;
  uint8_t decoded_size = 0;
  // Decode first octet (
  DECODE_U8(buffer + decoded_size, octet, decoded_size);
  if (octet != k5gsMobilityManagementMessages) return false;

  // Decode second octet
  DECODE_U8(buffer + decoded_size, octet, decoded_size);
  if (((octet & 0x0f) >= kPlain5gsMessage) and
      ((octet & 0x0f) <=
       IntegrityProtectedAndCipheredWithNew5GNASSecurityContext)) {
    type = static_cast<SecurityHeaderType_t>(octet & 0x0f);
    // Verify the minimum length
    switch (type) {
      case kPlain5gsMessage: {
        // Don't need to check again
        return true;
      } break;
      case IntegrityProtected:
      case IntegrityProtectedAndCiphered:
      case IntegrityProtectedWithNew5GNASSecurityContext:
      case IntegrityProtectedAndCipheredWithNew5GNASSecurityContext: {
        if (length < (kNasMessageMinLength +
                      kSecurityProtected5gsNasMessageHeaderLength))
          return false;
      } break;
      default: {
        Logger::amf_n1().error("Unknown NAS Message Type");
        return false;
      }
    }
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void amf_n1::identity_response_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    bstring plain_msg) {
  auto identity_response = std::make_unique<IdentityResponse>();
  if (!identity_response->Decode(
          (uint8_t*) bdata(plain_msg), blength(plain_msg))) {
    Logger::amf_n1().error("Decode Identity Response error");
    return;
  }

  std::string imsi_str = {};
  // TODO: avoid accessing member function directly
  oai::nas::SUCI_imsi_t imsi = {};
  identity_response->Get5gsMobileIdentity().GetSuciWithSupiImsi(imsi);
  imsi_str = imsi.mcc + imsi.mnc + imsi.msin;
  Logger::amf_n1().debug("Identity Response: SUCI (%s)", imsi_str.c_str());

  std::string ue_context_key =
      amf_conv::get_ue_context_key(ran_ue_ngap_id, amf_ue_ngap_id);

  std::shared_ptr<ue_context> uc = {};
  if (amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) {
    // Update UE context
    uc->supi = amf_conv::imsi_to_supi(imsi_str);
    // associate SUPI with UC
    // Verify if there's PDU session info in the old context
    std::shared_ptr<ue_context> old_uc = {};
    if (amf_app_inst->supi_2_ue_context(uc->supi, old_uc)) {
      uc->copy_pdu_sessions(old_uc);
    }
    amf_app_inst->set_supi_2_ue_context(uc->supi, uc);
    Logger::amf_n1().debug("Update UC context, SUPI %s", uc->supi.c_str());
  }

  std::shared_ptr<nas_context> nc = {};
  if (amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
    Logger::amf_n1().debug(
        "Find nas_context by amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
        amf_ue_ngap_id);
  } else {
    nc = std::make_shared<nas_context>();
    set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
    nc->ctx_avaliability_ind = false;
  }

  // Update Nas Context
  nc->ctx_avaliability_ind = true;
  nc->nas_status           = CM_CONNECTED;
  nc->amf_ue_ngap_id       = amf_ue_ngap_id;
  nc->ran_ue_ngap_id       = ran_ue_ngap_id;
  nc->is_imsi_present      = true;
  nc->imsi                 = imsi_str;
  std::string supi         = amf_conv::imsi_to_supi(nc->imsi);
  set_supi_2_amf_id(supi, amf_ue_ngap_id);
  set_supi_2_ran_id(supi, ran_ue_ngap_id);
  // Stop Mobile Reachable Timer/Implicit Deregistration Timer
  itti_inst->timer_remove(nc->mobile_reachable_timer);
  itti_inst->timer_remove(nc->implicit_deregistration_timer);

  // Continue the Registration Procedure
  if (nc->to_be_register_by_new_suci) {
    // Update 5GMM State
    ue_info_t ue_item;
    ue_item.cm_status       = CM_CONNECTED;
    ue_item.register_status = _5GMM_COMMON_PROCEDURE_INITIATED;
    ue_item.ranid           = ran_ue_ngap_id;
    ue_item.amfid           = amf_ue_ngap_id;
    ue_item.imsi            = nc->imsi;
    if (nc->guti.has_value()) ue_item.guti = nc->guti.value();

    // Find UE context
    std::shared_ptr<ue_context> uc = {};
    if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) {
      Logger::amf_n1().warn("Cannot find the UE context");
    } else {
      ue_item.mcc    = uc->cgi.mcc;
      ue_item.mnc    = uc->cgi.mnc;
      ue_item.cellId = uc->cgi.nrCellId;
    }

    stacs.update_ue_info(ue_item);
    set_5gmm_state(nc, _5GMM_COMMON_PROCEDURE_INITIATED);

    Logger::amf_n1().debug(
        "Signal the UE Registration State Event notification for SUPI %s",
        supi.c_str());
    // event_sub.ue_registration_state(supi, _5GMM_COMMON_PROCEDURE_INITIATED,
    // 1);
    // TODO: Trigger UE Location Report
    uint8_t cause = k5gmmCauseProtocolErrorUnspecified;
    if (!run_registration_procedure(nc, cause)) {
      Logger::amf_n1().warn("Failed to run Registration procedure!");
      // TODO: send registration reject?
    }
  }
}

//------------------------------------------------------------------------------
bool amf_n1::service_request_handle(
    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,
    const uint64_t amf_ue_ngap_id, bstring nas, uint8_t& service_reject_cause) {
  std::shared_ptr<ue_context> uc = {};

  if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) {
    service_reject_cause = k5gmmCauseUeIdentityCannotBeDerived;
    return false;
  }

  // Decode Service Request to get 5G-TMSI
  auto service_request = std::make_unique<ServiceRequest>();
  int decoded_size =
      service_request->Decode((uint8_t*) bdata(nas), blength(nas));
  // oai::utils::utils::bdestroy_wrapper(&nas);

  // Validate Service Request message
  if ((decoded_size != KEncodeDecodeError)) {
    uint16_t amf_set_id = {};
    uint8_t amf_pointer = {};
    std::string tmsi    = {};
    if (service_request->Get5gSTmsi(amf_set_id, amf_pointer, tmsi)) {
      std::string guti = amf_conv::tmsi_to_guti(
          uc->tai.mcc, uc->tai.mnc, amf_cfg.guami.region_id, amf_set_id,
          amf_pointer, tmsi);

      Logger::amf_app().debug(
          "GUTI %s, 5G-TMSI %s", guti.c_str(), tmsi.c_str());
    }
  }

  // If there's no appropriate context, send Service Reject
  if (!nc or !uc or !nc->security_ctx.has_value() or
      (decoded_size == KEncodeDecodeError)) {
    Logger::amf_n1().debug(
        "Cannot find NAS/UE context, send Service Reject to UE");
    service_reject_cause = k5gmmCauseUeIdentityCannotBeDerived;
    return false;
  }

  // Otherwise, continue to process Service Request message
  set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    service_reject_cause =
        k5gmmCauseSecurityModeRejectedUnspecified;  // TODO: verify the cause
    return false;
  }

  // Prepare Service Accept
  auto service_accept = std::make_unique<ServiceAccept>();
  std::string supi    = amf_conv::imsi_to_supi(nc->imsi);

  Logger::amf_n1().debug(
      "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT
      ", ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT,
      amf_ue_ngap_id, ran_ue_ngap_id);
  Logger::amf_n1().debug("Key for PDU Session context: SUPI %s", supi.c_str());

  // Uplink Data Status
  std::optional<uint16_t> uplink_data_status_opt =
      service_request->GetUplinkDataStatus();

  // PDU Session Status
  std::optional<uint16_t> pdu_session_status_opt =
      service_request->GetPduSessionStatus();

  // Get PDU session status from Service Request
  if (!uplink_data_status_opt.has_value() or
      !pdu_session_status_opt.has_value()) {
    // Get Uplink Data Status/PDU Session Status from NAS Message Container if
    // available

    bstring plain_msg = nullptr;
    if (service_request->GetNasMessageContainer(plain_msg)) {
      if (blength(plain_msg) < kNasMessageMinLength) {
        Logger::amf_n1().debug("NAS message is too short!");
        oai::utils::utils::bdestroy_wrapper(&plain_msg);
        service_reject_cause = k5gmmCauseSemanticallyIncorrect;
        return false;
      }

      uint8_t message_type =
          get_nas_message_type((uint8_t*) bdata(plain_msg), blength(plain_msg));
      Logger::amf_n1().debug("NAS message type 0x%x", message_type);

      switch (message_type) {
        case kRegistrationRequest: {
          Logger::nas_mm().debug(
              "TODO: NAS Message Container contains a Registration Request");
        } break;

        case kServiceRequest: {
          Logger::nas_mm().debug(
              "NAS Message Container contains a Service Request, handling ...");
          auto service_request_nas = std::make_unique<ServiceRequest>();
          int decoded_size         = service_request_nas->Decode(
              (uint8_t*) bdata(plain_msg), blength(plain_msg));
          oai::utils::utils::bdestroy_wrapper(&plain_msg);
          if (decoded_size == KEncodeDecodeError) {
            Logger::nas_mm().error("Decode Service Request message error");
            service_reject_cause = k5gmmCauseSemanticallyIncorrect;
            return false;
          }

          // Get Uplink Data Status from NAS message container if not available
          if (!uplink_data_status_opt.has_value()) {
            uplink_data_status_opt = service_request_nas->GetUplinkDataStatus();
            if (!uplink_data_status_opt.has_value())
              Logger::nas_mm().debug("IE Uplink Data Status is not present");
          }

          // Get PDU Session Status from NAS message container if not available
          if (!pdu_session_status_opt.has_value()) {
            pdu_session_status_opt = service_request_nas->GetPduSessionStatus();
            if (!pdu_session_status_opt.has_value())
              Logger::nas_mm().debug("IE PDU Session Status is not present");
          }
        } break;

        default:
          Logger::nas_mm().error(
              "NAS Message Container, unknown NAS message 0x%x", message_type);
      }
    }
  }

  uint16_t pdu_session_status = 0x0000;
  if (pdu_session_status_opt.has_value())
    pdu_session_status = pdu_session_status_opt.value();

  uint16_t uplink_data_status = 0x0000;
  if (uplink_data_status_opt.has_value())
    uplink_data_status = uplink_data_status_opt.value();

  std::vector<uint8_t> pdu_session_to_be_activated = {};
  if (uplink_data_status_opt.has_value())
    get_pdu_session_to_be_activated(
        uplink_data_status, pdu_session_to_be_activated);
  else if (pdu_session_status_opt.has_value())
    get_pdu_session_to_be_activated(
        pdu_session_status, pdu_session_to_be_activated);

  // Set default value for PDU Session Reactivation Result
  uint16_t pdu_session_reactivation_result = 0x0000;
  if (uplink_data_status_opt.has_value())
    service_accept->SetPduSessionReactivationResult(
        pdu_session_reactivation_result);

  // Set default value for PDU Session Status
  if (pdu_session_status_opt.has_value())
    service_accept->SetPduSessionStatus(pdu_session_status);

  // TODO: PDU session to be released
  // TODO: PDU session reactivation result IE

  // No PDU Sessions To Be Activated
  if (pdu_session_to_be_activated.size() == 0) {
    // TODO: should be updated
    Logger::amf_n1().debug("There is no PDU session to be activated");
    service_reject_cause =
        k5gmmCauseInsufficientUpResourcesForThePduSession;  // TODO: verify the
                                                            // cause
    return false;
  } else {
    // Contact SMF to activate UP for these sessions
    // PDU SESSION RESOURCE SETUP_REQUEST
    auto psrsr = std::make_shared<itti_pdu_session_resource_setup_request>(
        TASK_AMF_N1, TASK_AMF_N2);

    for (auto& pdu_session_id : pdu_session_to_be_activated) {
      std::shared_ptr<pdu_session_context> psc = {};
      if (!amf_app_inst->find_pdu_session_context(supi, pdu_session_id, psc)) {
        Logger::amf_n1().warn(
            "No PDU Session Context with PDU Session ID %d", pdu_session_id);
      }

      if (psc and
          (psc->up_cnx_state == up_cnx_state_e::UPCNX_STATE_DEACTIVATED)) {
        amf_app_inst->trigger_pdu_session_up_activation(pdu_session_id, uc);
      }

      pdu_session_info_t item = {};
      if (psc and psc->is_n2sm_available) {
        item.n2sm              = bstrcpy(psc->n2sm);
        item.is_n2sm_available = true;
      } else {
        item.is_n2sm_available = false;
        if (uplink_data_status_opt.has_value()) {
          set_pdu_session_reactivation_result(
              pdu_session_id, pdu_session_reactivation_result);
        }
        if (pdu_session_status_opt.has_value()) {
          set_pdu_session_status_inactive(pdu_session_id, pdu_session_status);
        }
        Logger::amf_n1().debug("Cannot get PDU session information");
      }

      psrsr->pdu_sessions.insert(
          std::pair<uint8_t, pdu_session_info_t>(pdu_session_id, item));
    }

    uint32_t msg_len = service_accept->GetLength();
    Logger::nas_mm().debug("Size of Service Accept message %ld", msg_len);

    uint8_t buffer[msg_len] = {0};
    int encoded_size        = service_accept->Encode(buffer, msg_len);
    if (encoded_size == KEncodeDecodeError) {
      Logger::nas_mm().error("Encode Service Accept message error");
      service_reject_cause = k5gmmCauseSemanticallyIncorrect;
      return false;
    }

    bstring protected_nas = nullptr;
    encode_nas_message_protected(
        nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
        NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

    psrsr->nas            = bstrcpy(protected_nas);
    psrsr->amf_ue_ngap_id = amf_ue_ngap_id;
    psrsr->ran_ue_ngap_id = ran_ue_ngap_id;

    int ret = itti_inst->send_msg(psrsr);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          psrsr->get_msg_name());
      oai::utils::utils::bdestroy_wrapper(&protected_nas);
      service_reject_cause = k5gmmCauseCongestion;
      return false;
    }
    oai::utils::utils::bdestroy_wrapper(&protected_nas);
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::service_request_handle(
    std::shared_ptr<nas_context> nc, const uint32_t ran_ue_ngap_id,
    const uint64_t amf_ue_ngap_id, bstring nas, uint8_t ulCount,
    uint8_t& service_reject_cause) {
  std::shared_ptr<ue_context> uc = {};

  if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) {
    service_reject_cause = k5gmmCauseUeIdentityCannotBeDerived;
    return false;
  }

  // Decode Service Request to get 5G-TMSI
  std::unique_ptr<ServiceRequest> service_request =
      std::make_unique<ServiceRequest>();
  int decoded_size =
      service_request->Decode((uint8_t*) bdata(nas), blength(nas));
  // oai::utils::utils::bdestroy_wrapper(&nas);

  // Get the old security context if necessary
  if ((decoded_size != KEncodeDecodeError) and (!nc->guti.has_value())) {
    uint16_t amf_set_id = {};
    uint8_t amf_pointer = {};
    std::string tmsi    = {};
    if (service_request->Get5gSTmsi(amf_set_id, amf_pointer, tmsi)) {
      std::string guti = amf_conv::tmsi_to_guti(
          uc->tai.mcc, uc->tai.mnc, amf_cfg.guami.region_id, amf_set_id,
          amf_pointer, tmsi);
      Logger::amf_app().debug(
          "GUTI %s, 5G-TMSI %s", guti.c_str(), tmsi.c_str());

      // Get Security Context from old NAS Context if neccesary
      std::shared_ptr<nas_context> old_nc = {};
      if (guti_2_nas_context(guti, old_nc)) {
        Logger::amf_app().debug("Get Security Context from old NAS Context");
        nc->security_ctx =
            std::make_optional<nas_secu_ctx>(old_nc->security_ctx.value());
        // Copy Kamf
        for (uint8_t j = 0; j < MAX_5GS_AUTH_VECTORS; j++) {
          for (uint8_t i = 0; i < AUTH_VECTOR_LENGTH_OCTETS; i++) {
            nc->kamf[j][i] = old_nc->kamf[j][i];
          }
        }

        nc->security_ctx.value().ul_count.seq_num = ulCount;
        Logger::amf_app().debug(
            "Get Security Context from old NAS Context: ulcount %d", ulCount);
        nc->imsi               = old_nc->imsi;
        nc->old_ran_ue_ngap_id = old_nc->ran_ue_ngap_id;
        nc->old_amf_ue_ngap_id = old_nc->amf_ue_ngap_id;
        if (old_nc->imeisv.has_value()) {
          nc->imeisv = std::make_optional<oai::nas::IMEI_IMEISV_t>(
              old_nc->imeisv.value());
          Logger::nas_mm().debug(
              "Stored IMEISV in the new NAS Context: %s",
              nc->imeisv.value().identity.c_str());
        }
        nc->requested_nssai = old_nc->requested_nssai;
        nc->allowed_nssai   = old_nc->allowed_nssai;
        for (auto r : nc->allowed_nssai) {
          Logger::nas_mm().debug("Allowed NSSAI: %s", r.ToString().c_str());
        }
        nc->subscribed_snssai = old_nc->subscribed_snssai;
        nc->configured_nssai  = old_nc->configured_nssai;

        for (const auto& sn : nc->subscribed_snssai) {
          if (sn.first) {
            SNSSAI_t snssai = sn.second;
            Logger::amf_n1().debug(
                "Configured S-NSSAI %s", snssai.ToString().c_str());
          }
        }
        // Get UE Security Capability from old context
        nc->ue_security_capability = old_nc->ue_security_capability;

        Logger::amf_n1().debug(
            "Current ran_ue_ngap_id (" GNB_UE_NGAP_ID_FMT
            "), current amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
            nc->ran_ue_ngap_id, nc->amf_ue_ngap_id);

        Logger::amf_n1().debug(
            "Old ran_ue_ngap_id (" GNB_UE_NGAP_ID_FMT
            "), old amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
            nc->old_ran_ue_ngap_id, nc->old_amf_ue_ngap_id);
      }
    }
  }

  // If there's no appropriate context, send Service Reject
  if (!nc or !uc or !nc->security_ctx or (decoded_size == KEncodeDecodeError)) {
    service_reject_cause = k5gmmCauseUeIdentityCannotBeDerived;
    return false;
  }

  // Otherwise, continue to process Service Request message
  set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    service_reject_cause =
        k5gmmCauseSecurityModeRejectedUnspecified;  // TODO: verify the cause
    return false;
  }

  // Update UE context
  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  uc->supi         = supi;
  set_supi_2_amf_id(supi, amf_ue_ngap_id);
  set_supi_2_ran_id(supi, ran_ue_ngap_id);

  Logger::amf_n1().debug(
      "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT
      ", ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT,
      amf_ue_ngap_id, ran_ue_ngap_id);
  Logger::amf_n1().debug("Key for PDU Session context: SUPI %s", supi.c_str());

  // Get the status of PDU Session context
  std::shared_ptr<ue_context> old_uc = {};
  if (amf_app_inst->supi_2_ue_context(supi, old_uc)) {
    uc->copy_pdu_sessions(old_uc);
  }

  // Associate SUPI with UC
  amf_app_inst->set_supi_2_ue_context(supi, uc);

  // First send UEContextReleaseCommand to release old NAS signalling
  if (((nc->old_ran_ue_ngap_id != nc->ran_ue_ngap_id) and
       (nc->old_amf_ue_ngap_id != INVALID_AMF_UE_NGAP_ID))) {
    Logger::amf_n1().debug(
        "Send UEContextReleaseCommand to release the old NAS connection if "
        "necessary");

    // Get UE Context
    std::string ue_context_key = amf_conv::get_ue_context_key(
        nc->old_ran_ue_ngap_id, nc->old_amf_ue_ngap_id);
    std::shared_ptr<ue_context> uc = {};

    if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) {
      Logger::amf_app().error(
          "No UE context for ran_amf_id %s, exit", ue_context_key.c_str());
    } else {
      // uc->copy_pdu_sessions(old_uc);

      std::shared_ptr<ue_ngap_context> unc = {};
      if (!amf_n2_inst->ran_ue_id_2_ue_ngap_context(
              nc->old_ran_ue_ngap_id, uc->gnb_id, unc)) {
        Logger::amf_n1().warn(
            "No UE NGAP context with ran_ue_ngap_id (" GNB_UE_NGAP_ID_FMT ")",
            nc->old_ran_ue_ngap_id);
      } else {
        auto itti_msg = std::make_shared<itti_ue_context_release_command>(
            TASK_AMF_N1, TASK_AMF_N2);
        itti_msg->amf_ue_ngap_id = nc->old_amf_ue_ngap_id;
        itti_msg->ran_ue_ngap_id = nc->old_ran_ue_ngap_id;
        itti_msg->cause.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
        itti_msg->cause.set(3);  // TODO: remove hardcoded value cause nas(3)

        int ret = itti_inst->send_msg(itti_msg);
        if (0 != ret) {
          Logger::amf_n1().error(
              "Could not send ITTI message %s to task TASK_AMF_N2",
              itti_msg->get_msg_name());
        }
      }
    }
  }

  auto service_accept = std::make_unique<ServiceAccept>();

  // Uplink Data Status
  std::optional<uint16_t> uplink_data_status_opt =
      service_request->GetUplinkDataStatus();

  // Get PDU session status from Service Request
  std::optional<uint16_t> pdu_session_status_opt =
      service_request->GetPduSessionStatus();

  if (!uplink_data_status_opt.has_value() or
      !pdu_session_status_opt.has_value()) {
    // Get Uplink Data Status/PDU Session Status from NAS Message Container if
    // available
    bstring plain_msg = nullptr;
    if (service_request->GetNasMessageContainer(plain_msg)) {
      if (blength(plain_msg) < kNasMessageMinLength) {
        Logger::amf_n1().debug("NAS message is too short!");
        oai::utils::utils::bdestroy_wrapper(&plain_msg);
        service_reject_cause = k5gmmCauseSemanticallyIncorrect;
        return false;
      }

      uint8_t message_type =
          get_nas_message_type((uint8_t*) bdata(plain_msg), blength(plain_msg));
      Logger::amf_n1().debug("NAS message type 0x%x", message_type);

      switch (message_type) {
        case kRegistrationRequest: {
          Logger::nas_mm().debug(
              "TODO: NAS Message Container contains a Registration Request");
        } break;

        case kServiceRequest: {
          Logger::nas_mm().debug(
              "NAS Message Container contains a Service Request, handling ...");
          auto service_request_nas = std::make_unique<ServiceRequest>();

          int decoded_size = service_request_nas->Decode(
              (uint8_t*) bdata(plain_msg), blength(plain_msg));
          oai::utils::utils::bdestroy_wrapper(&plain_msg);

          if (decoded_size == KEncodeDecodeError) {
            Logger::nas_mm().error("Decode Service Request message error");
            service_reject_cause = k5gmmCauseSemanticallyIncorrect;
            return false;
          }

          // Get Uplink Data Status from NAS message container if not available
          if (!uplink_data_status_opt.has_value()) {
            uplink_data_status_opt = service_request_nas->GetUplinkDataStatus();
            if (!uplink_data_status_opt.has_value())
              Logger::nas_mm().debug("IE Uplink Data Status is not present");
          }

          // Get PDU Session Status from NAS message container if not available
          if (!pdu_session_status_opt.has_value()) {
            pdu_session_status_opt = service_request_nas->GetPduSessionStatus();
            if (!pdu_session_status_opt.has_value())
              Logger::nas_mm().debug("IE PDU Session Status is not present");
          }

        } break;

        default:
          Logger::nas_mm().error(
              "NAS Message Container, unknown NAS message 0x%x", message_type);
      }
    }
  }

  uint16_t pdu_session_status = 0x0000;
  if (pdu_session_status_opt.has_value())
    pdu_session_status = pdu_session_status_opt.value();

  uint16_t uplink_data_status = 0x0000;
  if (uplink_data_status_opt.has_value())
    uplink_data_status = uplink_data_status_opt.value();

  std::vector<uint8_t> pdu_session_to_be_activated = {};
  if (uplink_data_status_opt.has_value())
    get_pdu_session_to_be_activated(
        uplink_data_status, pdu_session_to_be_activated);
  else if (pdu_session_status_opt.has_value())
    get_pdu_session_to_be_activated(
        pdu_session_status, pdu_session_to_be_activated);

  // Set default value for PDU Session Reactivation Result
  uint16_t pdu_session_reactivation_result = 0x0000;
  if (uplink_data_status_opt.has_value())
    service_accept->SetPduSessionReactivationResult(
        pdu_session_reactivation_result);

  // Set default value for PDU Session Status
  if (pdu_session_status_opt.has_value())
    service_accept->SetPduSessionStatus(pdu_session_status);

  // TODO: PDU session to be released
  // TODO: PDU session reactivation result IE

  // No PDU Sessions To Be Activated
  if (pdu_session_to_be_activated.size() == 0) {
    Logger::amf_n1().debug("There is no PDU session to be activated");
    uint32_t msg_len = service_accept->GetLength();
    Logger::nas_mm().debug("Size of Service Accept message %ld", msg_len);
    uint8_t buffer[msg_len] = {0};
    int encoded_size        = service_accept->Encode(buffer, msg_len);
    if (encoded_size == KEncodeDecodeError) {
      Logger::nas_mm().debug("Encode Service Accept message error");
      service_reject_cause = k5gmmCauseSemanticallyIncorrect;
      return false;
    }
    bstring protected_nas = nullptr;
    encode_nas_message_protected(
        nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
        NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

    // send using InitialContextSetupRequest
    uint8_t kamf[AUTH_VECTOR_LENGTH_OCTETS];
    uint8_t kgnb[AUTH_VECTOR_LENGTH_OCTETS];
    if (!nc->get_kamf(nc->security_ctx.value().vector_pointer, kamf)) {
      Logger::amf_n1().warn("No Kamf found");
      service_reject_cause =
          k5gmmCauseSecurityModeRejectedUnspecified;  // TODO: verify the cause
      return false;
    }
    uint32_t ulcount = nc->security_ctx.value().ul_count.seq_num |
                       (nc->security_ctx.value().ul_count.overflow << 8);
    Authentication_5gaka::derive_kgnb(
        ulcount, KAccessType3gppAccess, kamf, kgnb);
    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "Kamf", kamf, AUTH_VECTOR_LENGTH_OCTETS);

    auto itti_msg = std::make_shared<itti_initial_context_setup_request>(
        TASK_AMF_N1, TASK_AMF_N2);
    itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->nas            = bstrcpy(protected_nas);
    itti_msg->kgnb           = blk2bstr(kgnb, 32);
    itti_msg->is_sr          = true;  // Service Request indicator

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          itti_msg->get_msg_name());
      oai::utils::utils::bdestroy_wrapper(&protected_nas);
      service_reject_cause = k5gmmCauseCongestion;
      return false;
    } else {
      // Update 5GMM State
      stacs.update_5gmm_state(nc, _5GMM_REGISTERED);
      set_5gmm_state(nc, _5GMM_REGISTERED);
      stacs.display();
    }
    oai::utils::utils::bdestroy_wrapper(&protected_nas);

  } else {
    auto itti_msg = std::make_shared<itti_initial_context_setup_request>(
        TASK_AMF_N1, TASK_AMF_N2);

    for (auto& pdu_session_id : pdu_session_to_be_activated) {
      std::shared_ptr<pdu_session_context> psc = {};
      if (!amf_app_inst->find_pdu_session_context(supi, pdu_session_id, psc)) {
        Logger::amf_n1().warn(
            "No PDU Session Context with PDU Session ID %d", pdu_session_id);
      }

      if (psc and
          (psc->up_cnx_state == up_cnx_state_e::UPCNX_STATE_DEACTIVATED)) {
        amf_app_inst->trigger_pdu_session_up_activation(pdu_session_id, uc);
      }

      pdu_session_info_t item = {};
      if (psc and psc->is_n2sm_available) {
        item.n2sm              = bstrcpy(psc->n2sm);
        item.is_n2sm_available = true;
      } else {
        item.is_n2sm_available = false;
        if (uplink_data_status_opt.has_value()) {
          set_pdu_session_reactivation_result(
              pdu_session_id, pdu_session_reactivation_result);
        }
        if (pdu_session_status_opt.has_value()) {
          set_pdu_session_status_inactive(pdu_session_id, pdu_session_status);
        }
        Logger::amf_n1().debug("Cannot get PDU session information");
      }

      itti_msg->pdu_sessions.insert(
          std::pair<uint8_t, pdu_session_info_t>(pdu_session_id, item));
    }

    // Set PDU Session Reactivation Result
    if (uplink_data_status_opt.has_value()) {
      service_accept->SetPduSessionReactivationResult(
          pdu_session_reactivation_result);
    }
    // Set PDU Session Status
    if (pdu_session_status_opt.has_value()) {
      service_accept->SetPduSessionStatus(pdu_session_status);
    }

    uint32_t msg_len = service_accept->GetLength();
    Logger::nas_mm().debug("Size of Service Accept message %ld", msg_len);
    uint8_t buffer[msg_len] = {0};
    int encoded_size        = service_accept->Encode(buffer, msg_len);
    if (encoded_size == KEncodeDecodeError) {
      Logger::nas_mm().error("Encode Service Accept message error");
      service_reject_cause = k5gmmCauseSemanticallyIncorrect;
      return false;
    }
    bstring protected_nas = nullptr;
    encode_nas_message_protected(
        nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
        NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

    uint8_t kamf[AUTH_VECTOR_LENGTH_OCTETS];
    uint8_t kgnb[AUTH_VECTOR_LENGTH_OCTETS];
    if (!nc->get_kamf(nc->security_ctx.value().vector_pointer, kamf)) {
      Logger::amf_n1().warn("No Kamf found");
      service_reject_cause =
          k5gmmCauseSecurityModeRejectedUnspecified;  // TODO: verify the cause
      return false;
    }
    uint32_t ulcount = nc->security_ctx.value().ul_count.seq_num |
                       (nc->security_ctx.value().ul_count.overflow << 8);
    Logger::amf_n1().debug(
        "uplink count (%d)", nc->security_ctx.value().ul_count.seq_num);
    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "Kamf", kamf, AUTH_VECTOR_LENGTH_OCTETS);
    Authentication_5gaka::derive_kgnb(
        ulcount, KAccessType3gppAccess, kamf, kgnb);

    itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->nas            = bstrcpy(protected_nas);
    itti_msg->kgnb           = blk2bstr(kgnb, AUTH_VECTOR_LENGTH_OCTETS);
    itti_msg->is_sr          = true;  // Service Request indicator

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          itti_msg->get_msg_name());
      oai::utils::utils::bdestroy_wrapper(&protected_nas);
      service_reject_cause = k5gmmCauseCongestion;
      return false;
    }
    // Update 5GMM State
    stacs.update_5gmm_state(nc, _5GMM_REGISTERED);
    set_5gmm_state(nc, _5GMM_REGISTERED);

    ue_info_t ue_item;
    ue_item.cm_status       = CM_CONNECTED;
    ue_item.register_status = _5GMM_REGISTERED;
    ue_item.ranid           = ran_ue_ngap_id;
    ue_item.amfid           = amf_ue_ngap_id;
    ue_item.imsi            = nc->imsi;
    if (nc->guti.has_value()) ue_item.guti = nc->guti.value();
    ue_item.mcc    = uc->cgi.mcc;
    ue_item.mnc    = uc->cgi.mnc;
    ue_item.cellId = uc->cgi.nrCellId;

    stacs.update_ue_info(ue_item);
    stacs.display();

    event_sub.ue_registration_state(
        supi, _5GMM_REGISTERED, amf_cfg.support_features.http_version,
        ran_ue_ngap_id, amf_ue_ngap_id);

    oai::utils::utils::bdestroy_wrapper(&protected_nas);
  }

  return true;
}

//------------------------------------------------------------------------------
void amf_n1::send_service_reject(
    std::shared_ptr<nas_context>& nc, uint8_t cause) {
  Logger::amf_n1().debug("Send Service Reject with cause %d to UE", cause);

  std::unique_ptr<ServiceReject> service_reject =
      std::make_unique<ServiceReject>();
  service_reject->Set5gmmCause(cause);

  uint32_t msg_len = service_reject->GetLength();
  Logger::nas_mm().debug("Size of Service Reject message %ld", msg_len);
  uint8_t buffer[msg_len] = {0};
  int encoded_size        = service_reject->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::amf_n1().error("Encode Service Reject message error");
    return;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Service-Reject message buffer", buffer, encoded_size);

  auto dnt = std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
  dnt->nas = blk2bstr(buffer, encoded_size);
  dnt->amf_ue_ngap_id = nc->amf_ue_ngap_id;
  dnt->ran_ue_ngap_id = nc->ran_ue_ngap_id;

  int ret = itti_inst->send_msg(dnt);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        dnt->get_msg_name());
  } else {
    // Update 5GMM State
    stacs.update_5gmm_state(nc, _5GMM_DEREGISTERED);
    set_5gmm_state(nc, _5GMM_DEREGISTERED);
    stacs.display();
  }

  return;
}

//------------------------------------------------------------------------------
bool amf_n1::registration_request_handle(
    std::shared_ptr<nas_context>& nc, const uint32_t ran_ue_ngap_id,
    const uint64_t amf_ue_ngap_id, const std::string& snn, bstring reg,
    uint8_t& cause) {
  // Decode Registration Request message
  auto registration_request = std::make_unique<RegistrationRequest>();
  int decoded_size =
      registration_request->Decode((uint8_t*) bdata(reg), blength(reg));
  if (decoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decode Registration Request message error");
    cause = k5gmmCauseSemanticallyIncorrect;
    return false;
  }

  // store Registration request in Nas Context
  nc->registration_request = blk2bstr((uint8_t*) bdata(reg), blength(reg));
  nc->registration_request_is_set = true;

  // Find UE context
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) {
    cause = k5gmmCauseIllegalUe;  // TODO: verify the cause
    return false;
  }

  // Check 5GS Mobility Identity (Mandatory IE)
  std::string guti         = {};
  uint8_t mobility_id_type = registration_request->GetMobileIdentityType();
  switch (mobility_id_type) {
    case kSuci: {
      oai::nas::SUCI_imsi_t imsi = {};
      if (!registration_request->GetSuciSupiFormatImsi(imsi)) {
        Logger::amf_n1().warn("No SUCI and IMSI for SUPI Format");
      } else {
        if (!nc) {
          Logger::amf_n1().debug(
              "No existing nas_context with amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT
              ") --> Create new one",
              amf_ue_ngap_id);
          nc = std::shared_ptr<nas_context>(new nas_context);
          set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
          nc->ctx_avaliability_ind = false;
          // Change UE connection status CM-IDLE -> CM-CONNECTED
          nc->nas_status      = CM_CONNECTED;
          nc->amf_ue_ngap_id  = amf_ue_ngap_id;
          nc->ran_ue_ngap_id  = ran_ue_ngap_id;
          nc->serving_network = snn;
          // Stop Mobile Reachable Timer/Implicit Deregistration Timer
          itti_inst->timer_remove(nc->mobile_reachable_timer);
          itti_inst->timer_remove(nc->implicit_deregistration_timer);
        }

        nc->imsi            = amf_conv::get_imsi(imsi.mcc, imsi.mnc, imsi.msin);
        nc->is_imsi_present = true;
        Logger::amf_n1().debug("Received IMSI %s", nc->imsi.c_str());

        // Trigger UE Reachability Status Notify
        if (!nc->imsi.empty()) {
          std::string supi = amf_conv::imsi_to_supi(nc->imsi);
          Logger::amf_n1().debug(
              "Signal the UE Reachability Status Event notification for SUPI "
              "%s",
              supi.c_str());
          event_sub.ue_reachability_status(
              supi, CM_CONNECTED, amf_cfg.support_features.http_version);
        }

        // Get SUPI and associate with AMF UE NGAP ID/RAN UE NGAP ID
        std::string supi = amf_conv::imsi_to_supi(nc->imsi);
        set_supi_2_amf_id(supi, amf_ue_ngap_id);
        set_supi_2_ran_id(supi, ran_ue_ngap_id);
        // Update UE context
        uc->supi = supi;

        // Try to find old nas_context and release
        std::shared_ptr<nas_context> old_nc = {};
        if (supi_2_nas_context(supi, old_nc)) {
          old_nc.reset();
        }

        // Associate SUPI with Nas context
        set_supi_2_nas_context(supi, nc);
        Logger::amf_n1().info(
            "Associating SUPI (%s) with NAS context", supi.c_str());
        // Update 5GMM state
        ue_info_t ue_item;
        ue_item.cm_status       = CM_CONNECTED;
        ue_item.register_status = _5GMM_COMMON_PROCEDURE_INITIATED;
        ue_item.ranid           = ran_ue_ngap_id;
        ue_item.amfid           = amf_ue_ngap_id;
        ue_item.imsi            = nc->imsi;
        if (nc->guti.has_value()) ue_item.guti = nc->guti.value();
        ue_item.mcc    = uc->cgi.mcc;
        ue_item.mnc    = uc->cgi.mnc;
        ue_item.cellId = uc->cgi.nrCellId;

        stacs.update_ue_info(ue_item);
        set_5gmm_state(nc, _5GMM_COMMON_PROCEDURE_INITIATED);
      }
    } break;

    case k5gGuti: {
      guti = registration_request->Get5gGuti();
      Logger::amf_n1().debug(
          "Decoded GUTI from registration request message %s", guti.c_str());
      if (guti.empty()) {
        Logger::amf_n1().warn("No GUTI IE");
      }

      // Update UE context
      if (uc != nullptr) {
        oai::utils::conv::get_tmsi_from_guti(guti, uc->tmsi);
      }

      if (nc) {
        Logger::amf_n1().debug("Exiting NAS context");
        nc->is_5g_guti_present         = true;
        nc->to_be_register_by_new_suci = true;
      } else if (guti_2_nas_context(guti, nc)) {
        Logger::amf_n1().debug(
            "NAS context existed with GUTI %s", guti.c_str());
        set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
        // Update NAS context
        nc->amf_ue_ngap_id = amf_ue_ngap_id;
        nc->ran_ue_ngap_id = ran_ue_ngap_id;
        std::string supi   = amf_conv::imsi_to_supi(nc->imsi);
        set_supi_2_amf_id(supi, amf_ue_ngap_id);
        set_supi_2_ran_id(supi, ran_ue_ngap_id);
        nc->is_auth_vectors_present       = false;
        nc->is_current_security_available = false;
        if (nc->security_ctx.has_value())
          nc->security_ctx.value().sc_type = SECURITY_CTX_TYPE_NOT_AVAILABLE;
      } else {
        Logger::amf_n1().debug(
            "No existing NAS context with amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT
            ") --> Create new one",
            amf_ue_ngap_id);
        nc = std::shared_ptr<nas_context>(new nas_context);
        if (!nc) {
          Logger::amf_n1().error(
              "Cannot allocate memory for new nas_context, exit...");
          cause = k5gmmCauseCongestion;  // TODO: verify the cause
          return false;
        }
        Logger::amf_n1().info(
            "Created a new NAS context and associated with amf_ue_ngap_id "
            "(" AMF_UE_NGAP_ID_FMT ")",
            amf_ue_ngap_id);
        set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
        nc->ctx_avaliability_ind = false;
        // change UE connection status CM-IDLE -> CM-CONNECTED
        nc->nas_status                 = CM_CONNECTED;
        nc->amf_ue_ngap_id             = amf_ue_ngap_id;
        nc->ran_ue_ngap_id             = ran_ue_ngap_id;
        nc->serving_network            = snn;
        nc->is_5g_guti_present         = true;
        nc->to_be_register_by_new_suci = true;
        nc->ngksi = 100 & 0xf;  // TODO: remove hardcoded value

        // Stop Mobile Reachable Timer/Implicit Deregistration Timer
        itti_inst->timer_remove(nc->mobile_reachable_timer);
        itti_inst->timer_remove(nc->implicit_deregistration_timer);

        // Trigger UE Reachability Status Notify
        if (!nc->imsi.empty()) {
          std::string supi = amf_conv::imsi_to_supi(nc->imsi);
          Logger::amf_n1().debug(
              "Signal the UE Reachability Status Event notification for SUPI "
              "%s",
              supi.c_str());
          event_sub.ue_reachability_status(
              supi, CM_CONNECTED, amf_cfg.support_features.http_version);
        }
      }
    } break;

    default: {
      Logger::amf_n1().warn("Unknown UE Mobility Identity");
    }
  }

  // Create NAS context
  if (nc == nullptr) {
    // try to get the GUTI -> nas_context
    if (guti_2_nas_context(guti, nc)) {
      set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
      nc->amf_ue_ngap_id = amf_ue_ngap_id;
      nc->ran_ue_ngap_id = ran_ue_ngap_id;
      std::string supi   = amf_conv::imsi_to_supi(nc->imsi);
      set_supi_2_amf_id(supi, amf_ue_ngap_id);
      set_supi_2_ran_id(supi, ran_ue_ngap_id);

      nc->is_auth_vectors_present       = false;
      nc->is_current_security_available = false;
      if (nc->security_ctx.has_value())
        nc->security_ctx.value().sc_type = SECURITY_CTX_TYPE_NOT_AVAILABLE;
    } else {
      Logger::amf_n1().error("No NAS context with GUTI (%s)", guti.c_str());
      // release ue_ngap_context and ue_context
      if (uc) uc.reset();

      std::shared_ptr<ue_ngap_context> unc = {};
      if (!amf_n2_inst->ran_ue_id_2_ue_ngap_context(
              ran_ue_ngap_id, uc->gnb_id, unc)) {
        cause = k5gmmCauseIllegalUe;  // TODO: verify the cause
        return false;
      }

      if (unc) unc.reset();

      cause = k5gmmCauseUeIdentityCannotBeDerived;  // TODO: verify the cause
      return false;
    }
  } else {
    Logger::amf_n1().debug("Existing nas_context --> Update");
    set_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id, nc);
  }

  // Update NAS Context
  nc->ran_ue_ngap_id  = ran_ue_ngap_id;
  nc->amf_ue_ngap_id  = amf_ue_ngap_id;
  nc->serving_network = snn;

  if (nc->security_ctx.has_value())
    nc->security_ctx.value().sc_type = SECURITY_CTX_TYPE_NOT_AVAILABLE;

  // Check 5GS_Registration_type IE (Mandatory IE)
  uint8_t reg_type              = 0;
  bool is_follow_on_req_pending = false;
  if (!registration_request->Get5gsRegistrationType(
          is_follow_on_req_pending, reg_type)) {
    Logger::amf_n1().error("Missing Mandatory IE 5GS Registration type...");
    cause = k5gmmCauseInvalidMandatoryInfo;
    return false;
  }
  nc->registration_type         = reg_type;
  nc->follow_on_req_pending_ind = is_follow_on_req_pending;

  // Check ngKSI (Mandatory IE)
  uint8_t ngksi = 0;
  if (!registration_request->GetNgKsi(ngksi)) {
    Logger::amf_n1().error("Missing Mandatory IE ngKSI...");
    cause = k5gmmCauseInvalidMandatoryInfo;
    return false;
  }
  nc->ngksi = ngksi;

  // Get 5GMM Capability IE (optional), not
  // included for periodic registration updating procedure
  uint8_t _5g_mm_cap = 0;
  if (!registration_request->Get5gmmCapability(_5g_mm_cap)) {
    Logger::amf_n1().warn("No Optional IE 5GMMCapability available");
  }
  nc->_5gmm_capability[0] = _5g_mm_cap;

  // Get UE Security Capability IE (optional), not included for periodic
  // registration updating procedure
  auto ue_security_capability = registration_request->GetUeSecurityCapability();
  if (ue_security_capability.has_value()) {
    nc->ue_security_capability = ue_security_capability.value();
  }

  // Get Requested NSSAI (Optional IE), if provided
  if (!registration_request->GetRequestedNssai(nc->requested_nssai)) {
    Logger::amf_n1().debug("No Optional IE RequestedNssai available");
  }

  for (auto r : nc->requested_nssai) {
    Logger::nas_mm().debug("Requested NSSAI: %s", r.ToString().c_str());
  }

  nc->ctx_avaliability_ind = true;

  // Get Last visited registered TAI(Optional IE), if provided
  // Get S1 Ue network capability(Optional IE), if ue supports S1 mode
  // Uplink Data Status
  std::optional<uint16_t> uplink_data_status_opt =
      registration_request->GetUplinkDataStatus();
  // PDU Session Status
  std::optional<uint16_t> pdu_session_status_opt =
      registration_request->GetPduSessionStatus();

  bstring nas_msg = nullptr;
  bool is_messagecontainer =
      registration_request->GetNasMessageContainer(nas_msg);

  if (is_messagecontainer) {
    auto registration_request_msg_container =
        std::make_unique<RegistrationRequest>();
    registration_request_msg_container->Decode(
        (uint8_t*) bdata(nas_msg), blength(nas_msg));

    if (!registration_request_msg_container->GetRequestedNssai(
            nc->requested_nssai)) {
      Logger::amf_n1().debug(
          "No Optional IE RequestedNssai available in NAS Container");
    } else {
      for (auto s : nc->requested_nssai) {
        Logger::amf_n1().debug(
            "Requested NSSAI inside the NAS container: %s",
            s.ToString().c_str());
      }
    }

    // Get Uplink Data Status from NAS message container if not available
    if (!uplink_data_status_opt.has_value()) {
      uplink_data_status_opt =
          registration_request_msg_container->GetUplinkDataStatus();
      if (!uplink_data_status_opt.has_value())
        Logger::nas_mm().debug("IE Uplink Data Status is not present");
    }

    // Get PDU Session Status from NAS message container if not available
    if (!pdu_session_status_opt.has_value()) {
      pdu_session_status_opt =
          registration_request_msg_container->GetPduSessionStatus();
      if (!pdu_session_status_opt.has_value())
        Logger::nas_mm().debug("IE PDU Session Status is not present");
    }

  } else {
    Logger::amf_n1().debug(
        "No Optional NAS Container inside Registration Request message");
  }

  // Update UE context
  if (uc != nullptr) {
    std::string supi = amf_conv::imsi_to_supi(nc->imsi);
    uc->supi         = supi;

    if (uplink_data_status_opt.has_value() or
        pdu_session_status_opt.has_value()) {
      // Verify if there's PDU session info in the old context
      std::shared_ptr<ue_context> old_uc = {};
      if (amf_app_inst->supi_2_ue_context(uc->supi, old_uc)) {
        uc->copy_pdu_sessions(old_uc);
      }
    }

    amf_app_inst->set_supi_2_ue_context(supi, uc);
    Logger::amf_n1().debug("Update UC context, SUPI %s", supi.c_str());
  }

  // Store NAS information into nas_context
  // Run the corresponding registration procedure
  switch (reg_type) {
    case kInitialRegistration: {
      return run_registration_procedure(nc, cause);
    } break;

    case kMobilityRegistrationUpdating: {
      Logger::amf_n1().debug("Handling Mobility Registration Update...");
      return run_mobility_registration_update_procedure(
          nc, registration_request->GetUplinkDataStatus(),
          registration_request->GetPduSessionStatus(), cause);
    } break;

    case kPeriodicRegistrationUpdating: {
      Logger::amf_n1().debug("Handling Periodic Registration Update...");
      if (is_messagecontainer)
        return run_periodic_registration_update_procedure(nc, nas_msg, cause);
      else {
        uint16_t pdu_session_status = 0x0000;
        if (pdu_session_status_opt.has_value())
          pdu_session_status = pdu_session_status_opt.value();
        return run_periodic_registration_update_procedure(
            nc, pdu_session_status, cause);
      }
    } break;

    case kEmergencyRegistration: {
      if (!amf_cfg.is_emergency_support) {
        Logger::amf_n1().error(
            "Network does not support emergency registration, reject ...");
        cause = k5gmmCause5gsServicesNotAllowed;
        return false;
      }
    } break;

    default: {
      Logger::amf_n1().error("Unknown registration type ...");
      cause = k5gmmCauseInvalidMandatoryInfo;
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::amf_ue_id_2_nas_context(
    const uint64_t& amf_ue_ngap_id, std::shared_ptr<nas_context>& nc) const {
  std::shared_lock lock(m_amfueid2nas_context);
  if (amfueid2nas_context.count(amf_ue_ngap_id) > 0) {
    if (amfueid2nas_context.at(amf_ue_ngap_id) != nullptr) {
      nc = amfueid2nas_context.at(amf_ue_ngap_id);
      return true;
    }
  }
  Logger::amf_n1().warn(
      "No NAS context with amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT "",
      amf_ue_ngap_id);
  return false;
}

//------------------------------------------------------------------------------
void amf_n1::set_amf_ue_ngap_id_2_nas_context(
    const uint64_t& amf_ue_ngap_id, std::shared_ptr<nas_context> nc) {
  std::unique_lock lock(m_amfueid2nas_context);
  amfueid2nas_context[amf_ue_ngap_id] = nc;
}

//------------------------------------------------------------------------------
bool amf_n1::remove_amf_ue_ngap_id_2_nas_context(
    const uint64_t& amf_ue_ngap_id) {
  std::unique_lock lock(m_amfueid2nas_context);
  if (amfueid2nas_context.count(amf_ue_ngap_id) > 0) {
    amfueid2nas_context.erase(amf_ue_ngap_id);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void amf_n1::set_supi_2_amf_id(
    const std::string& supi, const uint64_t& amf_ue_ngap_id) {
  std::unique_lock lock(m_nas_context);
  supi2amfId[supi] = amf_ue_ngap_id;
}

//------------------------------------------------------------------------------
bool amf_n1::supi_2_amf_id(const std::string& supi, uint64_t& amf_ue_ngap_id) {
  std::shared_lock lock(m_nas_context);
  if (supi2amfId.count(supi) > 0) {
    amf_ue_ngap_id = supi2amfId.at(supi);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
bool amf_n1::remove_supi_2_amf_id(const std::string& supi) {
  std::unique_lock lock(m_nas_context);
  if (supi2amfId.count(supi) > 0) {
    supi2amfId.erase(supi);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void amf_n1::set_supi_2_ran_id(
    const std::string& supi, const uint32_t& ran_ue_ngap_id) {
  std::unique_lock lock(m_nas_context);
  supi2ranId[supi] = ran_ue_ngap_id;
}

//------------------------------------------------------------------------------
bool amf_n1::supi_2_ran_id(const std::string& supi, uint32_t& ran_ue_ngap_id) {
  std::shared_lock lock(m_nas_context);
  if (supi2amfId.count(supi) > 0) {
    ran_ue_ngap_id = supi2ranId.at(supi);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
bool amf_n1::remove_supi_2_ran_id(const std::string& supi) {
  std::unique_lock lock(m_nas_context);
  if (supi2ranId.count(supi) > 0) {
    supi2ranId.erase(supi);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
bool amf_n1::guti_2_nas_context(
    const std::string& guti, std::shared_ptr<nas_context>& nc) const {
  std::shared_lock lock(m_guti2nas_context);
  if (guti2nas_context.count(guti) > 0) {
    if (guti2nas_context.at(guti) != nullptr) {
      nc = guti2nas_context.at(guti);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void amf_n1::set_guti_2_nas_context(
    const std::string& guti, const std::shared_ptr<nas_context>& nc) {
  std::unique_lock lock(m_guti2nas_context);
  guti2nas_context[guti] = nc;
}

//------------------------------------------------------------------------------
bool amf_n1::remove_guti_2_nas_context(const std::string& guti) {
  std::unique_lock lock(m_guti2nas_context);
  if (guti2nas_context.count(guti) > 0) {
    guti2nas_context.erase(guti);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool amf_n1::supi_2_nas_context(
    const std::string& imsi, std::shared_ptr<nas_context>& nc) const {
  std::shared_lock lock(m_nas_context);
  if (supi2nas_context.count(imsi) > 0) {
    if (!supi2nas_context.at(imsi)) return false;
    nc = supi2nas_context.at(imsi);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void amf_n1::set_supi_2_nas_context(
    const std::string& imsi, const std::shared_ptr<nas_context>& nc) {
  std::unique_lock lock(m_nas_context);
  supi2nas_context[imsi] = nc;
}

//------------------------------------------------------------------------------
bool amf_n1::remove_supi_2_nas_context(const std::string& imsi) {
  std::unique_lock lock(m_nas_context);
  if (supi2nas_context.count(imsi) > 0) {
    supi2nas_context.erase(imsi);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void amf_n1::itti_send_dl_nas_buffer_to_task_n2(
    bstring& nas_msg, const uint32_t ran_ue_ngap_id,
    const uint64_t amf_ue_ngap_id) {
  auto msg = std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
  msg->ran_ue_ngap_id = ran_ue_ngap_id;
  msg->amf_ue_ngap_id = amf_ue_ngap_id;
  msg->nas            = bstrcpy(nas_msg);

  int ret = itti_inst->send_msg(msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_n1::send_registration_reject_msg(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    uint8_t cause_value) {
  Logger::amf_n1().debug("Create Registration Reject and send to UE");
  auto registration_reject = std::make_unique<RegistrationReject>();
  registration_reject->Set5gmmCause(cause_value);

  uint32_t msg_len = registration_reject->GetLength();
  Logger::nas_mm().debug("Size of Registration Reject message %ld", msg_len);
  uint8_t buffer[msg_len] = {0};
  int encoded_size        = registration_reject->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::amf_n1().error("Encode Registration Reject message error");
    return;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Registration-Reject message buffer", buffer, encoded_size);

  bstring b = blk2bstr(buffer, encoded_size);
  itti_send_dl_nas_buffer_to_task_n2(b, ran_ue_ngap_id, amf_ue_ngap_id);

  // Trigger CommunicationFailure Report notify
  oai::model::amf::CommunicationFailure comm_failure = {};
  std::shared_ptr<ue_context> uc                     = {};
  if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) {
    Logger::amf_n1().warn(
        "Cannot find the UE context, unable to notify CommunicationFailure "
        "Report");
    return;
  }
  std::string supi = uc->supi;
  Logger::amf_n1().debug(
      "Signal the UE CommunicationFailure Report Event notification for SUPI "
      "%s",
      supi.c_str());
  comm_failure.setNasReleaseCode(std::to_string(cause_value));
  event_sub.ue_communication_failure(
      supi, comm_failure, amf_cfg.support_features.http_version);
}

//------------------------------------------------------------------------------
bool amf_n1::run_registration_procedure(
    std::shared_ptr<nas_context>& nc, uint8_t& cause) {
  Logger::amf_n1().debug("Start to run Registration Procedure");
  if (!nc->ctx_avaliability_ind) {
    Logger::amf_n1().error("NAS context is not available");
    cause = k5gmmCauseIllegalUe;  // TODO: verify the cause
    return false;
  }

  nc->is_specific_procedure_for_registration_running = true;
  if (nc->is_imsi_present) {
    Logger::amf_n1().debug("SUCI SUPI format IMSI is available");
    if (!nc->is_auth_vectors_present) {
      Logger::amf_n1().debug(
          "Authentication vector in nas_context is not available");
      if (auth_vectors_generator(nc)) {
        ngksi_t ngksi = 0;
        if (nc->security_ctx.has_value() &&
            nc->ngksi != kNasKeySetIdentifierNotAvailable) {
          ngksi = (nc->amf_ue_ngap_id + 1);  // % (NGKSI_MAX_VALUE + 1);
        }
        nc->ngksi = ngksi;
      } else {
        Logger::amf_n1().error("Request Authentication Vectors failure");
        cause = k5gmmCauseIllegalUe;  // TODO: verify the cause
        return false;
      }
    } else {
      Logger::amf_n1().debug(
          "Authentication Vector in nas_context is available");
      ngksi_t ngksi = 0;
      if (nc->security_ctx.has_value() &&
          nc->ngksi != kNasKeySetIdentifierNotAvailable) {
        ngksi = (nc->amf_ue_ngap_id + 1);  // % (NGKSI_MAX_VALUE + 1);
        Logger::amf_n1().debug("New ngKSI (%d)", ngksi);
        // TODO: How to handle?
      }
      nc->ngksi = ngksi;
    }

    handle_auth_vector_successful_result(nc);

  } else if (nc->is_5g_guti_present) {
    Logger::amf_n1().debug("Start to run UE Identification Request procedure");
    nc->is_auth_vectors_present = false;
    auto identity_request       = std::make_unique<IdentityRequest>();
    identity_request->Set5gsIdentityType(kSuci);

    uint32_t msg_len = identity_request->GetLength();
    Logger::nas_mm().debug("Size of Identity Request message %ld", msg_len);
    uint8_t buffer[msg_len] = {0};
    int encoded_size        = identity_request->Encode(buffer, msg_len);
    if (encoded_size == KEncodeDecodeError) {
      Logger::nas_mm().error("Encode Identity Request message error");
      cause = k5gmmCauseSemanticallyIncorrect;
      return false;
    }
    auto dnt =
        std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
    dnt->nas            = blk2bstr(buffer, encoded_size);
    dnt->amf_ue_ngap_id = nc->amf_ue_ngap_id;
    dnt->ran_ue_ngap_id = nc->ran_ue_ngap_id;

    int ret = itti_inst->send_msg(dnt);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          dnt->get_msg_name());
      cause = k5gmmCauseCongestion;
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::auth_vectors_generator(std::shared_ptr<nas_context>& nc) {
  Logger::amf_n1().debug("Start to generate Authentication Vectors");
  if (amf_cfg.support_features.enable_external_ausf_udm) {
    // get authentication vectors from AUSF
    if (!get_authentication_vectors_from_ausf(nc)) return false;
  } else {  // Generate locally
    if (!authentication::get_instance().authentication_vectors_generator_in_udm(
            nc))
      return false;
    if (!authentication::get_instance()
             .authentication_vectors_generator_in_ausf(nc))
      return false;
    Logger::amf_n1().debug("Deriving kamf");
    for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
      Authentication_5gaka::derive_kamf(
          nc->imsi, nc->_5g_av[i].kseaf, nc->kamf[i],
          0x0000);  // second parameter: abba
                    // TODO: remove hardcoded value
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::get_authentication_vectors_from_ausf(
    std::shared_ptr<nas_context>& nc) {
  Logger::amf_n1().debug("Get Authentication Vectors from AUSF");
  // TODO: remove naked ptr

  UEAuthenticationCtx ue_authentication_ctx    = {};
  AuthenticationInfo authentication_info       = {};
  ResynchronizationInfo resynchronization_info = {};

  authentication_info.setSupiOrSuci(nc->imsi);
  authentication_info.setServingNetworkName(nc->serving_network);
  uint8_t auts_len    = blength(nc->auts);           // TODO
  uint8_t* auts_value = (uint8_t*) bdata(nc->auts);  // TODO

  std::string authentication_info_auts = {};
  std::string authentication_info_rand = {};

  if (auts_value) {
    Logger::amf_n1().debug("has AUTS");
    char* auts_s = (char*) malloc(auts_len * 2 + 1);
    memset(auts_s, 0, auts_len * 2);

    Logger::amf_n1().debug("AUTS len (%d)", auts_len);
    for (int i = 0; i < auts_len; i++) {
      sprintf(&auts_s[i * 2], "%02X", auts_value[i]);
    }

    authentication_info_auts = auts_s;
    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "AUTS", auts_value, auts_len);
    Logger::amf_n1().info("ausf_s (%s)", auts_s);

    std::map<std::string, std::string>::iterator iter;
    iter = rand_record.find(nc->imsi);
    if (iter != rand_record.end()) {
      authentication_info_rand = iter->second;
      Logger::amf_n1().info("rand_s (%s)", authentication_info_rand.c_str());
    } else {
      Logger::amf_n1().error("There's no last RAND");
    }

    resynchronization_info.setAuts(authentication_info_auts);
    resynchronization_info.setRand(authentication_info_rand);
    authentication_info.setResynchronizationInfo(resynchronization_info);
    oai::utils::utils::free_wrapper((void**) &auts_s);
  }

  // Get UE Authentication from AUSF
  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = amf_app_inst->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);
  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  amf_app_inst->add_promise(promise_id, p);

  std::shared_ptr<itti_sbi_ue_authentication_request> itti_msg =
      std::make_shared<itti_sbi_ue_authentication_request>(
          TASK_AMF_N1, TASK_AMF_SBI, promise_id);

  itti_msg->http_version = amf_cfg.support_features.http_version;
  itti_msg->auth_info    = authentication_info;
  itti_msg->promise_id   = promise_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }

  bool is_result_available = true;
  // Wait for the response available and process accordingly
  std::optional<nlohmann::json> result_opt = std::nullopt;
  oai::utils::utils::wait_for_result(f, result_opt);
  if (result_opt.has_value()) {
    nlohmann::json result = result_opt.value();
    Logger::amf_n1().debug("Got result for promise ID %ld", promise_id);
    if (result.find(kSbiResponseJsonData) != result.end()) {
      Logger::amf_n1().debug(
          "Got UE Authentication from AUSF: %s",
          result[kSbiResponseJsonData].dump());
      try {
        from_json(result[kSbiResponseJsonData], ue_authentication_ctx);
        is_result_available = true;
      } catch (std::exception& e) {
        Logger::amf_n1().warn("Could not parse UE Authentication from Json");
        is_result_available = false;
      }
    } else {
      is_result_available = false;
    }

  } else {
    Logger::amf_n1().debug(
        "Could not get Authorized Network Slice Info from NSSF");
    is_result_available = false;
  }

  if (!is_result_available) {
    Logger::amf_n1().info("Could not get expected response from AUSF");
    return false;
  }

  // Process the response
  unsigned char* r5g_auth_data_rand = amf_conv::format_string_as_hex(
      ue_authentication_ctx.getR5gAuthData().getRand());
  memcpy(nc->_5g_av[0].rand, r5g_auth_data_rand, RAND_LENGTH_OCTETS);
  rand_record[nc->imsi] = ue_authentication_ctx.getR5gAuthData().getRand();
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "5G AV: RAND", nc->_5g_av[0].rand, RAND_LENGTH_OCTETS);
  oai::utils::utils::free_wrapper((void**) &r5g_auth_data_rand);

  unsigned char* r5g_auth_data_autn = amf_conv::format_string_as_hex(
      ue_authentication_ctx.getR5gAuthData().getAutn());
  memcpy(nc->_5g_av[0].autn, r5g_auth_data_autn, AUTN_LENGTH_OCTETS);
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "5G AV: AUTN", nc->_5g_av[0].autn, AUTN_LENGTH_OCTETS);
  oai::utils::utils::free_wrapper((void**) &r5g_auth_data_autn);

  unsigned char* r5g_auth_data_hxresstar = amf_conv::format_string_as_hex(
      ue_authentication_ctx.getR5gAuthData().getHxresStar());
  memcpy(nc->_5g_av[0].hxresStar, r5g_auth_data_hxresstar, HXRES_LENGTH_OCTETS);
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "5G AV: hxres*", nc->_5g_av[0].hxresStar, HXRES_LENGTH_OCTETS);
  oai::utils::utils::free_wrapper((void**) &r5g_auth_data_hxresstar);

  std::map<std::string, LinksValueSchema>::iterator iter;
  iter = ue_authentication_ctx.getLinks().find("5G_AKA");

  if (iter != ue_authentication_ctx.getLinks().end()) {
    nc->href = iter->second.getHref();
    Logger::amf_n1().info("Links is: %s", nc->href.c_str());
  } else {
    Logger::amf_n1().error("Not found 5G_AKA");
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::_5g_aka_confirmation_from_ausf(
    std::shared_ptr<nas_context>& nc, bstring resStar) {
  Logger::amf_n1().debug("5G AKA Confirmation from AUSF");
  if (!nc) return false;
  std::string res_star_string = {};

  std::map<std::string, std::string>::iterator iter;
  iter = rand_record.find(nc->imsi);
  rand_record.erase(iter);
  uint8_t res_star_len    = blength(resStar);
  uint8_t* res_star_value = (uint8_t*) bdata(resStar);
  char* res_star_s        = (char*) malloc(res_star_len * 2 + 1);

  for (int i = 0; i < res_star_len; i++) {
    sprintf(&res_star_s[i * 2], "%02X", res_star_value[i]);
  }
  res_star_string = res_star_s;
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "resStar", res_star_value, res_star_len);
  Logger::amf_n1().info("resStar_s (%s)", res_star_s);

  nlohmann::json confirmation_data_json = {};
  ConfirmationData confirmation_data    = {};
  confirmation_data.setResStar(res_star_string);

  to_json(confirmation_data_json, confirmation_data);

  // Send Confirmation Data to AUSF
  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = amf_app_inst->generate_promise_id();
  Logger::amf_n1().debug("Promise ID generated %d", promise_id);
  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  amf_app_inst->add_promise(promise_id, p);

  std::shared_ptr<itti_sbi_ue_authentication_confirmation> itti_msg =
      std::make_shared<itti_sbi_ue_authentication_confirmation>(
          TASK_AMF_N1, TASK_AMF_SBI, promise_id);

  itti_msg->confirmation_data = confirmation_data_json;
  itti_msg->promise_id        = promise_id;
  itti_msg->uri               = nc->href;
  itti_msg->http_version      = amf_cfg.support_features.http_version;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }

  // Wait and process the response
  ConfirmationDataResponse confirmation_data_response = {};
  bool is_result_available                            = true;
  // Wait for the response available and process accordingly
  std::optional<nlohmann::json> result_opt = std::nullopt;
  oai::utils::utils::wait_for_result(f, result_opt);
  if (result_opt.has_value()) {
    nlohmann::json result = result_opt.value();
    Logger::amf_n1().debug("Got result for promise ID %ld", promise_id);
    if (result.find(kSbiResponseJsonData) != result.end()) {
      Logger::amf_n1().debug(
          "Got ConfirmationDataResponse from AUSF: %s",
          result[kSbiResponseJsonData].dump());
      try {
        from_json(result[kSbiResponseJsonData], confirmation_data_response);
        is_result_available = true;

        if (!confirmation_data_response.kseafIsSet()) return false;
        unsigned char* kseaf_hex = amf_conv::format_string_as_hex(
            confirmation_data_response.getKseaf());
        memcpy(nc->_5g_av[0].kseaf, kseaf_hex, AUTH_VECTOR_LENGTH_OCTETS);
        oai::utils::output_wrapper::print_buffer(
            "amf_n1", "5G AV: kseaf", nc->_5g_av[0].kseaf,
            AUTH_VECTOR_LENGTH_OCTETS);
        oai::utils::utils::free_wrapper((void**) &kseaf_hex);

        Logger::amf_n1().debug("Deriving Kamf");
        for (int i = 0; i < MAX_5GS_AUTH_VECTORS; i++) {
          Authentication_5gaka::derive_kamf(
              nc->imsi, nc->_5g_av[i].kseaf, nc->kamf[i],
              0x0000);  // second parameter: abba
          oai::utils::output_wrapper::print_buffer(
              "amf_n1", "Kamf", nc->kamf[i], AUTH_VECTOR_LENGTH_OCTETS);
        }

      } catch (std::exception& e) {
        Logger::amf_n1().warn(
            "Could not parse Confirmation Data Response from Json");
        is_result_available = false;
      }
    } else {
      is_result_available = false;
    }

  } else {
    Logger::amf_n1().debug(
        "Could not get Confirmation Data Response from AUSF");
    is_result_available = false;
  }

  if (!is_result_available) {
    Logger::amf_n1().info("Could not get expected response from AUSF");
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void amf_n1::handle_auth_vector_successful_result(
    std::shared_ptr<nas_context>& nc) {
  Logger::amf_n1().debug(
      "Received Security Vectors, try to setup security with the UE");
  nc->is_auth_vectors_present = true;
  ngksi_t ngksi               = 0;
  if (!nc->security_ctx.has_value()) {
    nc->security_ctx                 = std::make_optional<nas_secu_ctx>();
    nc->security_ctx.value().sc_type = SECURITY_CTX_TYPE_NOT_AVAILABLE;
    if (nc->security_ctx.has_value() &&
        nc->ngksi != kNasKeySetIdentifierNotAvailable)
      ngksi = (nc->amf_ue_ngap_id + 1) % (NGKSI_MAX_VALUE + 1);
    // ensure which vector is available?
    nc->ngksi = ngksi;
  }
  int vindex = nc->security_ctx.value().vector_pointer;
  if (!start_authentication_procedure(nc, vindex, nc->ngksi)) {
    Logger::amf_n1().error("Start Authentication Procedure Failure, reject...");
    Logger::amf_n1().error(
        "Ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT, nc->ran_ue_ngap_id);
    send_registration_reject_msg(
        nc->ran_ue_ngap_id, nc->amf_ue_ngap_id,
        k5gmmCauseInvalidMandatoryInfo);  // cause?
  } else {
    // update mm state -> COMMON-PROCEDURE-INITIATED
  }
}

//------------------------------------------------------------------------------
bool amf_n1::start_authentication_procedure(
    std::shared_ptr<nas_context>& nc, int vindex, uint8_t ngksi) {
  Logger::amf_n1().debug("Starting Authentication procedure");
  if (check_nas_common_procedure_on_going(nc)) {
    Logger::amf_n1().error("Existed NAS common procedure on going, reject...");
    send_registration_reject_msg(
        nc->ran_ue_ngap_id, nc->amf_ue_ngap_id,
        k5gmmCauseInvalidMandatoryInfo);  // cause?
    return false;
  }

  nc->is_common_procedure_for_authentication_running = true;
  auto auth_request = std::make_unique<AuthenticationRequest>();
  auth_request->SetNgKsi(kNasKeySetIdentifierNative, ngksi);
  uint8_t abba[2];
  abba[0] = 0x00;
  abba[1] = 0x00;
  auth_request->SetAbba(2, abba);
  auth_request->SetAuthenticationParameterRand(nc->_5g_av[vindex].rand);
  Logger::amf_n1().debug("Sending Authentication Request with RAND");
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "RAND", nc->_5g_av[vindex].rand,
      kAuthenticationParameterRandValueLength);

  uint8_t* autn = nc->_5g_av[vindex].autn;
  if (autn) auth_request->SetAuthenticationParameterAutn(autn);

  uint32_t msg_len = auth_request->GetLength();
  Logger::nas_mm().debug("Size of Authentication Request message %ld", msg_len);

  uint8_t buffer[msg_len] = {0};
  int encoded_size        = auth_request->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encode Authentication Request message error");
    return false;
  }

  bstring b = blk2bstr(buffer, encoded_size);
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Authentication-Request message buffer", (uint8_t*) bdata(b),
      blength(b));
  Logger::amf_n1().debug(
      "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT, nc->amf_ue_ngap_id);
  itti_send_dl_nas_buffer_to_task_n2(b, nc->ran_ue_ngap_id, nc->amf_ue_ngap_id);
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::check_nas_common_procedure_on_going(
    std::shared_ptr<nas_context>& nc) {
  if (nc->is_common_procedure_for_authentication_running) {
    Logger::amf_n1().debug("Existed Authentication procedure is running");
    return true;
  }
  if (nc->is_common_procedure_for_identification_running) {
    Logger::amf_n1().debug("Existed Identification procedure is running");
    return true;
  }
  if (nc->is_common_procedure_for_security_mode_control_running) {
    Logger::amf_n1().debug("Existed SMC procedure is running");
    return true;
  }
  if (nc->is_common_procedure_for_nas_transport_running) {
    Logger::amf_n1().debug("Existed NAS transport procedure is running");
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void amf_n1::authentication_response_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    bstring plain_msg) {
  std::shared_ptr<nas_context> nc = {};

  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
    send_registration_reject_msg(
        ran_ue_ngap_id, amf_ue_ngap_id,
        k5gmmCauseIllegalUe);  // cause?
    return;
  }

  Logger::amf_n1().info(
      "Found nas_context (%p) with amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
      (void*) nc.get(), amf_ue_ngap_id);
  // Stop timer? common procedure finished!
  nc->is_common_procedure_for_authentication_running = false;
  // MM state: COMMON-PROCEDURE-INITIATED -> DEREGISTRED
  // Decode AUTHENTICATION RESPONSE message
  auto auth_response = std::make_unique<AuthenticationResponse>();

  auth_response->Decode((uint8_t*) bdata(plain_msg), blength(plain_msg));
  bstring resStar = nullptr;
  bool isAuthOk   = true;
  // Get response RES*
  if (!auth_response->GetAuthenticationResponseParameter(resStar)) {
    Logger::amf_n1().warn(
        "Cannot receive AuthenticationResponseParameter (RES*)");
  } else {
    if (amf_cfg.support_features.enable_external_ausf_udm) {
      // std::string data = bdata(resStar);
      if (!_5g_aka_confirmation_from_ausf(nc, resStar)) isAuthOk = false;
    } else {
      // Get stored XRES*
      int secu_index = 0;
      if (nc->security_ctx.has_value())
        secu_index = nc->security_ctx.value().vector_pointer;

      uint8_t* hxresStar = nc->_5g_av[secu_index].hxresStar;
      // Calculate HRES* from received RES*, then compare with XRES stored in
      // nas_context
      if (hxresStar) {
        uint8_t inputstring[32];
        uint8_t* res = (uint8_t*) bdata(resStar);
        Logger::amf_n1().debug("Start to calculate HRES* from received RES*");
        memcpy(&inputstring[0], nc->_5g_av[secu_index].rand, 16);
        memcpy(&inputstring[16], res, blength(resStar));
        unsigned char sha256Out[Sha256::DIGEST_SIZE];
        authentication::get_instance().apply_sha256(
            (unsigned char*) inputstring, 16 + blength(resStar), sha256Out);
        uint8_t hres[16];
        for (int i = 0; i < 16; i++) hres[i] = (uint8_t) sha256Out[i];
        oai::utils::output_wrapper::print_buffer(
            "amf_n1", "Received RES* From Authentication-Response", res, 16);
        oai::utils::output_wrapper::print_buffer(
            "amf_n1", "Stored XRES* in 5G HE AV",
            nc->_5g_he_av[secu_index].xresStar, 16);
        oai::utils::output_wrapper::print_buffer(
            "amf_n1", "Stored XRES in 5G HE AV", nc->_5g_he_av[secu_index].xres,
            8);
        oai::utils::output_wrapper::print_buffer(
            "amf_n1", "Computed HRES* from RES*", hres, 16);
        oai::utils::output_wrapper::print_buffer(
            "amf_n1", "Computed HXRES* from XRES*", hxresStar, 16);
        for (int i = 0; i < 16; i++) {
          if (hxresStar[i] != hres[i]) isAuthOk = false;
        }
      } else {
        isAuthOk = false;
      }
    }
  }

  // If success, start SMC procedure; else if failure, response registration
  // reject message with corresponding cause
  if (!isAuthOk) {
    Logger::amf_n1().error(
        "Authentication failed for UE with amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT,
        amf_ue_ngap_id);
    send_registration_reject_msg(
        ran_ue_ngap_id, amf_ue_ngap_id,
        k5gmmCauseIllegalUe);  // cause?
    return;
  } else {
    Logger::amf_n1().debug("Authentication successful by network!");
    // Fix to work with ng4T
    // TODO: To verify UE/AMF behavior according to 3GPP TS 24.501
    // if (!nc->is_current_security_available) {
    if (!start_security_mode_control_procedure(nc)) {
      Logger::amf_n1().error("Start SMC procedure failure");
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::authentication_failure_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    bstring plain_msg) {
  std::shared_ptr<nas_context> nc = {};
  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) {
    send_registration_reject_msg(
        ran_ue_ngap_id, amf_ue_ngap_id,
        k5gmmCauseIllegalUe);  // cause?
    return;
  }

  nc->is_common_procedure_for_authentication_running = false;
  // 1. decode AUTHENTICATION FAILURE message
  auto auth_failure = std::make_unique<AuthenticationFailure>();

  int decoded_size =
      auth_failure->Decode((uint8_t*) bdata(plain_msg), blength(plain_msg));
  if (decoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decode Registration Request message error");
    send_registration_reject_msg(
        ran_ue_ngap_id, amf_ue_ngap_id, k5gmmCauseSemanticallyIncorrect);
    return;
  }

  uint8_t mm_cause = auth_failure->Get5gmmCause();
  if (mm_cause == -1) {
    Logger::amf_n1().error("Missing mandatory IE 5G_MM_CAUSE");
    send_registration_reject_msg(
        ran_ue_ngap_id, amf_ue_ngap_id,
        k5gmmCauseInvalidMandatoryInfo);  // cause?
    return;
  }

  switch (mm_cause) {
    case k5gmmCauseSynchFailure: {
      Logger::amf_n1().debug("Initial new Authentication procedure");
      bstring auts = nullptr;
      if (!auth_failure->GetAuthenticationFailureParameter(auts)) {
        Logger::amf_n1().warn(
            "IE Authentication Failure Parameter (AUTS) not received");
      }
      nc->auts = auts;
      oai::utils::output_wrapper::print_buffer(
          "amf_n1", "Received AUTS", (uint8_t*) bdata(auts), blength(auts));

      if (auth_vectors_generator(nc)) {
        handle_auth_vector_successful_result(nc);
      } else {
        Logger::amf_n1().error("Request Authentication Vectors failure");
        send_registration_reject_msg(
            nc->ran_ue_ngap_id, nc->amf_ue_ngap_id,
            k5gmmCauseIllegalUe);  // cause?
      }
      // authentication_failure_synch_failure_handle(nc, auts);
    } break;

    case k5gmmCauseNgksiAlreadyInUse: {
      Logger::amf_n1().debug(
          "ngKSI already in use, select a new ngKSI and restart the "
          "Authentication procedure!");
      // select new ngKSI and resend Authentication Request
      ngksi_t ngksi =
          (nc->ngksi + 1) % (NGKSI_MAX_VALUE + 1);  // To be verified
      nc->ngksi = ngksi;

      if (!nc->security_ctx.has_value()) {
        Logger::amf_n1().error("No Security Context found");
        // TODO:
        return;
      }
      int vindex = nc->security_ctx.value().vector_pointer;
      if (!start_authentication_procedure(nc, vindex, nc->ngksi)) {
        Logger::amf_n1().error(
            "Start Authentication procedure failure, reject...");
        Logger::amf_n1().error(
            "Ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT, nc->ran_ue_ngap_id);
        send_registration_reject_msg(
            nc->ran_ue_ngap_id, nc->amf_ue_ngap_id,
            k5gmmCauseInvalidMandatoryInfo);
      } else {
        // update mm state -> COMMON-PROCEDURE-INITIATED
      }

    } break;
    default: {
      Logger::amf_n1().warn(
          "Unknown Authentication Failure's cause %d", mm_cause);
      // TODO:
    }
  }
}

//------------------------------------------------------------------------------
bool amf_n1::start_security_mode_control_procedure(
    std::shared_ptr<nas_context>& nc) {
  Logger::amf_n1().debug("Start Security Mode Control procedure");
  nc->is_common_procedure_for_security_mode_control_running = true;
  bool security_context_is_new                              = false;
  uint8_t amf_nea                                           = kEa0_5g;
  uint8_t amf_nia                                           = kIa0_5g;
  // Decide which ea/ia alg used by UE, which is supported by network

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    return false;
  }

  if (nc->security_ctx.value().sc_type == SECURITY_CTX_TYPE_NOT_AVAILABLE &&
      nc->is_common_procedure_for_security_mode_control_running) {
    Logger::amf_n1().debug(
        "Using IntegrityProtectedWithNewSecurityContext for "
        "SecurityModeControl "
        "message");
    nc->security_ctx.value().ngksi             = nc->ngksi;
    nc->security_ctx.value().dl_count.overflow = 0;
    nc->security_ctx.value().dl_count.seq_num  = 0;
    nc->security_ctx.value().ul_count.overflow = 0;
    nc->security_ctx.value().ul_count.seq_num  = 0;
    security_select_algorithms(
        nc->ue_security_capability.GetEa(), nc->ue_security_capability.GetIa(),
        amf_nea, amf_nia);
    nc->security_ctx.value().nas_algs.integrity  = amf_nia;
    nc->security_ctx.value().nas_algs.encryption = amf_nea;
    nc->security_ctx.value().sc_type = SECURITY_CTX_TYPE_FULL_NATIVE;
    Authentication_5gaka::derive_knas(
        NAS_INT_ALG, nc->security_ctx.value().nas_algs.integrity,
        nc->kamf[nc->security_ctx.value().vector_pointer],
        nc->security_ctx.value().knas_int);
    Authentication_5gaka::derive_knas(
        NAS_ENC_ALG, nc->security_ctx.value().nas_algs.encryption,
        nc->kamf[nc->security_ctx.value().vector_pointer],
        nc->security_ctx.value().knas_enc);
    security_context_is_new           = true;
    nc->is_current_security_available = true;
  }

  auto smc = std::make_unique<SecurityModeCommand>();
  smc->SetNasSecurityAlgorithms(amf_nea, amf_nia);
  Logger::amf_n1().debug("Encoded ngKSI 0x%x", nc->ngksi);
  smc->SetNgKsi(kNasKeySetIdentifierNative, nc->ngksi & 0x07);
  smc->SetUeSecurityCapability(nc->ue_security_capability);
  smc->SetImeisvRequest(0xe1);  // TODO: remove hardcoded value
  smc->SetAdditional5gSecurityInformation(true, false);
  uint32_t msg_len = smc->GetLength();
  Logger::nas_mm().debug("Size of Security Mode Command message %ld", msg_len);
  uint8_t buffer[msg_len] = {0};
  int encoded_size        = smc->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encode Security Mode Command message error");
    return false;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Security-Mode-Command message buffer", buffer, encoded_size);

  std::string str = security_context_is_new ? "true" : "false";
  Logger::amf_n1().debug("Security Context status (is new:  %s)", str.c_str());

  bstring protected_nas = nullptr;
  encode_nas_message_protected(
      nc->security_ctx.value(), security_context_is_new,
      kIntegrityProtectedWithNewSecurityContext, NAS_MESSAGE_DOWNLINK, buffer,
      encoded_size, protected_nas);
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Encrypted Security-Mode-Command message buffer",
      (uint8_t*) bdata(protected_nas), blength(protected_nas));
  itti_send_dl_nas_buffer_to_task_n2(
      protected_nas, nc->ran_ue_ngap_id, nc->amf_ue_ngap_id);
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::security_select_algorithms(
    uint8_t nea, uint8_t nia, uint8_t& amf_nea, uint8_t& amf_nia) {
  bool found_nea = false;
  bool found_nia = false;
  for (int i = 0; i < amf_cfg.nas_cfg.prefered_ciphering_algorithm.size();
       i++) {
    if (nea & (0x80 >> (int) amf_cfg.nas_cfg.prefered_ciphering_algorithm[i])) {
      amf_nea = (int) amf_cfg.nas_cfg.prefered_ciphering_algorithm[i];
      Logger::amf_n1().debug("Selected AMF NEA: 0x%x", amf_nea);
      found_nea = true;
      break;
    }
  }
  for (int i = 0; i < amf_cfg.nas_cfg.prefered_integrity_algorithm.size();
       i++) {
    if (nia & (0x80 >> (int) amf_cfg.nas_cfg.prefered_integrity_algorithm[i])) {
      amf_nia = (int) amf_cfg.nas_cfg.prefered_integrity_algorithm[i];
      Logger::amf_n1().debug("Selected AMF NIA: 0x%x", amf_nia);
      found_nia = true;
      break;
    }
  }
  return (found_nea && found_nia);
}

//------------------------------------------------------------------------------
void amf_n1::security_mode_complete_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    bstring nas_msg) {
  Logger::amf_n1().debug("Handling Security Mode Complete ...");

  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) return;

  std::shared_ptr<nas_context> nc = {};
  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  // Decode Security Mode Complete
  auto security_mode_complete = std::make_unique<SecurityModeComplete>();
  security_mode_complete->Decode((uint8_t*) bdata(nas_msg), blength(nas_msg));

  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Security Mode Complete message buffer",
      (uint8_t*) bdata(nas_msg), blength(nas_msg));

  // Store UE Id (IMEISV) if available
  oai::nas::IMEI_IMEISV_t imeisv = {};
  if (security_mode_complete->GetImeisv(imeisv)) {
    Logger::nas_mm().debug(
        "Stored IMEISV in the NAS Context: %s", imeisv.identity.c_str());
    nc->imeisv = std::make_optional<oai::nas::IMEI_IMEISV_t>(imeisv);
  }

  std::optional<uint16_t> uplink_data_status_opt = std::nullopt;
  std::optional<uint16_t> pdu_session_status_opt = std::nullopt;

  // Process NAS Container
  bstring nas_msg_container = nullptr;
  if (security_mode_complete->GetNasMessageContainer(nas_msg_container)) {
    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "NAS Message Container", (uint8_t*) bdata(nas_msg_container),
        blength(nas_msg_container));

    uint8_t message_type = get_nas_message_type(
        (uint8_t*) bdata(nas_msg_container), blength(nas_msg_container));

    Logger::amf_n1().debug(
        "NAS Message Container, Message Type 0x%x", message_type);
    if (message_type == kRegistrationRequest) {
      Logger::amf_n1().debug("Registration Request in NAS Message Container");
      // Decode registration request message
      auto registration_request = std::make_unique<RegistrationRequest>();
      registration_request->Decode(
          (uint8_t*) bdata(nas_msg_container), blength(nas_msg_container));
      // oai::utils::utils::bdestroy_wrapper(&nas_msg_container);  // free
      // buffer

      // Get Requested NSSAI (Optional IE), if provided
      if (registration_request->GetRequestedNssai(nc->requested_nssai)) {
        for (auto s : nc->requested_nssai) {
          Logger::amf_n1().debug("Requested NSSAI: %s", s.ToString().c_str());
        }
      } else {
        Logger::amf_n1().debug("Optional IE RequestedNssai is not present");
      }

      // Get Uplink Data Status
      uplink_data_status_opt = registration_request->GetUplinkDataStatus();
      if (!uplink_data_status_opt.has_value())
        Logger::amf_n1().debug("Optional IE UplinkDataStatus is not present");

      // Get PDU session status
      pdu_session_status_opt = registration_request->GetPduSessionStatus();
      if (!pdu_session_status_opt.has_value())
        Logger::amf_n1().debug("Optional IE PDUSessionStatus is not present");
    }
  }

  // If current AMF can't handle this UE, reroute the Registration Request to
  // a target AMF
  bool reroute_result = true;
  if (reroute_registration_request(nc, reroute_result)) {
    return;
  }

  // If AMF can't handle this and there's an error when trying to handling the
  // UE to the target AMFs, thus encoding REGISTRATION REJECT
  if (!reroute_result) {
    uint8_t cause_value = 7;  // TODO: 5GS services not allowed - TO BE VERIFIED
    send_registration_reject_msg(ran_ue_ngap_id, amf_ue_ngap_id, cause_value);
    return;
  }

  // Process Uplink Data Status / PDU Session status
  uint16_t uplink_data_status              = 0x0000;
  uint16_t pdu_session_status              = 0x0000;
  uint16_t pdu_session_reactivation_result = 0x0000;
  if (uplink_data_status_opt.has_value())
    uplink_data_status = uplink_data_status_opt.value();
  if (pdu_session_status_opt.has_value())
    pdu_session_status = pdu_session_status_opt.value();

  // Get the list of PDU sessions to be activated
  std::vector<uint8_t> pdu_session_to_be_activated = {};
  if (uplink_data_status_opt.has_value())
    get_pdu_session_to_be_activated(
        uplink_data_status, pdu_session_to_be_activated);
  else if (pdu_session_status_opt.has_value())
    get_pdu_session_to_be_activated(
        pdu_session_status, pdu_session_to_be_activated);

  // Prepare Registration Accept
  auto registration_accept = std::make_unique<RegistrationAccept>();
  initialize_registration_accept(registration_accept, nc);

  std::string mcc = {};
  std::string mnc = {};
  uint32_t tmsi   = 0;
  if (!amf_app_inst->generate_5g_guti(
          ran_ue_ngap_id, amf_ue_ngap_id, mcc, mnc, tmsi)) {
    Logger::amf_n1().error("Generate 5G GUTI error, exit!");
    // TODO:
    return;
  }
  registration_accept->Set5gGuti(
      mcc, mnc, amf_cfg.guami.region_id, amf_cfg.guami.amf_set_id,
      amf_cfg.guami.amf_pointer, tmsi);

  std::string guti = amf_conv::tmsi_to_guti(
      mcc, mnc, amf_cfg.guami.region_id, amf_cfg.guami.amf_set_id,
      amf_cfg.guami.amf_pointer, amf_conv::tmsi_to_string(tmsi));
  Logger::amf_n1().debug(
      "Allocated GUTI %s (TMSI %s)", guti.c_str(),
      amf_conv::tmsi_to_string(tmsi).c_str());

  // registration_accept->SetT3512Value(0x5, T3512_TIMER_VALUE_MIN);

  set_guti_2_nas_context(guti, nc);
  nc->guti = std::make_optional<std::string>(guti);
  nc->is_common_procedure_for_security_mode_control_running = false;

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    return;
  }

  // Activate UP for these PDU sessions
  std::map<uint8_t, pdu_session_info_t> pdu_sessions;
  for (auto& pdu_session_id : pdu_session_to_be_activated) {
    std::shared_ptr<pdu_session_context> psc = {};
    if (!amf_app_inst->find_pdu_session_context(
            uc->supi, pdu_session_id, psc)) {
      Logger::amf_n1().warn(
          "No PDU Session Context with PDU Session ID %d", pdu_session_id);
    }

    // TODO:  need to check (psc->up_cnx_state ==
    // up_cnx_state_e::UPCNX_STATE_DEACTIVATED)?
    if (psc) {
      amf_app_inst->trigger_pdu_session_up_activation(pdu_session_id, uc);
    }

    pdu_session_info_t item = {};
    if (psc and psc->is_n2sm_available) {
      item.n2sm              = bstrcpy(psc->n2sm);
      item.is_n2sm_available = true;
    } else {
      item.is_n2sm_available = false;
      if (uplink_data_status_opt.has_value()) {
        set_pdu_session_reactivation_result(
            pdu_session_id, pdu_session_reactivation_result);
      }
      if (pdu_session_status_opt.has_value()) {
        set_pdu_session_status_inactive(pdu_session_id, pdu_session_status);
      }
      Logger::amf_n1().debug("Cannot get PDU session information");
    }

    pdu_sessions.insert(
        std::pair<uint8_t, pdu_session_info_t>(pdu_session_id, item));
  }

  // Set corresponding IE in Registration Accept
  if (uplink_data_status_opt.has_value()) {
    registration_accept->SetPduSessionReactivationResult(
        pdu_session_reactivation_result);
  }
  if (pdu_session_status_opt.has_value()) {
    registration_accept->SetPduSessionStatus(pdu_session_status);
  }

  // Encode Registration Accept
  bstring protected_nas = nullptr;
  uint32_t msg_len      = registration_accept->GetLength();
  Logger::nas_mm().debug("Size of Registration Accept message %ld", msg_len);
  uint8_t buffer[msg_len] = {0};
  int encoded_size        = registration_accept->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encode Registration Accept message error");
    return;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Registration-Accept message buffer", buffer, encoded_size);

  encode_nas_message_protected(
      nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
      NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

  if (!uc->is_ue_context_request) {
    // TODO: Use DownlinkNasTransport to convey Registration Accept
    Logger::amf_n1().debug(
        "UE Context is not requested, UE with "
        "ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT
        ", "
        "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT " attached",
        ran_ue_ngap_id, amf_ue_ngap_id);

    // IE: UEAggregateMaximumBitRate
    // AllowedNSSAI

    auto dnt =
        std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
    dnt->nas            = bstrcpy(protected_nas);
    dnt->amf_ue_ngap_id = amf_ue_ngap_id;
    dnt->ran_ue_ngap_id = ran_ue_ngap_id;

    int ret = itti_inst->send_msg(dnt);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          dnt->get_msg_name());
    }
  } else {
    // use InitialContextSetupRequest to convey Registration Accept
    uint8_t kamf[AUTH_VECTOR_LENGTH_OCTETS];
    uint8_t kgnb[AUTH_VECTOR_LENGTH_OCTETS];
    if (!nc->get_kamf(nc->security_ctx.value().vector_pointer, kamf)) {
      Logger::amf_n1().warn("No Kamf found");
      return;
    }
    uint32_t ulcount = nc->security_ctx.value().ul_count.seq_num |
                       (nc->security_ctx.value().ul_count.overflow << 8);
    Authentication_5gaka::derive_kgnb(
        ulcount, KAccessType3gppAccess, kamf, kgnb);

    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "Kamf", kamf, AUTH_VECTOR_LENGTH_OCTETS);

    auto itti_msg = std::make_shared<itti_initial_context_setup_request>(
        TASK_AMF_N1, TASK_AMF_N2);
    itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
    itti_msg->kgnb           = blk2bstr(kgnb, AUTH_VECTOR_LENGTH_OCTETS);
    itti_msg->is_sr          = false;  // TODO: for Service Request procedure
    itti_msg->nas            = bstrcpy(protected_nas);

    for (auto const& pdu_session : pdu_sessions) {
      pdu_session_info_t item = {};
      if (pdu_session.second.is_n2sm_available) {
        item.n2sm = bstrcpy(pdu_session.second.n2sm);
      }
      item.is_n2sm_available = pdu_session.second.is_n2sm_available;
      itti_msg->pdu_sessions.insert(
          std::pair<uint8_t, pdu_session_info_t>(pdu_session.first, item));
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          itti_msg->get_msg_name());
    }
  }

  Logger::amf_n1().info(
      "UE (IMSI %s, GUTI %s, current RAN ID %d, current AMF ID %d) has been "
      "registered to the network",
      nc->imsi.c_str(), guti.c_str(), ran_ue_ngap_id, amf_ue_ngap_id);

  stacs.update_5gmm_state(nc, _5GMM_REGISTERED);
  set_5gmm_state(nc, _5GMM_REGISTERED);
  stacs.display();

  // Trigger UE location Status Notify
  trigger_ue_location_report(ran_ue_ngap_id, amf_ue_ngap_id);

  // Trigger UE Registration Status Notify
  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  Logger::amf_n1().debug(
      "Signal the UE Registration State Event notification for SUPI %s",
      supi.c_str());
  event_sub.ue_registration_state(
      supi, _5GMM_REGISTERED, amf_cfg.support_features.http_version,
      ran_ue_ngap_id, amf_ue_ngap_id);

  // Trigger UE Connectivity Status Notify
  Logger::amf_n1().debug(
      "Signal the UE Connectivity Status Event notification for SUPI %s",
      supi.c_str());
  event_sub.ue_connectivity_state(
      supi, CM_CONNECTED, amf_cfg.support_features.http_version);
}

//------------------------------------------------------------------------------
void amf_n1::security_mode_reject_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    bstring nas_msg) {
  Logger::amf_n1().debug(
      "Receiving Security Mode Reject message, handling ...");
  // TODO:
  return;
}

//------------------------------------------------------------------------------
void amf_n1::registration_complete_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id,
    bstring nas_msg) {
  Logger::amf_n1().debug("Received Registration Complete message, processing");

  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) return;

  std::shared_ptr<nas_context> nc = {};
  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    return;
  }

  // Decode Registration Complete message
  auto registration_complete = std::make_unique<RegistrationComplete>();
  int decoded_size           = registration_complete->Decode(
      (uint8_t*) bdata(nas_msg), blength(nas_msg));
  if (decoded_size <= 0) {
    Logger::amf_n1().warn("Error when decoding Registration Complete");
    return;
  }

  // TODO: Configuration Update Command message causes issue for UERANSIM
  // (it does not accept the first PDU Session Establishment Accept, then it
  // will send a second PDU Session Establishment Request and accept the second
  // PDU Session Establishment Accept) Therefore, we disable this temporarily to
  // make it work with UERANSIM
  Logger::amf_n1().debug(
      "Do not sending Configuration Update Command in this version!");
  /*
  Logger::amf_n1().debug("Preparing Configuration Update Command message");
  // Encode Configuration Update Command
  auto configuration_update_command =
      std::make_unique<ConfigurationUpdateCommand>();

  configuration_update_command->SetFullNameForNetwork("Testing");   // TODO:
  configuration_update_command->SetShortNameForNetwork("Testing");  // TODO:

  uint8_t buffer[BUFFER_SIZE_1024] = {0};
  int encoded_size =
      configuration_update_command->Encode(buffer, BUFFER_SIZE_1024);
  if (encoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encode Configuration Update Command message error");
    return;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Configuration Update Command message Buffer", buffer,
      encoded_size);

  // Protect NAS message
  bstring protected_nas = nullptr;
  encode_nas_message_protected(
      security_ctx.value(), false, kIntegrityProtectedAndCiphered,
  NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

  std::shared_ptr<itti_dl_nas_transport> dnt =
      std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
  dnt->nas            = bstrcpy(protected_nas);
  dnt->amf_ue_ngap_id = amf_ue_ngap_id;
  dnt->ran_ue_ngap_id = ran_ue_ngap_id;

  int ret = itti_inst->send_msg(dnt);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        dnt->get_msg_name());
  }
  */
}

//------------------------------------------------------------------------------
void amf_n1::encode_nas_message_protected(
    nas_secu_ctx& nsc, bool is_secu_ctx_new, uint8_t security_header_type,
    uint8_t direction, uint8_t* input_nas_buf, int input_nas_len,
    bstring& protected_nas) {
  Logger::amf_n1().debug("Encoding nas_message_protected...");
  uint8_t protected_nas_buf[BUFFER_SIZE_4096];
  int encoded_size = 0;

  switch (security_header_type & 0x0f) {
    case kIntegrityProtected: {
    } break;

    case kIntegrityProtectedAndCiphered: {
      bstring input    = blk2bstr(input_nas_buf, input_nas_len);
      bstring ciphered = nullptr;
      // balloc(ciphered, blength(input));
      nas_message_cipher_protected(nsc, NAS_MESSAGE_DOWNLINK, input, ciphered);
      protected_nas_buf[0] = k5gsMobilityManagementMessages;
      protected_nas_buf[1] = kIntegrityProtectedAndCiphered;
      protected_nas_buf[kSecurityProtected5gsNasMessageSequenceNumberOctet] =
          (uint8_t) nsc.dl_count.seq_num;

      uint8_t* buf_tmp = (uint8_t*) bdata(ciphered);
      if (buf_tmp != nullptr)
        memcpy(
            &protected_nas_buf[kSecurityProtected5gsNasMessageHeaderLength],
            (uint8_t*) buf_tmp, blength(ciphered));

      uint32_t mac32 = 0;
      if (!(nas_message_integrity_protected(
              nsc, NAS_MESSAGE_DOWNLINK,
              protected_nas_buf +
                  kSecurityProtected5gsNasMessageSequenceNumberOctet,
              input_nas_len + 1, mac32))) {
        memcpy(protected_nas_buf, input_nas_buf, input_nas_len);
        encoded_size = input_nas_len;
      } else {
        *(uint32_t*) (protected_nas_buf + 2) = htonl(mac32);
        encoded_size =
            kSecurityProtected5gsNasMessageHeaderLength + input_nas_len;
      }

      oai::utils::utils::bdestroy_wrapper(&input);
      oai::utils::utils::bdestroy_wrapper(&ciphered);
    } break;

    case kIntegrityProtectedWithNewSecurityContext: {
      if (!is_secu_ctx_new) {
        Logger::amf_n1().error("Security context is too old");
        return;
      }
      protected_nas_buf[0] = k5gsMobilityManagementMessages;
      protected_nas_buf[1] = kIntegrityProtectedWithNewSecurityContext;
      protected_nas_buf[kSecurityProtected5gsNasMessageSequenceNumberOctet] =
          (uint8_t) nsc.dl_count.seq_num;
      memcpy(
          &protected_nas_buf[kSecurityProtected5gsNasMessageHeaderLength],
          input_nas_buf, input_nas_len);
      uint32_t mac32 = {};
      if (!(nas_message_integrity_protected(
              nsc, NAS_MESSAGE_DOWNLINK,
              protected_nas_buf +
                  kSecurityProtected5gsNasMessageSequenceNumberOctet,
              input_nas_len + 1, mac32))) {
        memcpy(protected_nas_buf, input_nas_buf, input_nas_len);
        encoded_size = input_nas_len;
      } else {
        Logger::amf_n1().debug("mac32: 0x%x", mac32);
        *(uint32_t*) (protected_nas_buf + 2) = htonl(mac32);
        encoded_size =
            kSecurityProtected5gsNasMessageHeaderLength + input_nas_len;
      }
    } break;

    case kIntegrityProtectedAndCipheredWithNewSecurityContext: {
    } break;
  }
  protected_nas = blk2bstr(protected_nas_buf, encoded_size);
  nsc.dl_count.seq_num++;
}

//------------------------------------------------------------------------------
bool amf_n1::nas_message_integrity_protected(
    nas_secu_ctx& nsc, uint8_t direction, uint8_t* input_nas, int input_nas_len,
    uint32_t& mac32) {
  uint32_t count = 0x00000000;
  if (direction) {
    count = 0x00000000 | ((nsc.dl_count.overflow & 0x0000ffff) << 8) |
            ((nsc.dl_count.seq_num & 0x000000ff));
  } else {
    count = 0x00000000 | ((nsc.ul_count.overflow & 0x0000ffff) << 8) |
            ((nsc.ul_count.seq_num & 0x000000ff));
  }
  nas_stream_cipher_t stream_cipher = {0};
  uint8_t mac[4];
  stream_cipher.key = nsc.knas_int;
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Parameters for NIA: Knas_int", nsc.knas_int,
      AUTH_KNAS_INT_SIZE);
  stream_cipher.key_length = AUTH_KNAS_INT_SIZE;
  stream_cipher.count      = *(input_nas);
  // stream_cipher.count = count;
  if (!direction) {
    nsc.ul_count.seq_num = stream_cipher.count;
    Logger::amf_n1().debug("Uplink count in uplink: %d", nsc.ul_count.seq_num);
  }
  Logger::amf_n1().debug("Parameters for NIA, count: 0x%x", count);
  stream_cipher.bearer = 0x01;  // 33.501 section 8.1.1
  Logger::amf_n1().debug(
      "Parameters for NIA, bearer: 0x%x", stream_cipher.bearer);
  stream_cipher.direction = direction;  // "1" for downlink
  Logger::amf_n1().debug("Parameters for NIA, direction: 0x%x", direction);
  stream_cipher.message = (uint8_t*) input_nas;
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Parameters for NIA, message: ", input_nas, input_nas_len);
  stream_cipher.blength = input_nas_len * 8;

  switch (nsc.nas_algs.integrity & 0x0f) {
    case kIa0_5g: {
      Logger::amf_n1().debug("Integrity with algorithms: 5G-IA0");
      return false;  // plain msg
    } break;

    case kIa1_128_5g: {
      Logger::amf_n1().debug("Integrity with algorithms: 128-5G-IA1");
      nas_algorithms::nas_stream_encrypt_nia1(&stream_cipher, mac);
      oai::utils::output_wrapper::print_buffer(
          "amf_n1", "Result for NIA1, mac: ", mac, 4);
      mac32 = ntohl(*((uint32_t*) mac));
      Logger::amf_n1().debug("Result for NIA1, mac32: 0x%x", mac32);
      return true;
    } break;

    case kIa2_128_5g: {
      Logger::amf_n1().debug("Integrity with algorithms: 128-5G-IA2");
      nas_algorithms::nas_stream_encrypt_nia2(&stream_cipher, mac);
      oai::utils::output_wrapper::print_buffer(
          "amf_n1", "Result for NIA2, mac: ", mac, 4);
      mac32 = ntohl(*((uint32_t*) mac));
      Logger::amf_n1().debug("Result for NIA2, mac32: 0x%x", mac32);
      return true;
    } break;
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::nas_message_cipher_protected(
    nas_secu_ctx& nsc, uint8_t direction, bstring input_nas,
    bstring& output_nas) {
  uint8_t* buf   = (uint8_t*) bdata(input_nas);
  int buf_len    = blength(input_nas);
  uint32_t count = 0x00000000;
  if (direction) {
    count = 0x00000000 | ((nsc.dl_count.overflow & 0x0000ffff) << 8) |
            ((nsc.dl_count.seq_num & 0x000000ff));
  } else {
    Logger::amf_n1().debug("nsc.ul_count.overflow %x", nsc.ul_count.overflow);
    count = 0x00000000 | ((nsc.ul_count.overflow & 0x0000ffff) << 8) |
            ((nsc.ul_count.seq_num & 0x000000ff));
  }
  nas_stream_cipher_t stream_cipher = {0};
  uint8_t mac[4];
  stream_cipher.key        = nsc.knas_enc;
  stream_cipher.key_length = AUTH_KNAS_ENC_SIZE;
  stream_cipher.count      = count;
  stream_cipher.bearer     = 0x01;       // 33.501 section 8.1.1
  stream_cipher.direction  = direction;  // "1" for downlink
  stream_cipher.message    = (uint8_t*) bdata(input_nas);
  stream_cipher.blength    = blength(input_nas) << 3;

  switch (nsc.nas_algs.encryption & 0x0f) {
    case kEa0_5g: {
      Logger::amf_n1().debug("Cipher protected with EA0_5G");
      output_nas = blk2bstr(buf, buf_len);
      return true;
    } break;

    case kEa1_128_5g: {
      Logger::amf_n1().debug("Cipher protected with EA1_128_5G");
      Logger::amf_n1().debug("stream_cipher.blength %d", stream_cipher.blength);
      Logger::amf_n1().debug(
          "stream_cipher.message %x", stream_cipher.message[0]);
      oai::utils::output_wrapper::print_buffer(
          "amf_n1", "stream_cipher.key ", stream_cipher.key, 16);
      Logger::amf_n1().debug("stream_cipher.count %x", stream_cipher.count);

      uint8_t* ciphered =
          (uint8_t*) malloc(((stream_cipher.blength + 31) / 32) * 4);

      nas_algorithms::nas_stream_encrypt_nea1(&stream_cipher, ciphered);
      output_nas = blk2bstr(ciphered, ((stream_cipher.blength + 31) / 32) * 4);
      free(ciphered);
    } break;

    case kEa2_128_5g: {
      Logger::amf_n1().debug("Cipher protected with EA2_128_5G");

      uint32_t len = stream_cipher.blength >> 3;
      if ((stream_cipher.blength & 0x7) > 0) len += 1;
      uint8_t* ciphered = (uint8_t*) malloc(len);
      nas_algorithms::nas_stream_encrypt_nea2(&stream_cipher, ciphered);
      output_nas = blk2bstr(ciphered, len);
      free(ciphered);
    } break;
  }
  return true;
}

//------------------------------------------------------------------------------
void amf_n1::ue_initiate_de_registration_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id, bstring nas) {
  Logger::amf_n1().debug("Handling UE-initiated De-registration Request");

  std::shared_ptr<nas_context> nc = {};
  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  // Decode NAS message
  auto dereg_request =
      std::make_unique<DeregistrationRequest>();  // UE originating
                                                  // de-registration
  dereg_request->Decode((uint8_t*) bdata(nas), blength(nas));

  // TODO: validate 5G Mobile Identity
  uint8_t mobile_id_type = 0;
  dereg_request->GetMobilityIdentityType(mobile_id_type);
  Logger::amf_n1().debug("5G Mobile Identity %X", mobile_id_type);
  switch (mobile_id_type) {
    case k5gGuti: {
      Logger::amf_n1().debug(
          "5G Mobile Identity, GUTI %s", dereg_request->Get5gGuti().c_str());
    } break;
    default: {
    }
  }

  // Send request to SMF to release the established PDU sessions if needed
  // Get list of PDU sessions
  std::vector<std::shared_ptr<pdu_session_context>> sessions_ctx;
  std::shared_ptr<ue_context> uc = {};

  if (!find_ue_context(nc, uc)) return;

  if (uc != nullptr) {
    if (uc->get_pdu_sessions_context(sessions_ctx)) {
      // Send Nsmf_PDUSession_ReleaseSMContext to SMF to release all existing
      // PDU sessions

      std::map<uint32_t, boost::shared_future<nlohmann::json>> smf_responses;
      for (auto session : sessions_ctx) {
        auto itti_msg =
            std::make_shared<itti_nsmf_pdusession_release_sm_context>(
                TASK_AMF_N1, TASK_AMF_SBI);

        // Generate a promise and associate this promise to the ITTI message
        uint32_t promise_id = amf_app_inst->generate_promise_id();
        Logger::amf_n1().debug("Promise ID generated %d", promise_id);

        boost::shared_ptr<boost::promise<nlohmann::json>> p =
            boost::make_shared<boost::promise<nlohmann::json>>();
        boost::shared_future<nlohmann::json> f = p->get_future();

        // Store the future to be processed later
        smf_responses.emplace(promise_id, f);
        amf_app_inst->add_promise(promise_id, p);

        itti_msg->supi             = uc->supi;
        itti_msg->pdu_session_id   = session->pdu_session_id;
        itti_msg->promise_id       = promise_id;
        itti_msg->context_location = session->smf_info.context_location;

        int ret = itti_inst->send_msg(itti_msg);
        if (0 != ret) {
          Logger::amf_n1().error(
              "Could not send ITTI message %s to task TASK_AMF_SBI",
              itti_msg->get_msg_name());
        }
      }

      // Wait for the response available and process accordingly
      while (!smf_responses.empty()) {
        // Wait for the result available and process accordingly
        std::optional<nlohmann::json> result = std::nullopt;
        oai::utils::utils::wait_for_result(
            smf_responses.begin()->second, result);

        if (result.has_value()) {
          Logger::amf_server().debug(
              "Got result for promise ID %d", smf_responses.begin()->first);
          nlohmann::json result_json  = result.value();
          uint32_t http_response_code = 0;
          if (result_json.find(kSbiResponseHttpResponseCode) !=
              result_json.end()) {
            http_response_code =
                result_json[kSbiResponseHttpResponseCode].get<int>();
            // Remove PDU session
            // TODO for multiple sessions
            if ((http_response_code == 200) or (http_response_code == 204)) {
              for (auto session : sessions_ctx) {
                uc->remove_pdu_sessions_context(session->pdu_session_id);
              }
            }
          } else {
            // TODO:
          }
        }

        smf_responses.erase(smf_responses.begin());
      }

    } else {
      Logger::amf_n1().debug("No PDU session available");
    }
  }

  // TODO: AMF-nitiated AM Policy Association Termination (if exist)
  // TODO: AMF-initiated UE Policy Association Termination (if exist)

  // Check Deregistration type
  uint8_t deregType = 0;
  dereg_request->GetDeregistrationType(deregType);
  Logger::amf_n1().debug("De-registration Type 0x%x", deregType);

  // If UE switch-off, don't need to send Deregistration Accept
  if ((deregType & kDeregistrationTypeMask) == 0) {
    // Prepare DeregistrationAccept
    auto dereg_accept = std::make_unique<DeregistrationAccept>();

    uint32_t msg_len = dereg_accept->GetLength();
    Logger::nas_mm().debug(
        "Size of Deregistration Accept message %ld", msg_len);
    uint8_t buffer[msg_len] = {0};
    int encoded_size        = dereg_accept->Encode(buffer, msg_len);
    if (encoded_size == KEncodeDecodeError) {
      Logger::nas_mm().error("Encode De-registration Accept message error");
      return;
    }
    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "De-registration Accept message buffer", buffer,
        encoded_size);

    bstring b = blk2bstr(buffer, encoded_size);
    itti_send_dl_nas_buffer_to_task_n2(b, ran_ue_ngap_id, amf_ue_ngap_id);
    // sleep 200ms
    usleep(200000);
  }

  stacs.update_5gmm_state(nc, _5GMM_DEREGISTERED);
  set_5gmm_state(nc, _5GMM_DEREGISTERED);
  stacs.display();

  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  // Trigger UE Registration Status Notify
  Logger::amf_n1().debug(
      "Signal the UE Registration State Event notification for SUPI %s",
      supi.c_str());
  event_sub.ue_registration_state(
      supi, _5GMM_DEREGISTERED, amf_cfg.support_features.http_version,
      ran_ue_ngap_id, amf_ue_ngap_id);

  // Trigger UE Loss of Connectivity Status Notify
  Logger::amf_n1().debug(
      "Signal the UE Loss of Connectivity Event notification for SUPI %s",
      supi.c_str());
  event_sub.ue_loss_of_connectivity(
      supi, DEREGISTERED, amf_cfg.support_features.http_version, ran_ue_ngap_id,
      amf_ue_ngap_id);

  // TODO: put once this scenario is implemented
  // Trigger UE Loss of Connectivity Status Notify
  // Logger::amf_n1().debug(
  //     "Signal the UE Loss of Connectivity Event notification for SUPI %s",
  //     supi.c_str());
  // event_sub.ue_loss_of_connectivity(supi, PURGED, 1, ran_ue_ngap_id,
  // amf_ue_ngap_id);

  // Update 5GMM state
  stacs.update_5gmm_state(nc, _5GMM_DEREGISTERED);

  // Remove NC context
  if (remove_amf_ue_ngap_id_2_nas_context(amf_ue_ngap_id)) {
    Logger::amf_n1().debug(
        "Deleted nas_context associated with "
        "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT,
        amf_ue_ngap_id);
  } else {
    Logger::amf_n1().debug(
        "Could not delete nas_context associated with "
        "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT,
        amf_ue_ngap_id);
  }

  if (remove_supi_2_nas_context(supi)) {
    Logger::amf_n1().debug(
        "Deleted nas_context associated SUPI %s ", supi.c_str());
  } else {
    Logger::amf_n1().debug(
        "Could not delete nas_context associated SUPI %s ", supi.c_str());
  }

  if (remove_guti_2_nas_context(dereg_request->Get5gGuti())) {
    Logger::amf_n1().debug(
        "Deleted nas_context associated GUTI %s ",
        dereg_request->Get5gGuti().c_str());
  } else {
    Logger::amf_n1().debug(
        "Could not delete nas_context associated GUTI %s ",
        dereg_request->Get5gGuti().c_str());
  }

  // TODO: AMF to AN: N2 UE Context Release Request
  // AMF sends N2 UE Release command to NG-RAN with Cause set to Deregistration
  // to release N2 signalling connection

  Logger::amf_n1().debug(
      "Sending ITTI UE Context Release Command to TASK_AMF_N2");

  auto itti_msg = std::make_shared<itti_ue_context_release_command>(
      TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
  itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
  itti_msg->cause.setChoiceOfCause(Ngap_Cause_PR_nas);
  itti_msg->cause.set(
      2);  // TODO: remove hardcoded value cause nas(2)--deregister

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }

  // Trigger UE Connectivity Status Notify
  Logger::amf_n1().debug(
      "Signal the UE Connectivity Status Event notification for SUPI %s",
      supi.c_str());
  event_sub.ue_connectivity_state(
      supi, CM_IDLE, amf_cfg.support_features.http_version);
}

//------------------------------------------------------------------------------
void amf_n1::ul_nas_transport_handle(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id, bstring nas,
    const plmn_t& plmn) {
  // Decode UL_NAS_TRANSPORT message
  Logger::amf_n1().debug("Handling UL NAS Transport");
  auto ul_nas = std::make_unique<UlNasTransport>();
  ul_nas->Decode((uint8_t*) bdata(nas), blength(nas));
  uint8_t payload_type   = ul_nas->GetPayloadContainerType();
  uint8_t pdu_session_id = 0;
  ul_nas->GetPduSessionId(pdu_session_id);

  uint8_t request_type = 0;
  if (!ul_nas->GetRequestType(request_type)) {
    Logger::amf_n1().debug("Request Type is not available");
    // TODO:
  }

  bstring sm_msg = nullptr;

  if (((request_type & 0x07) == kPduSessionInitialRequest) or
      ((request_type & 0x07) == kExistingPduSession)) {
    // SNSSAI
    SNSSAI_t snssai = {};
    if (!ul_nas->GetSNssai(snssai)) {  // If no SNSSAI in this message, use the
                                       // one in Registration Request
      Logger::amf_n1().debug(
          "No Requested NSSAI available in UlNasTransport, use NSSAI from "
          "Requested/Configured NSSAI!");

      std::shared_ptr<nas_context> nc = {};
      if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

      // TODO: Only use the first one for now if there's multiple requested
      // NSSAI since we don't know which slice associated with this PDU session
      if (nc->allowed_nssai.size() > 0) {
        snssai = nc->allowed_nssai[0];
        Logger::amf_n1().debug(
            "Use first Requested S-NSSAI %s", snssai.ToString().c_str());
      } else {
        // Otherwise, use first default subscribed S-NSSAI if available
        bool found = false;
        for (const auto& sn : nc->subscribed_snssai) {
          if (sn.first) {
            snssai = sn.second;
            Logger::amf_n1().debug(
                "Use Default Configured S-NSSAI %s", snssai.ToString().c_str());
            found = true;
            break;
          }
        }

        if (!found) {
          std::vector<struct SNSSAI_s> common_nssais;
          // Find UE Context
          std::shared_ptr<ue_context> uc = {};
          if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc))
            return;

          amf_n2_inst->get_common_NSSAI(
              nc->ran_ue_ngap_id, uc->gnb_id, common_nssais);

          // Use common NSSAI between gNB and AMF
          for (auto s : common_nssais) {
            snssai.sst = s.sst;
            snssai.sd  = s.sd;
            if (s.sd == SD_NO_VALUE) {
              snssai.length = SST_LENGTH;
            } else {
              snssai.length = SST_LENGTH + SD_LENGTH;
            }
            Logger::amf_n1().debug(
                "Use common S-NSSAI (SST 0x%x, SD 0x%x)", s.sst, s.sd);
            found = true;
            break;
          }
        }

        if (!found) {
          snssai.sst    = DEFAULT_SST;
          snssai.sd     = SD_NO_VALUE;
          snssai.length = SST_LENGTH;
          Logger::amf_n1().debug(
              "Default S-NSSAI (SST 0x%x, SD 0x%x)", snssai.sst, snssai.sd);
        }
      }
    }

    Logger::amf_n1().debug(
        "S_NSSAI for this PDU Session %s", snssai.ToString().c_str());

    bstring dnn = bfromcstr(DEFAULT_DNN);

    if (!ul_nas->GetDnn(dnn)) {
      Logger::amf_n1().debug(
          "No DNN available in UlNasTransport, use default DNN: %s",
          DEFAULT_DNN);
      // TODO: use default DNN for the corresponding NSSAI
    }

    // Use DNN as case insensitive
    amf_conv::to_lower(dnn);

    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "Decoded DNN Bit String", (uint8_t*) bdata(dnn),
        blength(dnn));

    switch (payload_type) {
      case kN1SmInformation: {
        // Get payload container
        ul_nas->GetPayloadContainer(sm_msg);

        auto itti_msg =
            std::make_shared<itti_nsmf_pdusession_create_sm_context>(
                TASK_AMF_N1, TASK_AMF_SBI);
        itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
        itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
        itti_msg->req_type       = request_type;
        itti_msg->pdu_sess_id    = pdu_session_id;
        itti_msg->dnn            = bstrcpy(dnn);
        itti_msg->sm_msg         = bstrcpy(sm_msg);
        itti_msg->snssai.sst     = snssai.sst;
        itti_msg->plmn.mnc       = plmn.mnc;
        itti_msg->plmn.mcc       = plmn.mcc;

        ngap_utils::sd_int_to_string_hex(snssai.sd, itti_msg->snssai.sd);

        int ret = itti_inst->send_msg(itti_msg);
        if (0 != ret) {
          Logger::amf_n1().error(
              "Could not send ITTI message %s to task TASK_AMF_SBI",
              itti_msg->get_msg_name());
        }

      } break;
      default: {
        Logger::amf_n1().debug("Transport message un supported");
      }
    }

  } else {
    switch (payload_type) {
      case kN1SmInformation: {
        // Get payload container
        ul_nas->GetPayloadContainer(sm_msg);

        auto itti_msg =
            std::make_shared<itti_nsmf_pdusession_update_sm_context>(
                TASK_AMF_N1, TASK_AMF_SBI);

        itti_msg->ran_ue_ngap_id = ran_ue_ngap_id;
        itti_msg->amf_ue_ngap_id = amf_ue_ngap_id;
        itti_msg->pdu_session_id = pdu_session_id;
        itti_msg->n1sm           = bstrcpy(sm_msg);
        itti_msg->is_n1sm_set    = true;

        int ret = itti_inst->send_msg(itti_msg);
        if (0 != ret) {
          Logger::amf_n1().error(
              "Could not send ITTI message %s to task TASK_AMF_SBI",
              itti_msg->get_msg_name());
        }

      } break;
      default: {
        Logger::amf_n1().debug("Transport message is not supported");
      }
    }
  }
}

//------------------------------------------------------------------------------
bool amf_n1::run_mobility_registration_update_procedure(
    std::shared_ptr<nas_context>& nc,
    const std::optional<uint16_t>& uplink_data_status_opt,
    const std::optional<uint16_t>& pdu_session_status_opt, uint8_t& cause) {
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc, uc)) {
    cause = k5gmmCauseIllegalUe;  // TODO: verify the cause
    return false;
  }

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().warn("No Security Context found");
    // Run Registration procedure
    return run_registration_procedure(nc, cause);
  }

  // Encoding REGISTRATION ACCEPT
  auto reg_accept = std::make_unique<RegistrationAccept>();
  initialize_registration_accept(reg_accept, nc);

  reg_accept->Set5gGuti(
      amf_cfg.guami.mcc, amf_cfg.guami.mnc, amf_cfg.guami.region_id,
      amf_cfg.guami.amf_set_id, amf_cfg.guami.amf_pointer, uc->tmsi);

  // Get list of PDU sessions to be activated
  uint16_t uplink_data_status              = 0x0000;
  uint16_t pdu_session_status              = 0x0000;
  uint16_t pdu_session_reactivation_result = 0x0000;

  if (uplink_data_status_opt.has_value())
    uplink_data_status = uplink_data_status_opt.value();

  if (pdu_session_status_opt.has_value())
    pdu_session_status = pdu_session_status_opt.value();

  std::vector<uint8_t> pdu_session_to_be_activated = {};
  if (uplink_data_status_opt.has_value())
    get_pdu_session_to_be_activated(
        uplink_data_status, pdu_session_to_be_activated);
  else if (pdu_session_status_opt.has_value())
    get_pdu_session_to_be_activated(
        pdu_session_status, pdu_session_to_be_activated);

  // Activate UP for these PDU sessions
  std::map<uint8_t, pdu_session_info_t> pdu_sessions;
  for (auto& pdu_session_id : pdu_session_to_be_activated) {
    std::shared_ptr<pdu_session_context> psc = {};
    if (!amf_app_inst->find_pdu_session_context(
            uc->supi, pdu_session_id, psc)) {
      Logger::amf_n1().warn(
          "No PDU Session Context with PDU Session ID %d", pdu_session_id);
    }

    // TODO:  need to check (psc->up_cnx_state ==
    // up_cnx_state_e::UPCNX_STATE_DEACTIVATED)?
    if (psc) {
      amf_app_inst->trigger_pdu_session_up_activation(pdu_session_id, uc);
    }

    pdu_session_info_t item = {};
    if (psc and psc->is_n2sm_available) {
      item.n2sm              = bstrcpy(psc->n2sm);
      item.is_n2sm_available = true;
    } else {
      item.is_n2sm_available = false;
      if (uplink_data_status_opt.has_value()) {
        set_pdu_session_reactivation_result(
            pdu_session_id, pdu_session_reactivation_result);
      }
      if (pdu_session_status_opt.has_value()) {
        set_pdu_session_status_inactive(pdu_session_id, pdu_session_status);
      }
      Logger::amf_n1().debug("Cannot get PDU session information");
    }

    pdu_sessions.insert(
        std::pair<uint8_t, pdu_session_info_t>(pdu_session_id, item));
  }

  // Set corresponding IE in Registration Accept
  if (uplink_data_status_opt.has_value()) {
    reg_accept->SetPduSessionReactivationResult(
        pdu_session_reactivation_result);
  }
  if (pdu_session_status_opt.has_value()) {
    reg_accept->SetPduSessionStatus(pdu_session_status);
  }

  // Encode Registration Accept
  uint32_t msg_len = reg_accept->GetLength();
  Logger::nas_mm().debug("Size of Registration Accept message %ld", msg_len);
  uint8_t buffer[msg_len] = {0};
  int encoded_size        = reg_accept->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encode Registration Accept message error");
    cause = k5gmmCauseSemanticallyIncorrect;  // TODO: verify the cause
    return false;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Registration-Accept Message Buffer", buffer, encoded_size);

  // protect nas message
  bstring protected_nas = nullptr;
  encode_nas_message_protected(
      nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
      NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

  if (!uc->is_ue_context_request) {
    // TODO: Use DownlinkNasTransport to convey Registration Accept
    Logger::amf_n1().debug(
        "UE Context is not requested, UE with "
        "ran_ue_ngap_id " GNB_UE_NGAP_ID_FMT
        ", "
        "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT " attached",
        nc->ran_ue_ngap_id, nc->amf_ue_ngap_id);

    // IE: UEAggregateMaximumBitRate
    // AllowedNSSAI

    auto itti_msg =
        std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
    itti_msg->ran_ue_ngap_id = nc->ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = nc->amf_ue_ngap_id;
    itti_msg->nas            = bstrcpy(protected_nas);

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          itti_msg->get_msg_name());
      cause = k5gmmCauseCongestion;
      return false;
    }

  } else {
    // use InitialContextSetupRequest to convey Registration Accept
    uint8_t kamf[AUTH_VECTOR_LENGTH_OCTETS];
    uint8_t kgnb[AUTH_VECTOR_LENGTH_OCTETS];
    if (!nc->get_kamf(nc->security_ctx.value().vector_pointer, kamf)) {
      Logger::amf_n1().warn("No Kamf found");
      cause =
          k5gmmCauseSecurityModeRejectedUnspecified;  // TODO: verify the cause
      return false;
    }
    uint32_t ulcount = nc->security_ctx.value().ul_count.seq_num |
                       (nc->security_ctx.value().ul_count.overflow << 8);

    Authentication_5gaka::derive_kgnb(
        ulcount, KAccessType3gppAccess, kamf, kgnb);
    oai::utils::output_wrapper::print_buffer(
        "amf_n1", "Kamf", kamf, AUTH_VECTOR_LENGTH_OCTETS);

    auto itti_msg = std::make_shared<itti_initial_context_setup_request>(
        TASK_AMF_N1, TASK_AMF_N2);
    itti_msg->ran_ue_ngap_id = nc->ran_ue_ngap_id;
    itti_msg->amf_ue_ngap_id = nc->amf_ue_ngap_id;
    itti_msg->kgnb           = blk2bstr(kgnb, AUTH_VECTOR_LENGTH_OCTETS);
    itti_msg->is_sr          = false;  // TODO: for Service Request procedure
    itti_msg->nas            = bstrcpy(protected_nas);

    for (auto const& pdu_session : pdu_sessions) {
      pdu_session_info_t item = {};
      if (pdu_session.second.is_n2sm_available) {
        item.n2sm = bstrcpy(pdu_session.second.n2sm);
      }
      item.is_n2sm_available = pdu_session.second.is_n2sm_available;
      itti_msg->pdu_sessions.insert(
          std::pair<uint8_t, pdu_session_info_t>(pdu_session.first, item));
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          itti_msg->get_msg_name());
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::run_periodic_registration_update_procedure(
    std::shared_ptr<nas_context>& nc,
    const std::optional<uint16_t>& pdu_session_status_opt, uint8_t& cause) {
  uint16_t pdu_session_status = 0x0000;
  if (pdu_session_status_opt.has_value())
    pdu_session_status = pdu_session_status_opt.value();

  // Encoding REGISTRATION ACCEPT
  auto reg_accept = std::make_unique<RegistrationAccept>();
  initialize_registration_accept(reg_accept, nc);

  // Get UE context
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc, uc)) {
    cause = k5gmmCauseIllegalUe;  // TODO: verify the cause
    return false;
  }

  reg_accept->Set5gGuti(
      amf_cfg.guami.mcc, amf_cfg.guami.mnc, amf_cfg.guami.region_id,
      amf_cfg.guami.amf_set_id, amf_cfg.guami.amf_pointer, uc->tmsi);

  if (pdu_session_status_opt.has_value()) {
    reg_accept->SetPduSessionStatus(pdu_session_status);
    Logger::amf_n1().debug(
        "PDU Session Status 0x%02x", htonl(pdu_session_status));
  }

  uint32_t msg_len = reg_accept->GetLength();
  Logger::nas_mm().debug("Size of Registration Accept message %ld", msg_len);
  uint8_t buffer[msg_len] = {0};

  int encoded_size = reg_accept->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encode Registration Accept message error");
    cause = k5gmmCauseSemanticallyIncorrect;  // TODO: verify the cause
    return false;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Registration-Accept Message Buffer", buffer, encoded_size);

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    cause =
        k5gmmCauseSecurityModeRejectedUnspecified;  // TODO: verify the cause
    return false;
  }

  bstring protected_nas = nullptr;
  encode_nas_message_protected(
      nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
      NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

  auto itti_msg =
      std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->ran_ue_ngap_id = nc->ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = nc->amf_ue_ngap_id;
  itti_msg->nas            = bstrcpy(protected_nas);

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
    cause = k5gmmCauseCongestion;
    oai::utils::utils::bdestroy_wrapper(&protected_nas);
    return false;
  }

  oai::utils::utils::bdestroy_wrapper(&protected_nas);
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::run_periodic_registration_update_procedure(
    std::shared_ptr<nas_context>& nc, bstring& nas_msg, uint8_t& cause) {
  // Experimental procedure

  // decoding REGISTRATION request
  auto registration_request = std::make_unique<RegistrationRequest>();
  int decoded_size =
      registration_request->Decode((uint8_t*) bdata(nas_msg), blength(nas_msg));

  if (decoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decode Registration Request message error");
    cause = k5gmmCauseSemanticallyIncorrect;
    return false;
  }

  // Encoding REGISTRATION ACCEPT
  auto reg_accept = std::make_unique<RegistrationAccept>();
  initialize_registration_accept(reg_accept, nc);

  // Get UE context
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc, uc)) {
    cause = k5gmmCauseIllegalUe;  // TODO: verify the cause
    return false;
  }

  reg_accept->Set5gGuti(
      amf_cfg.guami.mcc, amf_cfg.guami.mnc, amf_cfg.guami.region_id,
      amf_cfg.guami.amf_set_id, amf_cfg.guami.amf_pointer, uc->tmsi);

  uint16_t pdu_session_status = 0x0000;
  registration_request->GetPduSessionStatus(pdu_session_status);
  reg_accept->SetPduSessionStatus(pdu_session_status);
  Logger::amf_n1().debug(
      "PDU Session Status 0x%02x", htonl(pdu_session_status));

  uint32_t msg_len = reg_accept->GetLength();
  Logger::nas_mm().debug("Size of Registration Accept message %ld", msg_len);
  uint8_t buffer[msg_len] = {0};
  int encoded_size        = reg_accept->Encode(buffer, msg_len);
  if (encoded_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encode Registration Accept message error");
    cause = k5gmmCauseSemanticallyIncorrect;
    return false;
  }
  oai::utils::output_wrapper::print_buffer(
      "amf_n1", "Registration-Accept Message Buffer", buffer, encoded_size);

  if (!nc->security_ctx.has_value()) {
    Logger::amf_n1().error("No Security Context found");
    cause =
        k5gmmCauseSecurityModeRejectedUnspecified;  // TODO: verify the cause
    return false;
  }

  bstring protected_nas = nullptr;
  encode_nas_message_protected(
      nc->security_ctx.value(), false, kIntegrityProtectedAndCiphered,
      NAS_MESSAGE_DOWNLINK, buffer, encoded_size, protected_nas);

  std::shared_ptr<itti_dl_nas_transport> itti_msg =
      std::make_shared<itti_dl_nas_transport>(TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->ran_ue_ngap_id = nc->ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = nc->amf_ue_ngap_id;
  itti_msg->nas            = bstrcpy(protected_nas);

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
    cause = k5gmmCauseCongestion;
    oai::utils::utils::bdestroy_wrapper(&protected_nas);
    return false;
  }

  oai::utils::utils::bdestroy_wrapper(&protected_nas);

  return true;
}

//------------------------------------------------------------------------------
void amf_n1::set_5gmm_state(
    std::shared_ptr<nas_context>& nc, const _5gmm_state_t& state) {
  Logger::amf_n1().debug(
      "Set 5GMM state to %s",
      nas_context::fivegmm_state_to_string(state).c_str());
  std::unique_lock lock(m_nas_context);
  nc->_5gmm_state = state;
}

//------------------------------------------------------------------------------
void amf_n1::get_5gmm_state(
    const std::shared_ptr<nas_context>& nc, _5gmm_state_t& state) const {
  std::shared_lock lock(m_nas_context);
  state = nc->_5gmm_state;
}

//------------------------------------------------------------------------------
void amf_n1::set_5gcm_state(
    std::shared_ptr<nas_context>& nc, const cm_state_t& state) {
  std::shared_lock lock(m_nas_context);
  nc->nas_status = state;
}

//------------------------------------------------------------------------------
void amf_n1::get_5gcm_state(
    const std::shared_ptr<nas_context>& nc, cm_state_t& state) const {
  std::shared_lock lock(m_nas_context);
  state = nc->nas_status;
}

//------------------------------------------------------------------------------
void amf_n1::handle_ue_location_change(
    std::string supi, UserLocation user_location, uint8_t http_version) {
  Logger::amf_n1().debug(
      "Send request to SBI to trigger UE Location Report (SUPI "
      "%s )",
      supi.c_str());
  std::vector<std::shared_ptr<amf_subscription>> subscriptions = {};
  amf_app_inst->get_ee_subscriptions(
      amf_event_type_t::LOCATION_REPORT, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed event
    Logger::amf_n1().debug(
        "Send ITTI msg to AMF SBI to trigger the event notification");

    auto itti_msg = std::make_shared<itti_sbi_notify_subscribed_event>(
        TASK_AMF_N1, TASK_AMF_SBI);

    itti_msg->http_version = http_version;

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
      amf_event_type.set_value("LOCATION_REPORT");
      event_report.setType(amf_event_type);

      oai::model::amf::AmfEventState amf_event_state = {};
      amf_event_state.setActive(true);
      event_report.setState(amf_event_state);

      event_report.setLocation(user_location);

      event_report.setSupi(supi);
      ev_notif.add_report(event_report);

      itti_msg->event_notifs.push_back(ev_notif);
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::handle_ue_reachability_status_change(
    std::string supi, uint8_t status, uint8_t http_version) {
  Logger::amf_n1().debug(
      "Send request to SBI to trigger UE Reachability Report (SUPI "
      "%s )",
      supi.c_str());

  std::vector<std::shared_ptr<amf_subscription>> subscriptions = {};
  amf_app_inst->get_ee_subscriptions(
      amf_event_type_t::REACHABILITY_REPORT, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed event
    Logger::amf_n1().debug(
        "Send ITTI msg to AMF SBI to trigger the event notification");

    auto itti_msg = std::make_shared<itti_sbi_notify_subscribed_event>(
        TASK_AMF_N1, TASK_AMF_SBI);

    itti_msg->http_version = http_version;

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
      amf_event_type.set_value("REACHABILITY_REPORT");
      event_report.setType(amf_event_type);

      oai::model::amf::AmfEventState amf_event_state = {};
      amf_event_state.setActive(true);
      event_report.setState(amf_event_state);

      oai::model::amf::UeReachability ue_reachability = {};
      if (status == CM_CONNECTED)
        ue_reachability.set_value("REACHABLE");
      else
        ue_reachability.set_value("UNREACHABLE");

      event_report.setReachability(ue_reachability);
      event_report.setSupi(supi);
      ev_notif.add_report(event_report);

      itti_msg->event_notifs.push_back(ev_notif);
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::handle_ue_registration_state_change(
    std::string supi, uint8_t status, uint8_t http_version,
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id) {
  Logger::amf_n1().debug(
      "Send request to SBI to trigger UE Registration State Report (SUPI "
      "%s )",
      supi.c_str());

  std::vector<std::shared_ptr<amf_subscription>> subscriptions = {};
  amf_app_inst->get_ee_subscriptions(
      amf_event_type_t::REGISTRATION_STATE_REPORT, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed event
    Logger::amf_n1().debug(
        "Send ITTI msg to AMF SBI to trigger the event notification");

    auto itti_msg = std::make_shared<itti_sbi_notify_subscribed_event>(
        TASK_AMF_N1, TASK_AMF_SBI);

    itti_msg->http_version = http_version;

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
      amf_event_type.set_value("REGISTRATION_STATE_REPORT");
      event_report.setType(amf_event_type);

      oai::model::amf::AmfEventState amf_event_state = {};
      amf_event_state.setActive(true);
      event_report.setState(amf_event_state);

      std::vector<oai::model::amf::RmInfo> rm_infos;
      oai::model::amf::RmInfo rm_info   = {};
      oai::model::amf::RmState rm_state = {};

      if (status == _5GMM_DEREGISTERED)
        rm_state.set_value("DEREGISTERED");
      else if (status == _5GMM_REGISTERED)
        rm_state.set_value("REGISTERED");
      rm_info.setRmState(rm_state);

      AccessType access_type = {};
      access_type.setValue(
          AccessType::eAccessType::_3GPP_ACCESS);  // hard-coded
      rm_info.setAccessType(access_type);

      rm_infos.push_back(rm_info);
      event_report.setRmInfoList(rm_infos);

      event_report.setSupi(supi);
      event_report.setRanUeNgapId(ran_ue_ngap_id);
      event_report.setAmfUeNgapId(amf_ue_ngap_id);
      ev_notif.add_report(event_report);

      itti_msg->event_notifs.push_back(ev_notif);
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::handle_ue_connectivity_state_change(
    std::string supi, uint8_t status, uint8_t http_version) {
  Logger::amf_n1().debug(
      "Send request to SBI to trigger UE Connectivity State Report (SUPI "
      "%s )",
      supi.c_str());

  std::vector<std::shared_ptr<amf_subscription>> subscriptions = {};
  amf_app_inst->get_ee_subscriptions(
      amf_event_type_t::CONNECTIVITY_STATE_REPORT, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed event
    Logger::amf_n1().debug(
        "Send ITTI msg to AMF SBI to trigger the event notification");

    auto itti_msg = std::make_shared<itti_sbi_notify_subscribed_event>(
        TASK_AMF_N1, TASK_AMF_SBI);

    itti_msg->http_version = http_version;

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
      amf_event_type.set_value("CONNECTIVITY_STATE_REPORT");
      event_report.setType(amf_event_type);

      oai::model::amf::AmfEventState amf_event_state = {};
      amf_event_state.setActive(true);
      event_report.setState(amf_event_state);

      std::vector<oai::model::amf::CmInfo> cm_infos;
      oai::model::amf::CmInfo cm_info   = {};
      oai::model::amf::CmState cm_state = {};
      if (status == CM_IDLE)
        cm_state.set_value("IDLE");
      else if (status == CM_CONNECTED)
        cm_state.set_value("CONNECTED");
      cm_info.setCmState(cm_state);

      AccessType access_type = {};
      access_type.setValue(
          AccessType::eAccessType::_3GPP_ACCESS);  // hard-coded
      cm_info.setAccessType(access_type);
      cm_infos.push_back(cm_info);
      event_report.setCmInfoList(cm_infos);

      event_report.setSupi(supi);
      ev_notif.add_report(event_report);

      itti_msg->event_notifs.push_back(ev_notif);
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::handle_ue_communication_failure_change(
    std::string supi, oai::model::amf::CommunicationFailure comm_failure,
    uint8_t http_version) {
  Logger::amf_n1().debug(
      "Send request to SBI to trigger UE Communication Failure Report (SUPI "
      "%s )",
      supi.c_str());
  std::vector<std::shared_ptr<amf_subscription>> subscriptions = {};
  amf_app_inst->get_ee_subscriptions(
      amf_event_type_t::COMMUNICATION_FAILURE_REPORT, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed event
    Logger::amf_n1().debug(
        "Send ITTI msg to AMF SBI to trigger the event notification");

    auto itti_msg = std::make_shared<itti_sbi_notify_subscribed_event>(
        TASK_AMF_N1, TASK_AMF_SBI);

    itti_msg->http_version = http_version;

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

      event_report.setCommFailure(comm_failure);

      event_report.setSupi(supi);
      ev_notif.add_report(event_report);

      itti_msg->event_notifs.push_back(ev_notif);
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::handle_ue_loss_of_connectivity_change(
    std::string supi, uint8_t status, uint8_t http_version,
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id) {
  Logger::amf_n1().debug(
      "Send request to SBI to trigger UE Loss of Connectivity (SUPI "
      "%s )",
      supi.c_str());

  std::vector<std::shared_ptr<amf_subscription>> subscriptions = {};
  amf_app_inst->get_ee_subscriptions(
      amf_event_type_t::LOSS_OF_CONNECTIVITY, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to SBI to trigger the notification to the subscribed event
    Logger::amf_n1().debug(
        "Send ITTI msg to AMF SBI to trigger the event notification");

    auto itti_msg = std::make_shared<itti_sbi_notify_subscribed_event>(
        TASK_AMF_N1, TASK_AMF_SBI);

    itti_msg->http_version = http_version;

    for (auto i : subscriptions) {
      event_notification ev_notif = {};
      ev_notif.set_notify_correlation_id(i->notify_correlation_id);
      ev_notif.set_notify_uri(i->notify_uri);  // Direct subscription
      // ev_notif.set_subs_change_notify_correlation_id(i->notify_uri);

      oai::model::amf::AmfEventReport event_report = {};
      oai::model::amf::AmfEventType amf_event_type = {};
      amf_event_type.set_value("LOSS_OF_CONNECTIVITY");
      event_report.setType(amf_event_type);

      oai::model::amf::AmfEventState amf_event_state = {};
      amf_event_state.setActive(true);
      event_report.setState(amf_event_state);

      oai::model::amf::LossOfConnectivityReason ue_loss_of_connectivity_reason =
          {};
      if (status == DEREGISTERED)
        ue_loss_of_connectivity_reason.set_value("DEREGISTERED");
      else if (status == MAX_DETECTION_TIME_EXPIRED)
        ue_loss_of_connectivity_reason.set_value("MAX_DETECTION_TIME_EXPIRED");
      else if (status == PURGED)
        ue_loss_of_connectivity_reason.set_value("PURGED");

      event_report.setLossOfConnectReason(ue_loss_of_connectivity_reason);
      event_report.setRanUeNgapId(ran_ue_ngap_id);
      event_report.setAmfUeNgapId(amf_ue_ngap_id);
      event_report.setSupi(supi);
      ev_notif.add_report(event_report);

      itti_msg->event_notifs.push_back(ev_notif);
    }

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::trigger_ue_location_report(
    const uint32_t ran_ue_ngap_id, const uint64_t amf_ue_ngap_id) {
  // Find UE context
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc)) return;

  std::shared_ptr<ue_ngap_context> unc = {};
  if (amf_n2_inst->ran_ue_id_2_ue_ngap_context(
          ran_ue_ngap_id, uc->gnb_id, unc)) {
    std::shared_ptr<gnb_context> gc = {};
    if (!amf_n2_inst->assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
      Logger::amf_n1().error(
          "No existed gNB context with assoc_id (%d)", unc->gnb_assoc_id);
      return;
    } else {
      // TODO: get_user_location(uc);
      UserLocation user_location = {};
      NrLocation nr_location     = {};

      oai::model::common::Tai tai = {};
      nlohmann::json tai_json     = {};
      tai_json["plmnId"]["mcc"]   = uc->cgi.mcc;
      tai_json["plmnId"]["mnc"]   = uc->cgi.mnc;
      tai_json["tac"]             = std::to_string(uc->tai.tac);

      nlohmann::json global_ran_node_id_json        = {};
      global_ran_node_id_json["plmnId"]["mcc"]      = uc->cgi.mcc;
      global_ran_node_id_json["plmnId"]["mnc"]      = uc->cgi.mnc;
      global_ran_node_id_json["gNbId"]["bitLength"] = 32;
      global_ran_node_id_json["gNbId"]["gNBValue"] = std::to_string(gc->gnb_id);
      oai::model::common::GlobalRanNodeId global_ran_node_id = {};

      Ncgi ncgi = {};
      oai::model::common::PlmnId plmnId;
      plmnId.setMcc(uc->cgi.mcc);
      plmnId.setMnc(uc->cgi.mnc);
      ncgi.setNid("");
      ncgi.setNrCellId(std::to_string(uc->cgi.nrCellId));
      ncgi.setPlmnId(plmnId);

      try {
        from_json(tai_json, tai);
        from_json(global_ran_node_id_json, global_ran_node_id);
      } catch (std::exception& e) {
        Logger::amf_n1().error("Exception with Json: %s", e.what());
        return;
      }

      // uc->cgi.nrCellID;
      nr_location.setTai(tai);
      nr_location.setGlobalGnbId(global_ran_node_id);
      nr_location.setNcgi(ncgi);
      user_location.setNrLocation(nr_location);

      // Trigger UE Location Report
      std::string supi = uc->supi;
      Logger::amf_n1().debug(
          "Signal the UE Location Report Event notification for SUPI %s",
          supi.c_str());
      event_sub.ue_location_report(
          supi, user_location, amf_cfg.support_features.http_version);
    }
  }
}

//------------------------------------------------------------------------------
void amf_n1::get_pdu_session_to_be_activated(
    const uint16_t status, std::vector<uint8_t>& pdu_session_to_be_activated) {
  std::bitset<16> status_bits(status);

  for (int i = 0; i <= 15; i++) {
    if (status_bits.test(i)) {
      if (i <= 7)
        pdu_session_to_be_activated.push_back(8 + i);
      else if (i > 8)
        pdu_session_to_be_activated.push_back(i - 8);
    }
  }

  if (pdu_session_to_be_activated.size() > 0) {
    for (auto i : pdu_session_to_be_activated)
      Logger::amf_n1().debug("PDU session to be activated %d", i);
  }
}

//------------------------------------------------------------------------------
void amf_n1::initialize_registration_accept(
    std::unique_ptr<RegistrationAccept>& registration_accept,
    const std::shared_ptr<nas_context>& nc) {
  // Registration Result
  registration_accept->Set5gsRegistrationResult(
      false, false, false,
      0x01);  // 3GPP Access

  // Timer T3512
  registration_accept->SetT3512Value(0x5, kT3512TimerValueMin);

  // Timer T3502
  registration_accept->SetT3502Value(kT3502TimerDefaultValueMin);

  // LADN info (TODO)
  LadnInformation ladn_information = {};
  registration_accept->SetLadnInformation(ladn_information);

  // Find UE Context
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc)) return;

  // TAI List
  std::vector<p_tai_t> tai_list;
  for (auto p : amf_cfg.plmn_list) {
    p_tai_t item    = {};
    item.type       = 0x00;
    nas_plmn_t plmn = {};
    plmn.mcc        = p.mcc;
    plmn.mnc        = p.mnc;
    item.plmn_list.push_back(plmn);
    item.tac_list.push_back(p.tac);
    tai_list.push_back(item);
  }
  registration_accept->SetTaiList(tai_list);

  // Network Feature Support
  // TODO: remove hardcoded values
  registration_accept->Set5gsNetworkFeatureSupport(
      0x01, 0x00);  // 0x00, 0x00 to disable IMS

  // Allowed/Rejected/Configured NSSAI
  // Get the list of common SST, SD between UE and AMF
  std::vector<struct SNSSAI_s> common_nssais;
  amf_n2_inst->get_common_NSSAI(nc->ran_ue_ngap_id, uc->gnb_id, common_nssais);

  std::vector<struct SNSSAI_s> allowed_nssais;
  std::vector<RejectedSNssai> rejected_nssais;
  std::vector<struct SNSSAI_s> requested_nssai;

  // If no requested NSSAI available, use subscribed S-NSSAIs instead
  if (nc->requested_nssai.size() > 0) {
    requested_nssai = nc->requested_nssai;
  } else {
    for (const auto& ss : nc->subscribed_snssai)
      requested_nssai.push_back(ss.second);
  }

  // Allowed NSSAI
  for (auto s : common_nssais) {
    SNSSAI_t snssai = {};
    snssai.sst      = s.sst;
    snssai.sd       = s.sd;
    if (s.sd == SD_NO_VALUE) {
      snssai.length = SST_LENGTH;
    } else {
      snssai.length = SST_LENGTH + SD_LENGTH;
    }
    Logger::amf_n1().debug("Allowed S-NSSAI (SST 0x%x, SD 0x%x)", s.sst, s.sd);
    allowed_nssais.push_back(snssai);
    // Store in the NAS context
    nc->allowed_nssai.push_back(snssai);  // TODO: refactor NAS_Context
  }

  // Rejected NSSAIs
  for (auto rn : requested_nssai) {
    bool found = false;

    for (auto s : common_nssais) {
      SNSSAI_t snssai = {};
      snssai.sst      = s.sst;
      snssai.sd       = s.sd;

      if ((rn.sst == s.sst) and (rn.sd == s.sd)) {
        if (s.sd == SD_NO_VALUE) {
          snssai.length = SST_LENGTH;
        } else {
          snssai.length = SST_LENGTH + SD_LENGTH;
        }
        found = true;
        break;
      } else {
        Logger::amf_n1().debug(
            "Requested S-NSSAI (SST 0x%x, SD 0x%x), Configured S-NSSAI "
            "(SST "
            "0x%x, SD 0x%x)",
            rn.sst, rn.sd, s.sst, s.sd);
      }
    }

    if (!found) {
      // Add to list of Rejected NSSAIs
      RejectedSNssai rejected_snssai = {};
      rejected_snssai.SetSST(rn.sst);
      if (rn.sd != SD_NO_VALUE) {
        rejected_snssai.SetSd(rn.sd);
      }
      rejected_snssai.SetCause(1);  // TODO: Hardcoded, S-NSSAI not available in
                                    // the current registration area
      rejected_nssais.push_back(rejected_snssai);
      Logger::amf_n1().debug(
          "Rejected S-NSSAI (SST 0x%x, SD 0x%x)", rn.sst, rn.sd);
    }
  }

  registration_accept->SetAllowedNssai(allowed_nssais);
  registration_accept->SetRejectedNssai(rejected_nssais);
  registration_accept->SetConfiguredNssai(
      allowed_nssais);  // TODO: use Allowed NSSAIs for now
  return;
}

//------------------------------------------------------------------------------
bool amf_n1::find_ue_context(
    const std::shared_ptr<nas_context>& nc, std::shared_ptr<ue_context>& uc) {
  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  std::string ue_context_key =
      amf_conv::get_ue_context_key(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id);

  Logger::amf_n1().debug(
      "Key for UE context search: %s", ue_context_key.c_str());

  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::find_ue_context(
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id,
    std::shared_ptr<ue_context>& uc) {
  std::string ue_context_key =
      amf_conv::get_ue_context_key(ran_ue_ngap_id, amf_ue_ngap_id);

  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) return false;

  return true;
}

//------------------------------------------------------------------------------
void amf_n1::mobile_reachable_timer_timeout(
    timer_id_t& timer_id, const std::string amf_ue_ngap_id_str) {
  uint64_t amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  try {
    amf_ue_ngap_id = std::stol(amf_ue_ngap_id_str);
  } catch (const std::exception& err) {
    Logger::amf_n1().warn(
        "Can not covert AMF UE NGAP ID in string format to long!");
    return;
  }

  std::shared_ptr<nas_context> nc = {};
  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  set_mobile_reachable_timer_timeout(nc, true);

  // Trigger UE Loss of Connectivity Status Notify
  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  Logger::amf_n1().debug(
      "Signal the UE Loss of Connectivity Event notification for SUPI %s",
      supi.c_str());
  event_sub.ue_loss_of_connectivity(
      supi, MAX_DETECTION_TIME_EXPIRED, amf_cfg.support_features.http_version,
      nc->ran_ue_ngap_id, amf_ue_ngap_id);

  // TODO: Start the implicit de-registration timer
  timer_id_t tid = itti_inst->timer_setup(
      kImplicitDeregistrationTimerMin * 60, 0, TASK_AMF_N1,
      TASK_AMF_IMPLICIT_DEREGISTRATION_TIMER_EXPIRE,
      std::to_string(amf_ue_ngap_id));
  Logger::amf_n1().startup(
      "Started Implicit De-Registration Timer (tid %d)", tid);

  set_implicit_deregistration_timer(nc, tid);
}

//------------------------------------------------------------------------------
void amf_n1::implicit_deregistration_timer_timeout(
    timer_id_t timer_id, std::string amf_ue_ngap_id_str) {
  uint64_t amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  try {
    amf_ue_ngap_id = std::stol(amf_ue_ngap_id_str);
  } catch (const std::exception& err) {
    Logger::amf_n1().warn(
        "Can not covert AMF UE NGAP ID in string format to long!");
    return;
  }

  std::shared_ptr<nas_context> nc = {};
  if (!amf_ue_id_2_nas_context(amf_ue_ngap_id, nc)) return;

  // Implicitly de-register UE
  // TODO (4.2.2.3.3 Network-initiated Deregistration @3GPP TS 23.502V16.0.0):
  // If the UE is in CM-CONNECTED state, the AMF may explicitly deregister the
  // UE by sending a Deregistration Request message (Deregistration type, Access
  // Type) to the UE

  // Send PDU Session Release SM Context Request to SMF for each PDU Session
  std::shared_ptr<ue_context> uc = {};

  if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc)) return;

  std::vector<std::shared_ptr<pdu_session_context>> pdu_sessions;
  if (!uc->get_pdu_sessions_context(pdu_sessions)) return;

  for (auto p : pdu_sessions) {
    auto itti_msg = std::make_shared<itti_nsmf_pdusession_release_sm_context>(
        TASK_AMF_N1, TASK_AMF_SBI);
    itti_msg->supi           = uc->supi;
    itti_msg->pdu_session_id = p->pdu_session_id;

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }

  // Send N2 UE Release command to NG-RAN if there is a N2 signalling connection
  // to NG-RAN
  Logger::amf_n1().debug(
      "Sending ITTI UE Context Release Command to TASK_AMF_N2");

  auto itti_msg_cxt_release = std::make_shared<itti_ue_context_release_command>(
      TASK_AMF_N1, TASK_AMF_N2);
  itti_msg_cxt_release->amf_ue_ngap_id = nc->amf_ue_ngap_id;
  itti_msg_cxt_release->ran_ue_ngap_id = nc->ran_ue_ngap_id;
  itti_msg_cxt_release->cause.setChoiceOfCause(Ngap_Cause_PR_nas);
  itti_msg_cxt_release->cause.set(Ngap_CauseNas_deregister);

  int ret = itti_inst->send_msg(itti_msg_cxt_release);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg_cxt_release->get_msg_name());
  }

  // Trigger UE Connectivity Status Notify
  std::string supi = amf_conv::imsi_to_supi(nc->imsi);
  Logger::amf_n1().debug(
      "Signal the UE Connectivity Status Event notification for SUPI %s",
      supi.c_str());
  event_sub.ue_connectivity_state(
      supi, CM_IDLE, amf_cfg.support_features.http_version);
}

//------------------------------------------------------------------------------
void amf_n1::set_implicit_deregistration_timer(
    std::shared_ptr<nas_context>& nc, const timer_id_t& t) {
  std::unique_lock lock(m_nas_context);
  nc->implicit_deregistration_timer = t;
}
//------------------------------------------------------------------------------
void amf_n1::set_mobile_reachable_timer(
    std::shared_ptr<nas_context>& nc, const timer_id_t& t) {
  std::unique_lock lock(m_nas_context);
  nc->mobile_reachable_timer = t;
}

//------------------------------------------------------------------------------
void amf_n1::set_mobile_reachable_timer_timeout(
    std::shared_ptr<nas_context>& nc, const bool& b) {
  std::unique_lock lock(m_nas_context);
  nc->is_mobile_reachable_timer_timeout = b;
}

//------------------------------------------------------------------------------
void amf_n1::get_mobile_reachable_timer_timeout(
    const std::shared_ptr<nas_context>& nc, bool& b) const {
  std::shared_lock lock(m_nas_context);
  b = nc->is_mobile_reachable_timer_timeout;
}

//------------------------------------------------------------------------------
bool amf_n1::get_mobile_reachable_timer_timeout(
    const std::shared_ptr<nas_context>& nc) const {
  std::shared_lock lock(m_nas_context);
  return nc->is_mobile_reachable_timer_timeout;
}

//------------------------------------------------------------------------------
bool amf_n1::reroute_registration_request(
    std::shared_ptr<nas_context>& nc, bool& reroute_result) {
  // Verifying whether this AMF can handle the request (if not, AMF
  // re-allocation procedure will be executed to reroute the Registration "
  // Request to an appropriate AMF

  Logger::amf_n1().debug(
      "Verifying whether this AMF can handle the request...");

  /*
  // Check if the AMF can serve all the requested S-NSSAIs
  if (check_requested_nssai(nc)) {
    Logger::amf_n1().debug(
        "Current AMF can handle all the requested NSSAIs, do not need to "
        "perform AMF Re-allocation procedure");
    return false;
  }
*/

  // Get NSSAI from UDM
  oai::model::amf::Nssai nssai = {};
  if (!get_slice_selection_subscription_data(nc, nssai)) {
    Logger::amf_n1().debug(
        "Could not get the Slice Selection Subscription Data from UDM");
    return false;
  }

  // TODO: Update subscribed NSSAIs

  // Check that AMF can process the Requested NSSAIs or not
  if (check_subscribed_nssai(nc, nssai)) {
    Logger::amf_n1().debug(
        "Current AMF can handle the Requested/Subscribed NSSAIs, no need "
        "to perform AMF Re-allocation procedure");
    return false;
  }

  // If the current AMF can't process the Requested NSSAIs
  // find the appropriate AMFs and let them handle the UE

  // Process NS selection to select the appropriate AMF
  oai::model::amf::SliceInfoForRegistration slice_info = {};
  oai::model::amf::AuthorizedNetworkSliceInfo authorized_network_slice_info =
      {};

  std::vector<SubscribedSnssai> subscribed_snssais;
  for (auto n : nssai.getDefaultSingleNssais()) {
    SubscribedSnssai subscribed_snssai = {};
    subscribed_snssai.setSubscribedSnssai(n);
    subscribed_snssai.setDefaultIndication(true);
    subscribed_snssais.push_back(subscribed_snssai);
  }
  slice_info.setSubscribedNssai(subscribed_snssais);

  // Requested NSSAIs
  std::vector<Snssai> requested_nssais;
  for (auto s : nc->requested_nssai) {
    Snssai nssai = {};
    nssai.setSst(s.sst);
    nssai.setSd(std::to_string(s.sd));
    requested_nssais.push_back(nssai);
  }
  slice_info.setRequestedNssai(requested_nssais);

  if (!get_network_slice_selection(
          nc, amf_app_inst->get_nf_instance(), slice_info,
          authorized_network_slice_info)) {
    Logger::amf_n1().debug(
        "Could not get the Network Slice Selection information from NSSF");
    reroute_result = false;
    return false;
  }

  // Find the appropriate target AMF and send N1MessageNotify to the AMF
  // otherwise reroute NAS message via AN
  std::string target_amf = {};
  if (get_target_amf(nc, target_amf, authorized_network_slice_info)) {
    Logger::amf_n1().debug("Target AMF %s", target_amf.c_str());

    send_n1_message_notity(nc, target_amf);
    return true;
  } else {
    Logger::amf_n1().debug(
        "Could not find an appropriate target AMF to handle UE");
    // Reroute NAS message via AN
    std::string target_amf_set = {};
    if (authorized_network_slice_info.targetAmfSetIsSet()) {
      target_amf_set = authorized_network_slice_info.getTargetAmfSet();
      Logger::amf_n1().debug("Target AMF Set %s", target_amf_set.c_str());
    } else {
      Logger::amf_n1().debug("No Target AMF Set info available!");
      reroute_result = false;
      return false;
    }

    if (reroute_nas_via_an(nc, target_amf_set)) return true;
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::check_requested_nssai(const std::shared_ptr<nas_context>& nc) {
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc))
    return false;

  // If there no requested NSSAIs
  if (nc->requested_nssai.size() == 0) {
    return false;
  }

  bool result = false;
  for (auto p : amf_cfg.plmn_list) {
    // Check PLMN/TAC
    if ((uc->tai.mcc.compare(p.mcc) != 0) or
        (uc->tai.mnc.compare(p.mnc) != 0) or (uc->tai.tac != p.tac)) {
      continue;
    }

    result = true;
    // check if AMF can serve all the requested NSSAIs
    for (auto n : nc->requested_nssai) {
      bool found_nssai = false;
      for (auto s : p.slice_list) {
        if (s.sst == n.sst && s.get_sd_int() == n.sd) {
          found_nssai = true;
          Logger::amf_n1().debug("Found S-NSSAI (SST %d, SD %d)", s.sst, n.sd);
          break;
        }
      }
      if (!found_nssai) {
        result = false;
        break;
      }
    }
  }

  return result;
}

//------------------------------------------------------------------------------
bool amf_n1::check_subscribed_nssai(
    const std::shared_ptr<nas_context>& nc, oai::model::amf::Nssai& nssai) {
  Logger::amf_n1().debug(
      "Verifying whether this AMF can handle Requested/Subscribed S-NSSAIs");
  // Check if the AMF can serve all the requested/subscribed S-NSSAIs

  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc))
    return false;

  bool result = false;

  for (auto p : amf_cfg.plmn_list) {
    Logger::amf_n1().debug(
        "PLMN info: %s",
        p.to_json().dump().c_str());  // TODO: use to_string instead

    // Check PLMN/TAC
    if ((uc->tai.mcc.compare(p.mcc) != 0) or
        (uc->tai.mnc.compare(p.mnc) != 0) or (uc->tai.tac != p.tac)) {
      continue;
    }

    result = true;

    // Find the common NSSAIs between Requested NSSAIs and Subscribed NSSAIs
    Logger::amf_n1().debug(
        "Find the common NSSAIs between Requested NSSAIs and Subscribed "
        "NSSAIs");
    std::vector<Snssai> common_snssais;
    for (auto s : nc->requested_nssai) {
      // std::string sd = std::to_string(s.sd);
      // Check with default subscribed NSSAIs
      for (auto n : nssai.getDefaultSingleNssais()) {
        if (s.sst == n.getSst()) {
          uint32_t sd = n.getSdInt();
          if (sd == s.sd) {
            common_snssais.push_back(n);
            Logger::amf_n1().debug("Common S-NSSAI (SST %d, SD %s)", s.sst, sd);
            break;
          }
        }
      }

      // check with other subscribed NSSAIs
      for (auto n : nssai.getSingleNssais()) {
        if (s.sst == n.getSst()) {
          uint32_t sd = n.getSdInt();
          if (sd == s.sd) {
            common_snssais.push_back(n);
            Logger::amf_n1().debug("Common S-NSSAI (SST %d, SD %s)", s.sst, sd);
            break;
          }
        }
      }
    }

    // If there no requested NSSAIs or no common NSSAIs between requested NSSAIs
    // and Subscribed NSSAIs
    if ((nc->requested_nssai.size() == 0) or (common_snssais.size() == 0)) {
      // Each S-NSSAI in the Default Single NSSAIs must be in the AMF's Slice
      // List
      for (auto n : nssai.getDefaultSingleNssais()) {
        bool found_nssai = false;
        for (auto s : p.slice_list) {
          if (s.sst == n.getSst()) {
            if (n.getSdInt() == s.get_sd_int()) {
              found_nssai = true;
              Logger::amf_n1().debug(
                  "Found S-NSSAI (SST %d, SD %s)", s.sst, n.getSd().c_str());
              break;
            }
          }
        }

        if (!found_nssai) {
          result = false;
          break;
        }
      }
    } else {
      // check if AMF can serve all the common NSSAIs
      for (auto n : common_snssais) {
        bool found_nssai = false;
        for (auto s : p.slice_list) {
          if (s.sst == n.getSst()) {
            if (n.getSdInt() == s.get_sd_int()) {
              found_nssai = true;
              Logger::amf_n1().debug(
                  "Found S-NSSAI (SST %d, SD %s)", s.sst, n.getSd().c_str());
              break;
            }
          }
        }

        if (!found_nssai) {
          result = false;
          break;
        }
      }
    }
  }

  return result;
}

//------------------------------------------------------------------------------
bool amf_n1::get_slice_selection_subscription_data(
    const std::shared_ptr<nas_context>& nc, oai::model::amf::Nssai& nssai) {
  // TODO: UDM selection (from NRF or configuration file)
  if (amf_cfg.support_features.enable_external_ausf_udm) {
    Logger::amf_n1().debug(
        "Get the Slice Selection Subscription Data from UDM");

    std::shared_ptr<ue_context> uc = {};
    if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc))
      return false;

    auto itti_msg =
        std::make_shared<itti_sbi_slice_selection_subscription_data>(
            TASK_AMF_N1, TASK_AMF_SBI);

    // Generate a promise and associate this promise to the ITTI message
    uint32_t promise_id = amf_app_inst->generate_promise_id();
    Logger::amf_n1().debug("Promise ID generated %d", promise_id);

    boost::shared_ptr<boost::promise<nlohmann::json>> p =
        boost::make_shared<boost::promise<nlohmann::json>>();
    boost::shared_future<nlohmann::json> f = p->get_future();
    amf_app_inst->add_promise(promise_id, p);

    itti_msg->http_version = amf_cfg.support_features.http_version;
    itti_msg->supi         = nc->imsi;  // TODO: use SUPI in UDR, uc->supi;
    itti_msg->plmn.mcc     = uc->cgi.mcc;
    itti_msg->plmn.mnc     = uc->cgi.mnc;
    itti_msg->promise_id   = promise_id;

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }

    // Wait for the response available and process accordingly
    std::optional<nlohmann::json> result = std::nullopt;
    oai::utils::utils::wait_for_result(f, result);
    if (result.has_value()) {
      nlohmann::json nssai_json = result.value();
      Logger::amf_n1().debug("Got NSSAI from UDM: %s", nssai_json.dump());
      try {
        from_json(nssai_json, nssai);
      } catch (std::exception& e) {
        return false;
      }

      // Store this info in UE NAS Context
      std::vector<Snssai> default_snssais = nssai.getDefaultSingleNssais();
      // bool default_subscribed_snssai = true;
      for (const auto& ds : default_snssais) {
        oai::nas::SNSSAI_t subscribed_snssai = {};
        subscribed_snssai.sst                = ds.getSst();
        uint32_t subscribed_snssai_sd        = ds.getSdInt();
        subscribed_snssai.sd                 = subscribed_snssai_sd;
        std::pair<bool, oai::nas::SNSSAI_t> tmp;
        tmp.second = subscribed_snssai;
        tmp.first  = true;
        /*
        if (default_subscribed_snssai) {
          tmp.first                 = true;
          default_subscribed_snssai = false;
        } else {
          tmp.first = false;
        }
        */
        nc->subscribed_snssai.push_back(tmp);
      }
      return true;

    } else {
      return false;
    }

  } else {
    // TODO: get from the conf file
    return get_slice_selection_subscription_data_from_conf_file(nc, nssai);
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::get_slice_selection_subscription_data_from_conf_file(
    const std::shared_ptr<nas_context>& nc, oai::model::amf::Nssai& nssai) {
  Logger::amf_n1().debug(
      "Get the Slice Selection Subscription Data from configuration file");

  // For now, use the common NSSAIs, supported by AMF and gNB, as subscribed
  // NSSAIs

  // Get UE context
  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc))
    return false;

  // Get UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_n2_inst->ran_ue_id_2_ue_ngap_context(
          nc->ran_ue_ngap_id, uc->gnb_id, unc))
    return false;

  // Get gNB Context
  std::shared_ptr<gnb_context> gc = {};
  if (!amf_n2_inst->assoc_id_2_gnb_context(unc->gnb_assoc_id, gc)) {
    Logger::amf_n1().error(
        "No existed gNB context with assoc_id (%d)", unc->gnb_assoc_id);
    return false;
  }

  // Find the common NSSAIs between Requested NSSAIs and Subscribed NSSAIs
  std::vector<Snssai> common_snssais;
  // bool default_subscribed_snssai = true;

  for (auto ta : gc->supported_ta_list) {
    for (auto p : ta.plmnSliceSupportList) {
      for (auto s : p.sliceList) {
        Snssai nssai = {};
        uint8_t sst  = 0;
        try {
          sst = std::stoi(s.sst);
        } catch (const std::exception& err) {
          Logger::amf_n1().error("Invalid SST");
          return false;
        }
        nssai.setSst(sst);
        nssai.setSd(s.sd);
        Logger::amf_n1().debug(
            "Added S-NSSAI (SST %d, SD %s)", sst, s.sd.c_str());
        common_snssais.push_back(nssai);
        // Store this info in UE NAS Context
        oai::nas::SNSSAI_t subscribed_snssai = {};
        subscribed_snssai.sst                = sst;
        subscribed_snssai.sd                 = nssai.getSdInt();
        std::pair<bool, oai::nas::SNSSAI_t> tmp;
        tmp.second = subscribed_snssai;
        tmp.first  = true;
        /*
        if (default_subscribed_snssai) {
          tmp.first                 = true;
          default_subscribed_snssai = false;
        } else {
          tmp.first = false;
        }
        */
        nc->subscribed_snssai.push_back(tmp);
      }
    }
  }

  nssai.setDefaultSingleNssais(common_snssais);

  // Print out the list of subscribed NSSAIs
  for (auto n : nssai.getDefaultSingleNssais()) {
    Logger::amf_n1().debug(
        "Default Single NSSAIs: S-NSSAI (SST %d, SD %s)", n.getSst(),
        n.getSd().c_str());
  }

  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::get_network_slice_selection(
    const std::shared_ptr<nas_context>& nc, const std::string& nf_instance_id,
    const oai::model::amf::SliceInfoForRegistration& slice_info,
    oai::model::amf::AuthorizedNetworkSliceInfo&
        authorized_network_slice_info) {
  Logger::amf_n1().debug(
      "Get the Network Slice Selection Information from NSSF");

  std::shared_ptr<ue_context> uc = {};
  if (!find_ue_context(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id, uc))
    return false;

  if (amf_cfg.support_features.enable_nssf) {
    // Get Authorized Network Slice Info from an  external NSSF
    std::shared_ptr<itti_sbi_network_slice_selection_information> itti_msg =
        std::make_shared<itti_sbi_network_slice_selection_information>(
            TASK_AMF_N1, TASK_AMF_SBI);

    // Generate a promise and associate this promise to the ITTI message
    uint32_t promise_id = amf_app_inst->generate_promise_id();
    Logger::amf_n1().debug("Promise ID generated %d", promise_id);

    boost::shared_ptr<boost::promise<nlohmann::json>> p =
        boost::make_shared<boost::promise<nlohmann::json>>();
    boost::shared_future<nlohmann::json> f = p->get_future();
    amf_app_inst->add_promise(promise_id, p);

    itti_msg->http_version   = amf_cfg.support_features.http_version;
    itti_msg->nf_instance_id = nf_instance_id;
    itti_msg->slice_info     = slice_info;
    itti_msg->promise_id     = promise_id;
    // itti_msg->plmn.mcc       = uc->cgi.mcc;
    // itti_msg->plmn.mnc       = uc->cgi.mnc;
    itti_msg->tai.plmn.mcc = uc->tai.mcc;
    itti_msg->tai.plmn.mnc = uc->tai.mnc;
    itti_msg->tai.tac      = uc->tai.tac;

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }

    // Wait for the response available and process accordingly
    std::optional<nlohmann::json> result_opt = std::nullopt;
    oai::utils::utils::wait_for_result(f, result_opt);
    if (result_opt.has_value()) {
      nlohmann::json result = result_opt.value();
      Logger::amf_n1().debug("Got result for promise ID %ld", promise_id);
      if (result.find(kSbiResponseJsonData) != result.end()) {
        Logger::amf_n1().debug(
            "Got Authorized Network Slice Info from NSSF: %s",
            result[kSbiResponseJsonData].dump());
        try {
          from_json(
              result[kSbiResponseJsonData], authorized_network_slice_info);
        } catch (std::exception& e) {
          Logger::amf_n1().warn(
              "Could not parse Authorized Network Slice Info from Json");
          return false;
        }
      } else {
        return false;
      }

    } else {
      Logger::amf_n1().debug(
          "Could not get Authorized Network Slice Info from NSSF");
      return false;
    }

  } else {
    // TODO: Get Authorized Network Slice Info from local configuration file
    return get_network_slice_selection_from_conf_file(
        nf_instance_id, slice_info, authorized_network_slice_info);
  }
  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::get_network_slice_selection_from_conf_file(
    const std::string& nf_instance_id,
    const oai::model::amf::SliceInfoForRegistration& slice_info,
    oai::model::amf::AuthorizedNetworkSliceInfo& authorized_network_slice_info)
    const {
  Logger::amf_n1().debug(
      "Get the Network Slice Selection Information from configuration file");
  // TODO: Get Authorized Network Slice Info from local configuration file
  Logger::amf_n1().info("This feature has not been implemented yet!");

  return false;
}

//------------------------------------------------------------------------------
bool amf_n1::get_target_amf(
    const std::shared_ptr<nas_context>& nc, std::string& target_amf,
    const oai::model::amf::AuthorizedNetworkSliceInfo&
        authorized_network_slice_info) {
  // Get Target AMF from AuthorizedNetworkSliceInfo
  Logger::amf_n1().debug(
      "Get the list of candidates AMFs from the AuthorizedNetworkSliceInfo and "
      "select the appropriate one");
  std::string target_amf_set = {};
  std::string nrf_amf_set = {};  // The URI of NRF NFDiscovery Service to query
                                 // the list of AMF candidates

  if (authorized_network_slice_info.targetAmfSetIsSet()) {
    target_amf_set = authorized_network_slice_info.getTargetAmfSet();
    Logger::amf_n1().debug(
        "Target AMF Set from NSSF %s", target_amf_set.c_str());
    if (authorized_network_slice_info.nrfAmfSetIsSet()) {
      nrf_amf_set = authorized_network_slice_info.getNrfAmfSet();
      Logger::amf_n1().debug("NRF AMF Set from NSSF %s", nrf_amf_set.c_str());
    }
  } else {
    Logger::amf_n1().warn("No Target AMF Set available");
    return false;
  }

  std::vector<std::string> candidate_amf_list;
  if (authorized_network_slice_info.candidateAmfListIsSet()) {
    candidate_amf_list = authorized_network_slice_info.getCandidateAmfList();
    // TODO:
  }

  if (amf_cfg.support_features
          .enable_smf_selection) {  // TODO: define new option for external NRF
    // use NRF's URI from conf file if not available
    if (nrf_amf_set.empty()) {
      amf_sbi_helper::get_nrf_disc_search_nf_instances_uri(
          amf_cfg.nrf_addr, nrf_amf_set);
      Logger::amf_n1().debug(
          "NRF AMF Set from the configuration file %s", nrf_amf_set.c_str());
    }

    // Get list of AMF candidates from NRF
    std::shared_ptr<itti_sbi_nf_instance_discovery> itti_msg =
        std::make_shared<itti_sbi_nf_instance_discovery>(
            TASK_AMF_N1, TASK_AMF_SBI);

    // Generate a promise and associate this promise to the ITTI message
    uint32_t promise_id = amf_app_inst->generate_promise_id();
    Logger::amf_n1().debug("Promise ID generated %d", promise_id);

    boost::shared_ptr<boost::promise<nlohmann::json>> p =
        boost::make_shared<boost::promise<nlohmann::json>>();
    boost::shared_future<nlohmann::json> f = p->get_future();
    amf_app_inst->add_promise(promise_id, p);

    itti_msg->http_version          = amf_cfg.support_features.http_version;
    itti_msg->target_amf_set        = target_amf_set;
    itti_msg->target_amf_set_is_set = true;
    itti_msg->promise_id            = promise_id;
    itti_msg->target_nf_type        = "AMF";
    itti_msg->nrf_amf_set           = nrf_amf_set;

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_n1().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }

    // Wait for the response available and process accordingly
    std::optional<nlohmann::json> result = std::nullopt;
    oai::utils::utils::wait_for_result(f, result);
    if (result.has_value()) {
      nlohmann::json amf_candidate_list = result.value();
      Logger::amf_n1().debug(
          "Got List of AMF candidates from NRF: %s", amf_candidate_list.dump());
      // TODO: Select an AMF from the candidate list
      if (!select_target_amf(nc, target_amf, amf_candidate_list)) {
        Logger::amf_n1().debug(
            "Could not select an appropriate AMF from the AMF candidates");
        return false;
      } else {
        Logger::amf_n1().debug("Candidate AMF: %s", target_amf.c_str());
        return true;
      }

    } else {
      Logger::amf_n1().debug("Could not get List of AMF candidates from NRF");
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::select_target_amf(
    const std::shared_ptr<nas_context>& nc, std::string& target_amf,
    const nlohmann::json& amf_candidate_list) {
  Logger::amf_n1().debug(
      "Select the appropriate AMF from the list of candidates");
  bool result = false;
  // Process data to obtain the target AMF
  if (amf_candidate_list.find("nfInstances") != amf_candidate_list.end()) {
    for (auto& it : amf_candidate_list["nfInstances"].items()) {
      nlohmann::json instance_json = it.value();
      // TODO: do we need to check with sNSSAI?
      if (instance_json.find("sNssais") != instance_json.end()) {
        // Each S-NSSAI in the Default Single NSSAIs must be in the AMF's Slice
        // List
        for (auto& s : instance_json["sNssais"].items()) {
          nlohmann::json Snssai = s.value();  // TODO: validate NSSAIs
        }
      }
      // for now, just IP addr of AMF of the first NF instance
      if (instance_json.find("ipv4Addresses") != instance_json.end()) {
        if (instance_json["ipv4Addresses"].size() > 0)
          target_amf = instance_json["ipv4Addresses"].at(0).get<std::string>();
        result = true;
        break;
      }
    }
  }
  return result;
}

//------------------------------------------------------------------------------
void amf_n1::send_n1_message_notity(
    const std::shared_ptr<nas_context>& nc,
    const std::string& target_amf) const {
  Logger::amf_n1().debug(
      "Send a request to SBI to send N1 Message Notify to the target AMF");

  std::shared_ptr<itti_sbi_n1_message_notify> itti_msg =
      std::make_shared<itti_sbi_n1_message_notify>(TASK_AMF_N1, TASK_AMF_SBI);

  itti_msg->http_version = amf_cfg.support_features.http_version;
  if (nc->registration_request_is_set) {
    itti_msg->registration_request = nc->registration_request;
  }
  itti_msg->target_amf_uri = target_amf;
  itti_msg->supi           = nc->imsi;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
bool amf_n1::reroute_nas_via_an(
    const std::shared_ptr<nas_context>& nc, const std::string& target_amf_set) {
  Logger::amf_n1().debug(
      "Send a request to Reroute NAS message to the target AMF via AN");

  uint16_t amf_set_id = 0;
  if (!get_amf_set_id(target_amf_set, amf_set_id)) {
    Logger::amf_n1().warn("Could not extract AMF Set ID from Target AMF Set");
    return false;
  }

  std::shared_ptr<itti_rereoute_nas> itti_msg =
      std::make_shared<itti_rereoute_nas>(TASK_AMF_N1, TASK_AMF_N2);
  itti_msg->ran_ue_ngap_id = nc->ran_ue_ngap_id;
  itti_msg->amf_ue_ngap_id = nc->amf_ue_ngap_id;
  itti_msg->amf_set_id     = amf_set_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_n1().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        itti_msg->get_msg_name());
  }

  return true;
}

//------------------------------------------------------------------------------
bool amf_n1::get_amf_set_id(
    const std::string& target_amf_set, uint16_t& amf_set_id) {
  std::vector<std::string> words;
  boost::split(
      words, target_amf_set, boost::is_any_of("/"), boost::token_compress_on);
  if (words.size() != 4) {
    Logger::amf_n1().warn(
        "Bad value for Target AMF Set  %s", target_amf_set.c_str());
    return false;
  }
  if (words[3].size() != 3) {
    Logger::amf_n1().warn(
        "Bad value for Target AMF Set  %s", target_amf_set.c_str());
    return false;
  } else {
    try {
      amf_set_id = std::stoul(words[3].substr(0, 1), nullptr, 16)
                   << 8 + std::stoul(words[3].substr(1, 2), nullptr, 16);
    } catch (const std::exception& e) {
      Logger::amf_n1().warn(
          "Error when converting from string to int for AMF Set ID, "
          "error: %s",
          e.what());
    }
  }

  return true;
}

//------------------------------------------------------------------------------
uint8_t amf_n1::get_nas_message_type(uint8_t* buf, uint32_t len) {
  if (len < kNasMessageMinLength) return 0;
  return *(buf + 2);  // message type, 3rd octet
}

//------------------------------------------------------------------------------
void amf_n1::set_pdu_session_status_inactive(
    uint8_t pdu_session_id, uint16_t& pdu_session_status) {
  std::bitset<16> pdu_session_status_bits(pdu_session_status);

  if ((pdu_session_id > 0) and (pdu_session_id <= 7))
    pdu_session_status_bits.reset(pdu_session_id + 8);
  else if ((pdu_session_id > 7) and (pdu_session_id <= 15))
    pdu_session_status_bits.reset(pdu_session_id - 8);

  pdu_session_status = pdu_session_status_bits.to_ulong();
}

//------------------------------------------------------------------------------
void amf_n1::set_pdu_session_reactivation_result(
    uint8_t pdu_session_id, uint16_t& pdu_session_reactivation_result) {
  std::bitset<16> pdu_session_reactivation_result_bits(
      pdu_session_reactivation_result);

  if ((pdu_session_id > 0) and (pdu_session_id <= 7))
    pdu_session_reactivation_result_bits.set(pdu_session_id + 8);
  else if ((pdu_session_id > 7) and (pdu_session_id <= 15))
    pdu_session_reactivation_result_bits.set(pdu_session_id - 8);

  pdu_session_reactivation_result =
      pdu_session_reactivation_result_bits.to_ulong();
}
