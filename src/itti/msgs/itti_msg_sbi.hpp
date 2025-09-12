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

#ifndef ITTI_MSG_SBI_HPP_INCLUDED_
#define ITTI_MSG_SBI_HPP_INCLUDED_

#include <optional>

#include "3gpp_29.500.h"
#include "AuthenticationInfo.h"
#include "N1MessageNotification.h"
#include "N2InformationNotification.h"
#include "NonUeN2InfoSubscriptionCreateData.h"
#include "PatchItem.h"
#include "SliceInfoForRegistration.h"
#include "SmContextStatusNotification.h"
#include "UeN1N2InfoSubscriptionCreateData.h"
#include "amf.hpp"
#include "amf_msg.hpp"
#include "amf_profile.hpp"
#include "bstrlib.h"
#include "itti_msg.hpp"
#include "utils.hpp"
#include "PlmnIdNid.h"
#include "PolicyAssociationRequest.h"
#include "PolicyAssociationUpdateRequest.h"
#include "PolicyUpdate.h"
#include "TerminationNotification.h"
#include "SubscriptionData.h"
#include "AmfStatusChangeNotification.h"
#include "UeContextInfoClass.h"
#include "RequestLocInfo.h"

using namespace amf_application;

class itti_msg_n11 : public itti_msg {
 public:
  itti_msg_n11(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    ran_ue_ngap_id = 0;
  }
  itti_msg_n11(const itti_msg_n11& i) : itti_msg(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }
  virtual ~itti_msg_n11(){};

 public:
  uint64_t amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id;
};

