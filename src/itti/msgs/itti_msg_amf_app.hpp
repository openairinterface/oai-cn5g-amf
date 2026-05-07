/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_APP_ITTI_H_
#define _AMF_APP_ITTI_H_

#include <optional>
#include <string>

#include "amf.hpp"
#include "NgapIesStruct.hpp"
#include "N1N2MessageTransferCause_anyOf.h"
#include "itti_msg.hpp"
#include "../../common-src/model/GlobalRanNodeId.h"
#include "paging_types.hpp"
using namespace oai::ngap;
#include "bstrlib.h"

class itti_msg_amf_app : public itti_msg {
 public:
  itti_msg_amf_app(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {
    gnb_id         = 0;
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  }

  itti_msg_amf_app(const itti_msg_amf_app& i) : itti_msg(i) {
    gnb_id         = i.gnb_id;
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }

  uint32_t gnb_id;
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
};

class itti_nas_signalling_establishment_request : public itti_msg_amf_app {
 public:
  itti_nas_signalling_establishment_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(NAS_SIG_ESTAB_REQ, origin, destination) {
    rrc_cause            = 0;
    ue_ctx_req           = 0;
    cgi                  = {};
    tai                  = {};
    nas_buf              = nullptr;
    is_5g_s_tmsi_present = false;
    _5g_s_tmsi           = {};
  }
  itti_nas_signalling_establishment_request(
      const itti_nas_signalling_establishment_request& i)
      : itti_msg_amf_app(i) {
    rrc_cause            = i.rrc_cause;
    ue_ctx_req           = i.ue_ctx_req;
    cgi                  = i.cgi;
    tai                  = i.tai;
    nas_buf              = i.nas_buf;
    is_5g_s_tmsi_present = i.is_5g_s_tmsi_present;
    _5g_s_tmsi           = i._5g_s_tmsi;
  }

  int rrc_cause;
  int ue_ctx_req;
  NrCgi_t cgi;
  Tai_t tai;
  bstring nas_buf;
  bool is_5g_s_tmsi_present;
  std::string _5g_s_tmsi;
};

class itti_n1n2_message_transfer_request : public itti_msg_amf_app {
 public:
  itti_n1n2_message_transfer_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(N1N2_MESSAGE_TRANSFER_REQ, origin, destination) {
    supi                        = {};
    n1sm                        = nullptr;
    n2sm                        = nullptr;
    nrppa_pdu                   = nullptr;
    routing_id                  = nullptr;
    is_n2sm_set                 = false;
    is_n1sm_set                 = false;
    is_nrppa_pdu_set            = false;
    is_ppi_set                  = false;
    n1n2_failure_txf_notif_uri  = {};
    is_skip_ind_set             = false;
    skip_ind                    = false;
    is_last_msg_indication_set  = false;
    last_msg_indication         = false;
    lcs_correlation_id          = {};
    is_lcs_correlation_id_set   = false;
    is_arp_set                  = false;
    is_r5qi_set                 = false;
    r5qi                        = 0;
    is_smf_reallocation_ind_set = false;
    smf_reallocation_ind        = false;
    is_area_of_validity_set     = false;
    supported_features          = {};
    is_supported_features_set   = false;
    is_old_guami_set            = false;
    is_ma_accepted_ind_set      = false;
    ma_accepted_ind             = false;
    is_ext_buf_support_set      = false;
    ext_buf_support             = false;
    is_target_access_set        = false;
    nf_id                       = {};
    is_nf_id_set                = false;

    n2sm_info_type = {};
    pdu_session_id = 0;
    ppi            = 0;
  }
  itti_n1n2_message_transfer_request(
      const itti_n1n2_message_transfer_request& i)
      : itti_msg_amf_app(i) {
    supi                        = i.supi;
    n1sm                        = i.n1sm;
    n2sm                        = i.n2sm;
    nrppa_pdu                   = i.nrppa_pdu;
    routing_id                  = i.routing_id;
    is_n2sm_set                 = i.is_n2sm_set;
    is_n1sm_set                 = i.is_n1sm_set;
    is_nrppa_pdu_set            = i.is_nrppa_pdu_set;
    is_ppi_set                  = i.is_ppi_set;
    is_skip_ind_set             = i.is_skip_ind_set;
    skip_ind                    = i.skip_ind;
    is_last_msg_indication_set  = i.is_last_msg_indication_set;
    last_msg_indication         = i.last_msg_indication;
    lcs_correlation_id          = i.lcs_correlation_id;
    is_lcs_correlation_id_set   = i.is_lcs_correlation_id_set;
    arp                         = i.arp;
    is_arp_set                  = i.is_arp_set;
    is_r5qi_set                 = i.is_r5qi_set;
    r5qi                        = i.r5qi;
    is_smf_reallocation_ind_set = i.is_smf_reallocation_ind_set;
    smf_reallocation_ind        = i.smf_reallocation_ind;
    area_of_validity            = i.area_of_validity;
    is_area_of_validity_set     = i.is_area_of_validity_set;
    supported_features          = i.supported_features;
    is_supported_features_set   = i.is_supported_features_set;
    old_guami                   = i.old_guami;
    is_old_guami_set            = i.is_old_guami_set;
    is_ma_accepted_ind_set      = i.is_ma_accepted_ind_set;
    ma_accepted_ind             = i.ma_accepted_ind;
    is_ext_buf_support_set      = i.is_ext_buf_support_set;
    ext_buf_support             = i.ext_buf_support;
    target_access               = i.target_access;
    is_target_access_set        = i.is_target_access_set;
    nf_id                       = i.nf_id;
    is_nf_id_set                = i.is_nf_id_set;

    n2sm_info_type             = i.n2sm_info_type;
    pdu_session_id             = i.pdu_session_id;
    ppi                        = i.ppi;
    n1n2_failure_txf_notif_uri = i.n1n2_failure_txf_notif_uri;
  }

