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

#include "amf_sbi.hpp"

#include <curl/curl.h>

#include <nlohmann/json.hpp>

#include "3gpp_29.500.h"
#include "3gpp_29.502.h"
#include "3gpp_24.501.hpp"
#include "AmfEventReport.h"
#include "amf.hpp"
#include "amf_app.hpp"
#include "amf_config.hpp"
#include "amf_n1.hpp"
#include "output_wrapper.hpp"
#include "conversions.hpp"
#include "fqdn.hpp"
#include "itti.hpp"
#include "itti_msg_amf_app.hpp"
#include "itti_msg_n2.hpp"
#include "mime_parser.hpp"
#include "nas_context.hpp"
#include "ue_context.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

using namespace oai::config;
using namespace amf_application;
extern itti_mw* itti_inst;
extern amf_config amf_cfg;
extern amf_sbi* amf_sbi_inst;
extern amf_n1* amf_n1_inst;
extern amf_app* amf_app_inst;

//------------------------------------------------------------------------------
std::size_t callback(
    const char* in, std::size_t size, std::size_t num, std::string* out) {
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

//------------------------------------------------------------------------------
void octet_stream_2_hex_stream(uint8_t* buf, int len, std::string& out) {
  out       = "";
  char* tmp = (char*) calloc(1, 2 * len * sizeof(uint8_t) + 1);
  for (int i = 0; i < len; i++) {
    sprintf(tmp + 2 * i, "%02x", buf[i]);
  }
  tmp[2 * len] = '\0';
  out          = tmp;
  Logger::amf_sbi().debug("Buffer: %s", out.c_str());
}

//------------------------------------------------------------------------------
void amf_sbi_task(void*) {
  const task_id_t task_id = TASK_AMF_SBI;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case NSMF_PDU_SESSION_CREATE_SM_CTX: {
        Logger::amf_sbi().info("Running ITTI_SMF_PDU_SESSION_CREATE_SM_CTX");
        itti_nsmf_pdusession_create_sm_context* m =
            dynamic_cast<itti_nsmf_pdusession_create_sm_context*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case NSMF_PDU_SESSION_UPDATE_SM_CTX: {
        Logger::amf_sbi().info(
            "Receive Nsmf_PDUSessionUpdateSMContext, handling ...");
        itti_nsmf_pdusession_update_sm_context* m =
            dynamic_cast<itti_nsmf_pdusession_update_sm_context*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case NSMF_PDU_SESSION_RELEASE_SM_CTX: {
        Logger::amf_sbi().info(
            "Receive Nsmf_PDUSessionReleaseSMContext, handling ...");
        itti_nsmf_pdusession_release_sm_context* m =
            dynamic_cast<itti_nsmf_pdusession_release_sm_context*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case PDU_SESSION_RESOURCE_SETUP_RESPONSE: {
        Logger::amf_sbi().info(
            "Receive PDU Session Resource Setup response, handling ...");
        itti_pdu_session_resource_setup_response* m =
            dynamic_cast<itti_pdu_session_resource_setup_response*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_REGISTER_NF_INSTANCE_REQUEST: {
        Logger::amf_sbi().info(
            "Receive Register NF Instance Request, handling ...");
        itti_sbi_register_nf_instance_request* m =
            dynamic_cast<itti_sbi_register_nf_instance_request*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_UPDATE_NF_INSTANCE_REQUEST: {
        Logger::amf_sbi().info(
            "Receive Update NF Instance Request, handling ...");
        itti_sbi_update_nf_instance_request* m =
            dynamic_cast<itti_sbi_update_nf_instance_request*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_DEREGISTER_NF_INSTANCE_REQUEST: {
        Logger::amf_sbi().info(
            "Receive Deregister NF Instance Request, handling ...");
        itti_sbi_deregister_nf_instance_request* m =
            dynamic_cast<itti_sbi_deregister_nf_instance_request*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;
      case SBI_NOTIFY_SUBSCRIBED_EVENT: {
        Logger::amf_sbi().info(
            "Receive Notify Subscribed Event Request, handling ...");
        itti_sbi_notify_subscribed_event* m =
            dynamic_cast<itti_sbi_notify_subscribed_event*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_SLICE_SELECTION_SUBSCRIPTION_DATA: {
        Logger::amf_sbi().info(
            "Receive Slice Selection Subscription Data Retrieval Request, "
            "handling ...");
        itti_sbi_slice_selection_subscription_data* m =
            dynamic_cast<itti_sbi_slice_selection_subscription_data*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_NETWORK_SLICE_SELECTION_INFORMATION: {
        Logger::amf_sbi().info(
            "Receive Network Slice Selection Information Request, "
            "handling ...");
        itti_sbi_network_slice_selection_information* m =
            dynamic_cast<itti_sbi_network_slice_selection_information*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_N1_MESSAGE_NOTIFY: {
        Logger::amf_sbi().info(
            "Receive N1 Message Notify message, "
            "handling ...");
        itti_sbi_n1_message_notify* m =
            dynamic_cast<itti_sbi_n1_message_notify*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_N2_INFO_NOTIFY: {
        Logger::amf_sbi().info(
            "Receive N2 Info Notify message, "
            "handling ...");
        itti_sbi_n2_info_notify* m =
            dynamic_cast<itti_sbi_n2_info_notify*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_NF_INSTANCE_DISCOVERY: {
        Logger::amf_sbi().info(
            "Receive N1 NF Instance Discovery message, "
            "handling ...");
        itti_sbi_nf_instance_discovery* m =
            dynamic_cast<itti_sbi_nf_instance_discovery*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case SBI_DETERMINE_LOCATION_REQUEST: {
        Logger::amf_sbi().info(
            "Receive Determine Location Request message, "
            "handling ...");
        itti_sbi_determine_location_request* m =
            dynamic_cast<itti_sbi_determine_location_request*>(msg);
        amf_sbi_inst->handle_itti_message(ref(*m));
      } break;

      case TERMINATE: {
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::amf_sbi().info("Received terminate message");
          return;
        }
      } break;
      default: {
        Logger::amf_sbi().info(
            "Receive unknown message type %d", msg->msg_type);
      }
    }
  } while (true);
}

//------------------------------------------------------------------------------
amf_sbi::amf_sbi() {
  if (itti_inst->create_task(TASK_AMF_SBI, amf_sbi_task, nullptr)) {
    Logger::amf_sbi().error("Cannot create task TASK_AMF_SBI");
    throw std::runtime_error("Cannot create task TASK_AMF_SBI");
  }
  Logger::amf_sbi().startup("amf_sbi started");
}

//------------------------------------------------------------------------------
amf_sbi::~amf_sbi() {}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_pdu_session_resource_setup_response& itti_msg) {
  // TODO:
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_nsmf_pdusession_update_sm_context& itti_msg) {
  string ue_context_key = conv::get_ue_context_key(
      itti_msg.ran_ue_ngap_id, itti_msg.amf_ue_ngap_id);
  std::shared_ptr<ue_context> uc = {};
  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) {
    Logger::amf_sbi().error(
        "No UE context for %s exit", ue_context_key.c_str());
    return;
  }

  std::string supi = uc->supi;

  Logger::amf_sbi().debug(
      "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
      "ID %d)",
      supi.c_str(), itti_msg.pdu_session_id);

  std::shared_ptr<pdu_session_context> psc = {};
  if (!uc->find_pdu_session_context(itti_msg.pdu_session_id, psc)) {
    Logger::amf_sbi().error(
        "Could not find pdu_session_context with SUPI %s, Failed",
        supi.c_str());
    return;
  }

  std::string remote_uri = {};
  if (!amf_cfg.get_smf_pdu_session_context_uri(psc, remote_uri)) {
    Logger::amf_sbi().error("Could not find Nsmf_PDUSession URI");
    return;
  }
  remote_uri += NSMF_PDU_SESSION_MODIFY;

  Logger::amf_sbi().debug("SMF URI: %s", remote_uri.c_str());

  std::string n1sm_msg                      = {};
  std::string n2sm_msg                      = {};
  nlohmann::json pdu_session_update_request = {};

  if (itti_msg.is_n1sm_set) {
    pdu_session_update_request[N1_SM_CONTENT_ID]["contentId"] =
        N1_SM_CONTENT_ID;
    octet_stream_2_hex_stream(
        (uint8_t*) bdata(itti_msg.n1sm), blength(itti_msg.n1sm), n1sm_msg);
  }

  if (itti_msg.is_n2sm_set) {
    pdu_session_update_request["n2SmInfoType"] = itti_msg.n2sm_info_type;
    pdu_session_update_request["n2SmInfo"]["contentId"] = N2_SM_CONTENT_ID;
    octet_stream_2_hex_stream(
        (uint8_t*) bdata(itti_msg.n2sm), blength(itti_msg.n2sm), n2sm_msg);
  }

  // For N2 HO
  if (itti_msg.ho_state.compare("PREPARING") == 0) {
    pdu_session_update_request["hoState"] = "PREPARING";
  } else if (itti_msg.ho_state.compare("PREPARED") == 0) {
    pdu_session_update_request["hoState"] = "PREPARED";
  } else if (itti_msg.ho_state.compare("COMPLETED") == 0) {
    pdu_session_update_request["hoState"] = "COMPLETED";
  }

  // For Deactivation of User Plane connectivity
  if (itti_msg.up_cnx_state.compare("DEACTIVATED") == 0) {
    pdu_session_update_request["upCnxState"] = "DEACTIVATED";
  }

  // For Service Request
  if (itti_msg.up_cnx_state.compare("ACTIVATING") == 0) {
    pdu_session_update_request["upCnxState"] = "ACTIVATING";
  }

  std::string json_part = pdu_session_update_request.dump();

  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  bool curl_result = curl_http_client(
      remote_uri, json_part, n1sm_msg, n2sm_msg, supi, itti_msg.pdu_session_id,
      http_version, itti_msg.promise_id);

  if (curl_result and
      (itti_msg.n2sm_info_type.compare("PDU_RES_SETUP_RSP") == 0)) {
    psc->up_cnx_state = up_cnx_state_e::UPCNX_STATE_ACTIVATED;
  }
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(itti_nsmf_pdusession_create_sm_context& smf) {
  Logger::amf_sbi().debug("Handle ITTI SMF_PDU_SESSION_CREATE_SM_CTX");

  std::shared_ptr<nas_context> nc = {};
  if (!amf_n1_inst->amf_ue_id_2_nas_context(smf.amf_ue_ngap_id, nc)) {
    Logger::amf_sbi().error(
        "No UE NAS context with amf_ue_ngap_id (" AMF_UE_NGAP_ID_FMT ")",
        smf.amf_ue_ngap_id);
    return;
  }

  string supi = conv::imsi_to_supi(nc->imsi);
  string ue_context_key =
      conv::get_ue_context_key(nc->ran_ue_ngap_id, nc->amf_ue_ngap_id);
  std::shared_ptr<ue_context> uc = {};
  Logger::amf_sbi().info(
      "Find ue_context in amf_app using UE Context Key: %s",
      ue_context_key.c_str());
  if (!amf_app_inst->ran_amf_id_2_ue_context(ue_context_key, uc)) {
    Logger::amf_sbi().error(
        "No UE context for %s exit", ue_context_key.c_str());
    return;
  }

  // Create PDU Session Context if not available
  std::shared_ptr<pdu_session_context> psc = {};
  if (!uc->find_pdu_session_context(smf.pdu_sess_id, psc)) {
    psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
    uc->add_pdu_session_context(smf.pdu_sess_id, psc);
    psc->up_cnx_state = up_cnx_state_e::UPCNX_STATE_DEACTIVATED;
    Logger::amf_sbi().debug("Create a PDU Session Context");
  }

  if (!psc) {
    Logger::amf_sbi().error("No PDU Session Context found");
    return;
  }

  // Store corresponding info in PDU Session Context
  psc->amf_ue_ngap_id = nc->amf_ue_ngap_id;
  psc->ran_ue_ngap_id = nc->ran_ue_ngap_id;
  psc->req_type       = smf.req_type;
  psc->pdu_session_id = smf.pdu_sess_id;
  psc->snssai.sST     = smf.snssai.sST;
  psc->snssai.sD      = smf.snssai.sD;
  psc->plmn.mcc       = smf.plmn.mcc;
  psc->plmn.mnc       = smf.plmn.mnc;

  Logger::amf_sbi().debug(
      "PDU Session Context, NSSAI SST (0x%x) SD %s", psc->snssai.sST,
      psc->snssai.sD.c_str());

  // parse binary dnn and store
  std::string dnn = "default";  // If DNN doesn't available, use "default"
  if ((smf.dnn != nullptr) && (blength(smf.dnn) > 0)) {
    char* tmp = conv::bstring2charString(smf.dnn);
    dnn       = tmp;
    free_wrapper((void**) &tmp);
  }

  Logger::amf_sbi().debug("Requested DNN: %s", dnn.c_str());
  psc->dnn = dnn;

  std::string smf_uri_root    = {};
  std::string smf_api_version = {};
  if (!psc->smf_info.info_available) {
    if (amf_cfg.support_features.enable_smf_selection) {
      // Get NRF URI
      std::string nrf_uri = {};
      if (!amf_sbi::get_nrf_uri(psc->snssai, psc->plmn, psc->dnn, nrf_uri)) {
        Logger::amf_sbi().error("No NRF is available");
        return;
      }
      Logger::amf_sbi().debug("NRF NF Discover URI: %s", nrf_uri.c_str());
      // use NRF to find suitable SMF based on snssai, plmn and dnn
      if (!discover_smf(
              smf_uri_root, smf_api_version, psc->snssai, psc->plmn, psc->dnn,
              nrf_uri)) {
        Logger::amf_sbi().error("SMF Selection, no SMF candidate is available");
        return;
      }

    } else if (!smf_selection_from_configuration(
                   smf_uri_root, smf_api_version)) {
      Logger::amf_sbi().error(
          "No SMF candidate is available (from configuration file)");
      return;
    }

    // store smf info to be used with this PDU session
    psc->smf_info.info_available = true;
    psc->smf_info.uri_root       = smf_uri_root;
    psc->smf_info.api_version    = smf_api_version;
  } else {
    smf_uri_root    = psc->smf_info.uri_root;
    smf_api_version = psc->smf_info.api_version;
  }

  switch (smf.req_type & 0x07) {
    case PDU_SESSION_INITIAL_REQUEST: {
      // get pti
      uint8_t* sm_msg = (uint8_t*) bdata(smf.sm_msg);
      uint8_t pti     = sm_msg[2];
      Logger::amf_sbi().debug(
          "Decoded PTI for PDUSessionEstablishmentRequest(0x%x)", pti);
      psc->is_n2sm_avaliable = false;
      handle_pdu_session_initial_request(
          supi, psc, smf_uri_root, smf_api_version, smf.sm_msg, dnn);
    } break;
    case EXISTING_PDU_SESSION: {
      // TODO:
    } break;
    case PDU_SESSION_TYPE_MODIFICATION_REQUEST: {
      // TODO:
    } break;
    default: {
      // TODO: should be removed
      // send Nsmf_PDUSession_UpdateSM_Context to SMF e.g., for PDU Session
      // release request
      send_pdu_session_update_sm_context_request(supi, psc, smf.sm_msg, dnn);
    }
  }
}

//------------------------------------------------------------------------------
void amf_sbi::send_pdu_session_update_sm_context_request(
    const std::string& supi, std::shared_ptr<pdu_session_context>& psc,
    bstring sm_msg, const std::string& dnn) {
  Logger::amf_sbi().debug(
      "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
      "ID %d, %s)",
      supi.c_str(), psc->pdu_session_id, psc->smf_info.addr.c_str());

  std::string remote_uri = {};
  if (!amf_cfg.get_smf_pdu_session_context_uri(psc, remote_uri)) {
    Logger::amf_sbi().error("Could not find Nsmf_PDUSession URI");
    return;
  }
  remote_uri += NSMF_PDU_SESSION_MODIFY;

  Logger::amf_sbi().debug("SMF URI: %s", remote_uri.c_str());

  nlohmann::json pdu_session_update_request                 = {};
  pdu_session_update_request[N1_SM_CONTENT_ID]["contentId"] = N1_SM_CONTENT_ID;
  std::string json_part = pdu_session_update_request.dump();

  std::string n1sm_msg = {};
  octet_stream_2_hex_stream(
      (uint8_t*) bdata(sm_msg), blength(sm_msg), n1sm_msg);

  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  curl_http_client(
      remote_uri, json_part, n1sm_msg, "", supi, psc->pdu_session_id,
      http_version);
}

//------------------------------------------------------------------------------
void amf_sbi::handle_pdu_session_initial_request(
    const std::string& supi, std::shared_ptr<pdu_session_context>& psc,
    const std::string& smf_uri_root, const std::string& smf_api_version,
    bstring sm_msg, const std::string& dnn) {
  Logger::amf_sbi().debug(
      "Handle PDU Session Establishment Request (SUPI %s, PDU Session ID %d)",
      supi.c_str(), psc->pdu_session_id);

  // Provide http2 port if enabled
  std::string amf_port = to_string(amf_cfg.sbi.port);
  if (amf_cfg.support_features.use_http2)
    amf_port = to_string(amf_cfg.sbi_http2_port);

  std::string remote_uri =
      amf_cfg.get_smf_pdu_session_base_uri(smf_uri_root, smf_api_version);

  Logger::amf_sbi().debug("SMF URI: %s", remote_uri.c_str());

  nlohmann::json pdu_session_establishment_request;
  pdu_session_establishment_request["supi"]          = supi.c_str();
  pdu_session_establishment_request["pei"]           = "imei-200000000000001";
  pdu_session_establishment_request["gpsi"]          = "msisdn-200000000001";
  pdu_session_establishment_request["dnn"]           = dnn.c_str();
  pdu_session_establishment_request["sNssai"]["sst"] = psc->snssai.sST;
  pdu_session_establishment_request["sNssai"]["sd"]  = psc->snssai.sD.c_str();
  pdu_session_establishment_request["pduSessionId"]  = psc->pdu_session_id;
  pdu_session_establishment_request["requestType"] =
      "INITIAL_REQUEST";  // TODO: from SM_MSG
  pdu_session_establishment_request["servingNfId"] = "servingNfId";
  pdu_session_establishment_request["servingNetwork"]["mcc"] =
      psc->plmn.mcc.c_str();
  pdu_session_establishment_request["servingNetwork"]["mnc"] =
      psc->plmn.mnc.c_str();
  pdu_session_establishment_request["anType"] = "3GPP_ACCESS";  // TODO
  pdu_session_establishment_request["smContextStatusUri"] =
      "http://" +
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.sbi.addr4))) + ":" +
      amf_port + "/nsmf-pdusession/callback/" + supi + "/" +
      std::to_string(psc->pdu_session_id);

  pdu_session_establishment_request["n1MessageContainer"]["n1MessageClass"] =
      "SM";
  pdu_session_establishment_request["n1MessageContainer"]["n1MessageContent"]
                                   ["contentId"] = N1_SM_CONTENT_ID;

  std::string json_part = pdu_session_establishment_request.dump();

  Logger::amf_sbi().debug("Message body %s", json_part.c_str());

  std::string n1sm_msg = {};
  octet_stream_2_hex_stream(
      (uint8_t*) bdata(sm_msg), blength(sm_msg), n1sm_msg);

  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  curl_http_client(
      remote_uri, json_part, n1sm_msg, "", supi, psc->pdu_session_id,
      http_version);
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_nsmf_pdusession_release_sm_context& itti_msg) {
  std::shared_ptr<pdu_session_context> psc = {};
  if (!amf_app_inst->find_pdu_session_context(
          itti_msg.supi, itti_msg.pdu_session_id, psc)) {
    Logger::amf_sbi().warn(
        "PDU Session context for SUPI %s doesn't exit!", itti_msg.supi.c_str());
    return;
  }

  std::string remote_uri = {};
  if (!amf_cfg.get_smf_pdu_session_context_uri(psc, remote_uri)) {
    Logger::amf_sbi().error("Could not find Nsmf_PDUSession URI");
    return;
  }
  remote_uri += NSMF_PDU_SESSION_RELEASE;
  Logger::amf_sbi().debug("SMF URI: %s", remote_uri.c_str());

  nlohmann::json pdu_session_release_request;
  pdu_session_release_request["cause"] = "REL_DUE_TO_REACTIVATION";  // TODO:
  // pdu_session_release_request["ngApCause"] = "radioNetwork";
  // TODO: 5gMmCauseValue
  // TODO: UserLocation
  // TODO: N2SmInfo
  std::string msg_body = pdu_session_release_request.dump();
  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  nlohmann::json response_json = {};
  uint32_t response_code       = 0;

  curl_http_client(
      remote_uri, "POST", msg_body, response_json, response_code, http_version);

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    amf_app_inst->trigger_process_response(itti_msg.promise_id, response_code);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(itti_sbi_notify_subscribed_event& itti_msg) {
  Logger::amf_sbi().debug("Send notification for the subscribed events");

  for (auto i : itti_msg.event_notifs) {
    // Fill the json part
    nlohmann::json json_data         = {};
    json_data["notifyCorrelationId"] = i.get_notify_correlation_id();
    auto report_lists                = nlohmann::json::array();
    nlohmann::json report            = {};

    std::vector<oai::amf::model::AmfEventReport> event_reports = {};
    i.get_reports(event_reports);
    for (auto r : event_reports) {
      report["type"]            = r.getType().get_value();
      report["state"]["active"] = true;  // as boolean
      if (r.supiIsSet()) {
        report["supi"] = r.getSupi();
      }
      if (r.locationIsSet()) {
        report["location"] =
            r.getLocation();  // get eutraLocation, nrLocation, n3gaLocation?
      }
      if (r.rmInfoListIsSet()) {
        report["rmInfoList"] = r.getRmInfoList();
      }
      if (r.cmInfoListIsSet()) {
        report["cmInfoList"] = r.getCmInfoList();
      }
      if (r.reachabilityIsSet()) {
        report["reachability"] = r.getReachability().get_value();
      }
      if (r.lossOfConnectReasonIsSet()) {
        report["lossOfConnectReason"] = r.getLossOfConnectReason().get_value();
      }
      if (r.ranUeNgapIdIsSet()) {
        report["ranUeNgapId"] = r.getRanUeNgapId();
      }
      if (r.amfUeNgapIdIsSet()) {
        report["amfUeNgapId"] = r.getAmfUeNgapId();
      }
      if (r.commFailureIsSet()) {
        report["commFailure"] = r.getCommFailure();
      }

      // Timestamp
      std::time_t time_epoch_ntp = std::time(nullptr);
      uint64_t tv_ntp =
          time_epoch_ntp;            // not needed: + SECONDS_SINCE_FIRST_EPOCH;
      report["timeStamp"] = tv_ntp;  // don't convert to string, leave as int64
      report_lists.push_back(report);
    }

    json_data["reportList"] = report_lists;

    std::string body             = json_data.dump();
    nlohmann::json response_json = {};

    std::string url        = i.get_notify_uri();
    uint32_t response_code = 0;
    curl_http_client(url, "POST", body, response_json, response_code);
    // TODO: process the response
  }
  return;
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_sbi_slice_selection_subscription_data& itti_msg) {
  Logger::amf_sbi().debug(
      "Send Slice Selection Subscription Data Retrieval to UDM (HTTP version "
      "%d)",
      itti_msg.http_version);

  std::string url =
      amf_cfg.get_udm_slice_selection_subscription_data_retrieval_uri(
          itti_msg.supi);
  nlohmann::json plmn_id = {};
  plmn_id["mcc"]         = itti_msg.plmn.mcc;
  plmn_id["mnc"]         = itti_msg.plmn.mnc;

  std::string parameters = {};
  parameters             = "?plmn-id=" + plmn_id.dump();
  url += parameters;

  Logger::amf_sbi().debug(
      "Send Slice Selection Subscription Data Retrieval to UDM, URL %s",
      url.c_str());

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(url, "GET", "", response_data, response_code);

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    amf_app_inst->trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }

  return;
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_sbi_network_slice_selection_information& itti_msg) {
  Logger::amf_sbi().debug(
      "Send Network Slice Selection Information Request to NSSF (HTTP version "
      "%d)",
      itti_msg.http_version);

  std::string url = amf_cfg.get_nssf_network_slice_selection_information_uri();

  // Slice Info Request For Registration
  nlohmann::json slice_info = {};
  to_json(slice_info, itti_msg.slice_info);
  // TODO: home-plmn-id
  // nlohmann::json home_plmn_id = {};
  // home_plmn_id["mcc"]         = itti_msg.tai.plmn.mcc;
  // home_plmn_id["mnc"]         = itti_msg.tai.plmn.mnc;

  // TAI
  nlohmann::json tai   = {};
  tai["plmnId"]["mcc"] = itti_msg.tai.plmn.mcc;
  tai["plmnId"]["mnc"] = itti_msg.tai.plmn.mnc;
  tai["tac"]           = std::to_string(itti_msg.tai.tac);

  std::string parameters = {};
  parameters = "?nf-type=AMF&nf-id=" + amf_app_inst->get_nf_instance() +
               "&slice-info-request-for-registration=" + slice_info.dump() +
               "&tai=" + tai.dump();
  //"?home-plmn-id=" + home_plmn_id.dump();
  url += parameters;
  Logger::amf_sbi().debug(
      "Send Slice Selection Information Retrieval to NSSF, URL %s",
      url.c_str());

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(url, "GET", "", response_data, response_code);

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    amf_app_inst->trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }

  return;
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(itti_sbi_n1_message_notify& itti_msg) {
  Logger::amf_sbi().debug(
      "Send N1 Message Notify to the target AMF (HTTP version "
      "%d)",
      itti_msg.http_version);

  std::string url = itti_msg.target_amf_uri + "/ue-contexts/" + itti_msg.supi +
                    "/n1-message-notify";

  Logger::amf_sbi().debug("Target AMF URI: %s", url.c_str());

  nlohmann::json json_data                 = {};
  json_data[N1_SM_CONTENT_ID]["contentId"] = N1_SM_CONTENT_ID;
  std::string json_part                    = json_data.dump();

  std::string n1sm_msg = {};
  octet_stream_2_hex_stream(
      (uint8_t*) bdata(itti_msg.registration_request),
      blength(itti_msg.registration_request), n1sm_msg);

  uint8_t http_version   = 1;
  uint32_t response_code = 0;
  std::string n2sm_msg   = {};

  curl_http_client(
      url, json_part, n1sm_msg, n2sm_msg, http_version, response_code);

  // TODO: handle response
  return;
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(itti_sbi_n2_info_notify& itti_msg) {
  Logger::amf_sbi().debug("Send N2 Info Notify to the subscribed NF");

  Logger::amf_sbi().debug("NF URI: %s", itti_msg.nf_uri.c_str());

  nlohmann::json json_data = {};
  to_json(json_data, itti_msg.n2_info_notification);
  std::string json_part = json_data.dump();

  std::string n2_info_msg = {};
  octet_stream_2_hex_stream(
      (uint8_t*) bdata(itti_msg.n2_info.value()),
      blength(itti_msg.n2_info.value()), n2_info_msg);

  uint8_t http_version   = 1;
  uint32_t response_code = 0;
  std::string n1sm_msg   = {};
  if (amf_cfg.support_features.use_http2) {
    http_version = 2;
  }

  curl_http_client(
      itti_msg.nf_uri, json_part, n1sm_msg, n2_info_msg, http_version,
      response_code);

  if (response_code == 204) {
    Logger::amf_sbi().debug("Sent notification successfully!");
  }
  return;
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(itti_sbi_nf_instance_discovery& itti_msg) {
  Logger::amf_sbi().debug(
      "Send NF Instance Discovery to NRF (HTTP version %d)",
      itti_msg.http_version);

  Logger::amf_sbi().debug("NRF URI: %s", itti_msg.nrf_amf_set.c_str());

  uint8_t http_version = itti_msg.http_version;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  nlohmann::json json_data = {};
  std::string url          = itti_msg.nrf_amf_set;

  // TODO: remove hardcoded values
  url += "?target-nf-type=AMF&requester-nf-type=AMF";

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(url, "GET", "", response_data, response_code, http_version);

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    amf_app_inst->trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_sbi_register_nf_instance_request& itti_msg) {
  Logger::amf_sbi().debug(
      "Send NF Instance Registration to NRF (HTTP version %d)",
      itti_msg.http_version);
  nlohmann::json json_data = {};
  itti_msg.profile.to_json(json_data);

  std::string url = amf_cfg.get_nrf_nf_registration_uri(
      itti_msg.profile.get_nf_instance_id());

  Logger::amf_sbi().debug(
      "Send NF Instance Registration to NRF, NRF URL %s", url.c_str());

  std::string body = json_data.dump();
  Logger::amf_sbi().debug(
      "Send NF Instance Registration to NRF, msg body: \n %s", body.c_str());

  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(
      url, "PUT", body, response_data, response_code, http_version);

  // Send response to APP to process
  std::shared_ptr<itti_sbi_register_nf_instance_response> itti_msg_response =
      std::make_shared<itti_sbi_register_nf_instance_response>(
          TASK_AMF_SBI, TASK_AMF_APP);
  itti_msg_response->http_response_code = response_code;
  itti_msg_response->http_version       = itti_msg.http_version;

  if (response_code ==
      static_cast<uint32_t>(
          http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED)) {
    Logger::amf_sbi().debug("NFRegistration, got successful response from NRF");
    Logger::amf_sbi().debug(
        "NF Instance Registration, response from NRF, json data: \n %s",
        response_data.dump().c_str());

    Logger::amf_sbi().debug("Registered AMF profile (from NRF)");
    itti_msg_response->profile.from_json(response_data);
  } else {
    Logger::amf_app().warn(
        "NF Instance Registration, got issue when registering to NRF");
  }

  int ret = itti_inst->send_msg(itti_msg_response);
  if (RETURNok != ret) {
    Logger::amf_sbi().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg_response->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_sbi_update_nf_instance_request& itti_msg) {
  Logger::amf_sbi().debug("Send NF Update to NRF");
  nlohmann::json json_data = nlohmann::json::array();
  for (auto i : itti_msg.patch_items) {
    nlohmann::json item = {};
    to_json(item, i);
    json_data.push_back(item);
  }
  std::string body = json_data.dump();
  Logger::amf_sbi().debug("Send NF Update to NRF, Msg body %s", body.c_str());

  std::string url =
      amf_cfg.get_nrf_nf_registration_uri(itti_msg.amf_instance_id);

  Logger::amf_sbi().debug("Send NF Update to NRF, NRF URL %s", url.c_str());

  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(
      url, "PATCH", body, response_data, response_code, http_version);

  Logger::amf_sbi().debug(
      "NF Update, response from NRF, json data: \n %s",
      response_data.dump().c_str());

  // Send response to APP to process
  std::shared_ptr<itti_sbi_update_nf_instance_response> itti_msg_response =
      std::make_shared<itti_sbi_update_nf_instance_response>(
          TASK_AMF_SBI, TASK_AMF_APP);
  itti_msg_response->http_response_code = response_code;
  itti_msg_response->amf_instance_id    = itti_msg.amf_instance_id;
  Logger::amf_sbi().debug("Registered AMF profile (from NRF)");

  int ret = itti_inst->send_msg(itti_msg_response);
  if (RETURNok != ret) {
    Logger::amf_sbi().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg_response->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_sbi_deregister_nf_instance_request& itti_msg) {
  Logger::amf_sbi().debug(
      "Send NF Deregistration to NRF (HTTP version %d)", itti_msg.http_version);

  std::string url =
      amf_cfg.get_nrf_nf_registration_uri(itti_msg.amf_instance_id);

  Logger::amf_sbi().debug(
      "Send NF Deregistration to NRF, NRF URL %s", url.c_str());

  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(
      url, "DELETE", "", response_data, response_code, http_version);

  // Send response to APP to process
  std::shared_ptr<itti_sbi_deregister_nf_instance_response> itti_msg_response =
      std::make_shared<itti_sbi_deregister_nf_instance_response>(
          TASK_AMF_SBI, TASK_AMF_APP);
  itti_msg_response->amf_instance_id    = itti_msg.amf_instance_id;
  itti_msg_response->http_response_code = response_code;
  itti_msg_response->http_version       = itti_msg.http_version;

  // TODO: Response code 307/308
  int ret = itti_inst->send_msg(itti_msg_response);
  if (RETURNok != ret) {
    Logger::amf_sbi().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        itti_msg_response->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_sbi::handle_itti_message(
    itti_sbi_determine_location_request& itti_msg) {
  Logger::amf_sbi().debug(
      "Send Determine Location Request to LMF (HTTP version %d)",
      itti_msg.http_version);

  std::string url = amf_cfg.get_lmf_determine_location_uri();
  Logger::amf_sbi().debug(
      "Send Determine Location Request to LMF, URL %s", url.c_str());

  std::string body = itti_msg.input_data.dump();
  Logger::amf_sbi().debug(
      "Send Determine Location Request to AUSF, msg body: \n %s", body.c_str());

  uint32_t response_code       = 0;
  nlohmann::json response_data = {};
  curl_http_client(
      url, "POST", body, response_data, response_code, itti_msg.http_version);

  Logger::amf_sbi().debug(
      "Determine Location, response from LMF, HTTP Code: %d", response_code);

  if ((response_code == 200) or
      (response_code == 201)) {  // TODO: remove hardcoded value
    Logger::amf_sbi().debug(
        "Determine Location, response from LMF\n, %s ",
        response_data.dump().c_str());

  } else {
    Logger::amf_sbi().warn(
        "Determine Location, could not get response from LMF");
  }

  // Notify to the result
  if (itti_msg.promise_id > 0) {
    amf_app_inst->trigger_process_response(itti_msg.promise_id, response_data);
    return;
  }
}
//------------------------------------------------------------------------------
bool amf_sbi::smf_selection_from_configuration(
    std::string& smf_uri_root, std::string& smf_api_version) {
  smf_uri_root = amf_cfg.smf_addr.uri_root;
  // smf_uri_root.append(inet_ntoa(amf_cfg.smf_addr.ipv4_addr)).append(":").append(std::to_string(
  // amf_cfg.smf_addr.port));
  smf_api_version = amf_cfg.smf_addr.api_version;
  return true;
}

//------------------------------------------------------------------------------
void amf_sbi::handle_post_sm_context_response_error(
    const long code, const std::string& cause, bstring n1sm,
    const std::string& supi, const uint8_t pdu_session_id) {
  output_wrapper::print_buffer(
      "amf_sbi", "N1 SM", (uint8_t*) bdata(n1sm), blength(n1sm));
  itti_n1n2_message_transfer_request* itti_msg =
      new itti_n1n2_message_transfer_request(TASK_AMF_SBI, TASK_AMF_APP);
  itti_msg->n1sm           = bstrcpy(n1sm);
  itti_msg->is_n2sm_set    = false;
  itti_msg->supi           = supi;
  itti_msg->pdu_session_id = pdu_session_id;
  itti_msg->is_ppi_set     = false;
  std::shared_ptr<itti_n1n2_message_transfer_request> i =
      std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_sbi().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        i->get_msg_name());
  }
}

//-----------------------------------------------------------------------------------------------------
bool amf_sbi::discover_smf(
    std::string& smf_uri_root, std::string& smf_api_version,
    const snssai_t& snssai, const plmn_t& plmn, const std::string& dnn,
    const std::string& nrf_uri) {
  Logger::amf_sbi().debug(
      "Send NFDiscovery to NRF to discover the available SMFs");
  bool result          = false;
  std::string smf_addr = {};
  int smf_port         = DEFAULT_HTTP1_PORT;

  uint8_t http_version = 1;
  if (amf_cfg.support_features.use_http2) http_version = 2;

  nlohmann::json json_data = {};
  std::string url          = {};

  if (!nrf_uri.empty()) {
    url = nrf_uri;
  } else {
    url = amf_cfg.get_nrf_nf_discovery_service_uri();
  }

  // TODO: remove hardcoded values
  url += "?target-nf-type=SMF&requester-nf-type=AMF";

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(url, "GET", "", response_data, response_code, http_version);

  Logger::amf_sbi().debug(
      "NFDiscovery, response from NRF, json data: \n %s",
      response_data.dump().c_str());

  if (static_cast<http_response_codes_e>(response_code) !=
      http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) {
    Logger::amf_sbi().warn("NFDiscovery, could not get response from NRF");
    result = false;
  } else {
    // Process data to obtain SMF info
    if (response_data.find("nfInstances") != response_data.end()) {
      for (auto& it : response_data["nfInstances"].items()) {
        nlohmann::json instance_json = it.value();
        // TODO: convert instance_json to SMF profile
        // TODO: add SMF to the list of available SMF
        // check with sNSSAI
        if (instance_json.find("sNssais") != instance_json.end()) {
          for (auto& s : instance_json["sNssais"].items()) {
            nlohmann::json Snssai = s.value();
            int sst               = 0;
            uint32_t sd           = SD_NO_VALUE;  // Default value
            if (Snssai.count("sst") > 0) sst = Snssai["sst"].get<int>();
            if (Snssai.count("sd") > 0) {
              conv::sd_string_to_int(Snssai["sd"].get<string>(), sd);
            }
            if (sst == snssai.sST) {
              uint32_t input_sd = SD_NO_VALUE;  // Default value
              conv::sd_string_to_int(snssai.sD, input_sd);
              if (sd == input_sd) {
                Logger::amf_sbi().debug(
                    "S-NSSAI [SST- %d, SD -%s] is matched for SMF profile",
                    snssai.sST, snssai.sD.c_str());
                result = true;
                break;  // NSSAI is included in the list of supported slices
                        // from SMF
              }
            }
          }
        }

        if (!result) {
          Logger::amf_sbi().debug("S-NSSAI is not matched for SMF profile");
          // continue;
        }

        // TODO: check DNN
        // TODO: PLMN (need to add plmnList into NRF profile, SMF profile)
        // for now, just IP addr of SMF of the first NF instance
        if (instance_json.find("ipv4Addresses") != instance_json.end()) {
          if (instance_json["ipv4Addresses"].size() > 0)
            smf_addr = instance_json["ipv4Addresses"].at(0).get<std::string>();
        }
        if (instance_json.find("nfServices") != instance_json.end()) {
          if (instance_json["nfServices"].size() > 0) {
            nlohmann::json nf_service = instance_json["nfServices"].at(0);
            // Can check services provided by SMF e.g., SMF pdu session
            if (nf_service.find("ipEndPoints") != nf_service.end()) {
              nlohmann::json nf_ip_endpoint = nf_service["ipEndPoints"].at(0);
              if (nf_ip_endpoint.find("port") != nf_ip_endpoint.end()) {
                smf_port = nf_ip_endpoint["port"].get<int>();
              }
            }

            if (nf_service.find("versions") != nf_service.end()) {
              nlohmann::json nf_version = nf_service["versions"].at(0);
              if (nf_version.find("apiVersionInUri") != nf_version.end()) {
                smf_api_version =
                    nf_version["apiVersionInUri"].get<std::string>();
              }
            }
          }
        }

        // Break after first matching SMF instance for requested S-NSSAI
        if (result) break;
      }
    }

    Logger::amf_sbi().debug(
        "NFDiscovery, SMF Info: Addr %s, Port %d, API Version %s",
        smf_addr.c_str(), smf_port, smf_api_version.c_str());
    smf_uri_root = {};
    smf_uri_root.append(smf_addr).append(":").append(std::to_string(smf_port));
  }

  return result;
}

//-----------------------------------------------------------------------------------------------------
bool amf_sbi::send_ue_authentication_request(
    const oai::amf::model::AuthenticationInfo& auth_info,
    oai::amf::model::UEAuthenticationCtx& ue_auth_ctx,
    const uint8_t& http_version) {
  Logger::amf_sbi().debug(
      "Send UE Authentication Request to AUSF (HTTP version %d)", http_version);

  nlohmann::json json_data = {};
  to_json(json_data, auth_info);
  std::string url = amf_cfg.get_ausf_ue_authentications_uri();

  Logger::amf_sbi().debug(
      "Send UE Authentication Request to AUSF, URL %s", url.c_str());

  std::string body = json_data.dump();
  Logger::amf_sbi().debug(
      "Send UE Authentication Request to AUSF, msg body: \n %s", body.c_str());

  nlohmann::json response_data = {};
  uint32_t response_code       = 0;
  curl_http_client(
      url, "POST", body, response_data, response_code, http_version);

  Logger::amf_sbi().debug(
      "UE Authentication, response from AUSF, HTTP Code: %lu", response_code);

  if ((static_cast<http_response_codes_e>(response_code) ==
       http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) or
      (static_cast<http_response_codes_e>(response_code) ==
       http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED)) {
    Logger::amf_sbi().debug(
        "UE Authentication, response from AUSF\n, %s ",
        response_data.dump().c_str());
    try {
      from_json(response_data, ue_auth_ctx);
    } catch (std::exception& e) {
      return false;
    }
  } else {
    Logger::amf_sbi().warn(
        "UE Authentication, could not get response from AUSF");
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------------------------------
bool amf_sbi::send_determine_location_request(
    const nlohmann::json& input_data, nlohmann::json& location_data,
    const uint8_t& http_version) {
  Logger::amf_sbi().debug(
      "Send Determine Location Request to LMF (HTTP version %d)", http_version);

  std::string url = amf_cfg.get_lmf_determine_location_uri();
  Logger::amf_sbi().debug(
      "Send Determine Location Request to LMF, URL %s", url.c_str());

  std::string body = input_data.dump();
  Logger::amf_sbi().debug(
      "Send Determine Location Request to AUSF, msg body: \n %s", body.c_str());

  uint32_t response_code = 0;
  curl_http_client(
      url, "POST", body, location_data, response_code, http_version);

  Logger::amf_sbi().debug(
      "Determine Location, response from LMF, HTTP Code: %d", response_code);

  if ((response_code == 200) or
      (response_code == 201)) {  // TODO: remove hardcoded value
    Logger::amf_sbi().debug(
        "Determine Location, response from LMF\n, %s ",
        location_data.dump().c_str());
    try {
      // from_json(response_data, location_data);
    } catch (std::exception& e) {
      return false;
    }
  } else {
    Logger::amf_sbi().warn(
        "Determine Location, could not get response from LMF");
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool amf_sbi::curl_http_client(
    const std::string& remote_uri, const std::string& json_data,
    const std::string& n1sm_msg, const std::string& n2sm_msg,
    const std::string& supi, const uint8_t& pdu_session_id,
    const uint8_t& http_version, const uint32_t& promise_id) {
  bool curl_result = false;
  Logger::amf_sbi().debug("Call NF service: %s", remote_uri.c_str());

  mime_parser parser                       = {};
  std::string body                         = {};
  std::shared_ptr<pdu_session_context> psc = {};
  bool is_multipart                        = true;

  if (!amf_app_inst->find_pdu_session_context(supi, pdu_session_id, psc)) {
    Logger::amf_sbi().warn(
        "PDU Session context for SUPI %s doesn't exit!", supi.c_str());
    return curl_result;
  }

  if ((n1sm_msg.size() > 0) and (n2sm_msg.size() > 0)) {
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, json_data, CURL_MIME_BOUNDARY, n1sm_msg, n2sm_msg);
  } else if (n1sm_msg.size() > 0) {  // only N1 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, json_data, CURL_MIME_BOUNDARY, n1sm_msg,
        multipart_related_content_part_e::NAS);
  } else if (n2sm_msg.size() > 0) {  // only N2 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, json_data, CURL_MIME_BOUNDARY, n2sm_msg,
        multipart_related_content_part_e::NGAP);
  } else {
    body         = json_data;
    is_multipart = false;
  }

  Logger::amf_sbi().debug("Send HTTP message to NF with body %s", body.c_str());

  uint32_t str_len = body.length();
  char* body_data  = (char*) malloc(str_len + 1);
  memset(body_data, 0, str_len + 1);
  memcpy((void*) body_data, (void*) body.c_str(), str_len);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    std::string content_type   = {};
    if (is_multipart) {
      content_type = "content-type: multipart/related; boundary=" +
                     std::string(CURL_MIME_BOUNDARY);
    } else {
      content_type = "content-type: application/json";
    }
    headers = curl_slist_append(headers, content_type.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, remote_uri.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.sbi.if_name.c_str());

    if (http_version == 2) {
      if (Logger::should_log(spdlog::level::debug))
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      // we use a self-signed test server, skip verification during debugging
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      curl_easy_setopt(
          curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
    }

    // Response information
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    std::unique_ptr<std::string> httpHeaderData(new std::string());

    // Hook up data handling function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, httpHeaderData.get());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_data);

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get cause from the response
    std::string response            = *httpData.get();
    std::string json_data_response  = {};
    std::optional<std::string> n1sm = {};
    std::optional<std::string> n2sm = {};
    nlohmann::json response_data    = {};
    bstring n1sm_hex                = nullptr;
    bstring n2sm_hex                = nullptr;

    Logger::amf_sbi().info("Get response with HTTP code (%ld)", httpCode);
    Logger::amf_sbi().info("Response body %s", response.c_str());

    if (static_cast<http_response_codes_e>(httpCode) ==
        http_response_codes_e::HTTP_RESPONSE_CODE_0) {
      // TODO: should be removed
      Logger::amf_sbi().error(
          "Cannot get response when calling %s", remote_uri.c_str());
      // free curl before returning
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      curl_global_cleanup();
      free_wrapper((void**) &body_data);
      return curl_result;
    }

    if (response.size() > 0) {
      if (!parser.parse(response)) {
        json_data_response = response;
      } else {
        parser.get(JSON_CONTENT_ID_MIME, json_data_response);
        parser.get(N1_SM_CONTENT_ID, n1sm);
        parser.get(N2_SM_CONTENT_ID, n2sm);
      }
    }

    Logger::amf_sbi().info("JSON part %s", json_data_response.c_str());

    if ((static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_204_NO_CONTENT)) {
      // ERROR
      if (response.size() < 1) {
        Logger::amf_sbi().error("There's no content in the response");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        free_wrapper((void**) &body_data);
        // TODO: send context response error
        return curl_result;
      }
      // TODO: HO

      // Transfer N1 to gNB/UE if available
      if (n1sm.has_value()) {
        try {
          response_data = nlohmann::json::parse(json_data_response);
        } catch (nlohmann::json::exception& e) {
          Logger::amf_sbi().warn(
              "Could not get JSON content from the response");
          // Set the default Cause
          response_data["error"]["cause"] = "504 Gateway Timeout";
        }

        Logger::amf_sbi().debug(
            "Get response with json_data: %s", json_data_response.c_str());
        conv::msg_str_2_msg_hex(n1sm.value(), n1sm_hex);
        output_wrapper::print_buffer(
            "amf_sbi", "Get response with n1sm:", (uint8_t*) bdata(n1sm_hex),
            blength(n1sm_hex));

        std::string cause = response_data["error"]["cause"];
        Logger::amf_sbi().debug(
            "Network Function services failure (with cause %s)", cause.c_str());
        //         if (!cause.compare("DNN_DENIED"))
        handle_post_sm_context_response_error(
            httpCode, cause, n1sm_hex, supi, pdu_session_id);
      }

    } else {  // Response with success code
      // Store location of the created context in case of PDU Session
      // Establishment
      std::string header_response = *httpHeaderData.get();
      std::string CRLF            = "\r\n";
      std::size_t location_pos    = header_response.find("Location");
      if (location_pos == std::string::npos)
        location_pos = header_response.find("location");

      if (location_pos != std::string::npos) {
        std::size_t crlf_pos = header_response.find(CRLF, location_pos);
        if (crlf_pos != std::string::npos) {
          std::string location = header_response.substr(
              location_pos + 10, crlf_pos - (location_pos + 10));
          Logger::amf_sbi().info(
              "Location of the created SMF context: %s", location.c_str());
          psc->smf_info.context_location = location;
        }
      }

      try {
        response_data = nlohmann::json::parse(json_data_response);
      } catch (nlohmann::json::exception& e) {
        Logger::amf_sbi().warn("Could not get JSON content from the response");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        free_wrapper((void**) &body_data);
        // TODO:
        return curl_result;
      }

      curl_result = true;

      std::string promise_result = {};

      bool is_ho_procedure              = false;
      bool is_up_deactivation_procedure = false;
      bool is_service_request           = false;
      // For N2 HO
      if (response_data.find("hoState") != response_data.end()) {
        is_ho_procedure = true;

        std::string ho_state = {};
        response_data.at("hoState").get_to(ho_state);
        if (ho_state.compare("COMPLETED") == 0) {
          if (response_data.find("pduSessionId") != response_data.end())
            response_data.at("pduSessionId").get_to(promise_result);
        } else if (n2sm.has_value()) {
          promise_result = n2sm.value();
        }
      }

      // UP deactivation
      if (response_data.find("upCnxState") != response_data.end()) {
        std::string up_cnx_state = {};
        response_data.at("upCnxState").get_to(up_cnx_state);
        if (up_cnx_state.compare("DEACTIVATED") == 0) {
          is_up_deactivation_procedure = true;
          promise_result               = std::to_string(httpCode);
        }

        // Service Request
        if (up_cnx_state.compare("ACTIVATING") == 0) {
          is_service_request = true;
          promise_result     = std::to_string(httpCode);
          // Update Pdu Session Context
          if (n2sm.has_value()) {
            conv::msg_str_2_msg_hex(n2sm.value(), n2sm_hex);
            output_wrapper::print_buffer(
                "amf_sbi", "[Service Request] Get response N2 SM:",
                (uint8_t*) bdata(n2sm_hex), blength(n2sm_hex));
            psc->n2sm              = bstrcpy(n2sm_hex);
            psc->is_n2sm_avaliable = true;
          }
        }
      }

      // Notify to the result
      if ((promise_id > 0) and
          (is_ho_procedure or is_up_deactivation_procedure or
           is_service_request)) {
        amf_app_inst->trigger_process_response(promise_id, promise_result);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        free_wrapper((void**) &body_data);
        bdestroy_wrapper(&n1sm_hex);
        return curl_result;
      }

      // Transfer N1/N2 to gNB/UE if available
      if (n1sm.has_value() or n2sm.has_value()) {
        itti_n1n2_message_transfer_request* itti_msg =
            new itti_n1n2_message_transfer_request(TASK_AMF_SBI, TASK_AMF_APP);

        itti_msg->is_n1sm_set = false;
        itti_msg->is_n2sm_set = false;
        itti_msg->is_ppi_set  = false;

        if (n1sm.has_value() > 0) {
          conv::msg_str_2_msg_hex(n1sm.value(), n1sm_hex);
          output_wrapper::print_buffer(
              "amf_sbi", "Get response N1 SM:", (uint8_t*) bdata(n1sm_hex),
              blength(n1sm_hex));
          itti_msg->n1sm        = bstrcpy(n1sm_hex);
          itti_msg->is_n1sm_set = true;
        }
        if (n2sm.has_value() > 0) {
          conv::msg_str_2_msg_hex(n2sm.value(), n2sm_hex);
          output_wrapper::print_buffer(
              "amf_sbi", "Get response N2 SM:", (uint8_t*) bdata(n2sm_hex),
              blength(n2sm_hex));
          itti_msg->n2sm        = bstrcpy(n2sm_hex);
          itti_msg->is_n2sm_set = true;
          itti_msg->n2sm_info_type =
              response_data["n2SmInfoType"].get<std::string>();
        }

        itti_msg->supi           = supi;
        itti_msg->pdu_session_id = pdu_session_id;
        std::shared_ptr<itti_n1n2_message_transfer_request> i =
            std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (0 != ret) {
          Logger::amf_sbi().error(
              "Could not send ITTI message %s to task TASK_AMF_APP",
              i->get_msg_name());
        }
      }
    }

    bdestroy_wrapper(&n1sm_hex);
    bdestroy_wrapper(&n2sm_hex);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  free_wrapper((void**) &body_data);
  return curl_result;
}

//------------------------------------------------------------------------------
void amf_sbi::curl_http_client(
    const std::string& remote_uri, std::string& json_data,
    std::string& n1sm_msg, std::string& n2sm_msg, const uint8_t& http_version,
    uint32_t& response_code, const uint32_t& promise_id) {
  Logger::amf_sbi().debug("Call NF service: %s", remote_uri.c_str());

  uint8_t number_parts = 0;
  mime_parser parser   = {};
  std::string body     = {};

  bool is_multipart = true;

  if ((n1sm_msg.size() > 0) and (n2sm_msg.size() > 0)) {
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, json_data, CURL_MIME_BOUNDARY, n1sm_msg, n2sm_msg);
  } else if (n1sm_msg.size() > 0) {  // only N1 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, json_data, CURL_MIME_BOUNDARY, n1sm_msg,
        multipart_related_content_part_e::NAS);
  } else if (n2sm_msg.size() > 0) {  // only N2 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, json_data, CURL_MIME_BOUNDARY, n2sm_msg,
        multipart_related_content_part_e::NGAP);
  } else {
    body         = json_data;
    is_multipart = false;
  }

  Logger::amf_sbi().debug("Send HTTP message to NF with body %s", body.c_str());

  uint32_t str_len = body.length();
  char* body_data  = (char*) malloc(str_len + 1);
  memset(body_data, 0, str_len + 1);
  memcpy((void*) body_data, (void*) body.c_str(), str_len);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    std::string content_type   = {};
    if (is_multipart) {
      content_type = "content-type: multipart/related; boundary=" +
                     std::string(CURL_MIME_BOUNDARY);
    } else {
      content_type = "content-type: application/json";
    }
    headers = curl_slist_append(headers, content_type.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, remote_uri.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.sbi.if_name.c_str());

    if (http_version == 2) {
      if (Logger::should_log(spdlog::level::debug))
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      // we use a self-signed test server, skip verification during debugging
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      curl_easy_setopt(
          curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
    }

    // Response information
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    std::unique_ptr<std::string> httpHeaderData(new std::string());

    // Hook up data handling function
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, httpHeaderData.get());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_data);

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get cause from the response
    std::string response           = *httpData.get();
    std::string json_data_response = {};
    std::string n1sm               = {};
    std::string n2sm               = {};
    nlohmann::json response_data   = {};

    // clear input
    n1sm_msg  = {};
    n2sm_msg  = {};
    json_data = {};

    Logger::amf_sbi().info("Get response with HTTP code (%ld)", httpCode);
    Logger::amf_sbi().info("Response body %s", response.c_str());

    response_code = httpCode;
    if (static_cast<http_response_codes_e>(httpCode) ==
        http_response_codes_e::HTTP_RESPONSE_CODE_0) {
      // TODO: should be removed
      Logger::amf_sbi().error(
          "Cannot get response when calling %s", remote_uri.c_str());
      // free curl before returning
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      curl_global_cleanup();
      free_wrapper((void**) &body_data);
      return;
    }

    if (response.size() > 0) {
      number_parts = parser.parse(response, json_data_response, n1sm, n2sm);
    }

    if (number_parts == 0) {
      json_data_response = response;
    }

    Logger::amf_sbi().info("JSON part %s", json_data_response.c_str());

    if ((static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_204_NO_CONTENT)) {
      // TODO:

    } else {  // Response with success code

      try {
        response_data = nlohmann::json::parse(json_data_response);
      } catch (nlohmann::json::exception& e) {
        Logger::amf_sbi().warn("Could not get JSON content from the response");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        free_wrapper((void**) &body_data);
        // TODO:
        return;
      }

      // Transfer N1/N2 to gNB/UE if available
      if (number_parts > 1) {
        if (n1sm.size() > 0) {
          n1sm_msg = n1sm;
        }
        if (n2sm.size() > 0) {
          n2sm_msg = n2sm;
        }
      }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  free_wrapper((void**) &body_data);
}

//-----------------------------------------------------------------------------------------------------
void amf_sbi::curl_http_client(
    const std::string& remote_uri, const std::string& method,
    const std::string& msg_body, nlohmann::json& response_json,
    uint32_t& response_code, const uint8_t& http_version) {
  Logger::amf_sbi().info("Send HTTP message to %s", remote_uri.c_str());
  Logger::amf_sbi().info("HTTP message Body: %s", msg_body.c_str());

  uint32_t str_len = msg_body.length();
  char* body_data  = (char*) malloc(str_len + 1);
  memset(body_data, 0, str_len + 1);
  memcpy((void*) body_data, (void*) msg_body.c_str(), str_len);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;

    std::string content_type = "Content-Type: application/json";
    headers                  = curl_slist_append(headers, content_type.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, remote_uri.c_str());

    if (method.compare("POST") == 0)
      curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
    else if (method.compare("PATCH") == 0)
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    else if (method.compare("PUT") == 0)
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    else if (method.compare("DELETE") == 0)
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    else  // GET
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.sbi.if_name.c_str());

    if (http_version == 2) {
      if (Logger::should_log(spdlog::level::debug))
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      // we use a self-signed test server, skip verification during debugging
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      curl_easy_setopt(
          curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
    }

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    std::unique_ptr<std::string> httpHeaderData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, httpHeaderData.get());
    if ((method.compare("POST") == 0) or (method.compare("PATCH") == 0) or
        (method.compare("PUT") == 0)) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, msg_body.length());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_data);
    }

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get the response
    std::string response = *httpData.get();
    std::string resMsg   = {};
    bool is_response_ok  = true;
    Logger::amf_sbi().info("Get response with HTTP code (%ld)", httpCode);

    response_code = httpCode;

    if (static_cast<http_response_codes_e>(httpCode) ==
        http_response_codes_e::HTTP_RESPONSE_CODE_0) {
      Logger::amf_sbi().info(
          "Cannot get response when calling %s", remote_uri.c_str());
      // free curl before returning
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      return;
    }

    if ((static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) and
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED) and
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_204_NO_CONTENT)) {
      is_response_ok = false;

      if (response.size() < 1) {
        Logger::amf_sbi().info("There's no content in the response");
        response_json = {};
        return;
      }
    }

    try {
      response_json = nlohmann::json::parse(response);
    } catch (nlohmann::json::exception& e) {
      Logger::amf_sbi().info("Could not get JSON content from the response");
      response_json = {};
    }

    Logger::amf_sbi().info(
        "Get response with Json content: %s", response_json.dump().c_str());

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  if (body_data) {
    free(body_data);
    body_data = NULL;
  }
}

//------------------------------------------------------------------------------
bool amf_sbi::get_nrf_uri(
    const snssai_t& snssai, const plmn_t& plmn, const std::string& dnn,
    std::string& nrf_uri) {
  if (!amf_cfg.support_features.enable_nssf) {
    // Get NRF info from configuration file if available
    nrf_uri = amf_cfg.get_nrf_nf_discovery_service_uri();
    return true;
  } else {  // Get NRF info from NSSF
            // TODO: check if external NSSF feature is supported
    Logger::amf_sbi().debug(
        "Send NS Selection to NSSF to discover the appropriate NRF");

    bool result = false;

    uint8_t http_version = 1;
    if (amf_cfg.support_features.use_http2) http_version = 2;

    // Get NSI information from NSSF
    nlohmann::json slice_info  = {};
    nlohmann::json snssai_info = {};
    snssai_info["sst"]         = snssai.sST;
    if (!snssai.sD.empty()) snssai_info["sd"] = snssai.sD;
    slice_info["sNssai"]            = snssai_info;
    slice_info["roamingIndication"] = "NON_ROAMING";
    // ToDo Add TAI

    std::string nssf_url =
        amf_cfg.get_nssf_network_slice_selection_information_uri();

    std::string parameters = {};
    parameters = "?nf-type=AMF&nf-id=" + amf_app_inst->get_nf_instance() +
                 "&slice-info-request-for-pdu-session=" + slice_info.dump();
    nssf_url += parameters;

    Logger::amf_sbi().debug(
        "Send Network Slice Information Retrieval during PDU session "
        "establishment procedure, URL %s",
        nssf_url.c_str());

    nlohmann::json response_data = {};
    uint32_t response_code       = 0;
    curl_http_client(
        nssf_url, "GET", "", response_data, response_code, http_version);

    Logger::amf_sbi().debug(
        "NS Selection, response from NSSF, json data: \n %s",
        response_data.dump().c_str());

    if (static_cast<http_response_codes_e>(response_code) !=
        http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) {
      Logger::amf_sbi().warn("NS Selection, could not get response from NSSF");
      result = false;
    } else {
      // Process data to obtain NRF info
      if (response_data.find("nsiInformation") != response_data.end()) {
        if (response_data["nsiInformation"].count("nrfId") > 0) {
          // nrf_uri =
          // response_data["nsiInformation"]["nrfId"].get<std::string>();

          // TODO: Should be remove when NSSF is updated with NRF Disc URI
          std::string nrf_id =
              response_data["nsiInformation"]["nrfId"].get<std::string>();
          std::vector<std::string> split_result;
          boost::split(split_result, nrf_id, boost::is_any_of("/"));
          if (split_result.size() > 4) {
            nrf_uri = split_result[2] + "/nnrf-disc/" + split_result[4] +
                      "/nf-instances";
          }

          Logger::amf_sbi().debug(
              "NSI Information is successfully retrieved from NSSF");
          Logger::amf_sbi().debug(
              "NS Selection, NRF's URI: %s", nrf_uri.c_str());
          result = true;
        }

        std::string nsi_id = {};
        if (response_data["nsiInformation"].count("nsi_id") > 0)
          nsi_id = response_data["nsiInformation"]["nsiId"].get<std::string>();
      }
    }

    return result;
  }
  return true;
}