class itti_nsmf_pdusession_create_sm_context : public itti_msg_n11 {
 public:
  itti_nsmf_pdusession_create_sm_context(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(NSMF_PDU_SESSION_CREATE_SM_CTX, origin, destination) {
    req_type    = 0;
    pdu_sess_id = 0;
    dnn         = nullptr;
    sm_msg      = nullptr;
    snssai      = {};
    plmn        = {};
  }
  itti_nsmf_pdusession_create_sm_context(
      const itti_nsmf_pdusession_create_sm_context& i)
      : itti_msg_n11(i) {
    req_type    = i.req_type;
    pdu_sess_id = i.pdu_sess_id;
    dnn         = i.dnn;
    sm_msg      = i.sm_msg;
    snssai      = {};
    plmn        = {};
  }
  virtual ~itti_nsmf_pdusession_create_sm_context() {
    oai::utils::utils::bdestroy_wrapper(&dnn);
    oai::utils::utils::bdestroy_wrapper(&sm_msg);
  }

  const char* get_msg_name() { return "NSMF_PDU_SESSION_CREATE_SM_CTX"; };

 public:
  uint8_t req_type;
  uint8_t pdu_sess_id;
  bstring dnn;
  bstring sm_msg;
  snssai_t snssai;
  plmn_t plmn;
};

class itti_pdu_session_resource_setup_response : public itti_msg_n11 {
 public:
  itti_pdu_session_resource_setup_response(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(PDU_SESSION_RESOURCE_SETUP_RESPONSE, origin, destination) {
    pdu_session_id = 0;
    n2sm           = nullptr;
  }
  itti_pdu_session_resource_setup_response(
      const itti_pdu_session_resource_setup_response& i)
      : itti_msg_n11(i) {
    pdu_session_id = i.pdu_session_id;
    n2sm           = i.n2sm;
  }
  virtual ~itti_pdu_session_resource_setup_response() {
    oai::utils::utils::bdestroy_wrapper(&n2sm);
  }

  const char* get_msg_name() { return "PDU_SESSION_RESOURCE_SETUP_RESPONSE"; };

 public:
  uint8_t pdu_session_id;
  bstring n2sm;
};

class itti_nsmf_pdusession_update_sm_context : public itti_msg_n11 {
 public:
  itti_nsmf_pdusession_update_sm_context(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(NSMF_PDU_SESSION_UPDATE_SM_CTX, origin, destination) {
    supi           = {};
    pdu_session_id = 0;
    n1sm           = nullptr;
    is_n1sm_set    = false;
    n2sm           = nullptr;
    is_n2sm_set    = false;
    n2sm_info_type = {};
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    promise_id     = 0;
    ho_state       = {};
    up_cnx_state   = {};
  }
  itti_nsmf_pdusession_update_sm_context(
      const itti_nsmf_pdusession_update_sm_context& i)
      : itti_msg_n11(i) {
    supi           = i.supi;
    pdu_session_id = i.pdu_session_id;
    n1sm           = i.n1sm;
    is_n1sm_set    = i.is_n1sm_set;
    n2sm           = i.n2sm;
    is_n2sm_set    = i.is_n2sm_set;
    n2sm_info_type = i.n2sm_info_type;
    promise_id     = i.promise_id;
    ho_state       = i.ho_state;
    up_cnx_state   = i.up_cnx_state;
  }
  virtual ~itti_nsmf_pdusession_update_sm_context() {
    oai::utils::utils::bdestroy_wrapper(&n2sm);
  }

  const char* get_msg_name() { return "NSMF_PDU_SESSION_UPDATE_SM_CTX"; };

 public:
  std::string supi;
  uint8_t pdu_session_id;
  bstring n1sm;
  bool is_n1sm_set;
  bstring n2sm;
  bool is_n2sm_set;
  std::string n2sm_info_type;
  uint32_t promise_id;
  std::string ho_state;
  std::string up_cnx_state;
};

class itti_nsmf_pdusession_release_sm_context : public itti_msg_n11 {
 public:
  itti_nsmf_pdusession_release_sm_context(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n11(NSMF_PDU_SESSION_RELEASE_SM_CTX, origin, destination) {}
  itti_nsmf_pdusession_release_sm_context(
      const itti_nsmf_pdusession_release_sm_context& i)
      : itti_msg_n11(i) {
    supi             = i.supi;
    pdu_session_id   = i.pdu_session_id;
    promise_id       = i.promise_id;
    context_location = i.context_location;
  }
  virtual ~itti_nsmf_pdusession_release_sm_context() {}
  const char* get_msg_name() { return "NSMF_PDU_SESSION_RELEASE_SM_CTX"; };

 public:
  std::string supi;
  uint8_t pdu_session_id;
  uint32_t promise_id;
  std::string context_location;
};

class itti_sbi_msg : public itti_msg {
 public:
  itti_sbi_msg(
      const itti_msg_type_t msg_type, const task_id_t orig,
      const task_id_t dest)
      : itti_msg(msg_type, orig, dest) {}
  itti_sbi_msg(const itti_sbi_msg& i) : itti_msg(i) {}
  itti_sbi_msg(
      const itti_sbi_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(i) {
    origin      = orig;
    destination = dest;
  }
  const char* get_msg_name() {
    switch (msg_type) {
      case ITTI_MSG_TYPE_NONE:
        return "ITTI_MSG_TYPE_NONE";
      case ASYNC_SHELL_CMD:
        return "ASYNC_SHELL_CMD";
      case NEW_SCTP_ASSOCIATION:
        return "NEW_SCTP_ASSOCIATION";
      case NG_SETUP_REQ:
        return "NG_SETUP_REQ";
      case NG_RESET:
        return "NG_RESET";
      case NG_SHUTDOWN:
        return "NG_SHUTDOWN";
      case INITIAL_UE_MSG:
        return "INITIAL_UE_MSG";
      case ITTI_UL_NAS_TRANSPORT:
        return "ITTI_UL_NAS_TRANSPORT";
      case ITTI_DL_NAS_TRANSPORT:
        return "ITTI_DL_NAS_TRANSPORT";
      case INITIAL_CONTEXT_SETUP_REQUEST:
        return "INITIAL_CONTEXT_SETUP_REQUEST";
      case PDU_SESSION_RESOURCE_SETUP_REQUEST:
        return "PDU_SESSION_RESOURCE_SETUP_REQUEST";
      case PDU_SESSION_RESOURCE_MODIFY_REQUEST:
        return "PDU_SESSION_RESOURCE_MODIFY_REQUEST";
      case PDU_SESSION_RESOURCE_RELEASE_COMMAND:
        return "PDU_SESSION_RESOURCE_RELEASE_COMMAND";
      case PDU_SESSION_RESOURCE_SETUP_RESPONSE:
        return "PDU_SESSION_RESOURCE_SETUP_RESPONSE";
      case NSMF_PDU_SESSION_RELEASE_SM_CTX:
        return "NSMF_PDU_SESSION_RELEASE_SM_CTX";
      case UE_CONTEXT_RELEASE_REQUEST:
        return "UE_CONTEXT_RELEASE_REQUEST";
      case UE_CONTEXT_RELEASE_COMMAND:
        return "UE_CONTEXT_RELEASE_COMMAND";
      case UE_CONTEXT_RELEASE_COMPLETE:
        return "UE_CONTEXT_RELEASE_COMPLETE";
      case UE_RADIO_CAP_IND:
        return "UE_RADIO_CAP_IND";
      case UL_NAS_DATA_IND:
        return "UL_NAS_DATA_IND";
      case DOWNLINK_NAS_TRANSFER:
        return "DOWNLINK_NAS_TRANSFER";
      case NAS_SIG_ESTAB_REQ:
        return "NAS_SIG_ESTAB_REQ";
      case N1N2_MESSAGE_TRANSFER_REQ:
        return "N1N2_MESSAGE_TRANSFER_REQ";
      case NON_UE_N2_MESSAGE_TRANSFER_REQ:
        return "NON_UE_N2_MESSAGE_TRANSFER_REQ";
      case REROUTE_NAS_REQ:
        return "REROUTE_NAS_REQ";
      case NSMF_PDU_SESSION_CREATE_SM_CTX:
        return "NSMF_PDU_SESSION_CREATE_SM_CTX";
      case NSMF_PDU_SESSION_UPDATE_SM_CTX:
        return "NSMF_PDU_SESSION_UPDATE_SM_CTX";
      case SBI_N1_MESSAGE_NOTIFY:
        return "SBI_N1_MESSAGE_NOTIFY";
      case SBI_N2_INFO_NOTIFY:
        return "SBI_N2_INFO_NOTIFY";
      case SBI_REGISTER_NF_INSTANCE_REQUEST:
        return "SBI_REGISTER_NF_INSTANCE_REQUEST";
      case SBI_REGISTER_NF_INSTANCE_RESPONSE:
        return "SBI_REGISTER_NF_INSTANCE_RESPONSE";
      case SBI_UPDATE_NF_INSTANCE_REQUEST:
        return "SBI_UPDATE_NF_INSTANCE_REQUEST";
      case SBI_UPDATE_NF_INSTANCE_RESPONSE:
        return "SBI_UPDATE_NF_INSTANCE_RESPONSE";
      case SBI_DEREGISTER_NF_INSTANCE_REQUEST:
        return "SBI_DEREGISTER_NF_INSTANCE_REQUEST";
      case SBI_DEREGISTER_NF_INSTANCE_RESPONSE:
        return "SBI_DEREGISTER_NF_INSTANCE_RESPONSE";
      case SBI_SLICE_SELECTION_SUBSCRIPTION_DATA:
        return "SBI_SLICE_SELECTION_SUBSCRIPTION_DATA";
      case SBI_NETWORK_SLICE_SELECTION_INFORMATION:
        return "SBI_NETWORK_SLICE_SELECTION_INFORMATION";
      case SBI_NETWORK_SLICE_SELECTION_DISCOVERY:
        return "SBI_NETWORK_SLICE_SELECTION_DISCOVERY";
      case SBI_NF_INSTANCE_DISCOVERY:
        return "SBI_NF_INSTANCE_DISCOVERY";
      case SBI_AMF_CONFIGURATION:
        return "SBI_AMF_CONFIGURATION";
      case SBI_UPDATE_AMF_CONFIGURATION:
        return "SBI_UPDATE_AMF_CONFIGURATION";
      case SBI_EVENT_EXPOSURE_REQUEST:
        return "SBI_EVENT_EXPOSURE_REQUEST";
      case SBI_NOTIFICATION_DATA:
        return "SBI_NOTIFICATION_DATA";
      case SBI_NOTIFY_SUBSCRIBED_EVENT:
        return "SBI_NOTIFY_SUBSCRIBED_EVENT";
      case SBI_N1_MESSAGE_NOTIFICATION:
        return "SBI_N1_MESSAGE_NOTIFICATION";
      case SBI_N1N2_MESSAGE_SUBSCRIBE:
        return "SBI_N1N2_MESSAGE_SUBSCRIBE";
      case SBI_N1N2_MESSAGE_UNSUBSCRIBE:
        return "SBI_N1N2_MESSAGE_UNSUBSCRIBE";
      case SBI_NON_UE_N2_INFO_SUBSCRIBE:
        return "SBI_NON_UE_N2_INFO_SUBSCRIBE";
      case SBI_NON_UE_N2_INFO_UNSUBSCRIBE:
        return "SBI_NON_UE_N2_INFO_UNSUBSCRIBE";
      case SBI_NON_UE_N2_INFO_NOTIFY:
        return "SBI_NON_UE_N2_INFO_NOTIFY";
      case SBI_PDU_SESSION_RELEASE_NOTIF:
        return "SBI_PDU_SESSION_RELEASE_NOTIF";
      case SBI_DETERMINE_LOCATION_REQUEST:
        return "SBI_DETERMINE_LOCATION_REQUEST";
      case SBI_UE_AUTHENTICATION_REQUEST:
        return "SBI_UE_AUTHENTICATION_REQUEST";
      case SBI_UE_AUTHENTICATION_CONFIRMATION:
        return "SBI_UE_AUTHENTICATION_CONFIRMATION";
      case SBI_REGISTER_WITH_UDM:
        return "SBI_REGISTER_WITH_UDM";
      case SBI_REGISTER_WITH_UDM_RESPONSE:
        return "SBI_REGISTER_WITH_UDM_RESPONSE";
      case SBI_RETRIEVE_AM_DATA:
        return "SBI_RETRIEVE_AM_DATA";
      case SBI_RETRIEVE_AM_DATA_RESPONSE:
        return "SBI_RETRIEVE_AM_DATA_RESPONSE";
      case SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA:
        return "SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA";
      case SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE:
        return "SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE";
      case SBI_PCF_DISCOVERY:
        return "SBI_PCF_DISCOVERY";
      case SBI_PCF_DISCOVERY_RESPONSE:
        return "SBI_PCF_DISCOVERY_RESPONSE";
      case SBI_AM_POLICY_ASSOCIATION:
        return "SBI_AM_POLICY_ASSOCIATION";
      case SBI_AM_POLICY_ASSOCIATION_RESPONSE:
        return "SBI_AM_POLICY_ASSOCIATION_RESPONSE";
      case SBI_AM_POLICY_ASSOCIATION_TERMINATION:
        return "SBI_AM_POLICY_ASSOCIATION_TERMINATION";
      case SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE:
        return "SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE";
      case SBI_AM_POLICY_ASSOCIATION_UPDATE:
        return "SBI_AM_POLICY_ASSOCIATION_UPDATE";
      case SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE:
        return "SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE";
      case SBI_AM_POLICY_ASSOCIATION_RETRIEVAL:
        return "SBI_AM_POLICY_ASSOCIATION_RETRIEVAL";
      case SBI_AM_POLICY_ASSOCIATION_RETRIEVAL_RESPONSE:
        return "SBI_AM_POLICY_ASSOCIATION_RETRIEVAL_RESPONSE";
      case SBI_AM_POLICY_UPDATE_NOTIFICATION:
        return "SBI_AM_POLICY_UPDATE_NOTIFICATION";
      case SBI_AM_POLICY_UPDATE_NOTIFICATION_RESPONSE:
        return "SBI_AM_POLICY_UPDATE_NOTIFICATION_RESPONSE";
      case SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION:
        return "SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION";
      case SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION_RESPONSE:
        return "SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION_RESPONSE";
      case SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL:
        return "SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL";
      case SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE:
        return "SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE";
      case SBI_AMF_STATUS_CHANGE_SUBSCRIBE_REQUEST:
        return "SBI_AMF_STATUS_CHANGE_SUBSCRIBE_REQUEST";
      case SBI_AMF_STATUS_CHANGE_UNSUBSCRIBE_REQUEST:
        return "SBI_AMF_STATUS_CHANGE_UNSUBSCRIBE_REQUEST";
      case SBI_AMF_STATUS_CHANGE_SUBSCRIBE_MODIFY:
        return "SBI_AMF_STATUS_CHANGE_SUBSCRIBE_MODIFY";
      case SBI_AMF_STATUS_CHANGE_NOTIFICATION:
        return "SBI_AMF_STATUS_CHANGE_NOTIFICATION";
      case SBI_PROVIDE_DOMAIN_SELECTION_INFO:
        return "SBI_PROVIDE_DOMAIN_SELECTION_INFO";
      case SBI_PROVIDE_LOCATION_INFO:
        return "SBI_PROVIDE_LOCATION_INFO";
      case HANDOVER_REQUIRED_MSG:
        return "HANDOVER_REQUIRED_MSG";
      case HANDOVER_REQUEST_ACK:
        return "HANDOVER_REQUEST_ACK";
      case HANDOVER_NOTIFY:
        return "HANDOVER_NOTIFY";
      case UPLINK_RAN_STATUS_TRANSFER:
        return "UPLINK_RAN_STATUS_TRANSFER";
      case DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT:
        return "DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT";
      case UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT:
        return "UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT";
      case DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT:
        return "DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT";
      case UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT:
        return "UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT";
      case PAGING:
        return "PAGING";
      case TIME_OUT:
        return "TIME_OUT";
      case HEALTH_PING:
        return "HEALTH_PING";
      case TERMINATE:
        return "TERMINATE";
      case ITTI_MSG_TYPE_MAX:
        return "ITTI_MSG_TYPE_MAX";
      default:
        return "UNKNOWN";
    }
  };

 public:
};

//-----------------------------------------------------------------------------
class itti_sbi_register_nf_instance_request : public itti_sbi_msg {
 public:
  itti_sbi_register_nf_instance_request(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_REGISTER_NF_INSTANCE_REQUEST, orig, dest),
        profile(),
        nrf_uri() {}
  itti_sbi_register_nf_instance_request(
      const itti_sbi_register_nf_instance_request& i)
      : itti_sbi_msg(i), profile(i.profile), nrf_uri(i.nrf_uri) {}
  itti_sbi_register_nf_instance_request(
      const itti_sbi_register_nf_instance_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest), profile(i.profile), nrf_uri(i.nrf_uri) {}
  virtual ~itti_sbi_register_nf_instance_request() {}
  const char* get_msg_name() { return "SBI_REGISTER_NF_INSTANCE_REQUEST"; };

