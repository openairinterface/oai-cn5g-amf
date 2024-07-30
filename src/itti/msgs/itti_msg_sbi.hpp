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

using namespace amf_application;

class itti_msg_n11 : public itti_msg {
 public:
  itti_msg_n11(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    ran_ue_ngap_id = 0;
    http_version   = 1;
  }
  itti_msg_n11(const itti_msg_n11& i) : itti_msg(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    http_version   = i.http_version;
  }
  virtual ~itti_msg_n11(){};

 public:
  uint8_t http_version;
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
      : itti_msg(msg_type, orig, dest) {
    http_version = 1;
  }
  itti_sbi_msg(const itti_sbi_msg& i) : itti_msg(i) {
    http_version = i.http_version;
  }
  itti_sbi_msg(
      const itti_sbi_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(i) {
    origin       = orig;
    destination  = dest;
    http_version = i.http_version;
  }
  virtual ~itti_sbi_msg() {}

 public:
  uint8_t http_version;
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
  uint8_t http_response_code;
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

  std::vector<oai::model::common::PatchItem> patch_items;
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
  oai::model::amf::SliceInfoForRegistration slice_info;
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
    http_version         = 0;
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
  oai::model::amf::N2InformationNotification n2_info_notification;
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

  oai::model::amf::N1MessageNotification notification_msg;
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
  oai::model::amf::UeN1N2InfoSubscriptionCreateData subscription_data;
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

  oai::model::amf::NonUeN2InfoSubscriptionCreateData subscription_data;
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
  oai::model::amf::SmContextStatusNotification smContextStatusNotification;
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
  oai::model::amf::AuthenticationInfo auth_info;
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

#endif /* ITTI_MSG_SBI_HPP_INCLUDED_ */
