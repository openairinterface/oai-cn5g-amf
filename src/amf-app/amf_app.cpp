/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "amf_app.hpp"

#include <chrono>
#include <gmp.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "3gpp_29.500.h"
#include "DlNasTransport.hpp"
#include "GlobalRanNodeId.h"
#include "NonUeN2InfoSubscriptionCreatedData.h"
#include "RegistrationContextContainer.h"
#include "UeN1N2InfoSubscriptionCreatedData.h"
#include "amf_config.hpp"
#include "amf_conversions.hpp"
#include "amf_n1.hpp"
#include "amf_n2.hpp"
#include "amf_sbi.hpp"
#include "amf_sbi_helper.hpp"
#include "amf_statistics.hpp"
#include "itti.hpp"
#include "ngap_app.hpp"
#include "output_wrapper.hpp"
#include "utils.hpp"
#include "AccessAndMobilitySubscriptionData.h"
#include "SmfSelectionSubscriptionData.h"
#include "PolicyAssociation.h"
#include "Amf3GppAccessRegistration.h"
#include "Av5gAka.h"
#include "SearchResult.h"
#include "NFProfile.h"
#include "PcfInfo.h"
#include "PolicyUpdate.h"
#include "AmRequestedValueRep.h"
#include "UeContextInSmfData.h"
#include "UeContextInfo.h"
#include "ProvideLocInfo.h"

using namespace std::chrono;
using namespace oai::ngap;
using namespace oai::nas;
using namespace amf_application;
using namespace oai::config;
using namespace oai::amf::api;

extern amf_app* amf_app_inst;
extern itti_mw* itti_inst;
amf_n2* amf_n2_inst   = nullptr;
amf_n1* amf_n1_inst   = nullptr;
amf_sbi* amf_sbi_inst = nullptr;
extern std::unique_ptr<oai::config::amf_config> amf_cfg;
extern statistics stacs;

void amf_app_task(void*);

//------------------------------------------------------------------------------
amf_app::amf_app()
    : m_ue_contexts(), m_supi2ue_ctx(), m_sbi_response_handlers() {
  ue_contexts           = {};
  supi2ue_ctx           = {};
  sbi_response_handlers = {};
  registered_nrfs       = {};

  Logger::amf_app().startup("Creating AMF application functionality layer");
  if (itti_inst->create_task(TASK_AMF_APP, amf_app_task, nullptr)) {
    Logger::amf_app().error("Cannot create task TASK_AMF_APP");
    throw std::runtime_error("Cannot create task TASK_AMF_APP");
  }

  try {
    amf_n1_inst = new amf_n1();
    amf_n2_inst =
        new amf_n2(std::string(inet_ntoa(amf_cfg->n2.addr4)), amf_cfg->n2.port);
    amf_sbi_inst = new amf_sbi();
  } catch (std::exception& e) {
    Logger::amf_app().error("Cannot create AMF APP: %s", e.what());
    throw;
  }

  // Generate NF Instance ID (UUID)
  generate_uuid();

  // Generate an AMF profile
  generate_amf_profile();

  timer_id_t tid = itti_inst->timer_setup(
      amf_cfg->statistics_interval, 0, TASK_AMF_APP,
      TASK_AMF_APP_PERIODIC_STATISTICS);
  Logger::amf_app().startup("Started timer (%d)", tid);
}

//------------------------------------------------------------------------------
amf_app::~amf_app() {
  Logger::amf_app().info("Starting destruction of amf_app object");

  if (amf_n1_inst) {
    Logger::amf_app().info("Deleting N1 object");
    delete amf_n1_inst;
  }
  if (amf_n2_inst) {
    Logger::amf_app().info("Deleting N2 object");
    delete amf_n2_inst;
  }
  if (amf_sbi_inst) {
    Logger::amf_app().info("Deleting SBI object");
    delete amf_sbi_inst;
  }
}