  amf_application::amf_profile profile;
  std::string nrf_uri;
};

//-----------------------------------------------------------------------------
class itti_sbi_register_nf_instance_response : public itti_sbi_msg {
 public:
  itti_sbi_register_nf_instance_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_REGISTER_NF_INSTANCE_RESPONSE, orig, dest) {}
  virtual ~itti_sbi_register_nf_instance_response(){};
  const char* get_msg_name() { return "SBI_REGISTER_NF_INSTANCE_RESPONSE"; };

  amf_application::amf_profile profile;
  uint16_t http_response_code;
  std::string nrf_uri;
};

//-----------------------------------------------------------------------------
class itti_sbi_update_nf_instance_request : public itti_sbi_msg {
 public:
  itti_sbi_update_nf_instance_request(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_UPDATE_NF_INSTANCE_REQUEST, orig, dest) {}
  virtual ~itti_sbi_update_nf_instance_request(){};
  const char* get_msg_name() { return "SBI_UPDATE_NF_INSTANCE_REQUEST"; };

  std::vector<oai::_3gpp::model::PatchItem> patch_items;
  std::string amf_instance_id;
  std::string nrf_uri;
};

//-----------------------------------------------------------------------------
class itti_sbi_update_nf_instance_response : public itti_sbi_msg {
 public:
  itti_sbi_update_nf_instance_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_UPDATE_NF_INSTANCE_RESPONSE, orig, dest) {}
  virtual ~itti_sbi_update_nf_instance_response(){};
  const char* get_msg_name() { return "SBI_UPDATE_NF_INSTANCE_RESPONSE"; };