  std::string supi;
  bstring n1sm;
  bstring n2sm;
  bstring nrppa_pdu;
  bstring routing_id;
  bool is_n2sm_set;
  bool is_n1sm_set;
  bool is_nrppa_pdu_set;
  uint8_t pdu_session_id;
  std::string n2sm_info_type;
  bool is_ppi_set;
  uint8_t ppi;
  std::string n1n2_failure_txf_notif_uri;  // TS 29.518 §6.1.5.6 N1N2 Transfer
                                           // Failure Notification
  bool is_skip_ind_set;
  bool skip_ind;
  bool is_last_msg_indication_set;
  bool last_msg_indication;
  std::string lcs_correlation_id;
  bool is_lcs_correlation_id_set;
  oai::_3gpp::model::Arp arp;
  bool is_arp_set;
  bool is_r5qi_set;
  uint8_t r5qi;
  bool is_smf_reallocation_ind_set;
  bool smf_reallocation_ind;
  oai::_3gpp::model::AreaOfValidity area_of_validity;
  bool is_area_of_validity_set;
  std::string supported_features;
  bool is_supported_features_set;
  oai::_3gpp::model::Guami old_guami;
  bool is_old_guami_set;
  bool is_ma_accepted_ind_set;
  bool ma_accepted_ind;
  bool is_ext_buf_support_set;
  bool ext_buf_support;
  oai::_3gpp::model::AccessType target_access;
  bool is_target_access_set;
  std::string nf_id;
  bool is_nf_id_set;