//------------------------------------------------------------------------------
void amf_app_task(void*) {
  const task_id_t task_id = TASK_AMF_APP;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    timer_id_t tid                       = {};
    switch (msg->msg_type) {
      case NAS_SIG_ESTAB_REQ: {
        Logger::amf_app().debug("Received NAS_SIG_ESTAB_REQ");
        itti_nas_signalling_establishment_request* m =
            dynamic_cast<itti_nas_signalling_establishment_request*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case N1N2_MESSAGE_TRANSFER_REQ: {
        Logger::amf_app().debug("Received N1N2_MESSAGE_TRANSFER_REQ");
        itti_n1n2_message_transfer_request* m =
            dynamic_cast<itti_n1n2_message_transfer_request*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case NON_UE_N2_MESSAGE_TRANSFER_REQ: {
        Logger::amf_app().debug("Received NON_UE_N2_MESSAGE_TRANSFER_REQ");
        itti_non_ue_n2_message_transfer_request* m =
            dynamic_cast<itti_non_ue_n2_message_transfer_request*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_N1_MESSAGE_NOTIFICATION: {
        Logger::amf_app().debug("Received SBI_N1_MESSAGE_NOTIFICATION");
        itti_sbi_n1_message_notification* m =
            dynamic_cast<itti_sbi_n1_message_notification*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_N1N2_MESSAGE_SUBSCRIBE: {
        Logger::amf_app().debug("Received SBI_N1N2_MESSAGE_SUBSCRIBE");
        itti_sbi_n1n2_message_subscribe* m =
            dynamic_cast<itti_sbi_n1n2_message_subscribe*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_N1N2_MESSAGE_UNSUBSCRIBE: {
        Logger::amf_app().debug("Received SBI_N1N2_MESSAGE_UNSUBSCRIBE");
        itti_sbi_n1n2_message_unsubscribe* m =
            dynamic_cast<itti_sbi_n1n2_message_unsubscribe*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_NON_UE_N2_INFO_SUBSCRIBE: {
        Logger::amf_app().debug("Received SBI_NON_UE_N2_INFO_SUBSCRIBE");
        itti_sbi_non_ue_n2_info_subscribe* m =
            dynamic_cast<itti_sbi_non_ue_n2_info_subscribe*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_NON_UE_N2_INFO_UNSUBSCRIBE: {
        Logger::amf_app().debug("Received SBI_NON_UE_N2_INFO_UNSUBSCRIBE");
        itti_sbi_non_ue_n2_info_unsubscribe* m =
            dynamic_cast<itti_sbi_non_ue_n2_info_unsubscribe*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_PDU_SESSION_RELEASE_NOTIF: {
        Logger::amf_app().debug("Received SBI_PDU_SESSION_RELEASE_NOTIF");
        itti_sbi_pdu_session_release_notif* m =
            dynamic_cast<itti_sbi_pdu_session_release_notif*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AMF_CONFIGURATION: {
        Logger::amf_app().debug("Received SBI_AMF_CONFIGURATION");
        itti_sbi_amf_configuration* m =
            dynamic_cast<itti_sbi_amf_configuration*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_UPDATE_AMF_CONFIGURATION: {
        Logger::amf_app().debug("Received SBI_UPDATE_AMF_CONFIGURATION");
        itti_sbi_update_amf_configuration* m =
            dynamic_cast<itti_sbi_update_amf_configuration*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_REGISTER_NF_INSTANCE_RESPONSE: {
        Logger::amf_app().debug("Received SBI_REGISTER_NF_INSTANCE_RESPONSE");
        itti_sbi_register_nf_instance_response* m =
            dynamic_cast<itti_sbi_register_nf_instance_response*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_UPDATE_NF_INSTANCE_RESPONSE: {
        Logger::amf_app().debug("Received SBI_UPDATE_NF_INSTANCE_RESPONSE");
        itti_sbi_update_nf_instance_response* m =
            dynamic_cast<itti_sbi_update_nf_instance_response*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_DEREGISTER_NF_INSTANCE_RESPONSE: {
        Logger::amf_app().debug("Received SBI_DEREGISTER_NF_INSTANCE_RESPONSE");
        itti_sbi_deregister_nf_instance_response* m =
            dynamic_cast<itti_sbi_deregister_nf_instance_response*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_REGISTER_WITH_UDM_RESPONSE: {
        Logger::amf_app().debug("Received SBI_REGISTER_WITH_UDM_RESPONSE");
        itti_sbi_register_with_udm_response* m =
            dynamic_cast<itti_sbi_register_with_udm_response*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_RETRIEVE_AM_DATA_RESPONSE: {
        Logger::amf_app().debug("Received SBI_RETRIEVE_AM_DATA_RESPONSE");
        itti_sbi_retrieve_am_data_response* m =
            dynamic_cast<itti_sbi_retrieve_am_data_response*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE: {
        Logger::amf_app().debug(
            "Received SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE");
        itti_sbi_retrieve_smf_selection_subscription_data_response* m =
            dynamic_cast<
                itti_sbi_retrieve_smf_selection_subscription_data_response*>(
                msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AM_POLICY_ASSOCIATION_RESPONSE: {
        Logger::amf_app().debug("Received SBI_AM_POLICY_ASSOCIATION_RESPONSE");
        itti_sbi_am_policy_association_response* m =
            dynamic_cast<itti_sbi_am_policy_association_response*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE: {
        Logger::amf_app().debug(
            "Received SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE");
        itti_sbi_am_policy_association_termination_response* m =
            dynamic_cast<itti_sbi_am_policy_association_termination_response*>(
                msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE: {
        Logger::amf_app().debug(
            "Received SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE");
        itti_sbi_am_policy_association_update_response* m =
            dynamic_cast<itti_sbi_am_policy_association_update_response*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AM_POLICY_UPDATE_NOTIFICATION: {
        Logger::amf_app().debug("Received SBI_AM_POLICY_UPDATE_NOTIFICATION");
        itti_sbi_am_policy_update_notification* m =
            dynamic_cast<itti_sbi_am_policy_update_notification*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION: {
        Logger::amf_app().debug(
            "Received SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION");
        itti_sbi_am_policy_association_termination_notification* m =
            dynamic_cast<
                itti_sbi_am_policy_association_termination_notification*>(msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE: {
        Logger::amf_app().debug(
            "Received SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE");
        itti_sbi_ue_context_in_smf_data_retrieval_response* m =
            dynamic_cast<itti_sbi_ue_context_in_smf_data_retrieval_response*>(
                msg);
        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AMF_STATUS_CHANGE_SUBSCRIBE_REQUEST: {
        itti_sbi_amf_status_change_subscribe_request* m =
            dynamic_cast<itti_sbi_amf_status_change_subscribe_request*>(msg);
        Logger::amf_app().debug("Received %s", m->get_msg_name());

        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AMF_STATUS_CHANGE_UNSUBSCRIBE_REQUEST: {
        itti_sbi_amf_status_change_unsubscribe_request* m =
            dynamic_cast<itti_sbi_amf_status_change_unsubscribe_request*>(msg);
        Logger::amf_app().debug("Received %s", m->get_msg_name());

        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_AMF_STATUS_CHANGE_SUBSCRIBE_MODIFY: {
        itti_sbi_amf_status_change_subscribe_modify* m =
            dynamic_cast<itti_sbi_amf_status_change_subscribe_modify*>(msg);
        Logger::amf_app().debug("Received %s", m->get_msg_name());

        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_PROVIDE_DOMAIN_SELECTION_INFO: {
        itti_sbi_provide_domain_selection_info* m =
            dynamic_cast<itti_sbi_provide_domain_selection_info*>(msg);
        Logger::amf_app().debug("Received %s", m->get_msg_name());

        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case SBI_PROVIDE_LOCATION_INFO: {
        itti_sbi_provide_location_info* m =
            dynamic_cast<itti_sbi_provide_location_info*>(msg);
        Logger::amf_app().debug("Received %s", m->get_msg_name());

        amf_app_inst->handle_itti_message(std::ref(*m));
      } break;

      case TIME_OUT: {
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          switch (to->arg1_user) {
            case TASK_AMF_APP_PERIODIC_STATISTICS:
              tid = itti_inst->timer_setup(
                  amf_cfg->statistics_interval, 0, TASK_AMF_APP,
                  TASK_AMF_APP_PERIODIC_STATISTICS);
              stacs.display();
              break;
            case TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT:
              amf_app_inst->timer_nrf_heartbeat_timeout(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_AMF_APP_TIMEOUT_NRF_REGISTRATION:
              amf_app_inst->timer_nrf_registration_timeout(
                  to->timer_id, to->arg2_user);
              break;
            default:
              Logger::amf_app().info(
                  "No handler for timer(%d) with arg1_user(%d) ", to->timer_id,
                  to->arg1_user);
          }
        }
      } break;

      case TERMINATE: {
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::amf_app().info("Received terminate message");
          return;
        }
      } break;
      default:
        Logger::amf_app().info("No handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
void amf_app::start() {
  if (amf_app_inst) {
    // Register to NRF if needed
    if (amf_cfg->support_features.enable_nf_registration) register_to_nrf();
  }
}

//------------------------------------------------------------------------------
void amf_app::stop() {
  if (amf_app_inst and amf_cfg->support_features.enable_nf_registration) {
    amf_app_inst->deregister_to_nrf();
  }
}

//------------------------------------------------------------------------------
uint64_t amf_app::generate_amf_ue_ngap_id() {
  uint64_t tmp = 0;
  tmp          = __sync_fetch_and_add(&amf_app_ue_ngap_id_generator, 1);
  return tmp & 0x00ffffffff;  // 40 bits
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_context> amf_app::get_ue_context(
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id) const {
  std::string ue_context_key =
      amf_conv::get_ue_context_key(ran_ue_ngap_id, amf_ue_ngap_id);
  Logger::amf_app().debug(
      "Key for UE context search: %s", ue_context_key.c_str());

  std::shared_lock lock(m_ue_contexts);
  if (ue_contexts.count(ue_context_key) > 0) {
    return ue_contexts.at(ue_context_key);
  }

  return nullptr;
}

//------------------------------------------------------------------------------
void amf_app::set_ue_context(
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id,
    const std::shared_ptr<ue_context>& uc) {
  std::string ue_context_key =
      amf_conv::get_ue_context_key(ran_ue_ngap_id, amf_ue_ngap_id);

  std::unique_lock lock(m_ue_contexts);
  ue_contexts[ue_context_key] = uc;
}

//------------------------------------------------------------------------------
bool amf_app::remove_ue_context(
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id) {
  std::string ue_context_key =
      amf_conv::get_ue_context_key(ran_ue_ngap_id, amf_ue_ngap_id);
  std::unique_lock lock(m_ue_contexts);
  if (ue_contexts.count(ue_context_key) > 0) {
    ue_contexts.erase(ue_context_key);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_context> amf_app::get_ue_context(
    const std::string& supi) const {
  std::shared_lock lock(m_supi2ue_ctx);
  if (supi2ue_ctx.count(supi) > 0) {
    return supi2ue_ctx.at(supi);
  }
  Logger::amf_app().warn("No UE context with UE SUPI %s", supi);
  return nullptr;
}

//------------------------------------------------------------------------------
void amf_app::set_ue_context(
    const std::string& supi, const std::shared_ptr<ue_context>& uc) {
  std::unique_lock lock(m_supi2ue_ctx);
  supi2ue_ctx[supi] = uc;
}

//------------------------------------------------------------------------------
bool amf_app::get_pdu_session_context(
    const std::string& supi, const std::uint8_t pdu_session_id,
    std::shared_ptr<pdu_session_context>& psc) const {
  std::shared_ptr<ue_context> uc = get_ue_context(supi);
  if (!uc) return false;
  if (!uc->get_pdu_session_context(pdu_session_id, psc)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool amf_app::get_pdu_sessions_context(
    const std::string& supi,
    std::vector<std::shared_ptr<pdu_session_context>>& sessions_ctx) const {
  std::shared_ptr<ue_context> uc = get_ue_context(supi);
  if (!uc) return false;
  if (!uc->get_pdu_sessions_context(sessions_ctx)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool amf_app::update_pdu_sessions_context(
    const std::string& supi, uint8_t pdu_session_id,
    const oai::_3gpp::model::SmContextStatusNotification& statusNotification) {
  std::shared_ptr<ue_context> uc = get_ue_context(supi);
  if (!uc) return false;
  auto pdu_session_status = statusNotification.getStatus().getEnumValue();
  if (pdu_session_status == oai::_3gpp::model::SmContextStatus_anyOf::
                                eSmContextStatus_anyOf::RELEASED) {
    if (uc->remove_pdu_sessions_context(pdu_session_id)) return true;
  }
  return false;
}

//------------------------------------------------------------------------------
evsub_id_t amf_app::generate_ev_subscription_id() {
  return evsub_id_generator.get_uid();
}

//------------------------------------------------------------------------------
n1n2sub_id_t amf_app::generate_n1n2_message_subscription_id() {
  return n1n2sub_id_generator.get_uid();
}

//------------------------------------------------------------------------------
std::string amf_app::generate_amf_status_change_sub_id_generator() {
  return std::to_string(amf_status_change_sub_id_generator.get_uid());
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_n1n2_message_transfer_request& itti_msg) {
  if (itti_msg.is_ppi_set) {  // Paging procedure
    Logger::amf_app().info(
        "Handle ITTI N1N2 Message Transfer Request for Paging");
    auto paging_msg = std::make_shared<itti_paging>(TASK_AMF_APP, TASK_AMF_N2);
    amf_n1_inst->supi_2_amf_id(itti_msg.supi, paging_msg->amf_ue_ngap_id);
    amf_n1_inst->supi_2_ran_id(itti_msg.supi, paging_msg->ran_ue_ngap_id);

    int ret = itti_inst->send_msg(paging_msg);
    if (ret != RETURNok) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          paging_msg->get_msg_name());
    }
  } else if (itti_msg.is_nrppa_pdu_set) {
    Logger::amf_app().info(
        "Handle ITTI N1N2 Message Transfer Request for NRPPa PDU");
    auto dl_msg = std::make_shared<itti_downlink_ue_associated_nrppa_transport>(
        TASK_AMF_APP, TASK_AMF_N2);
    dl_msg->nrppa_pdu  = bstrcpy(itti_msg.nrppa_pdu);
    dl_msg->routing_id = bstrcpy(itti_msg.routing_id);
    amf_n1_inst->supi_2_amf_id(itti_msg.supi, dl_msg->amf_ue_ngap_id);
    amf_n1_inst->supi_2_ran_id(itti_msg.supi, dl_msg->ran_ue_ngap_id);
    int ret = itti_inst->send_msg(dl_msg);
    if (ret != RETURNok) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          dl_msg->get_msg_name());
    }
  } else if (itti_msg.is_n1sm_set or itti_msg.is_n2sm_set) {
    Logger::amf_app().info("Handle ITTI N1N2 Message Transfer Request");
    auto dl_msg =
        std::make_shared<itti_downlink_nas_transfer>(TASK_AMF_APP, TASK_AMF_N1);

    if (itti_msg.is_n1sm_set) {
      // Encode DL NAS TRANSPORT message(NAS message)
      auto dl = std::make_unique<DlNasTransport>();
      dl->SetPayloadContainerType(kN1SmInformation);
      dl->SetPayloadContainer(
          (uint8_t*) bdata(bstrcpy(itti_msg.n1sm)), blength(itti_msg.n1sm));
      dl->SetPduSessionId(itti_msg.pdu_session_id);

      uint32_t msg_len = dl->GetLength();
      Logger::nas_mm().debug("Size of DL NAS Transport message %ld", msg_len);
      if (msg_len == 0) return;
      uint8_t nas[msg_len] = {0};
      int encoded_size     = dl->Encode(nas, msg_len);
      oai::utils::output_wrapper::print_buffer(
          "amf_app", "N1N2 message transfer", nas, encoded_size);
      dl_msg->dl_nas = blk2bstr(nas, encoded_size);
    }

    if (!itti_msg.is_n2sm_set) {
      dl_msg->is_n2sm_set = false;
    } else {
      dl_msg->n2sm           = bstrcpy(itti_msg.n2sm);
      dl_msg->pdu_session_id = itti_msg.pdu_session_id;
      dl_msg->is_n2sm_set    = true;
      dl_msg->n2sm_info_type = itti_msg.n2sm_info_type;
    }

    amf_n1_inst->supi_2_amf_id(itti_msg.supi, dl_msg->amf_ue_ngap_id);
    amf_n1_inst->supi_2_ran_id(itti_msg.supi, dl_msg->ran_ue_ngap_id);

    int ret = itti_inst->send_msg(dl_msg);
    if (ret != RETURNok) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_N1",
          dl_msg->get_msg_name());
    }
  } else if (itti_msg.is_n1lpp_set) {
    Logger::amf_app().info("Handle ITTI N1N2 Message Transfer Request LPP-NAS");
    std::shared_ptr<itti_downlink_nas_transfer> dl_msg =
        std::make_shared<itti_downlink_nas_transfer>(TASK_AMF_APP, TASK_AMF_N1);
    // Encode DL NAS TRANSPORT message(NAS message)
    auto dl = std::make_unique<DlNasTransport>();
    dl->SetHeader(kPlain5gsMessage);
    dl->SetPayloadContainerType(kLtePositioningProtocol);
    dl->SetPayloadContainer(
        (uint8_t*) bdata(bstrcpy(itti_msg.n1lpp)), blength(itti_msg.n1lpp));

    uint8_t nas[BUFFER_SIZE_1024];
    int encoded_size = dl->Encode(nas, BUFFER_SIZE_1024);
    oai::utils::output_wrapper::print_buffer(
        "amf_app", "N1N2 message transfer", nas, encoded_size);
    dl_msg->dl_nas = blk2bstr(nas, encoded_size);
    amf_n1_inst->supi_2_amf_id(itti_msg.supi, dl_msg->amf_ue_ngap_id);
    amf_n1_inst->supi_2_ran_id(itti_msg.supi, dl_msg->ran_ue_ngap_id);

    int ret = itti_inst->send_msg(dl_msg);
    if (ret != RETURNok) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_N1",
          dl_msg->get_msg_name());
    }
  } else {
    Logger::amf_app().warn("Unknown N1N2 Message Transfer Request type");
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_non_ue_n2_message_transfer_request& itti_msg) {
  if (itti_msg.is_nrppa_pdu_set) {
    Logger::amf_app().info(
        "Handle ITTI Non UE N2 Message Transfer Request for NRPPa PDU");
    auto dl_msg =
        std::make_shared<itti_downlink_non_ue_associated_nrppa_transport>(
            TASK_AMF_APP, TASK_AMF_N2);
    dl_msg->nrppa_pdu  = bstrcpy(itti_msg.nrppa_pdu);
    dl_msg->routing_id = bstrcpy(itti_msg.routing_id);

    dl_msg->global_ran_node_list = itti_msg.global_ran_node_list;

    int ret = itti_inst->send_msg(dl_msg);
    if (ret != RETURNok) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_N2",
          dl_msg->get_msg_name());
    }
  } else {
    Logger::amf_app().info(
        "Handle ITTI Non UE N2 Message Transfer Request: No NRPPa PDU "
        "available!");
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_nas_signalling_establishment_request& itti_msg) {
  uint64_t amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;

  // Generate amf_ue_ngap_id if necessary
  if ((amf_ue_ngap_id = itti_msg.amf_ue_ngap_id) == INVALID_AMF_UE_NGAP_ID) {
    amf_ue_ngap_id = generate_amf_ue_ngap_id();
  }

  // Get UE context, if the context doesn't exist, create a new one
  std::shared_ptr<ue_context> uc =
      get_ue_context(itti_msg.ran_ue_ngap_id, amf_ue_ngap_id);
  if (uc == nullptr) {
    Logger::amf_app().debug(
        "No existing UE Context, Create a new one with "
        "amf_ue_ngap_id " AMF_UE_NGAP_ID_FMT
        ", ran_ue_ngap_id " RAN_UE_NGAP_ID_FMT,
        amf_ue_ngap_id, itti_msg.ran_ue_ngap_id);

    uc = std::make_shared<ue_context>();
    set_ue_context(itti_msg.ran_ue_ngap_id, amf_ue_ngap_id, uc);
  }

  // Update AMF UE NGAP ID
  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_n2_inst->ran_ue_id_2_ue_ngap_context(
          itti_msg.ran_ue_ngap_id, itti_msg.gnb_id, unc)) {
    Logger::amf_app().error(
        "Could not find UE NGAP Context with ran_ue_ngap_id "
        "(" RAN_UE_NGAP_ID_FMT
        "), gNB ID "
        "(" GNB_ID_FMT ")",
        itti_msg.ran_ue_ngap_id, itti_msg.gnb_id);
  } else {
    unc->amf_ue_ngap_id = amf_ue_ngap_id;
    amf_n2_inst->set_amf_ue_ngap_id_2_ue_ngap_context(amf_ue_ngap_id, unc);
  }

  // Store related information
  uc->cgi                   = itti_msg.cgi;
  uc->tai                   = itti_msg.tai;
  uc->rrc_estb_cause        = itti_msg.rrc_cause;
  uc->is_ue_context_request = itti_msg.ue_ctx_req;
  uc->ran_ue_ngap_id        = itti_msg.ran_ue_ngap_id;
  uc->amf_ue_ngap_id        = amf_ue_ngap_id;
  uc->gnb_id                = itti_msg.gnb_id;

  std::string guti   = {};
  bool is_guti_valid = false;
  if (itti_msg.is_5g_s_tmsi_present) {
    guti = amf_conv::tmsi_to_guti(
        itti_msg.tai.mcc, itti_msg.tai.mnc, amf_cfg->guami.region_id,
        itti_msg._5g_s_tmsi);
    is_guti_valid = true;
    Logger::amf_app().debug("Receiving GUTI %s", guti.c_str());
  }

  // Send NAS PDU to task_amf_n1 for further processing
  auto itti_n1_msg =
      std::make_shared<itti_uplink_nas_data_ind>(TASK_AMF_APP, TASK_AMF_N1);

  itti_n1_msg->amf_ue_ngap_id              = amf_ue_ngap_id;
  itti_n1_msg->ran_ue_ngap_id              = itti_msg.ran_ue_ngap_id;
  itti_n1_msg->is_nas_signalling_estab_req = true;
  itti_n1_msg->nas_msg                     = itti_msg.nas_buf;
  itti_n1_msg->mcc                         = itti_msg.tai.mcc;
  itti_n1_msg->mnc                         = itti_msg.tai.mnc;
  itti_n1_msg->is_guti_valid               = is_guti_valid;
  if (is_guti_valid) {
    itti_n1_msg->guti = guti;
  }

  int ret = itti_inst->send_msg(itti_n1_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N1",
        itti_n1_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_n1_message_notification& itti_msg) {
  Logger::amf_app().info(
      "Target AMF, handling a N1 Message Notification from the initial AMF");

  // Step 1. Get UE, gNB related information

  // Get NAS message (RegistrationRequest, this message included
  // in N1 Message Notify is actually is RegistrationRequest from UE to the
  // initial AMF)
  bstring n1sm = nullptr;
  amf_conv::msg_str_2_msg_hex(itti_msg.n1sm, n1sm);

  // get RegistrationContextContainer including gNB info
  // UE context information, N1 message from UE, AN address
  oai::_3gpp::model::RegistrationContextContainer registration_context =
      itti_msg.notification_msg.getRegistrationCtxtContainer();

  // Step 2. Create gNB context if necessary

  // TODO: How to get SCTP-related information and establish a new SCTP
  // connection between the Target AMF and gNB?
  std::shared_ptr<gnb_context> gc = {};

  // GlobalRAN Node ID (~in NGSetupRequest)
  oai::_3gpp::model::GlobalRanNodeId ran_node_id =
      registration_context.getRanNodeId();
  // RAN UE NGAP ID
  uint32_t ran_ue_ngap_id = registration_context.getAnN2ApId();
  uint32_t gnb_id         = {};

  if (ran_node_id.gNbIdIsSet()) {
    oai::_3gpp::model::GNbId gnb_id_model = ran_node_id.getGNbId();
    try {
      gnb_id = std::stoul(gnb_id_model.getGNBValue(), nullptr, 10);
    } catch (const std::exception& e) {
      Logger::amf_app().warn(
          "Error when converting from string to uint32_t for gNB Value: %s",
          e.what());
      return;
    }

    gc->gnb_id = gnb_id;
    // TODO:   gc->gnb_name
    // TODO: DefaultPagingDRX
    // TODO: Supported TA List
    amf_n2_inst->set_gnb_id_2_gnb_context(gnb_id, gc);
  }

  // UserLocation getUserLocation()
  // std::string getAnN2IPv4Addr()
  // AllowedNssai getAllowedNssai()
  // std::vector<ConfiguredSnssai>& getConfiguredNssai();
  // rejectedNssaiInPlmn
  // rejectedNssaiInTa
  // std::string getInitialAmfName()

  // Step 3. Create UE Context
  oai::_3gpp::model::UeContext ue_ctx = registration_context.getUeContext();
  std::string supi                    = {};
  std::shared_ptr<ue_context> uc      = {};

  if (ue_ctx.supiIsSet()) {
    supi = ue_ctx.getSupi();
    // Update UE Context
    uc = get_ue_context(supi);
    if (!uc) {
      // Create a new UE Context
      Logger::amf_app().debug(
          "No existing UE Context, Create a new one with SUPI %s",
          supi.c_str());
      uc                 = std::shared_ptr<ue_context>(new ue_context());
      uc->amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
      uc->supi           = supi;
      uc->gnb_id         = gnb_id;
      set_ue_context(supi, uc);
    }
  }

  // TODO: 5gMmCapability
  // TODO: MmContext
  // TODO: PduSessionContext

  uint64_t amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  // Generate AMF UE NGAP ID if necessary
  if (!uc) {  // No UE context existed
    amf_ue_ngap_id = generate_amf_ue_ngap_id();
  } else {
    if ((amf_ue_ngap_id = uc->amf_ue_ngap_id) == INVALID_AMF_UE_NGAP_ID) {
      amf_ue_ngap_id = generate_amf_ue_ngap_id();
    }
  }

  if (get_ue_context(ran_ue_ngap_id, amf_ue_ngap_id) == nullptr) {
    // Create a new UE Context
    Logger::amf_app().debug(
        "Create a new UE Context with AMF UE NGAP ID "
        "(" AMF_UE_NGAP_ID_FMT
        "), RAN UE NGAP ID "
        "(" RAN_UE_NGAP_ID_FMT ")",
        amf_ue_ngap_id, ran_ue_ngap_id);

    uc         = std::make_shared<ue_context>();
    uc->gnb_id = gnb_id;
    set_ue_context(ran_ue_ngap_id, amf_ue_ngap_id, uc);
  } else {
    uc = get_ue_context(ran_ue_ngap_id, amf_ue_ngap_id);
  }

  // Update info for UE context
  uc->amf_ue_ngap_id = amf_ue_ngap_id;
  uc->ran_ue_ngap_id = ran_ue_ngap_id;
  // RrcEstCause
  if (registration_context.rrcEstCauseIsSet()) {
    uint8_t rrc_cause = {};
    try {
      rrc_cause =
          std::stoul(registration_context.getRrcEstCause(), nullptr, 10);
    } catch (const std::exception& e) {
      Logger::amf_app().warn(
          "Error when converting from string to int for RrcEstCause: %s",
          e.what());
      rrc_cause = 0;
    }

    uc->rrc_estb_cause = rrc_cause;
  }
  // ueContextRequest
  uc->is_ue_context_request = registration_context.isUeContextRequest();

  // Step 4. Create UE NGAP Context if necessary
  // Create/Update UE NGAP Context
  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_n2_inst->ran_ue_id_2_ue_ngap_context(ran_ue_ngap_id, gnb_id, unc)) {
    Logger::amf_app().debug(
        "Create a new UE NGAP context with ran_ue_ngap_id " RAN_UE_NGAP_ID_FMT,
        ran_ue_ngap_id);
    unc = std::shared_ptr<ue_ngap_context>(new ue_ngap_context());
    amf_n2_inst->set_ran_ue_ngap_id_2_ue_ngap_context(
        ran_ue_ngap_id, gnb_id, unc);
  }

  // Store related information into UE NGAP context
  unc->ran_ue_ngap_id = ran_ue_ngap_id;
  // TODO:  unc->sctp_stream_recv
  // TODO: unc->sctp_stream_send
  // TODO: gc->next_sctp_stream
  // TODO: unc->gnb_assoc_id
  // TODO: unc->tai

  // Step 5. Trigger the procedure following RegistrationRequest

  auto itti_n1_msg =
      std::make_shared<itti_uplink_nas_data_ind>(TASK_AMF_APP, TASK_AMF_N1);

  itti_n1_msg->amf_ue_ngap_id              = amf_ue_ngap_id;
  itti_n1_msg->ran_ue_ngap_id              = ran_ue_ngap_id;
  itti_n1_msg->is_nas_signalling_estab_req = true;
  itti_n1_msg->nas_msg                     = bstrcpy(n1sm);
  itti_n1_msg->mcc                         = ran_node_id.getPlmnId().getMcc();
  itti_n1_msg->mnc                         = ran_node_id.getPlmnId().getMnc();
  itti_n1_msg->is_guti_valid               = false;

  int ret = itti_inst->send_msg(itti_n1_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N1",
        itti_n1_msg->get_msg_name());
  }

  oai::utils::utils::bdestroy_wrapper(&n1sm);
  return;
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_n1n2_message_subscribe& itti_msg) {
  Logger::amf_app().info("Handle an N1N2MessageSubscribe from a NF");

  // Generate a subscription ID Id and store the corresponding information in a
  // map (subscription id, info)
  n1n2sub_id_t n1n2sub_id = generate_n1n2_message_subscription_id();
  auto subscription_data =
      std::make_shared<oai::_3gpp::model::UeN1N2InfoSubscriptionCreateData>(
          itti_msg.subscription_data);
  add_n1n2_message_subscription(
      itti_msg.ue_cxt_id, n1n2sub_id, subscription_data);

  std::string location = amf_sbi_helper::get_amf_n1n2_message_subscribe_uri(
      amf_cfg->sbi, itti_msg.ue_cxt_id, std::to_string((uint32_t) n1n2sub_id));

  // Trigger the response from AMF API Server
  oai::_3gpp::model::UeN1N2InfoSubscriptionCreatedData created_data = {};
  created_data.setN1n2NotifySubscriptionId(
      std::to_string((uint32_t) n1n2sub_id));
  nlohmann::json created_data_json = {};
  to_json(created_data_json, created_data);

  nlohmann::json response_data        = {};
  response_data[kSbiResponseJsonData] = created_data;
  response_data[kSbiResponseHttpResponseCode] =
      oai::common::sbi::http_status_code::CREATED;
  response_data[kSbiResponseHeaderLocation] = location;

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_n1n2_message_unsubscribe& itti_msg) {
  Logger::amf_app().info("Handle an N1N2MessageUnSubscribe from a NF");

  // Process the request and trigger the response from AMF API Server
  nlohmann::json response_data = {};
  if (remove_n1n2_message_subscription(
          itti_msg.ue_cxt_id, itti_msg.subscription_id)) {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::NO_CONTENT;
  } else {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::BAD_REQUEST;
    oai::_3gpp::model::ProblemDetails problem_details = {};
    // TODO set problem_details
    to_json(response_data["ProblemDetails"], problem_details);
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_non_ue_n2_info_subscribe& itti_msg) {
  Logger::amf_app().info("Handle an NonUEN2InfoSubscribe from a NF");

  // Generate a subscription ID Id and store the corresponding information in a
  // map (subscription id, info)
  n1n2sub_id_t n2sub_id = generate_n1n2_message_subscription_id();
  auto subscription_data =
      std::make_shared<oai::_3gpp::model::NonUeN2InfoSubscriptionCreateData>(
          itti_msg.subscription_data);

  add_non_ue_n2_info_subscription(n2sub_id, subscription_data);

  std::string location = amf_sbi_helper::get_non_ue_n2_info_subscribe_uri(
      amf_cfg->sbi, std::to_string((uint32_t) n2sub_id));

  // Trigger the response from AMF API Server
  oai::_3gpp::model::NonUeN2InfoSubscriptionCreatedData created_data = {};

  created_data.setN2NotifySubscriptionId(std::to_string((uint32_t) n2sub_id));

  nlohmann::json created_data_json = {};
  to_json(created_data_json, created_data);

  nlohmann::json response_data        = {};
  response_data[kSbiResponseJsonData] = created_data;
  response_data[kSbiResponseHttpResponseCode] =
      oai::common::sbi::http_status_code::CREATED;
  response_data[kSbiResponseHeaderLocation] = location;

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_non_ue_n2_info_unsubscribe& itti_msg) {
  Logger::amf_app().info("Handle an NonUEN2InfoUnsubscribe from a NF");

  // Process the request and trigger the response from AMF API Server
  nlohmann::json response_data = {};
  if (remove_non_ue_n2_info_subscription(itti_msg.subscription_id)) {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::NO_CONTENT;
  } else {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::BAD_REQUEST;
    oai::_3gpp::model::ProblemDetails problem_details = {};
    // TODO set problem_details
    to_json(response_data["ProblemDetails"], problem_details);
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}
//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_pdu_session_release_notif& itti_msg) {
  Logger::amf_app().info("Handle an PDU Session Release notification from SMF");

  // Process the request and trigger the response from AMF API Server
  nlohmann::json response_data = {};
  if (update_pdu_sessions_context(
          itti_msg.ue_id, itti_msg.pdu_session_id,
          itti_msg.smContextStatusNotification)) {
    Logger::amf_app().debug("Update PDU Session Release successfully");
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::NO_CONTENT;

  } else {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::NO_CONTENT;
    // TODO check if we set problem_details
    Logger::amf_app().debug("Update PDU Session Release failed");
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_amf_configuration& itti_msg) {
  Logger::amf_app().info("Handle an SBIAMFConfiguration from a NF");

  // Process the request and trigger the response from AMF API Server
  nlohmann::json response_data        = {};
  response_data[kSbiResponseJsonData] = {};
  if (read_amf_configuration(response_data[kSbiResponseJsonData])) {
    Logger::amf_app().debug(
        "AMF configuration:\n %s",
        response_data[kSbiResponseJsonData].dump().c_str());
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::OK;
  } else {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::BAD_REQUEST;
    oai::_3gpp::model::ProblemDetails problem_details = {};
    // TODO set problem_details
    to_json(response_data["ProblemDetails"], problem_details);
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_update_amf_configuration& itti_msg) {
  Logger::amf_app().info("Handle a request UpdateAMFConfiguration from a NF");

  // Process the request and trigger the response from AMF API Server
  nlohmann::json response_data        = {};
  response_data[kSbiResponseJsonData] = itti_msg.configuration;

  if (update_amf_configuration(response_data[kSbiResponseJsonData])) {
    Logger::amf_app().debug(
        "AMF configuration:\n %s",
        response_data[kSbiResponseJsonData].dump().c_str());
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::OK;

    // Update AMF profile
    generate_amf_profile();

    // Update AMF profile (complete replacement of the existing profile by a new
    // one)
    if (amf_cfg->support_features.enable_nf_registration) register_to_nrf();

  } else {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::BAD_REQUEST;
    oai::_3gpp::model::ProblemDetails problem_details = {};
    // TODO set problem_details
    to_json(response_data["ProblemDetails"], problem_details);
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_register_nf_instance_response& r) {
  Logger::amf_app().debug("Handle NF Instance Registration response");

  if ((r.http_response_code == oai::common::sbi::http_status_code::OK) or
      (r.http_response_code == oai::common::sbi::http_status_code::CREATED)) {
    Logger::amf_app().debug("AMF has successfully registered to NRF.");
    nf_instance_profile = r.profile;
    // Set heartbeat timer
    Logger::amf_app().debug(
        "Set value of NRF Heartbeat timer to %d",
        r.profile.get_nf_heartBeat_timer());
    timer_id_t timer_nrf_heartbeat = itti_inst->timer_setup(
        r.profile.get_nf_heartBeat_timer(), 0, TASK_AMF_APP,
        TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT, r.nrf_uri);
    timer_nrfs_heartbeat.emplace(
        std::make_pair(r.nrf_uri, timer_nrf_heartbeat));
  } else {
    Logger::amf_app().warn(
        "NF Instance Registration, got issue when registering to NRF, try "
        "again ...");
    // Set timer to try again with NF Registration
    itti_inst->timer_setup(
        20, 0, TASK_AMF_APP, TASK_AMF_APP_TIMEOUT_NRF_REGISTRATION, r.nrf_uri);
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_deregister_nf_instance_response& r) {
  Logger::amf_app().debug("Handle NF Deregistration response");

  auto response_code = r.http_response_code;
  if (response_code == oai::common::sbi::http_status_code::NO_CONTENT) {
    Logger::amf_app().debug("AMF has successfully deregistered to NRF.");
    // Stop Heartbeat with this NRF
    if (timer_nrfs_heartbeat.count(r.nrf_uri) > 0) {
      itti_inst->timer_remove(timer_nrfs_heartbeat.at(r.nrf_uri));
    }

  } else if (
      (response_code ==
       oai::common::sbi::http_status_code::TEMPORARY_REDIRECT) or
      (response_code ==
       oai::common::sbi::http_status_code::PERMANENT_REDIRECT)) {
    // TODO: send new request to new NRF
  } else {
    // Deregistration failed, just ignore for the moment
    Logger::amf_app().debug("AMF has failed to deregister to NRF.");
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_update_nf_instance_response& r) {
  Logger::amf_app().debug("Handle NF Update response");
  if ((r.http_response_code !=
       oai::common::sbi::http_status_code::NO_CONTENT) and
      (r.http_response_code != oai::common::sbi::http_status_code::OK)) {
    register_to_nrf(r.nrf_uri);  // trigger again registration procedure
  } else {
    Logger::amf_app().debug(
        "Set a timer to the next Heart-beat (%d)",
        nf_instance_profile.get_nf_heartBeat_timer());

    timer_id_t timer_nrf_heartbeat = itti_inst->timer_setup(
        nf_instance_profile.get_nf_heartBeat_timer(), 0, TASK_AMF_APP,
        TASK_AMF_APP_TIMEOUT_NRF_HEARTBEAT, r.nrf_uri);
    timer_nrfs_heartbeat.emplace(
        std::make_pair(r.nrf_uri, timer_nrf_heartbeat));

    // Store the registered NRF
    registered_nrfs.insert(r.nrf_uri);
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_register_with_udm_response& r) {
  Logger::amf_app().debug("Handle SBI_REGISTER_WITH_UDM_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::NO_CONTENT) {
    // TODO:
  } else if (response_code == oai::common::sbi::http_status_code::CREATED) {
    // Store location
    if (r.response_data.find(kSbiResponseHeaderLocation) !=
        r.response_data.end()) {
      std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
      if (uc) {
        uc->amf_3gpp_access_location =
            r.response_data[kSbiResponseHeaderLocation].get<std::string>();
      }
    }
    // TODO:
  } else if (response_code == oai::common::sbi::http_status_code::OK) {
    // TODO:
  } else {
    Logger::amf_app().debug("AMF has failed to register to UDM.");
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_retrieve_am_data_response& r) {
  Logger::amf_app().debug("Handle SBI_RETRIEVE_AM_DATA_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::OK) {
    // Store Access and Mobility Subscription Data
    if (r.response_data.find(kSbiResponseJsonData) != r.response_data.end()) {
      std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
      if (uc) {
        try {
          oai::_3gpp::model::AccessAndMobilitySubscriptionData am_data = {};
          from_json(r.response_data[kSbiResponseJsonData], am_data);
          //  uc->am_data =
          //  std::make_optional<oai::_3gpp::model::AccessAndMobilitySubscriptionData>(am_data);
        } catch (std::exception& e) {
          Logger::amf_app().warn(
              "Could not parse Access and Mobility Subscription Data from "
              "Json");
        }
      }
    }
  } else {
    Logger::amf_app().debug(
        "AMF has failed to get Access and Mobility Subscription Data from "
        "UDM.");
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_retrieve_smf_selection_subscription_data_response& r) {
  Logger::amf_app().debug(
      "Handle SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::OK) {
    // Store Access and Mobility Subscription Data
    if (r.response_data.find(kSbiResponseJsonData) != r.response_data.end()) {
      std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
      if (uc) {
        try {
          oai::_3gpp::model::SmfSelectionSubscriptionData
              smf_selection_subscription_data = {};
          from_json(
              r.response_data[kSbiResponseJsonData],
              smf_selection_subscription_data);
          //  uc->smf_selection_subscription_data =
          //  std::make_optional<oai::model::udm::SmfSelectionSubscriptionData>(smf_selection_subscription_data);
        } catch (std::exception& e) {
          Logger::amf_app().warn(
              "Could not parse SMF Selection Subscription Data from "
              "Json");
        }
      }
    }
  } else {
    Logger::amf_app().debug(
        "AMF has failed to get SMF Selection Subscription Data from "
        "UDM.");
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_am_policy_association_response& r) {
  Logger::amf_app().debug("Handle SBI_AM_POLICY_ASSOCIATION_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::OK) {
    // Store PolicyAssociation
    if (r.response_data.find(kSbiResponseJsonData) != r.response_data.end()) {
      std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
      if (uc) {
        try {
          oai::_3gpp::model::PolicyAssociation policy_association = {};
          from_json(r.response_data[kSbiResponseJsonData], policy_association);
          // Store the policy association in the UE context
          uc->policy_association =
              std::make_optional<oai::_3gpp::model::PolicyAssociation>(
                  policy_association);
          // Store the location of the Policy Association
          if (r.response_data.find(kSbiResponseHeaderLocation) !=
              r.response_data.end()) {
            uc->policy_association_location =
                r.response_data[kSbiResponseHeaderLocation].get<std::string>();
          }
        } catch (std::exception& e) {
          Logger::amf_app().warn(
              "Could not parse the Policy Association from "
              "Json");
        }
      }
    }
  } else {
    Logger::amf_app().debug(
        "AMF has failed to get the Policy Association from "
        "PCF.");
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_am_policy_association_termination_response& r) {
  Logger::amf_app().debug(
      "Handle SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::NO_CONTENT) {
    // Remove PolicyAssociation
    std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
    if (uc) {
      uc->policy_association_location = {};
      uc->policy_association          = {};
    }
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_am_policy_association_update_response& r) {
  Logger::amf_app().debug(
      "Handle SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::OK) {
    // Process Policy Update

    if (r.response_data.find(kSbiResponseJsonData) != r.response_data.end()) {
      std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
      if (uc) {
        try {
          oai::_3gpp::model::PolicyUpdate policy_update = {};
          from_json(r.response_data[kSbiResponseJsonData], policy_update);

          // Store the updated policy association in the UE context
          oai::_3gpp::model::PolicyAssociation policy_association = {};
          // TODO:
          uc->policy_association =
              std::make_optional<oai::_3gpp::model::PolicyAssociation>(
                  policy_association);

        } catch (std::exception& e) {
          Logger::amf_app().warn(
              "Could not parse the Policy Association from "
              "Json");
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_am_policy_association_retrieval_response& r) {
  Logger::amf_app().debug(
      "Handle SBI_AM_POLICY_ASSOCIATION_RETRIEVAL_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::OK) {
    // Process PolicyAssociation and update the UE context

    if (r.response_data.find(kSbiResponseJsonData) != r.response_data.end()) {
      std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
      if (uc) {
        try {
          oai::_3gpp::model::PolicyAssociation policy_association = {};
          from_json(r.response_data[kSbiResponseJsonData], policy_association);
          uc->policy_association =
              std::make_optional<oai::_3gpp::model::PolicyAssociation>(
                  policy_association);

        } catch (std::exception& e) {
          Logger::amf_app().warn(
              "Could not parse the Policy Association from "
              "Json");
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_am_policy_update_notification& itti_msg) {
  Logger::amf_app().debug("Handle SBI_AM_POLICY_UPDATE_NOTIFICATION");

  nlohmann::json response_data = {};

  std::shared_ptr<ue_context> uc = get_ue_context(itti_msg.supi);
  if (uc) {
    // TODO: Store the updated policy association in the UE context
    oai::_3gpp::model::PolicyAssociation policy_association = {};
    uc->policy_association =
        std::make_optional<oai::_3gpp::model::PolicyAssociation>(
            policy_association);

    // TODO: Prepare the response
    oai::_3gpp::model::AmRequestedValueRep am_requested_value_rep = {};

    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::OK;
    to_json(response_data[kSbiResponseJsonData], am_requested_value_rep);

  } else {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::BAD_REQUEST;
    oai::_3gpp::model::ProblemDetails problem_details = {};
    // TODO set problem_details
    to_json(response_data[kSbiResponseJsonData], problem_details);
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_am_policy_association_termination_notification& itti_msg) {
  Logger::amf_app().debug(
      "Handle SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION");

  nlohmann::json response_data = {};
  // Process the Termination Notification and clear the policy association

  std::shared_ptr<ue_context> uc = get_ue_context(itti_msg.supi);
  if (uc) {
    uc->policy_association = std::nullopt;
  } else {
    response_data[kSbiResponseHttpResponseCode] =
        oai::common::sbi::http_status_code::BAD_REQUEST;
    oai::_3gpp::model::ProblemDetails problem_details = {};
    // TODO set problem_details
    to_json(response_data[kSbiResponseJsonData], problem_details);
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_ue_context_in_smf_data_retrieval_response& r) {
  Logger::amf_app().debug(
      "Handle SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE response");

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;
  if (r.response_data.find(kSbiResponseHttpResponseCode) !=
      r.response_data.end()) {
    response_code = r.response_data[kSbiResponseHttpResponseCode].get<int>();
  }

  if (response_code == oai::common::sbi::http_status_code::OK) {
    // Process the response

    if (r.response_data.find(kSbiResponseJsonData) != r.response_data.end()) {
      std::shared_ptr<ue_context> uc = get_ue_context(r.supi);
      if (uc) {
        try {
          oai::_3gpp::model::UeContextInSmfData ue_context = {};
          from_json(r.response_data[kSbiResponseJsonData], ue_context);
          // Store the UE Context
          // TODO:
        } catch (std::exception& e) {
          Logger::amf_app().warn(
              "Could not parse the UeContextInSmfData from "
              "Json");
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_amf_status_change_subscribe_request& itti_msg) {
  Logger::amf_app().debug("Handle %s", itti_msg.get_msg_name());

  // Generate a subscription ID Id and store the corresponding information in a
  // map (subscription id, subscription data)
  std::string amf_status_change_sub_id =
      generate_amf_status_change_sub_id_generator();

  auto sd = std::make_shared<oai::_3gpp::model::SubscriptionData>(
      itti_msg.subscription_data);
  // Store subscription data
  add_amf_status_change_subscription(amf_status_change_sub_id, sd);

  nlohmann::json response_data = {};
  response_data[kSbiResponseHttpResponseCode] =
      oai::common::sbi::http_status_code::CREATED;
  response_data[kSbiResponseHeaderLocation] =
      amf_sbi_helper::get_amf_status_change_subscribe_uri(
          amf_cfg->sbi, amf_status_change_sub_id);

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }

  return;
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_amf_status_change_unsubscribe_request& itti_msg) {
  Logger::amf_app().debug("Handle %s", itti_msg.get_msg_name());

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;

  // Remove subscription data
  if (remove_amf_status_change_subscription(itti_msg.subscription_id)) {
    Logger::amf_app().debug(
        "AMF status change subscription %s is removed",
        itti_msg.subscription_id);
    response_code = oai::common::sbi::http_status_code::NO_CONTENT;
  } else {
    Logger::amf_app().debug(
        "AMF status change subscription %s could not be removed ",
        itti_msg.subscription_id);
    response_code = oai::common::sbi::http_status_code::NOT_FOUND;
  }

  nlohmann::json response_data                = {};
  response_data[kSbiResponseHttpResponseCode] = response_code;

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }

  return;
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_amf_status_change_subscribe_modify& itti_msg) {
  Logger::amf_app().debug("Handle %s", itti_msg.get_msg_name());

  // Update subscription data
  auto sd = std::make_shared<oai::_3gpp::model::SubscriptionData>(
      itti_msg.subscription_data);

  uint32_t response_code = oai::common::sbi::http_status_code::NO_RESPONSE;

  if (update_amf_status_change_subscription(itti_msg.subscription_id, sd)) {
    Logger::amf_app().debug(
        "AMF status change subscription %s is updated",
        itti_msg.subscription_id);
    response_code = oai::common::sbi::http_status_code::NO_CONTENT;
  } else {
    Logger::amf_app().debug(
        "AMF status change subscription %s could not be updated ",
        itti_msg.subscription_id);
    response_code = oai::common::sbi::http_status_code::BAD_REQUEST;
  }

  nlohmann::json response_data                = {};
  response_data[kSbiResponseHttpResponseCode] = response_code;

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }

  return;
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_sbi_provide_domain_selection_info& itti_msg) {
  Logger::amf_app().debug("Handle %s", itti_msg.get_msg_name());

  // TODO: process the request and prepare the response
  uint32_t response_code = oai::common::sbi::http_status_code::OK;

  oai::_3gpp::model::UeContextInfo ue_context_info = {};
  ue_context_info.setSupportVoPS(true);
  nlohmann::json ue_context_info_json = {};
  to_json(ue_context_info_json, ue_context_info);

  nlohmann::json response_data                = {};
  response_data[kSbiResponseJsonData]         = ue_context_info_json;
  response_data[kSbiResponseHttpResponseCode] = response_code;

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }

  return;
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(itti_sbi_provide_location_info& itti_msg) {
  Logger::amf_app().debug("Handle %s", itti_msg.get_msg_name());

  nlohmann::json response_data = {};
  uint32_t response_code = oai::common::sbi::http_status_code::BAD_REQUEST;

  std::shared_ptr<ue_context> uc = get_ue_context(itti_msg.ue_context_id);
  if (uc) {
    oai::_3gpp::model::ProvideLocInfo provide_loc_info = {};
    // Current Loc
    provide_loc_info.setCurrentLoc(true);

    // UserLocation
    oai::_3gpp::model::UserLocation user_location = {};
    oai::_3gpp::model::NrLocation nr_location     = {};
    oai::_3gpp::model::Tai tai                    = {};
    oai::_3gpp::model::PlmnId plmn_id             = {};
    plmn_id.setMcc(uc->tai.mcc);
    plmn_id.setMnc(uc->tai.mnc);
    tai.setPlmnId(plmn_id);
    tai.setTac(std::to_string(uc->tai.tac));

    nr_location.setTai(tai);
    // nr_location.setNcgi(uc->cgi);

    oai::_3gpp::model::GNbId gnb_id = {};
    gnb_id.setBitLength(32);
    gnb_id.setGNBValue(std::to_string(uc->gnb_id));
    oai::_3gpp::model::GlobalRanNodeId global_ran_node_id = {};
    global_ran_node_id.setGNbId(gnb_id);
    global_ran_node_id.setPlmnId(plmn_id);
    oai::_3gpp::model::Ncgi ncgi = {};
    // ncgi.setNid(""); //TODO:
    std::string nr_cell_id_str = {};
    amf_conv::int_to_string_hex(uc->cgi.nrCellId, nr_cell_id_str, 9);
    ncgi.setNrCellId(nr_cell_id_str);
    ncgi.setPlmnId(plmn_id);
    // nr_location.setTai(tai);
    nr_location.setGlobalGnbId(global_ran_node_id);
    nr_location.setNcgi(ncgi);
    user_location.setNrLocation(nr_location);

    provide_loc_info.setLocation(user_location);

    // RAT Type
    oai::_3gpp::model::RatType rat_type = {};
    rat_type.setEnumValue(oai::_3gpp::model::RatType_anyOf::eRatType_anyOf::NR);
    provide_loc_info.setRatType(rat_type);

    // Old GUAMI

    nlohmann::json provide_loc_info_json = {};
    to_json(provide_loc_info_json, provide_loc_info);
    response_data[kSbiResponseJsonData] = provide_loc_info_json;
  } else {
    // TODO: problem details
    response_code = oai::common::sbi::http_status_code::BAD_REQUEST;
  }

  response_data[kSbiResponseHttpResponseCode] = response_code;

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }

  return;
}

//------------------------------------------------------------------------------
void amf_app::register_3gpp_access(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("AMF registers for 3GPP access with UDM");

  oai::_3gpp::model::Amf3GppAccessRegistration registration_data = {};
  // AMF Instance ID
  registration_data.setAmfInstanceId(amf_app_inst->get_nf_instance());
  // Callback URI
  std::string amf_callback_deregistration_notification_uri =
      oai::amf::api::amf_sbi_helper::
          get_amf_callback_deregistration_notification_uri(uc->supi);
  registration_data.setDeregCallbackUri(
      amf_callback_deregistration_notification_uri);
  // Initial Registration
  registration_data.setInitialRegistrationInd(true);
  // TODO: Pei
  // Guami
  oai::_3gpp::model::Guami guami       = {};
  oai::_3gpp::model::PlmnIdNid plmn_id = {};
  for (auto g : amf_cfg->guami_list) {
    if (boost::iequals(uc->tai.mcc, g.mcc) and
        boost::iequals(uc->tai.mnc, g.mnc)) {
      std::string amf_id = {};
      amf_conv::get_amf_id(g.region_id, g.amf_set_id, g.amf_pointer, amf_id);
      guami.setAmfId(amf_id);
      plmn_id.setMcc(g.mcc);
      plmn_id.setMnc(g.mnc);
      guami.setPlmnId(plmn_id);
      break;
    }
  }
  registration_data.setGuami(guami);
  // Rat type
  oai::_3gpp::model::RatType rat_type = {};
  rat_type.setEnumValue(oai::_3gpp::model::RatType_anyOf::eRatType_anyOf::NR);
  registration_data.setRatType(rat_type);

  // Send request to SBI to trigger registering to UDM and wait for the
  // response
  nlohmann::json registration_data_json = {};
  to_json(registration_data_json, registration_data);

  std::shared_ptr<itti_sbi_register_with_udm> itti_msg =
      std::make_shared<itti_sbi_register_with_udm>(TASK_AMF_APP, TASK_AMF_SBI);

  itti_msg->registration_data = registration_data_json;
  itti_msg->supi              = uc->supi;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::get_access_and_mobility_subscription_data(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug(
      "Retrieving a UE's Access and Mobility Subscription Data from UDM");

  oai::_3gpp::model::PlmnIdNid plmn_id = {};
  plmn_id.setMcc(uc->tai.mcc);
  plmn_id.setMnc(uc->tai.mnc);

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = {};
  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  amf_app_inst->store_promise(promise_id, p);
  boost::shared_future<nlohmann::json> f = p->get_future();
  Logger::amf_app().debug("Promise ID generated %ld", promise_id);

  std::shared_ptr<itti_sbi_retrieve_am_data> itti_msg =
      std::make_shared<itti_sbi_retrieve_am_data>(
          TASK_AMF_APP, TASK_AMF_SBI, promise_id);

  itti_msg->promise_id = promise_id;
  itti_msg->supi       = uc->supi;
  itti_msg->plmn_id    = plmn_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }

  bool is_result_available = true;

  oai::_3gpp::model::AccessAndMobilitySubscriptionData am_data = {};

  // Wait for the response available and process accordingly
  std::optional<nlohmann::json> result_opt = std::nullopt;
  oai::utils::utils::wait_for_result(f, result_opt);
  if (result_opt.has_value()) {
    nlohmann::json result = result_opt.value();
    Logger::amf_app().debug("Got result for promise ID %ld", promise_id);
    if (result.find(kSbiResponseJsonData) != result.end()) {
      Logger::amf_app().debug(
          "Got Access and Mobility Subscription Data Retrievel response from "
          "UDM: %s",
          result[kSbiResponseJsonData].dump());

      uint32_t http_response_code =
          oai::common::sbi::http_status_code::NO_RESPONSE;
      if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
        http_response_code = result[kSbiResponseHttpResponseCode].get<int>();
        if (http_response_code == oai::common::sbi::http_status_code::OK) {
          // Process the content
          try {
            from_json(result[kSbiResponseJsonData], am_data);
            is_result_available = true;
            // TODO: store AM Data
          } catch (std::exception& e) {
            Logger::amf_app().warn(
                "Could not parse Access and Mobility Subscription Data from "
                "Json");
            is_result_available = false;
          }
          // TODO: process locations
        }
      }

    } else {
      is_result_available = false;
    }

  } else {
    is_result_available = false;
  }

  // Remove the promise from the list since the result is processed or not
  // available
  amf_app_inst->remove_promise(promise_id);

  if (!is_result_available) {
    Logger::amf_app().warn(
        "Could not get Access and Mobility Subscription Data from UDM");
  }
}

//------------------------------------------------------------------------------
void amf_app::get_smf_selection_subscription_data(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug(
      "Retrieving SMF Selection Subscription Data from UDM");

  oai::_3gpp::model::PlmnIdNid plmn_id = {};
  plmn_id.setMcc(uc->tai.mcc);
  plmn_id.setMnc(uc->tai.mnc);

  std::shared_ptr<itti_sbi_retrieve_smf_selection_subscription_data> itti_msg =
      std::make_shared<itti_sbi_retrieve_smf_selection_subscription_data>(
          TASK_AMF_APP, TASK_AMF_SBI);

  itti_msg->supi    = uc->supi;
  itti_msg->plmn_id = plmn_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::discover_pcf(std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Discovering PCF for the UE");
  // TODO: enable PCF discovery feature flag:
  // amf_cfg->support_features.enable_pcf_discovery
  if (false) {
    oai::_3gpp::model::PlmnIdNid plmn_id = {};
    plmn_id.setMcc(uc->tai.mcc);
    plmn_id.setMnc(uc->tai.mnc);

    // Generate a promise and associate this promise to the ITTI message
    uint32_t promise_id = {};
    boost::shared_ptr<boost::promise<nlohmann::json>> p =
        boost::make_shared<boost::promise<nlohmann::json>>();
    amf_app_inst->store_promise(promise_id, p);
    boost::shared_future<nlohmann::json> f = p->get_future();
    Logger::amf_app().debug("Promise ID generated %ld", promise_id);

    std::shared_ptr<itti_sbi_pcf_discovery> itti_msg =
        std::make_shared<itti_sbi_pcf_discovery>(
            TASK_AMF_APP, TASK_AMF_SBI, promise_id);

    itti_msg->promise_id = promise_id;
    itti_msg->supi       = uc->supi;
    // itti_msg->snssai     = uc->snssai;
    itti_msg->plmn_id = plmn_id;
    // TODO: add support for PCF Set ID
    // TODO: add support for PCF Group ID
    // itti_msg->dnn = uc->dnn;
    // TODO: add support for PCF Selection Assistance Info and PCF ID(s) serving
    // the established PDU Sessions

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }

    bool is_result_available = false;

    oai::_3gpp::model::SearchResult search_result = {};

    std::string pcf_addr = {};

    // Wait for the response available and process accordingly
    std::optional<nlohmann::json> result_opt = std::nullopt;
    oai::utils::utils::wait_for_result(f, result_opt);
    if (result_opt.has_value()) {
      nlohmann::json result = result_opt.value();
      Logger::amf_app().debug("Got result for promise ID %ld", promise_id);
      if (result.find(kSbiResponseJsonData) != result.end()) {
        Logger::amf_app().debug(
            "Got Search Result response from "
            "NRF: %s",
            result[kSbiResponseJsonData].dump());

        uint32_t http_response_code =
            oai::common::sbi::http_status_code::NO_RESPONSE;
        if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
          http_response_code = result[kSbiResponseHttpResponseCode].get<int>();
          if (http_response_code == oai::common::sbi::http_status_code::OK) {
            // Process the content
            try {
              from_json(result[kSbiResponseJsonData], search_result);
              std::vector<oai::_3gpp::model::NFProfile> nf_instances =
                  search_result.getNfInstances();
              for (auto const& it : nf_instances) {
                // PCF info
                if (it.pcfInfoIsSet()) {
                  oai::_3gpp::model::PcfInfo pcf_info = it.getPcfInfo();
                }

                // TODO: PCF selection

                // IPv4 addresses (get first IP v4 address)
                if (it.ipv4AddressesIsSet()) {
                  if (it.getIpv4Addresses().size() == 0) {
                    Logger::amf_app().warn(
                        "No IPv4 Addresses found in Search Result");
                  } else {
                    pcf_addr = it.getIpv4Addresses().at(0);
                    break;
                  }
                }

                Logger::amf_app().debug("PCF Address: %s", pcf_addr.c_str());
                // TODO: Port
              }

              if (pcf_addr.empty()) {
                Logger::amf_app().warn("No PCF Address found in Search Result");
                is_result_available = false;
              } else {
                // Store PCF URI in UE context
                std::string pcf_uri_root = {};
                uint32_t pcf_port        = DEFAULT_HTTP2_PORT;
                pcf_uri_root.append(pcf_addr).append(":").append(
                    std::to_string(pcf_port));
                uc->pcf_addr.uri_root    = pcf_uri_root;
                uc->pcf_addr.api_version = "v1";  // TODO: get from PCF
                is_result_available      = true;
              }

            } catch (std::exception& e) {
              Logger::amf_app().warn(
                  "Could not parse Search Result from "
                  "Json");
              is_result_available = false;
            }
          }
        }

      } else {
        is_result_available = false;
      }

    } else {
      is_result_available = false;
    }

    // Remove the promise from the list since the result is processed or not
    // available
    amf_app_inst->remove_promise(promise_id);

    if (!is_result_available) {
      Logger::amf_app().warn("Could not get Search Result from NRF");
    }
  } else {
    // Store PCF Addr in UE context
    uc->pcf_addr = amf_cfg->pcf_addr;
  }
}

//------------------------------------------------------------------------------
void amf_app::perform_am_policy_association(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Perform AM Policy Association with PCF for the UE");

  oai::_3gpp::model::PolicyAssociationRequest policy_assc_request = {};
  policy_assc_request.setSupi(uc->supi);
  // Set Notification URI
  std::string notification_uri =
      amf_sbi_helper::get_pcf_policy_update_notification_uri(uc->supi);
  policy_assc_request.setNotificationUri(notification_uri);
  // TODO: Add support for Support Features
  std::string support_features = {};
  policy_assc_request.setSuppFeat(support_features);

  // Send request to SBI to trigger AM Policy Associtiation with PCF

  std::shared_ptr<itti_sbi_am_policy_association> itti_msg =
      std::make_shared<itti_sbi_am_policy_association>(
          TASK_AMF_APP, TASK_AMF_SBI);

  itti_msg->policy_assoc_req = policy_assc_request;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::perform_am_policy_association_termination(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug(
      "Perform AM Policy Association Termination with PCF for the UE");

  // Send request to SBI to trigger AM Policy Associtiation Termination with PCF
  std::shared_ptr<itti_sbi_am_policy_association_termination> itti_msg =
      std::make_shared<itti_sbi_am_policy_association_termination>(
          TASK_AMF_APP, TASK_AMF_SBI);

  itti_msg->supi = uc->supi;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::perform_am_policy_association_update(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug(
      "Perform AM Policy Association Update with PCF for the UE");

  oai::_3gpp::model::PolicyAssociationUpdateRequest policy_assc_update_request =
      {};
  // Set the info that AMF wants to update e.g., Notification URI, AMBR, Slice
  // MBRS, SMF Selection Data, etc.

  // Send request to SBI to trigger AM Policy Associtiation Update with PCF
  std::shared_ptr<itti_sbi_am_policy_association_update> itti_msg =
      std::make_shared<itti_sbi_am_policy_association_update>(
          TASK_AMF_APP, TASK_AMF_SBI);

  itti_msg->supi                    = uc->supi;
  itti_msg->policy_assoc_update_req = policy_assc_update_request;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::get_am_policy_association(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Retrieve AM Policy Association for the UE");

  // Send request to SBI to trigger AM Policy Associtiation Retrieval with PCF
  std::shared_ptr<itti_sbi_am_policy_association_retrieval> itti_msg =
      std::make_shared<itti_sbi_am_policy_association_retrieval>(
          TASK_AMF_APP, TASK_AMF_SBI);

  itti_msg->supi = uc->supi;

  int ret = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::get_ue_context_in_smf_data(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Retrieve UE Context In SMF Data");

  // Send request to SBI to trigger UE Context In SMF Data Retrieval with UDM
  std::shared_ptr<itti_sbi_ue_context_in_smf_data_retrieval> itti_msg =
      std::make_shared<itti_sbi_ue_context_in_smf_data_retrieval>(
          TASK_AMF_APP, TASK_AMF_SBI);
  itti_msg->supi = uc->supi;
  int ret        = itti_inst->send_msg(itti_msg);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//---------------------------------------------------------------------------------------------
bool amf_app::read_amf_configuration(nlohmann::json& json_data) const {
  amf_cfg->to_json(json_data);
  return true;
}

//---------------------------------------------------------------------------------------------
bool amf_app::update_amf_configuration(nlohmann::json& json_data) {
  if (stacs.get_number_connected_gnbs() > 0) {
    Logger::amf_app().info(
        "Could not update AMF configuration (connected with gNBs)");
    return false;
  }
  return amf_cfg->from_json(json_data);
}

//---------------------------------------------------------------------------------------------
void amf_app::add_n1n2_message_subscription(
    const std::string& ue_ctx_id, const n1n2sub_id_t& sub_id,
    std::shared_ptr<oai::_3gpp::model::UeN1N2InfoSubscriptionCreateData>&
        subscription_data) {
  Logger::amf_app().debug("Add an N1N2 Message Subscribe (Sub ID %d)", sub_id);
  std::unique_lock lock(m_n1n2_message_subscribe);
  n1n2_message_subscribe.emplace(
      std::make_pair(ue_ctx_id, sub_id), subscription_data);
}

//---------------------------------------------------------------------------------------------
bool amf_app::remove_n1n2_message_subscription(
    const std::string& ue_ctx_id, const std::string& sub_id) {
  Logger::amf_app().debug(
      "Remove an N1N2 Message Subscribe (UE Context ID %s, Sub ID %s)",
      ue_ctx_id.c_str(), sub_id.c_str());

  // Verify Subscription ID
  n1n2sub_id_t n1n2sub_id = {};
  try {
    n1n2sub_id = std::stoi(sub_id);
  } catch (const std::exception& err) {
    Logger::amf_app().warn(
        "Received a Unsubscribe Request, couldn't find the corresponding "
        "subscription");
    return false;
  }

  // Remove from the list
  std::unique_lock lock(m_n1n2_message_subscribe);
  if (n1n2_message_subscribe.erase(std::make_pair(ue_ctx_id, n1n2sub_id)) == 1)
    return true;
  else
    return false;
  return true;
}

//---------------------------------------------------------------------------------------------
void amf_app::find_n1n2_info_subscriptions(
    const std::string& ue_ctx_id,
    std::optional<
        oai::_3gpp::model::N1MessageClass_anyOf::eN1MessageClass_anyOf>&
        n1_message_class,
    std::optional<
        oai::_3gpp::model::N2InformationClass_anyOf::eN2InformationClass_anyOf>&
        n2_info_class,
    std::map<
        n1n2sub_id_t,
        std::shared_ptr<oai::_3gpp::model::UeN1N2InfoSubscriptionCreateData>>&
        subscriptions) {
  Logger::amf_app().debug("Find an N1N2 Info Subscription");

  std::shared_lock lock(m_n1n2_message_subscribe);
  for (auto subscription : n1n2_message_subscribe) {
    if ((subscription.first.first == ue_ctx_id)) {
      if (n1_message_class.has_value()) {
        if (subscription.second.get()->getN1MessageClass().getEnumValue() ==
            n1_message_class.value()) {
          subscriptions.insert(
              std::pair<
                  n1n2sub_id_t,
                  std::shared_ptr<
                      oai::_3gpp::model::UeN1N2InfoSubscriptionCreateData>>(
                  subscription.first.second, subscription.second));
        }
      }
      if (n2_info_class.has_value()) {
        if (subscription.second.get()->getN2InformationClass().getEnumValue() ==
            n2_info_class.value()) {
          subscriptions.insert(
              std::pair<
                  n1n2sub_id_t,
                  std::shared_ptr<
                      oai::_3gpp::model::UeN1N2InfoSubscriptionCreateData>>(
                  subscription.first.second, subscription.second));
        }
      }
    }
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::add_non_ue_n2_info_subscription(
    const n1n2sub_id_t& sub_id,
    std::shared_ptr<oai::_3gpp::model::NonUeN2InfoSubscriptionCreateData>&
        subscription_data) {
  Logger::amf_app().debug(
      "Add an Non UE N2 Info Subscribe (Sub ID %d)", sub_id);
  std::unique_lock lock(m_non_ue_n2_info_subscribe);
  non_ue_n2_info_subscribe.emplace(std::make_pair(sub_id, subscription_data));
}

//---------------------------------------------------------------------------------------------
bool amf_app::remove_non_ue_n2_info_subscription(const std::string& sub_id) {
  Logger::amf_app().debug(
      "Remove an Non UE N2 Info Unsubscribe (Sub ID %s)", sub_id.c_str());

  // Verify Subscription ID
  n1n2sub_id_t n2sub_id = {};
  try {
    n2sub_id = std::stoi(sub_id);
  } catch (const std::exception& err) {
    Logger::amf_app().warn(
        "Received a Unsubscribe Request, could not find the corresponding "
        "subscription");
    return false;
  }

  // Remove from the list
  std::unique_lock lock(m_non_ue_n2_info_subscribe);
  if (non_ue_n2_info_subscribe.erase(n2sub_id) == 1)
    return true;
  else
    return false;

  return false;
}

//---------------------------------------------------------------------------------------------
void amf_app::find_non_ue_n2_info_subscriptions(
    const std::string& nf_id,
    const oai::_3gpp::model::N2InformationClass_anyOf::
        eN2InformationClass_anyOf& n2_info_class,
    std::map<
        n1n2sub_id_t,
        std::shared_ptr<oai::_3gpp::model::NonUeN2InfoSubscriptionCreateData>>&
        subscriptions) {
  Logger::amf_app().debug("Find an Non UE N2 Info Subscription");

  std::shared_lock lock(m_non_ue_n2_info_subscribe);
  for (const auto& subscription : non_ue_n2_info_subscribe) {
    if ((subscription.second->getN2InformationClass().getEnumValue() ==
         n2_info_class) and
        (subscription.second->getNfId() == nf_id)) {
      subscriptions.insert(
          std::pair<
              n1n2sub_id_t,
              std::shared_ptr<
                  oai::_3gpp::model::NonUeN2InfoSubscriptionCreateData>>(
              subscription.first, subscription.second));
    }
  }
}

//------------------------------------------------------------------------------
uint32_t amf_app::generate_random_tmsi() {
  // Use the getrandom() system call
  // Note: for RHEL only supported by RHEL 8 Beta+
  uint32_t rand_number_generated;
  if (getrandom(&rand_number_generated, sizeof(uint32_t), GRND_NONBLOCK) ==
      -1) {
    Logger::amf_app().warn(
        "Error when generating a random number using getrandom()");
  } else {
    Logger::amf_app().debug(
        "Random number generated: %ld", rand_number_generated);
  }

  return rand_number_generated;
}

//------------------------------------------------------------------------------
bool amf_app::generate_5g_guti(
    const uint32_t ranid, const long amfid, std::string& mcc, std::string& mnc,
    uint32_t& tmsi) {
  std::string ue_context_key     = amf_conv::get_ue_context_key(ranid, amfid);
  std::shared_ptr<ue_context> uc = get_ue_context(ranid, amfid);

  if (uc == nullptr) return false;

  mcc      = uc->tai.mcc;
  mnc      = uc->tai.mnc;
  tmsi     = generate_random_tmsi();
  uc->tmsi = tmsi;
  return true;
}

//------------------------------------------------------------------------------
evsub_id_t amf_app::handle_event_exposure_subscription(
    std::shared_ptr<itti_sbi_event_exposure_request> msg) {
  Logger::amf_app().info(
      "Handle an Event Exposure Subscription Request from a NF");

  // Generate a subscription ID Id and store the corresponding information in a
  // map (subscription id, info)
  evsub_id_t evsub_id = generate_ev_subscription_id();

  std::vector<amf_event_t> event_subscriptions =
      msg->event_exposure.get_event_subs();

  // store subscription
  for (auto i : event_subscriptions) {
    auto ss    = std::make_shared<amf_subscription>();
    ss->sub_id = evsub_id;
    if (msg->event_exposure.is_supi_is_set()) {
      ss->supi        = msg->event_exposure.get_supi();
      ss->supi_is_set = true;
    }
    ss->notify_correlation_id = msg->event_exposure.get_notify_correlation_id();
    ss->notify_uri            = msg->event_exposure.get_notify_uri();
    ss->nf_id                 = msg->event_exposure.get_nf_id();
    ss->ev_type               = i.type;
    add_event_subscription(evsub_id, i.type, ss);

    if (i.type == LOCATION_REPORT) {
      // Determine Location
      if (amf_cfg->support_features.enable_lmf)
        handle_determine_location_request();
    }
    ss->display();
  }
  return evsub_id;
}

//---------------------------------------------------------------------------------------------
bool amf_app::handle_event_exposure_delete(const std::string& subscription_id) {
  // verify Subscription ID
  evsub_id_t sub_id = {};
  try {
    sub_id = std::stoi(subscription_id);
  } catch (const std::exception& err) {
    Logger::amf_app().warn(
        "Received a Unsubscribe Request, couldn't find the corresponding "
        "subscription");
    return false;
  }

  return remove_event_subscription(sub_id);
}

//------------------------------------------------------------------------------
bool amf_app::handle_nf_status_notification(
    std::shared_ptr<itti_sbi_notification_data>& msg,
    oai::_3gpp::model::ProblemDetails& problem_details, uint32_t& http_code) {
  Logger::amf_app().info("Handle a NF status notification from NRF");
  http_code =
      static_cast<uint32_t>(oai::common::sbi::http_status_code::NO_CONTENT);
  return true;
}

//------------------------------------------------------------------------------
void amf_app::handle_determine_location_request() {
  for (const auto& kvp : supi2ue_ctx) {
    nlohmann::json input_data = {};
    input_data["supi"]        = kvp.first;

    // Generate a promise and associate this promise to the ITTI message
    uint32_t promise_id = {};
    auto p              = boost::make_shared<boost::promise<nlohmann::json>>();
    boost::shared_future<nlohmann::json> f = p->get_future();
    store_promise(promise_id, p);
    Logger::amf_app().debug("Promise ID generated %d", promise_id);

    auto itti_msg = std::make_shared<itti_sbi_determine_location_request>(
        TASK_AMF_APP, TASK_AMF_SBI, promise_id);
    itti_msg->input_data = input_data;
    itti_msg->promise_id = promise_id;

    int ret = itti_inst->send_msg(itti_msg);
    if (0 != ret) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }

    // Wait for the response available and process accordingly
    std::optional<nlohmann::json> location_data = std::nullopt;
    oai::utils::utils::wait_for_result(f, location_data);
    if (location_data.has_value()) {
      nlohmann::json location_data_json = location_data.value();

      uint32_t http_response_code = 0;
      if (location_data_json.find(kSbiResponseHttpResponseCode) !=
          location_data_json.end()) {
        http_response_code =
            location_data_json[kSbiResponseHttpResponseCode].get<int>();
        // TODO:
      }
      if (location_data_json.find(kSbiResponseJsonData) !=
          location_data_json.end()) {
        ;
        Logger::amf_app().info(
            "Determine Location Response (SUPI: %s) : \n%s", kvp.first,
            location_data_json[kSbiResponseJsonData].dump(2).c_str());
      }
    } else {
      Logger::amf_app().error(
          "Determine Location failed (SUPI: %s)...\n", kvp.first);
    }

    // Remove the promise from the list since the result is processed or not
    // available
    amf_app_inst->remove_promise(promise_id);
  }
}
//------------------------------------------------------------------------------
void amf_app::generate_uuid() {
  std::shared_mutex m_generate_uuid;
  std::unique_lock lock(m_generate_uuid);
  amf_instance_id = to_string(boost::uuids::random_generator()());
}

//---------------------------------------------------------------------------------------------
void amf_app::add_event_subscription(
    const evsub_id_t& sub_id, const amf_event_type_t& ev,
    std::shared_ptr<amf_subscription> ss) {
  Logger::amf_app().debug(
      "Add an Event subscription (Sub ID %d, Event %d)", sub_id, (uint8_t) ev);
  std::unique_lock lock(m_amf_event_subscriptions);
  amf_event_subscriptions.emplace(std::make_pair(sub_id, ev), ss);
}

//---------------------------------------------------------------------------------------------
bool amf_app::remove_event_subscription(const evsub_id_t& sub_id) {
  Logger::amf_app().debug("Remove an Event subscription (Sub ID %d)", sub_id);
  std::unique_lock lock(m_amf_event_subscriptions);
  for (auto it = amf_event_subscriptions.cbegin();
       it != amf_event_subscriptions.cend();) {
    if ((uint8_t) std::get<0>(it->first) == (uint32_t) sub_id) {
      Logger::amf_app().debug(
          "Found an event subscription (Event ID %d)",
          (uint8_t) std::get<0>(it->first));
      amf_event_subscriptions.erase(it++);
      return true;
    } else {
      ++it;
    }
  }
  return false;
}

//---------------------------------------------------------------------------------------------
void amf_app::get_ee_subscriptions(
    const amf_event_type_t& ev,
    std::vector<std::shared_ptr<amf_subscription>>& subscriptions) {
  for (auto const& i : amf_event_subscriptions) {
    if ((uint8_t) std::get<1>(i.first) == (uint8_t) ev) {
      Logger::amf_app().debug(
          "Found an event subscription (Event ID %d, Event %d)",
          (uint8_t) std::get<0>(i.first), (uint8_t) ev);
      subscriptions.push_back(i.second);
    }
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::get_ee_subscriptions(
    const evsub_id_t& sub_id,
    std::vector<std::shared_ptr<amf_subscription>>& subscriptions) {
  for (auto const& i : amf_event_subscriptions) {
    if (i.first.first == sub_id) {
      subscriptions.push_back(i.second);
    }
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::get_ee_subscriptions(
    const amf_event_type_t& ev, std::string& supi,
    std::vector<std::shared_ptr<amf_subscription>>& subscriptions) {
  for (auto const& i : amf_event_subscriptions) {
    if ((i.first.second == ev) && (i.second->supi == supi)) {
      subscriptions.push_back(i.second);
    }
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::generate_amf_profile() {
  nf_instance_profile.set_nf_instance_id(amf_instance_id);
  nf_instance_profile.set_nf_instance_name(amf_cfg->amf_name);
  nf_instance_profile.set_nf_type("AMF");
  nf_instance_profile.set_nf_status("REGISTERED");
  nf_instance_profile.set_nf_heartBeat_timer(
      50);                                   // TODO: remove hardcoded value
  nf_instance_profile.set_nf_priority(1);    // TODO: remove hardcoded value
  nf_instance_profile.set_nf_capacity(100);  // TODO: remove hardcoded value
  nf_instance_profile.delete_nf_ipv4_addresses();
  nf_instance_profile.add_nf_ipv4_addresses(amf_cfg->sbi.addr4);

  // NF services
  // IP Endpoint (common for each service)
  ip_endpoint_t endpoint = {};
  endpoint.ipv4_address  = amf_cfg->sbi.addr4;
  endpoint.transport     = "TCP";
  endpoint.port          = amf_cfg->sbi.port;

  // namf_communication
  oai::common::sbi::nf_service_t nf_service = {};
  nf_service.service_instance_id            = amf_instance_id;
  nf_service.service_name                   = "namf-comm";
  nf_service_version_t version              = {};
  if (amf_cfg->sbi.api_version.has_value())
    version.api_version_in_uri =
        amf_cfg->sbi.api_version.value_or(DEFAULT_SBI_API_VERSION);
  version.api_full_version = "1.0.0";  // TODO: to be updated
  nf_service.versions.push_back(version);
  nf_service.scheme            = "http";
  nf_service.nf_service_status = "REGISTERED";

  nf_service.ip_endpoints.push_back(endpoint);

  nf_instance_profile.add_nf_service_list(nf_service);

  // namf-evts
  oai::common::sbi::nf_service_t nf_service_events = {};
  nf_service_events.service_instance_id            = "namf-evts";
  nf_service_events.service_name                   = "namf-evts";
  nf_service_version_t version_events              = {};
  if (amf_cfg->sbi.api_version.has_value())
    version_events.api_version_in_uri =
        amf_cfg->sbi.api_version.value_or(DEFAULT_SBI_API_VERSION);
  version_events.api_full_version = "1.0.0";  // TODO: to be updated
  nf_service_events.versions.push_back(version_events);
  nf_service_events.scheme            = "http";
  nf_service_events.nf_service_status = "REGISTERED";
  nf_service_events.ip_endpoints.push_back(endpoint);

  nf_instance_profile.delete_nf_services();
  nf_instance_profile.add_nf_service(nf_service);
  nf_instance_profile.add_nf_service(nf_service_events);

  // TODO: custom info
  // AMF info
  oai::common::sbi::amf_info_t info = {};
  oai::utils::conv::int_to_string_hex(
      amf_cfg->guami.region_id, info.amf_region_id, AMF_REGION_ID_LENGTH);
  oai::utils::conv::int_to_string_hex(
      amf_cfg->guami.amf_set_id, info.amf_set_id, AMF_SET_ID_LENGTH);
  for (auto g : amf_cfg->guami_list) {
    guami_t guami = {};
    amf_conv::get_amf_id(
        g.region_id, g.amf_set_id, g.amf_pointer, guami.amf_id);
    guami.plmn.mcc = g.mcc;
    guami.plmn.mnc = g.mnc;
    info.guami_list.push_back(guami);
  }

  nf_instance_profile.set_amf_info(info);

  std::vector<snssai_t> amf_snssai;
  for (auto p : amf_cfg->plmn_list) {
    for (auto s : p.slice_list) {
      amf_snssai.push_back(s);
    }
  }
  nf_instance_profile.set_nf_snssais(amf_snssai);

  // Display the profile
  nf_instance_profile.display();
}

//---------------------------------------------------------------------------------------------
std::string amf_app::get_nf_instance() const {
  return amf_instance_id;
}

//---------------------------------------------------------------------------------------------
void amf_app::register_to_nrf() {
  // Send request to SBI to send NF registration to NRF
  Logger::amf_app().debug(
      "Send ITTI msg to SBI task to trigger the registration request towards "
      "all appropriate NRFs");

  // Get all appropriate NRFs
  std::unordered_set<std::string> nrfs;
  get_nrfs(nrfs);

  for (const auto& nrf_instance : nrfs) {
    auto itti_msg = std::make_shared<itti_sbi_register_nf_instance_request>(
        TASK_AMF_APP, TASK_AMF_SBI);
    itti_msg->profile = nf_instance_profile;
    itti_msg->nrf_uri = nrf_instance;

    int ret = itti_inst->send_msg(itti_msg);
    if (RETURNok != ret) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::register_to_nrf(const std::string& nrf_uri) {
  // Send request to SBI to send NF registration to NRF
  Logger::amf_app().debug(
      "Send ITTI msg to SBI task to trigger the registration request towards "
      "NRF");

  auto itti_msg = std::make_shared<itti_sbi_register_nf_instance_request>(
      TASK_AMF_APP, TASK_AMF_SBI);
  itti_msg->profile = nf_instance_profile;
  itti_msg->nrf_uri = nrf_uri;

  int ret = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::get_nrfs(std::unordered_set<std::string>& nrfs) {
  if (amf_cfg->support_features.enable_nssf) {
    // Get all related NRFs from NSSF
    std::map<uint32_t, boost::shared_future<nlohmann::json>> nssf_responses;

    // Send requests to get appropriate Network Slice Informations from NSSF
    for (const auto& plmn : amf_cfg->plmn_list) {
      for (auto s : plmn.slice_list) {
        std::shared_ptr<itti_sbi_network_slice_selection_discovery> itti_msg =
            std::make_shared<itti_sbi_network_slice_selection_discovery>(
                TASK_AMF_APP, TASK_AMF_SBI);

        // Generate a promise and associate this promise to the ITTI message
        uint32_t promise_id = {};
        auto p = boost::make_shared<boost::promise<nlohmann::json>>();
        boost::shared_future<nlohmann::json> f = p->get_future();
        store_promise(promise_id, p);
        Logger::amf_app().debug("Promise ID generated %d", promise_id);

        // Store the future to be processed later
        nssf_responses.emplace(promise_id, f);

        itti_msg->nf_instance_id = amf_instance_id;
        itti_msg->plmn.mcc       = plmn.mcc;
        itti_msg->plmn.mnc       = plmn.mnc;
        itti_msg->snssai.sst     = s.sst;
        itti_msg->snssai.sd      = s.sd;
        itti_msg->promise_id     = promise_id;
        int ret                  = itti_inst->send_msg(itti_msg);
        if (0 != ret) {
          Logger::amf_app().error(
              "Could not send ITTI message %s to task TASK_AMF_SBI",
              itti_msg->get_msg_name());
        }
      }
    }

    // Process the response
    while (!nssf_responses.empty()) {
      // Wait for the result available and process accordingly
      std::optional<nlohmann::json> result_opt = std::nullopt;
      oai::utils::utils::wait_for_result(
          nssf_responses.begin()->second, result_opt);

      if (result_opt.has_value()) {
        nlohmann::json result = result_opt.value();
        Logger::amf_app().debug(
            "Got result for promise ID %ld, JSON content %s",
            nssf_responses.begin()->first, result.dump());

        uint32_t http_response_code = 0;
        if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
          http_response_code = result[kSbiResponseHttpResponseCode].get<int>();
          // if (http_response_code != oai::common::sbi::http_status_code::OK)
          // continue;
        }

        if (result.find(kSbiResponseJsonData) != result.end()) {
          nlohmann::json authorized_network_slice_info =
              result[kSbiResponseJsonData];
          NsiInformation nsi_information = {};
          if (authorized_network_slice_info.find("nsiInformation") !=
              authorized_network_slice_info.end()) {
            try {
              from_json(
                  authorized_network_slice_info["nsiInformation"],
                  nsi_information);
              nrfs.insert(nsi_information.getNrfNfMgtUri());
            } catch (std::exception& e) {
              Logger::amf_app().warn(
                  "Could not parse NSI Information from Json");
            }
          }
        }
      }
      // Remove the promise from the list
      nssf_responses.erase(nssf_responses.begin());
    }

  } else {
    // Get all NRFs from configuration files
    // For now we only have 1 NRF from conf file
    std::string nrf_uri = {};
    amf_sbi_helper::get_nrf_nf_instance_uri(
        amf_cfg->nrf_addr, amf_instance_id, nrf_uri);
    nrfs.insert(nrf_uri);
  }
  return;
}

//------------------------------------------------------------------------------
void amf_app::deregister_to_nrf() {
  Logger::amf_app().debug(
      "Send ITTI msg to SBI task to trigger the deregistration request to NRF");

  for (const auto& nrf_instance_uri : registered_nrfs) {
    auto itti_msg = std::make_shared<itti_sbi_deregister_nf_instance_request>(
        TASK_AMF_APP, TASK_AMF_SBI);
    itti_msg->amf_instance_id = amf_instance_id;
    itti_msg->nrf_uri         = nrf_instance_uri;
    int ret                   = itti_inst->send_msg(itti_msg);
    if (RETURNok != ret) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_msg->get_msg_name());
    }
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::timer_nrf_heartbeat_timeout(
    timer_id_t timer_id, std::string nrf_uri) {
  // Send ITTI msg to SBI task to trigger NRF Heartbeat

  auto itti_msg = std::make_shared<itti_sbi_update_nf_instance_request>(
      TASK_AMF_APP, TASK_AMF_SBI);

  oai::_3gpp::model::PatchItem patch_item = {};
  oai::_3gpp::model::PatchOperation op;
  op.setEnumValue(
      oai::_3gpp::model::PatchOperation_anyOf::ePatchOperation_anyOf::REPLACE);
  patch_item.setOp(op);
  patch_item.setPath("/nfStatus");
  patch_item.setValue("REGISTERED");
  itti_msg->patch_items.push_back(patch_item);
  itti_msg->amf_instance_id = amf_instance_id;
  itti_msg->nrf_uri         = nrf_uri;

  int ret = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}

//---------------------------------------------------------------------------------------------
void amf_app::timer_nrf_registration_timeout(
    timer_id_t timer_id, std::string nrf_uri) {
  register_to_nrf(nrf_uri);
}

//---------------------------------------------------------------------------------------------
void amf_app::remove_promise(const uint32_t pid) {
  std::unique_lock lock(m_sbi_response_handlers);
  sbi_response_handlers.erase(pid);
}

//---------------------------------------------------------------------------------------------
void amf_app::store_promise(
    uint32_t& pid, const boost::shared_ptr<boost::promise<nlohmann::json>>& p) {
  // Generate promise ID
  pid = generate_promise_id();
  std::unique_lock lock(m_sbi_response_handlers);
  sbi_response_handlers.emplace(pid, p);
}

//------------------------------------------------------------------------------
void amf_app::trigger_process_response(
    const uint32_t pid, const nlohmann::json& json_data) {
  Logger::amf_app().debug(
      "Trigger process response: Set promise with ID %u "
      "to ready",
      pid);
  std::unique_lock lock(m_sbi_response_handlers);
  if (sbi_response_handlers.count(pid) > 0) {
    sbi_response_handlers[pid]->set_value(json_data);
    // Remove this promise from list
    sbi_response_handlers.erase(pid);
  }
}

//------------------------------------------------------------------------------
void amf_app::trigger_pdu_session_release(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Trigger PDU Session Release towards SMF");
  std::vector<std::shared_ptr<pdu_session_context>> sessions_ctx;

  if (uc->get_pdu_sessions_context(sessions_ctx)) {
    // Send request to SMF to release all existing PDU sessions
    std::map<uint32_t, boost::shared_future<nlohmann::json>> smf_responses;
    for (auto session : sessions_ctx) {
      auto itti_msg = std::make_shared<itti_nsmf_pdusession_release_sm_context>(
          TASK_AMF_N2, TASK_AMF_SBI);

      // Generate a promise and associate this promise to the ITTI message
      uint32_t promise_id = {};
      auto p = boost::make_shared<boost::promise<nlohmann::json>>();
      boost::shared_future<nlohmann::json> f = p->get_future();
      // Store the future to be processed later
      amf_app_inst->store_promise(promise_id, p);
      smf_responses.emplace(promise_id, f);
      Logger::amf_app().debug("Promise ID generated %d", promise_id);

      itti_msg->supi             = uc->supi;
      itti_msg->pdu_session_id   = session->pdu_session_id;
      itti_msg->promise_id       = promise_id;
      itti_msg->context_location = session->smf_info.context_location;

      int ret = itti_inst->send_msg(itti_msg);
      if (0 != ret) {
        Logger::amf_app().error(
            "Could not send ITTI message %s to task TASK_AMF_SBI",
            itti_msg->get_msg_name());
      }
    }

    while (!smf_responses.empty()) {
      // Wait for the result available and process accordingly
      std::optional<nlohmann::json> result_opt = std::nullopt;
      oai::utils::utils::wait_for_result(
          smf_responses.begin()->second, result_opt);

      if (result_opt.has_value()) {
        nlohmann::json result = result_opt.value();
        Logger::amf_app().debug(
            "Got result for promise ID %d, json content %s",
            smf_responses.begin()->first, result.dump());

        uint32_t http_response_code = 0;
        if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
          http_response_code = result[kSbiResponseHttpResponseCode].get<int>();
          // Remove PDU session
          // TODO for multiple sessions
          if ((http_response_code == oai::common::sbi::http_status_code::OK) or
              (http_response_code ==
               oai::common::sbi::http_status_code::NO_CONTENT)) {
            for (auto session : sessions_ctx) {
              uc->remove_pdu_sessions_context(session->pdu_session_id);
            }
          }
        } else {
          // TODO:
        }
      }
      // Remove the promise from the list since the result is processed or not
      // available
      smf_responses.erase(smf_responses.begin());
    }

  } else {
    Logger::amf_app().debug("No PDU session available");
  }
}

//------------------------------------------------------------------------------
void amf_app::trigger_pdu_session_up_deactivation(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Trigger PDU Session UP Deactivation towards SMF");

  std::vector<std::shared_ptr<pdu_session_context>> sessions_ctx;
  if (uc->get_pdu_sessions_context(sessions_ctx)) {
    // Send PDUSessionUpdateSMContextRequest to SMF for each PDU session
    std::map<uint32_t, boost::shared_future<nlohmann::json>> smf_responses;
    for (auto session : sessions_ctx) {
      Logger::amf_app().debug("PDU Session ID %d", session->pdu_session_id);
      // Generate a promise and associate this promise to the curl handle
      uint32_t promise_id = {};
      auto p = boost::make_shared<boost::promise<nlohmann::json>>();
      boost::shared_future<nlohmann::json> f = p->get_future();

      // Store the future to be processed later
      amf_app_inst->store_promise(promise_id, p);
      smf_responses.emplace(session->pdu_session_id, f);
      Logger::amf_app().debug("Promise ID generated %d", promise_id);

      Logger::amf_app().debug(
          "Sending ITTI to trigger PDUSessionUpdateSMContextRequest to SMF to "
          "task TASK_AMF_SBI");

      auto itti_n11_msg =
          std::make_shared<itti_nsmf_pdusession_update_sm_context>(
              TASK_NGAP, TASK_AMF_SBI);

      itti_n11_msg->pdu_session_id = session->pdu_session_id;
      itti_n11_msg->is_n2sm_set    = false;
      itti_n11_msg->amf_ue_ngap_id = uc->amf_ue_ngap_id;
      itti_n11_msg->ran_ue_ngap_id = uc->ran_ue_ngap_id;
      itti_n11_msg->supi           = uc->supi;
      itti_n11_msg->pdu_session_id = session->pdu_session_id;
      itti_n11_msg->promise_id     = promise_id;
      itti_n11_msg->up_cnx_state   = "DEACTIVATED";

      int ret = itti_inst->send_msg(itti_n11_msg);
      if (0 != ret) {
        Logger::amf_app().error(
            "Could not send ITTI message %s to task TASK_AMF_SBI",
            itti_n11_msg->get_msg_name());
      }
    }

    bool is_up_activated = true;
    while (!smf_responses.empty()) {
      // Wait for the result available and process accordingly
      std::optional<nlohmann::json> result_opt = std::nullopt;
      oai::utils::utils::wait_for_result(
          smf_responses.begin()->second, result_opt);

      if (result_opt.has_value()) {
        nlohmann::json result = result_opt.value();
        Logger::amf_app().debug(
            "Got result from a promise with PDU Session Id %d, json content %s",
            smf_responses.begin()->first, result.dump());

        uint32_t http_response_code = 0;
        if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
          is_up_activated    = is_up_activated && true;
          http_response_code = result[kSbiResponseHttpResponseCode].get<int>();

          if ((http_response_code == oai::common::sbi::http_status_code::OK) or
              (http_response_code ==
               oai::common::sbi::http_status_code::NO_CONTENT)) {
            uc->set_up_cnx_state(
                smf_responses.begin()->first,
                up_cnx_state_e::UPCNX_STATE_DEACTIVATED);
          }

        } else {
          is_up_activated = false;
          Logger::amf_app().warn("Could not get the HTTP response code");
        }
      } else {
        is_up_activated = false;
        Logger::amf_app().warn("Could not get the HTTP response code");
      }
      // Remove the promise from the list since the result is processed or not
      // available
      smf_responses.erase(smf_responses.begin());
    }
  } else {
    Logger::amf_app().debug("No PDU session available");
  }
}

//------------------------------------------------------------------------------
bool amf_app::trigger_pdu_session_up_activation(
    const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Trigger PDU Session UP Activation towards SMF");
  bool activation_result = false;

  std::vector<std::shared_ptr<pdu_session_context>> sessions_ctx;
  if (uc->get_pdu_sessions_context(sessions_ctx)) {
    // Send PDUSessionUpdateSMContextRequest to SMF for each PDU session
    std::map<uint32_t, boost::shared_future<nlohmann::json>> smf_responses;
    for (auto session : sessions_ctx) {
      Logger::amf_app().debug("PDU Session ID %d", session->pdu_session_id);
      // Generate a promise and associate this promise to the response handler
      uint32_t promise_id = {};
      auto p = boost::make_shared<boost::promise<nlohmann::json>>();
      boost::shared_future<nlohmann::json> f = p->get_future();

      // Store the future to be processed later
      amf_app_inst->store_promise(promise_id, p);
      smf_responses.emplace(session->pdu_session_id, f);
      Logger::amf_app().debug("Promise ID generated %d", promise_id);

      Logger::amf_app().debug(
          "Sending ITTI to trigger PDUSessionUpdateSMContextRequest to SMF to "
          "task TASK_AMF_SBI");

      auto itti_n11_msg =
          std::make_shared<itti_nsmf_pdusession_update_sm_context>(
              TASK_NGAP, TASK_AMF_SBI);

      itti_n11_msg->pdu_session_id = session->pdu_session_id;
      itti_n11_msg->is_n2sm_set    = false;
      itti_n11_msg->amf_ue_ngap_id = uc->amf_ue_ngap_id;
      itti_n11_msg->ran_ue_ngap_id = uc->ran_ue_ngap_id;
      itti_n11_msg->supi           = uc->supi;
      itti_n11_msg->pdu_session_id = session->pdu_session_id;
      itti_n11_msg->promise_id     = promise_id;
      itti_n11_msg->up_cnx_state   = "ACTIVATING";

      int ret = itti_inst->send_msg(itti_n11_msg);
      if (0 != ret) {
        Logger::amf_app().error(
            "Could not send ITTI message %s to task TASK_AMF_SBI",
            itti_n11_msg->get_msg_name());
        return false;
      }
    }

    while (!smf_responses.empty()) {
      // Wait for the result available and process accordingly
      std::optional<nlohmann::json> result_opt = std::nullopt;
      oai::utils::utils::wait_for_result(
          smf_responses.begin()->second, result_opt);

      if (result_opt.has_value()) {
        nlohmann::json result = result_opt.value();
        Logger::amf_app().debug(
            "Got result from a promise for PDU session Id %d, json content %s",
            smf_responses.begin()->first, result.dump());

        uint32_t http_response_code = 0;
        if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
          http_response_code = result[kSbiResponseHttpResponseCode].get<int>();
          if ((http_response_code == oai::common::sbi::http_status_code::OK) or
              (http_response_code ==
               oai::common::sbi::http_status_code::NO_CONTENT)) {
            uc->set_up_cnx_state(
                smf_responses.begin()->first,
                up_cnx_state_e::UPCNX_STATE_ACTIVATED);
            activation_result = activation_result && true;
          } else {
            Logger::amf_app().warn(
                "Failed to activate the UP for this PDU session (PDU Session "
                "Id %d)!",
                smf_responses.begin()->first);
            activation_result = false;
          }
        }
      } else {
        Logger::amf_app().warn("Could not get response from SMF");
        activation_result = false;
      }
      // Remove the promise from the list since the result is processed or not
      // available
      smf_responses.erase(smf_responses.begin());
    }
  } else {
    Logger::amf_app().debug("No PDU session available");
  }
  return activation_result;
}

//------------------------------------------------------------------------------
bool amf_app::trigger_pdu_session_up_activation(
    uint8_t pdu_session_id, const std::shared_ptr<ue_context>& uc) const {
  Logger::amf_app().debug("Trigger PDU Session UP Activation towards SMF");

  std::shared_ptr<pdu_session_context> psc = {};
  if (uc->get_pdu_session_context(pdu_session_id, psc)) {
    // Send PDUSessionUpdateSMContextRequest to SMF
    Logger::amf_app().debug("PDU Session ID %d", pdu_session_id);
    // Generate a promise and associate this promise to the curl handle
    uint32_t promise_id = {};
    auto p              = boost::make_shared<boost::promise<nlohmann::json>>();
    boost::shared_future<nlohmann::json> f = p->get_future();
    amf_app_inst->store_promise(promise_id, p);
    Logger::amf_app().debug("Promise ID generated %d", promise_id);

    Logger::amf_app().debug(
        "Sending ITTI to trigger PDUSessionUpdateSMContextRequest to SMF to "
        "task TASK_AMF_SBI");

    auto itti_n11_msg =
        std::make_shared<itti_nsmf_pdusession_update_sm_context>(
            TASK_NGAP, TASK_AMF_SBI);

    itti_n11_msg->pdu_session_id = pdu_session_id;
    itti_n11_msg->is_n2sm_set    = false;
    itti_n11_msg->amf_ue_ngap_id = uc->amf_ue_ngap_id;
    itti_n11_msg->ran_ue_ngap_id = uc->ran_ue_ngap_id;
    itti_n11_msg->supi           = uc->supi;
    itti_n11_msg->pdu_session_id = pdu_session_id;
    itti_n11_msg->promise_id     = promise_id;
    itti_n11_msg->up_cnx_state   = "ACTIVATING";

    int ret = itti_inst->send_msg(itti_n11_msg);
    if (0 != ret) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_SBI",
          itti_n11_msg->get_msg_name());
      return false;
    }

    // Wait for the result available and process accordingly
    std::optional<nlohmann::json> result_opt = std::nullopt;
    oai::utils::utils::wait_for_result(f, result_opt);

    if (result_opt.has_value()) {
      nlohmann::json result = result_opt.value();
      Logger::amf_app().debug(
          "Got result from a promise (promise Id %ld) for PDU session Id %d, "
          "JSON content %s",
          promise_id, pdu_session_id, result.dump());

      uint32_t http_response_code = 0;
      if (result.find(kSbiResponseHttpResponseCode) != result.end()) {
        http_response_code = result[kSbiResponseHttpResponseCode].get<int>();
        if ((http_response_code == oai::common::sbi::http_status_code::OK) or
            (http_response_code ==
             oai::common::sbi::http_status_code::NO_CONTENT)) {
          uc->set_up_cnx_state(
              pdu_session_id, up_cnx_state_e::UPCNX_STATE_ACTIVATED);
          return true;
        } else {
          Logger::amf_app().warn(
              "Failed to activate the UP for this PDU session!");
        }
      }
    } else {
      Logger::amf_app().warn("Could not get response from SMF");
    }

    // Remove the promise from the list since the result is processed or not
    // available
    amf_app_inst->remove_promise(promise_id);

  } else {
    Logger::amf_app().warn("Could not find PDU session info");
  }

  return false;
}

//------------------------------------------------------------------------------
void amf_app::add_amf_status_change_subscription(
    const std::string& subscription_id,
    const std::shared_ptr<oai::_3gpp::model::SubscriptionData>& sd) {
  Logger::amf_app().debug(
      "Add an AMF Status Change Subscription (Sub ID %s)", subscription_id);
  std::unique_lock lock(m_amf_status_change_subscriptions);
  amf_status_change_subscriptions.emplace(subscription_id, sd);
}

//------------------------------------------------------------------------------
bool amf_app::remove_amf_status_change_subscription(
    const std::string& subscription_id) {
  std::unique_lock lock(m_amf_status_change_subscriptions);
  if (amf_status_change_subscriptions.count(subscription_id) > 0) {
    amf_status_change_subscriptions.erase(subscription_id);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool amf_app::update_amf_status_change_subscription(
    const std::string& subscription_id,
    const std::shared_ptr<oai::_3gpp::model::SubscriptionData>& sd) {
  std::unique_lock lock(m_amf_status_change_subscriptions);
  // Update the subscription if it exists, otherwise add a new one
  amf_status_change_subscriptions[subscription_id] = sd;
  return true;
}

//------------------------------------------------------------------------------
std::vector<std::string> amf_app::get_amf_status_change_subscription_uris(
    const std::vector<oai::_3gpp::model::Guami>& guamis) const {
  std::vector<std::string> uris;
  // TODO: filter the subscriptions based on the guami list
  std::shared_lock lock(m_amf_status_change_subscriptions);
  for (auto const& it : amf_status_change_subscriptions) {
    if (it.second) {
      uris.push_back(it.second->getAmfStatusUri());
    }
  }
  return uris;
}

//------------------------------------------------------------------------------
void amf_app::perform_amf_status_change_notification(
    const oai::_3gpp::model::StatusChange& status_change) const {
  // Prepare the info
  oai::_3gpp::model::AmfStatusChangeNotification
      amf_status_change_notification               = {};
  oai::_3gpp::model::AmfStatusInfo amf_status_info = {};
  std::vector<oai::_3gpp::model::AmfStatusInfo> amf_status_info_list;

  // Guami List
  oai::_3gpp::model::Guami guami = {};
  std::vector<oai::_3gpp::model::Guami> guamis;
  oai::_3gpp::model::PlmnIdNid plmn_id = {};
  for (auto g : amf_cfg->guami_list) {
    std::string amf_id = {};
    amf_conv::get_amf_id(g.region_id, g.amf_set_id, g.amf_pointer, amf_id);
    guami.setAmfId(amf_id);
    plmn_id.setMcc(g.mcc);
    plmn_id.setMnc(g.mnc);
    guami.setPlmnId(plmn_id);
    guamis.push_back(guami);
  }
  amf_status_info.setGuamiList(guamis);

  // Status Change
  amf_status_info.setStatusChange(status_change);
  // Target AMF removal
  amf_status_info.setTargetAmfRemoval(amf_cfg->amf_name);
  // target AMF failure

  amf_status_info_list.push_back(amf_status_info);
  amf_status_change_notification.setAmfStatusInfoList(amf_status_info_list);

  std::vector<std::string> notification_uris =
      get_amf_status_change_subscription_uris(guamis);

  // Send request to SBI to send AMF status change notification to subscribed NF
  // instances
  auto itti_msg = std::make_shared<itti_sbi_amf_status_change_notification>(
      TASK_AMF_APP, TASK_AMF_SBI);
  itti_msg->amf_status_change_notification = amf_status_change_notification;
  itti_msg->notification_uris              = notification_uris;

  int ret = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_SBI",
        itti_msg->get_msg_name());
  }
}