  std::string amf_instance_id;
  uint32_t http_response_code;
  std::string nrf_uri;
};

//-----------------------------------------------------------------------------
class itti_sbi_deregister_nf_instance_request : public itti_sbi_msg {
 public:
  itti_sbi_deregister_nf_instance_request(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_DEREGISTER_NF_INSTANCE_REQUEST, orig, dest) {}
  virtual ~itti_sbi_deregister_nf_instance_request(){};
  const char* get_msg_name() { return "SBI_DEREGISTER_NF_INSTANCE_REQUEST"; };

  std::string amf_instance_id;
  std::string nrf_uri;
};

//-----------------------------------------------------------------------------
class itti_sbi_deregister_nf_instance_response : public itti_sbi_msg {
 public:
  itti_sbi_deregister_nf_instance_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_DEREGISTER_NF_INSTANCE_RESPONSE, orig, dest),
        http_response_code() {}
  virtual ~itti_sbi_deregister_nf_instance_response(){};
  const char* get_msg_name() { return "SBI_DEREGISTER_NF_INSTANCE_RESPONSE"; };

  std::string amf_instance_id;
  uint32_t http_response_code;
  std::string nrf_uri;
  // TODO: Redirect response;
  // TODO: Header location
};

//-----------------------------------------------------------------------------
class itti_sbi_slice_selection_subscription_data : public itti_sbi_msg {
 public:
  itti_sbi_slice_selection_subscription_data(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_SLICE_SELECTION_SUBSCRIPTION_DATA, orig, dest) {}
  virtual ~itti_sbi_slice_selection_subscription_data(){};
  const char* get_msg_name() {
    return "SBI_SLICE_SELECTION_SUBSCRIPTION_DATA";
  };