  amf_application::paging::paging_transaction to_paging_transaction() const {
    amf_application::paging::paging_transaction tx = {};
    tx.supi                                        = supi;
    tx.n2sm_info_type                              = n2sm_info_type;
    tx.pdu_session_id                              = pdu_session_id;
    tx.failure_notification_uri                    = n1n2_failure_txf_notif_uri;
    tx.has_n1sm                                    = is_n1sm_set && n1sm;
    tx.has_n2sm                                    = is_n2sm_set && n2sm;
    tx.has_nrppa_pdu = is_nrppa_pdu_set && nrppa_pdu;
    if (is_skip_ind_set) tx.skip_ind = skip_ind;
    if (is_last_msg_indication_set)
      tx.last_msg_indication = last_msg_indication;
    if (is_lcs_correlation_id_set) tx.lcs_correlation_id = lcs_correlation_id;
    if (tx.has_n1sm) {
      tx.n1sm_payload.assign(
          reinterpret_cast<const char*>(bdata(n1sm)), blength(n1sm));
    }
    if (tx.has_n2sm) {
      tx.n2sm_payload.assign(
          reinterpret_cast<const char*>(bdata(n2sm)), blength(n2sm));
    }
    if (tx.has_nrppa_pdu) {
      tx.nrppa_pdu_payload.assign(
          reinterpret_cast<const char*>(bdata(nrppa_pdu)), blength(nrppa_pdu));
    }
    if (routing_id) {
      tx.routing_id_payload.assign(
          reinterpret_cast<const char*>(bdata(routing_id)),
          blength(routing_id));
    }
    if (is_ppi_set) {
      tx.ppi = ppi;
    }
    if (is_arp_set) tx.arp = arp;
    if (is_r5qi_set) tx.r5qi = r5qi;
    if (is_smf_reallocation_ind_set)
      tx.smf_reallocation_ind = smf_reallocation_ind;
    if (is_area_of_validity_set) tx.area_of_validity = area_of_validity;
    if (is_supported_features_set) tx.supported_features = supported_features;
    if (is_old_guami_set) tx.old_guami = old_guami;
    if (is_ma_accepted_ind_set) tx.ma_accepted_ind = ma_accepted_ind;
    if (is_ext_buf_support_set) tx.ext_buf_support = ext_buf_support;
    if (is_target_access_set) tx.target_access = target_access;
    if (is_nf_id_set) tx.nf_id = nf_id;
    return tx;
  }