  std::string supi;
  plmn_t plmn;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_network_slice_selection_information : public itti_sbi_msg {
 public:
  itti_sbi_network_slice_selection_information(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NETWORK_SLICE_SELECTION_INFORMATION, orig, dest) {}
  virtual ~itti_sbi_network_slice_selection_information(){};
  const char* get_msg_name() {
    return "SBI_NETWORK_SLICE_SELECTION_INFORMATION";
  };

  std::string nf_instance_id;
  oai::_3gpp::model::SliceInfoForRegistration slice_info;
  tai_t tai;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_network_slice_selection_discovery : public itti_sbi_msg {
 public:
  itti_sbi_network_slice_selection_discovery(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NETWORK_SLICE_SELECTION_DISCOVERY, orig, dest) {}
  virtual ~itti_sbi_network_slice_selection_discovery(){};
  const char* get_msg_name() {
    return "SBI_NETWORK_SLICE_SELECTION_DISCOVERY";
  };

  std::string nf_instance_id;
  snssai_t snssai;
  plmn_t plmn;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_nf_instance_discovery : public itti_sbi_msg {
 public:
  itti_sbi_nf_instance_discovery(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NF_INSTANCE_DISCOVERY, orig, dest),
        target_amf_set_is_set(false) {}
  virtual ~itti_sbi_nf_instance_discovery(){};
  const char* get_msg_name() { return "SBI_NF_INSTANCE_DISCOVERY"; };

  std::string target_amf_set;
  bool target_amf_set_is_set;
  std::string target_nf_type;
  std::string nrf_amf_set;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_n1_message_notify : public itti_sbi_msg {
 public:
  itti_sbi_n1_message_notify(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_N1_MESSAGE_NOTIFY, orig, dest) {
    target_amf_uri       = {};
    supi                 = {};
    registration_request = nullptr;
  }
  virtual ~itti_sbi_n1_message_notify() {
    oai::utils::utils::bdestroy_wrapper(&registration_request);
  };

  const char* get_msg_name() { return "SBI_N1_MESSAGE_NOTIFY"; };

  std::string target_amf_uri;
  std::string supi;
  bstring registration_request;
};

//-----------------------------------------------------------------------------
class itti_sbi_n2_info_notify : public itti_sbi_msg {
 public:
  itti_sbi_n2_info_notify(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_N2_INFO_NOTIFY, orig, dest) {
    nf_uri               = {};
    n2_info_notification = {};
    n1_message           = std::nullopt;
    n2_info              = std::nullopt;
  }
  virtual ~itti_sbi_n2_info_notify() {
    if (n1_message.has_value())
      oai::utils::utils::bdestroy_wrapper(&n1_message.value());
    if (n2_info.has_value())
      oai::utils::utils::bdestroy_wrapper(&n2_info.value());
  };

  const char* get_msg_name() { return "SBI_N2_INFO_NOTIFY"; };

  std::string nf_uri;
  oai::_3gpp::model::N2InformationNotification n2_info_notification;
  std::optional<bstring> n1_message;
  std::optional<bstring> n2_info;
};

//-----------------------------------------------------------------------------
class itti_sbi_event_exposure_request : public itti_sbi_msg {
 public:
  itti_sbi_event_exposure_request(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_EVENT_EXPOSURE_REQUEST, orig, dest),
        event_exposure() {}
  itti_sbi_event_exposure_request(const itti_sbi_event_exposure_request& i)
      : itti_sbi_msg(i), event_exposure(i.event_exposure) {}
  itti_sbi_event_exposure_request(
      const itti_sbi_event_exposure_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest), event_exposure(i.event_exposure) {}
  virtual ~itti_sbi_event_exposure_request(){};
  const char* get_msg_name() { return "SBI_EVENT_EXPOSURE_REQUEST"; };

  event_exposure_msg event_exposure;
};

//-----------------------------------------------------------------------------
class itti_sbi_notification_data : public itti_sbi_msg {
 public:
  itti_sbi_notification_data(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NOTIFICATION_DATA, orig, dest), notification_msg() {}
  itti_sbi_notification_data(const itti_sbi_notification_data& i)
      : itti_sbi_msg(i), notification_msg(i.notification_msg) {}
  itti_sbi_notification_data(
      const itti_sbi_notification_data& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest), notification_msg(i.notification_msg) {}
  virtual ~itti_sbi_notification_data(){};
  const char* get_msg_name() { return "SBI_NOTIFICATION_DATA"; };

  data_notification_msg notification_msg;
};

//-----------------------------------------------------------------------------
class itti_sbi_notify_subscribed_event : public itti_sbi_msg {
 public:
  itti_sbi_notify_subscribed_event(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NOTIFY_SUBSCRIBED_EVENT, orig, dest), notif_id() {}

  itti_sbi_notify_subscribed_event(const itti_sbi_notify_subscribed_event& i)
      : itti_sbi_msg(i), notif_id(i.notif_id) {}
  itti_sbi_notify_subscribed_event(
      const itti_sbi_notify_subscribed_event& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest), notif_id(i.notif_id) {}
  virtual ~itti_sbi_notify_subscribed_event(){};
  const char* get_msg_name() { return "SBI_NOTIFY_SUBSCRIBED_EVENT"; };

  std::string notif_id;
  std::vector<amf_application::event_notification> event_notifs;
};

//-----------------------------------------------------------------------------
class itti_sbi_n1_message_notification : public itti_sbi_msg {
 public:
  itti_sbi_n1_message_notification(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_N1_MESSAGE_NOTIFICATION, orig, dest),
        notification_msg(),
        ue_id(),
        n1sm() {}
  itti_sbi_n1_message_notification(const itti_sbi_n1_message_notification& i)
      : itti_sbi_msg(i),
        notification_msg(i.notification_msg),
        ue_id(i.ue_id),
        n1sm(i.n1sm) {}
  itti_sbi_n1_message_notification(
      const itti_sbi_n1_message_notification& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        notification_msg(i.notification_msg),
        ue_id(i.ue_id),
        n1sm(i.n1sm) {}
  virtual ~itti_sbi_n1_message_notification(){};
  const char* get_msg_name() { return "SBI_N1_MESSAGE_NOTIFICATION"; };

  oai::_3gpp::model::N1MessageNotification notification_msg;
  std::string ue_id;
  std::string n1sm;
};

//-----------------------------------------------------------------------------
class itti_sbi_n1n2_message_subscribe : public itti_sbi_msg {
 public:
  itti_sbi_n1n2_message_subscribe(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_N1N2_MESSAGE_SUBSCRIBE, orig, dest),
        ue_cxt_id(),
        subscription_data(),
        promise_id(pid) {}
  itti_sbi_n1n2_message_subscribe(const itti_sbi_n1n2_message_subscribe& i)
      : itti_sbi_msg(i),
        ue_cxt_id(i.ue_cxt_id),
        subscription_data(i.subscription_data),
        promise_id() {}
  itti_sbi_n1n2_message_subscribe(
      const itti_sbi_n1n2_message_subscribe& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        ue_cxt_id(i.ue_cxt_id),
        subscription_data(i.subscription_data),
        promise_id(i.promise_id) {}
  virtual ~itti_sbi_n1n2_message_subscribe(){};
  const char* get_msg_name() { return "SBI_N1N2_MESSAGE_SUBSCRIBE"; };

  std::string ue_cxt_id;
  oai::_3gpp::model::UeN1N2InfoSubscriptionCreateData subscription_data;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_n1n2_message_unsubscribe : public itti_sbi_msg {
 public:
  itti_sbi_n1n2_message_unsubscribe(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_N1N2_MESSAGE_UNSUBSCRIBE, orig, dest),
        ue_cxt_id(),
        subscription_id(),
        promise_id(pid) {}
  itti_sbi_n1n2_message_unsubscribe(const itti_sbi_n1n2_message_unsubscribe& i)
      : itti_sbi_msg(i),
        ue_cxt_id(i.ue_cxt_id),
        subscription_id(i.subscription_id),
        promise_id() {}
  itti_sbi_n1n2_message_unsubscribe(
      const itti_sbi_n1n2_message_unsubscribe& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        ue_cxt_id(i.ue_cxt_id),
        subscription_id(i.subscription_id),
        promise_id(i.promise_id) {}
  virtual ~itti_sbi_n1n2_message_unsubscribe(){};
  const char* get_msg_name() { return "SBI_N1N2_MESSAGE_UNSUBSCRIBE"; };

  std::string ue_cxt_id;
  std::string subscription_id;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_non_ue_n2_info_subscribe : public itti_sbi_msg {
 public:
  itti_sbi_non_ue_n2_info_subscribe(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_NON_UE_N2_INFO_SUBSCRIBE, orig, dest),
        subscription_data(),
        promise_id(pid) {}
  itti_sbi_non_ue_n2_info_subscribe(const itti_sbi_non_ue_n2_info_subscribe& i)
      : itti_sbi_msg(i), subscription_data(i.subscription_data), promise_id() {}
  itti_sbi_non_ue_n2_info_subscribe(
      const itti_sbi_non_ue_n2_info_subscribe& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        subscription_data(i.subscription_data),
        promise_id(i.promise_id) {}
  virtual ~itti_sbi_non_ue_n2_info_subscribe(){};
  const char* get_msg_name() { return "NON UE N2 INFO SUBSCRIBE"; };

  oai::_3gpp::model::NonUeN2InfoSubscriptionCreateData subscription_data;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_non_ue_n2_info_unsubscribe : public itti_sbi_msg {
 public:
  itti_sbi_non_ue_n2_info_unsubscribe(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_NON_UE_N2_INFO_UNSUBSCRIBE, orig, dest),
        subscription_id(),
        promise_id(pid) {}
  itti_sbi_non_ue_n2_info_unsubscribe(
      const itti_sbi_non_ue_n2_info_unsubscribe& i)
      : itti_sbi_msg(i), subscription_id(i.subscription_id), promise_id() {}
  itti_sbi_non_ue_n2_info_unsubscribe(
      const itti_sbi_non_ue_n2_info_unsubscribe& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        subscription_id(i.subscription_id),
        promise_id(i.promise_id) {}
  virtual ~itti_sbi_non_ue_n2_info_unsubscribe(){};
  const char* get_msg_name() { return "NON UE N2 INFO UNSUBSCRIBE"; };

  std::string subscription_id;
  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_amf_configuration : public itti_sbi_msg {
 public:
  itti_sbi_amf_configuration(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_AMF_CONFIGURATION, orig, dest), promise_id(pid) {}
  virtual ~itti_sbi_amf_configuration(){};
  const char* get_msg_name() { return "SBI_AMF_CONFIGURATION"; };

  uint32_t promise_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_update_amf_configuration : public itti_sbi_msg {
 public:
  itti_sbi_update_amf_configuration(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_UPDATE_AMF_CONFIGURATION, orig, dest),
        promise_id(pid) {}
  virtual ~itti_sbi_update_amf_configuration(){};
  const char* get_msg_name() { return "SBI_UPDATE_AMF_CONFIGURATION"; };

  uint32_t promise_id;
  nlohmann::json configuration;
};

//-----------------------------------------------------------------------------
class itti_sbi_pdu_session_release_notif : public itti_sbi_msg {
 public:
  itti_sbi_pdu_session_release_notif(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_PDU_SESSION_RELEASE_NOTIF, orig, dest),
        promise_id(pid),
        ue_id(),
        pdu_session_id(),
        smContextStatusNotification() {}
  itti_sbi_pdu_session_release_notif(
      const itti_sbi_pdu_session_release_notif& i)
      : itti_sbi_msg(i),
        promise_id(),
        ue_id(),
        pdu_session_id(),
        smContextStatusNotification(i.smContextStatusNotification) {}
  itti_sbi_pdu_session_release_notif(
      const itti_sbi_pdu_session_release_notif& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        promise_id(i.promise_id),
        ue_id(i.ue_id),
        pdu_session_id(i.pdu_session_id),
        smContextStatusNotification(i.smContextStatusNotification) {}

  virtual ~itti_sbi_pdu_session_release_notif(){};
  const char* get_msg_name() { return "SBI_PDU_SESSION_RELEASE_NOTIF"; };

  uint32_t promise_id;
  std::string ue_id;
  uint8_t pdu_session_id;
  oai::_3gpp::model::SmContextStatusNotification smContextStatusNotification;
};

//-----------------------------------------------------------------------------
class itti_sbi_determine_location_request : public itti_sbi_msg {
 public:
  itti_sbi_determine_location_request(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_DETERMINE_LOCATION_REQUEST, orig, dest),
        promise_id(pid),
        input_data() {}
  itti_sbi_determine_location_request(
      const itti_sbi_determine_location_request& i)
      : itti_sbi_msg(i) {
    promise_id = i.promise_id;
    input_data = i.input_data;
  }
  itti_sbi_determine_location_request(
      const itti_sbi_determine_location_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest) {
    promise_id = i.promise_id;
    input_data = i.input_data;
  }

  virtual ~itti_sbi_determine_location_request(){};
  const char* get_msg_name() { return "SBI_DETERMINE_LOCATION_REQUEST"; };

  uint32_t promise_id;
  nlohmann::json input_data;
};

//-----------------------------------------------------------------------------
class itti_sbi_ue_authentication_request : public itti_sbi_msg {
 public:
  itti_sbi_ue_authentication_request(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_UE_AUTHENTICATION_REQUEST, orig, dest),
        promise_id(pid),
        auth_info() {}
  itti_sbi_ue_authentication_request(
      const itti_sbi_ue_authentication_request& i)
      : itti_sbi_msg(i) {
    promise_id = i.promise_id;
    auth_info  = i.auth_info;
  }
  itti_sbi_ue_authentication_request(
      const itti_sbi_ue_authentication_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest) {
    promise_id = i.promise_id;
    auth_info  = i.auth_info;
  }

  virtual ~itti_sbi_ue_authentication_request(){};
  const char* get_msg_name() { return "SBI_UE_AUTHENTICATION_REQUEST"; };

  uint32_t promise_id;
  oai::_3gpp::model::AuthenticationInfo auth_info;
};

//-----------------------------------------------------------------------------
class itti_sbi_ue_authentication_confirmation : public itti_sbi_msg {
 public:
  itti_sbi_ue_authentication_confirmation(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_UE_AUTHENTICATION_CONFIRMATION, orig, dest),
        uri(),
        promise_id(pid),
        confirmation_data() {}
  itti_sbi_ue_authentication_confirmation(
      const itti_sbi_ue_authentication_confirmation& i)
      : itti_sbi_msg(i) {
    uri               = i.uri;
    promise_id        = i.promise_id;
    confirmation_data = i.confirmation_data;
  }
  itti_sbi_ue_authentication_confirmation(
      const itti_sbi_ue_authentication_confirmation& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest) {
    uri               = i.uri;
    promise_id        = i.promise_id;
    confirmation_data = i.confirmation_data;
  }

  virtual ~itti_sbi_ue_authentication_confirmation(){};
  const char* get_msg_name() { return "SBI_UE_AUTHENTICATION_CONFIRMATION"; };

  std::string uri;
  uint32_t promise_id;
  nlohmann::json confirmation_data;
};

//-----------------------------------------------------------------------------
class itti_sbi_register_with_udm : public itti_sbi_msg {
 public:
  itti_sbi_register_with_udm(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_REGISTER_WITH_UDM, orig, dest), supi() {}

  itti_sbi_register_with_udm(const itti_sbi_register_with_udm& i)
      : itti_sbi_msg(i) {
    supi              = i.supi;
    registration_data = i.registration_data;
  }
  virtual ~itti_sbi_register_with_udm(){};
  const char* get_msg_name() { return "SBI_REGISTER_WITH_UDM"; };

  std::string supi;
  nlohmann::json registration_data;  // Amf3GppAccessRegistration
};

//-----------------------------------------------------------------------------
class itti_sbi_register_with_udm_response : public itti_sbi_msg {
 public:
  itti_sbi_register_with_udm_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_REGISTER_WITH_UDM_RESPONSE, orig, dest), supi() {}

  itti_sbi_register_with_udm_response(
      const itti_sbi_register_with_udm_response& i)
      : itti_sbi_msg(i) {
    supi          = i.supi;
    response_data = i.response_data;
  }
  virtual ~itti_sbi_register_with_udm_response(){};
  const char* get_msg_name() { return "SBI_REGISTER_WITH_UDM_RESPONSE"; };

  std::string supi;
  nlohmann::json response_data;
};

//-----------------------------------------------------------------------------
class itti_sbi_retrieve_am_data : public itti_sbi_msg {
 public:
  itti_sbi_retrieve_am_data(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_RETRIEVE_AM_DATA, orig, dest),
        promise_id(pid),
        supi(),
        plmn_id() {}

  itti_sbi_retrieve_am_data(const itti_sbi_retrieve_am_data& i)
      : itti_sbi_msg(i) {
    promise_id = i.promise_id;
    supi       = i.supi;
    plmn_id    = i.plmn_id;
  }
  virtual ~itti_sbi_retrieve_am_data(){};
  const char* get_msg_name() { return "SBI_RETRIEVE_AM_DATA"; };

  uint32_t promise_id;
  std::string supi;
  oai::_3gpp::model::PlmnIdNid plmn_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_retrieve_am_data_response : public itti_sbi_msg {
 public:
  itti_sbi_retrieve_am_data_response(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_RETRIEVE_AM_DATA_RESPONSE, orig, dest),
        supi(),
        response_data() {}

  itti_sbi_retrieve_am_data_response(
      const itti_sbi_retrieve_am_data_response& i)
      : itti_sbi_msg(i) {
    supi          = i.supi;
    response_data = i.response_data;
  }
  virtual ~itti_sbi_retrieve_am_data_response(){};
  const char* get_msg_name() { return "SBI_RETRIEVE_AM_DATA_RESPONSE"; };

  std::string supi;
  nlohmann::json response_data;
};

//-----------------------------------------------------------------------------
class itti_sbi_retrieve_smf_selection_subscription_data : public itti_sbi_msg {
 public:
  itti_sbi_retrieve_smf_selection_subscription_data(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA, orig, dest),
        supi(),
        plmn_id() {}

  itti_sbi_retrieve_smf_selection_subscription_data(
      const itti_sbi_retrieve_smf_selection_subscription_data& i)
      : itti_sbi_msg(i) {
    supi    = i.supi;
    plmn_id = i.plmn_id;
  }
  virtual ~itti_sbi_retrieve_smf_selection_subscription_data(){};
  const char* get_msg_name() {
    return "SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA";
  };

  std::string supi;
  oai::_3gpp::model::PlmnIdNid plmn_id;
};

//-----------------------------------------------------------------------------
class itti_sbi_retrieve_smf_selection_subscription_data_response
    : public itti_sbi_msg {
 public:
  itti_sbi_retrieve_smf_selection_subscription_data_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(
            SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE, orig, dest),
        supi(),
        response_data() {}

  itti_sbi_retrieve_smf_selection_subscription_data_response(
      const itti_sbi_retrieve_smf_selection_subscription_data_response& i)
      : itti_sbi_msg(i) {
    supi          = i.supi;
    response_data = i.response_data;
  }
  virtual ~itti_sbi_retrieve_smf_selection_subscription_data_response(){};
  const char* get_msg_name() {
    return "SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE";
  };

  std::string supi;
  nlohmann::json response_data;
};

class itti_sbi_pcf_discovery : public itti_sbi_msg {
 public:
  itti_sbi_pcf_discovery(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_PCF_DISCOVERY, orig, dest),
        promise_id(pid),
        supi(),
        nrf_uri(),
        dnn(),
        plmn_id(),
        snssai() {}

  itti_sbi_pcf_discovery(const itti_sbi_pcf_discovery& i) : itti_sbi_msg(i) {
    promise_id = i.promise_id;
    supi       = i.supi;
    nrf_uri    = i.nrf_uri;
    dnn        = i.dnn;
    plmn_id    = i.plmn_id;
    snssai     = i.snssai;
  }
  virtual ~itti_sbi_pcf_discovery(){};
  const char* get_msg_name() { return "SBI_PCF_DISCOVERY"; };

  uint32_t promise_id;
  std::string supi;
  std::string nrf_uri;
  std::string dnn;
  oai::_3gpp::model::PlmnIdNid plmn_id;
  snssai_t snssai;
};

class itti_sbi_am_policy_association : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AM_POLICY_ASSOCIATION, orig, dest),
        policy_assoc_req() {}

  itti_sbi_am_policy_association(const itti_sbi_am_policy_association& i)
      : itti_sbi_msg(i) {
    policy_assoc_req = i.policy_assoc_req;
  }
  virtual ~itti_sbi_am_policy_association(){};
  const char* get_msg_name() { return "SBI_AM_POLICY_ASSOCIATION"; };

  oai::_3gpp::model::PolicyAssociationRequest policy_assoc_req;
};

class itti_sbi_am_policy_association_response : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AM_POLICY_ASSOCIATION_RESPONSE, orig, dest),
        supi(),
        response_data(),
        policy_assoc_location() {}

  itti_sbi_am_policy_association_response(
      const itti_sbi_am_policy_association_response& i)
      : itti_sbi_msg(i) {
    supi                  = i.supi;
    response_data         = i.response_data;
    policy_assoc_location = i.policy_assoc_location;
  }
  virtual ~itti_sbi_am_policy_association_response(){};
  const char* get_msg_name() { return "SBI_AM_POLICY_ASSOCIATION_RESPONSE"; };

  std::string supi;
  nlohmann::json response_data;
  std::string policy_assoc_location;
};

class itti_sbi_am_policy_association_termination : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_termination(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AM_POLICY_ASSOCIATION_TERMINATION, orig, dest),
        supi() {}

  itti_sbi_am_policy_association_termination(
      const itti_sbi_am_policy_association_termination& i)
      : itti_sbi_msg(i) {
    supi = i.supi;
  }
  virtual ~itti_sbi_am_policy_association_termination(){};
  const char* get_msg_name() {
    return "SBI_AM_POLICY_ASSOCIATION_TERMINATION";
  };

  std::string supi;
};

class itti_sbi_am_policy_association_termination_response
    : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_termination_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(
            SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE, orig, dest),
        supi(),
        response_data() {}

  itti_sbi_am_policy_association_termination_response(
      const itti_sbi_am_policy_association_termination_response& i)
      : itti_sbi_msg(i) {
    supi          = i.supi;
    response_data = i.response_data;
  }
  virtual ~itti_sbi_am_policy_association_termination_response(){};
  const char* get_msg_name() {
    return "SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE";
  };

  std::string supi;
  nlohmann::json response_data;
};

class itti_sbi_am_policy_association_update : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_update(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AM_POLICY_ASSOCIATION_UPDATE, orig, dest),
        supi(),
        policy_assoc_update_req() {}

  itti_sbi_am_policy_association_update(
      const itti_sbi_am_policy_association_update& i)
      : itti_sbi_msg(i) {
    supi                    = i.supi;
    policy_assoc_update_req = i.policy_assoc_update_req;
  }
  virtual ~itti_sbi_am_policy_association_update(){};
  const char* get_msg_name() { return "SBI_AM_POLICY_ASSOCIATION_UPDATE"; };

  std::string supi;
  oai::_3gpp::model::PolicyAssociationUpdateRequest policy_assoc_update_req;
};

class itti_sbi_am_policy_association_update_response : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_update_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE, orig, dest),
        supi(),
        response_data() {}

  itti_sbi_am_policy_association_update_response(
      const itti_sbi_am_policy_association_update_response& i)
      : itti_sbi_msg(i) {
    supi          = i.supi;
    response_data = i.response_data;
  }
  virtual ~itti_sbi_am_policy_association_update_response(){};
  const char* get_msg_name() {
    return "SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE";
  };

  std::string supi;
  nlohmann::json response_data;
};

class itti_sbi_am_policy_association_retrieval : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_retrieval(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AM_POLICY_ASSOCIATION_RETRIEVAL, orig, dest), supi() {}

  itti_sbi_am_policy_association_retrieval(
      const itti_sbi_am_policy_association_retrieval& i)
      : itti_sbi_msg(i) {
    supi = i.supi;
  }
  virtual ~itti_sbi_am_policy_association_retrieval(){};
  const char* get_msg_name() { return "SBI_AM_POLICY_ASSOCIATION_RETRIEVAL"; };

  std::string supi;
};

class itti_sbi_am_policy_association_retrieval_response : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_retrieval_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AM_POLICY_ASSOCIATION_RETRIEVAL_RESPONSE, orig, dest),
        supi(),
        response_data() {}

  itti_sbi_am_policy_association_retrieval_response(
      const itti_sbi_am_policy_association_retrieval_response& i)
      : itti_sbi_msg(i) {
    supi          = i.supi;
    response_data = i.response_data;
  }
  virtual ~itti_sbi_am_policy_association_retrieval_response(){};
  const char* get_msg_name() {
    return "SBI_AM_POLICY_ASSOCIATION_RETRIEVAL_RESPONSE";
  };

  std::string supi;
  nlohmann::json response_data;
};

class itti_sbi_am_policy_update_notification : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_update_notification(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_AM_POLICY_UPDATE_NOTIFICATION, orig, dest),
        promise_id(pid),
        supi(),
        policy_update() {}

  itti_sbi_am_policy_update_notification(
      const itti_sbi_am_policy_update_notification& i)
      : itti_sbi_msg(i) {
    promise_id    = i.promise_id;
    supi          = i.supi;
    policy_update = i.policy_update;
  }
  virtual ~itti_sbi_am_policy_update_notification(){};
  const char* get_msg_name() { return "SBI_AM_POLICY_UPDATE_NOTIFICATION"; };

  std::string supi;
  uint32_t promise_id;
  oai::_3gpp::model::PolicyUpdate policy_update;
};

class itti_sbi_am_policy_association_termination_notification
    : public itti_sbi_msg {
 public:
  itti_sbi_am_policy_association_termination_notification(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(
            SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION, orig, dest),
        promise_id(pid),
        supi(),
        termination_notification() {}

  itti_sbi_am_policy_association_termination_notification(
      const itti_sbi_am_policy_association_termination_notification& i)
      : itti_sbi_msg(i) {
    promise_id               = i.promise_id;
    supi                     = i.supi;
    termination_notification = i.termination_notification;
  }
  virtual ~itti_sbi_am_policy_association_termination_notification(){};
  const char* get_msg_name() {
    return "SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION";
  };

  std::string supi;
  uint32_t promise_id;
  oai::_3gpp::model::TerminationNotification termination_notification;
};

class itti_sbi_ue_context_in_smf_data_retrieval : public itti_sbi_msg {
 public:
  itti_sbi_ue_context_in_smf_data_retrieval(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL, orig, dest),
        supi() {}

  itti_sbi_ue_context_in_smf_data_retrieval(
      const itti_sbi_ue_context_in_smf_data_retrieval& i)
      : itti_sbi_msg(i) {
    supi = i.supi;
  }
  virtual ~itti_sbi_ue_context_in_smf_data_retrieval(){};
  const char* get_msg_name() { return "SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL"; };

  std::string supi;
};

class itti_sbi_ue_context_in_smf_data_retrieval_response : public itti_sbi_msg {
 public:
  itti_sbi_ue_context_in_smf_data_retrieval_response(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE, orig, dest),
        supi(),
        response_data() {}

  itti_sbi_ue_context_in_smf_data_retrieval_response(
      const itti_sbi_ue_context_in_smf_data_retrieval_response& i)
      : itti_sbi_msg(i) {
    supi          = i.supi;
    response_data = i.response_data;
  }
  virtual ~itti_sbi_ue_context_in_smf_data_retrieval_response(){};
  const char* get_msg_name() {
    return "SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE";
  };

  std::string supi;
  nlohmann::json response_data;
};

class itti_sbi_amf_status_change_subscribe_request : public itti_sbi_msg {
 public:
  itti_sbi_amf_status_change_subscribe_request(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_AMF_STATUS_CHANGE_SUBSCRIBE_REQUEST, orig, dest),
        subscription_data(),
        promise_id(pid) {}

  itti_sbi_amf_status_change_subscribe_request(
      const itti_sbi_amf_status_change_subscribe_request& i)
      : itti_sbi_msg(i) {
    promise_id        = i.promise_id;
    subscription_data = i.subscription_data;
  }
  virtual ~itti_sbi_amf_status_change_subscribe_request(){};

  uint32_t promise_id;
  oai::_3gpp::model::SubscriptionData subscription_data;
};

class itti_sbi_amf_status_change_unsubscribe_request : public itti_sbi_msg {
 public:
  itti_sbi_amf_status_change_unsubscribe_request(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_AMF_STATUS_CHANGE_UNSUBSCRIBE_REQUEST, orig, dest),
        subscription_id(),
        promise_id(pid) {}

  itti_sbi_amf_status_change_unsubscribe_request(
      const itti_sbi_amf_status_change_unsubscribe_request& i)
      : itti_sbi_msg(i) {
    promise_id      = i.promise_id;
    subscription_id = i.subscription_id;
  }
  virtual ~itti_sbi_amf_status_change_unsubscribe_request(){};

  uint32_t promise_id;
  std::string subscription_id;
};

class itti_sbi_amf_status_change_subscribe_modify : public itti_sbi_msg {
 public:
  itti_sbi_amf_status_change_subscribe_modify(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_AMF_STATUS_CHANGE_SUBSCRIBE_MODIFY, orig, dest),
        subscription_id(),
        subscription_data(),
        promise_id(pid) {}

  itti_sbi_amf_status_change_subscribe_modify(
      const itti_sbi_amf_status_change_subscribe_modify& i)
      : itti_sbi_msg(i) {
    promise_id        = i.promise_id;
    subscription_id   = i.subscription_id;
    subscription_data = i.subscription_data;
  }
  virtual ~itti_sbi_amf_status_change_subscribe_modify(){};

  uint32_t promise_id;
  std::string subscription_id;
  oai::_3gpp::model::SubscriptionData subscription_data;
};

class itti_sbi_amf_status_change_notification : public itti_sbi_msg {
 public:
  itti_sbi_amf_status_change_notification(
      const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_AMF_STATUS_CHANGE_NOTIFICATION, orig, dest),

        amf_status_change_notification(),
        notification_uris() {}

  itti_sbi_amf_status_change_notification(
      const itti_sbi_amf_status_change_notification& i)
      : itti_sbi_msg(i) {
    amf_status_change_notification = i.amf_status_change_notification;
    notification_uris              = i.notification_uris;
  }
  virtual ~itti_sbi_amf_status_change_notification(){};

  std::vector<std::string> notification_uris;
  oai::_3gpp::model::AmfStatusChangeNotification amf_status_change_notification;
};

class itti_sbi_provide_domain_selection_info : public itti_sbi_msg {
 public:
  itti_sbi_provide_domain_selection_info(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_PROVIDE_DOMAIN_SELECTION_INFO, orig, dest),
        ue_context_id(),
        ue_context_info_class(),
        promise_id(pid) {}

  itti_sbi_provide_domain_selection_info(
      const itti_sbi_provide_domain_selection_info& i)
      : itti_sbi_msg(i) {
    promise_id            = i.promise_id;
    ue_context_id         = i.ue_context_id;
    ue_context_info_class = i.ue_context_info_class;
  }
  virtual ~itti_sbi_provide_domain_selection_info(){};

  uint32_t promise_id;
  std::string ue_context_id;
  oai::_3gpp::model::UeContextInfoClass ue_context_info_class;
};

class itti_sbi_provide_location_info : public itti_sbi_msg {
 public:
  itti_sbi_provide_location_info(
      const task_id_t orig, const task_id_t dest, uint32_t pid)
      : itti_sbi_msg(SBI_PROVIDE_LOCATION_INFO, orig, dest),
        ue_context_id(),
        request_loc_info(),
        promise_id(pid) {}

  itti_sbi_provide_location_info(const itti_sbi_provide_location_info& i)
      : itti_sbi_msg(i) {
    promise_id       = i.promise_id;
    ue_context_id    = i.ue_context_id;
    request_loc_info = i.request_loc_info;
  }
  virtual ~itti_sbi_provide_location_info(){};

  uint32_t promise_id;
  std::string ue_context_id;
  oai::_3gpp::model::RequestLocInfo request_loc_info;
};
#endif /* ITTI_MSG_SBI_HPP_INCLUDED_ */