  void from_paging_transaction(
      const amf_application::paging::paging_transaction& tx) {
    supi                       = tx.supi;
    n2sm_info_type             = tx.n2sm_info_type;
    pdu_session_id             = tx.pdu_session_id;
    is_n1sm_set                = tx.has_n1sm;
    is_n2sm_set                = tx.has_n2sm;
    is_nrppa_pdu_set           = tx.has_nrppa_pdu;
    is_ppi_set                 = tx.ppi.has_value();
    ppi                        = tx.ppi.value_or(0);
    n1n2_failure_txf_notif_uri = tx.failure_notification_uri;
    is_skip_ind_set            = tx.skip_ind.has_value();
    skip_ind                   = tx.skip_ind.value_or(false);
    is_last_msg_indication_set = tx.last_msg_indication.has_value();
    last_msg_indication        = tx.last_msg_indication.value_or(false);
    is_lcs_correlation_id_set  = tx.lcs_correlation_id.has_value();
    lcs_correlation_id         = tx.lcs_correlation_id.value_or("");
    is_arp_set                 = tx.arp.has_value();
    if (is_arp_set) arp = tx.arp.value();
    is_r5qi_set                 = tx.r5qi.has_value();
    r5qi                        = tx.r5qi.value_or(0);
    is_smf_reallocation_ind_set = tx.smf_reallocation_ind.has_value();
    smf_reallocation_ind        = tx.smf_reallocation_ind.value_or(false);
    is_area_of_validity_set     = tx.area_of_validity.has_value();
    if (is_area_of_validity_set) area_of_validity = tx.area_of_validity.value();
    is_supported_features_set = tx.supported_features.has_value();
    supported_features        = tx.supported_features.value_or("");
    is_old_guami_set          = tx.old_guami.has_value();
    if (is_old_guami_set) old_guami = tx.old_guami.value();
    is_ma_accepted_ind_set = tx.ma_accepted_ind.has_value();
    ma_accepted_ind        = tx.ma_accepted_ind.value_or(false);
    is_ext_buf_support_set = tx.ext_buf_support.has_value();
    ext_buf_support        = tx.ext_buf_support.value_or(false);
    is_target_access_set   = tx.target_access.has_value();
    if (is_target_access_set) target_access = tx.target_access.value();
    is_nf_id_set = tx.nf_id.has_value();
    nf_id        = tx.nf_id.value_or("");
    if (n1sm) bdestroy(n1sm);
    if (n2sm) bdestroy(n2sm);
    if (nrppa_pdu) bdestroy(nrppa_pdu);
    if (routing_id) bdestroy(routing_id);
    n1sm = tx.has_n1sm ?
               blk2bstr(tx.n1sm_payload.data(), tx.n1sm_payload.size()) :
               nullptr;
    n2sm = tx.has_n2sm ?
               blk2bstr(tx.n2sm_payload.data(), tx.n2sm_payload.size()) :
               nullptr;
    nrppa_pdu =
        tx.has_nrppa_pdu ?
            blk2bstr(tx.nrppa_pdu_payload.data(), tx.nrppa_pdu_payload.size()) :
            nullptr;
    routing_id =
        tx.routing_id_payload.empty() ?
            nullptr :
            blk2bstr(
                tx.routing_id_payload.data(), tx.routing_id_payload.size());
  }
};

// TS 23.502 §5.2.2.2.7A / TS 29.518 §6.1.5.6
// Carries the information needed for the Namf_Communication
// N1N2TransferFailureNotification callback.  Owner: TASK_AMF_SBI
// (uses http_client::send_http_request — non-blocking from caller's
//  perspective because it runs inside the SBI task thread).
class itti_n1n2_transfer_failure_notification : public itti_msg_amf_app {
 public:
  itti_n1n2_transfer_failure_notification(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(
            N1N2_TRANSFER_FAILURE_NOTIFICATION, origin, destination) {}

  itti_n1n2_transfer_failure_notification(
      const itti_n1n2_transfer_failure_notification& i)
      : itti_msg_amf_app(i) {
    supi                  = i.supi;
    failure_txf_notif_uri = i.failure_txf_notif_uri;
    cause                 = i.cause;
    max_waiting_time      = i.max_waiting_time;
    pdu_session_id        = i.pdu_session_id;
    ng_ap_cause           = i.ng_ap_cause;
  }

  std::string supi;

  // Mandatory: the n1n2FailureTxfNotifURI provided by the Trigger NF
  std::string failure_txf_notif_uri;

  // Mandatory: notification cause per TS 29.518 §6.1.5.6 Table 6.1.5.6-1
  oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf cause =
          oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
              eN1N2MessageTransferCause_anyOf::INVALID_VALUE_OPENAPI_GENERATED;

  // Optional: deferred-queue expiry in seconds
  std::optional<int32_t> max_waiting_time;

  // Optional: echoed back PDU session ID (zero = not set)
  uint8_t pdu_session_id = 0;

  // Optional
  std::optional<std::string> ng_ap_cause;
};

class itti_non_ue_n2_message_transfer_request : public itti_msg_amf_app {
 public:
  itti_non_ue_n2_message_transfer_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(NON_UE_N2_MESSAGE_TRANSFER_REQ, origin, destination) {
    nrppa_pdu        = nullptr;
    routing_id       = nullptr;
    is_nrppa_pdu_set = false;
  }
  itti_non_ue_n2_message_transfer_request(
      const itti_non_ue_n2_message_transfer_request& i)
      : itti_msg_amf_app(i) {
    nrppa_pdu        = i.nrppa_pdu;
    routing_id       = i.routing_id;
    is_nrppa_pdu_set = i.is_nrppa_pdu_set;
  }

  bstring nrppa_pdu;
  bstring routing_id;
  bool is_nrppa_pdu_set;
  std::vector<oai::_3gpp::model::GlobalRanNodeId> global_ran_node_list;
};

#endif
