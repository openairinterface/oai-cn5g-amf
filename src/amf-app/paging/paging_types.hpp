/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_PAGING_TYPES_H_
#define _AMF_PAGING_TYPES_H_

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

#include "AccessType.h"
#include "Arp.h"
#include "AreaOfValidity.h"
#include "Guami.h"
#include "N1N2MessageTransferCause_anyOf.h"

namespace amf_application {
namespace paging {

enum class admission_decision {
  DIRECT_DELIVERY = 0,
  PAGING,
  DEFER_AWAITING_REGISTRATION,
  DEFER_TEMPORARY_UNREACHABLE,
  REJECT
};

enum class paging_response_class {
  NON_QUALIFYING = 0,
  SERVICE_REQUEST,
  CONTROL_PLANE_SERVICE_REQUEST,
  REGISTRATION_REQUEST,
  REGISTRATION_COMPLETE,
  NOTIFICATION,
  NOTIFICATION_RESPONSE,
  NGAP_RESUME
};

struct paging_response_gate {
  paging_response_class response_class = paging_response_class::NON_QUALIFYING;
  bool terminal_candidate              = false;
  bool requires_integrity_checked_nas  = false;
  bool allows_registration_security_success = false;
  bool allows_non_3gpp_allowed_status       = false;
  bool lower_layer_terminal                 = false;
  const char* name                          = "NON_QUALIFYING";
};

constexpr paging_response_gate gate_for_response(
    paging_response_class response_class) {
  switch (response_class) {
    case paging_response_class::SERVICE_REQUEST:
      return {response_class,   true, true, false, false, false,
              "SERVICE_REQUEST"};
    case paging_response_class::CONTROL_PLANE_SERVICE_REQUEST:
      return {
          response_class,
          true,
          true,
          false,
          false,
          false,
          "CONTROL_PLANE_SERVICE_REQUEST"};
    case paging_response_class::REGISTRATION_REQUEST:
      return {response_class,        false, false, true, true, false,
              "REGISTRATION_REQUEST"};
    case paging_response_class::REGISTRATION_COMPLETE:
      return {response_class,         true, true, false, false, false,
              "REGISTRATION_COMPLETE"};
    case paging_response_class::NGAP_RESUME:
      return {response_class, true, false, false, false, true, "NGAP_RESUME"};
    case paging_response_class::NOTIFICATION:
      return {response_class, false, false,         false,
              false,          false, "NOTIFICATION"};
    case paging_response_class::NOTIFICATION_RESPONSE:
      return {response_class,         false, false, false, false, false,
              "NOTIFICATION_RESPONSE"};
    case paging_response_class::NON_QUALIFYING:
    default:
      return {
          paging_response_class::NON_QUALIFYING,
          false,
          false,
          false,
          false,
          false,
          "NON_QUALIFYING"};
  }
}

enum class paging_outcome {
  DIRECT_DELIVERY = 0,
  PAGING_ACCEPTED,
  PAGING_SUCCESS,
  PAGING_TIMEOUT,
  DEFERRED_AWAITING_REGISTRATION,
  DEFERRED_AWAITING_REGISTRATION_EXPIRED,
  DEFERRED_TEMPORARY_UNREACHABLE,
  DEFERRED_TEMPORARY_UNREACHABLE_EXPIRED,
  REJECTED,
  NO_TARGET
};

struct paging_transaction {
  std::string supi;
  std::string n1sm_payload;
  std::string n2sm_payload;
  std::string nrppa_pdu_payload;
  std::string routing_id_payload;
  std::string n2sm_info_type;
  std::string failure_notification_uri;
  std::optional<bool> skip_ind;
  std::optional<bool> last_msg_indication;
  std::optional<std::string> lcs_correlation_id;
  uint8_t pdu_session_id = 0;
  bool has_n1sm          = false;
  bool has_n2sm          = false;
  bool has_nrppa_pdu     = false;
  std::optional<uint8_t> ppi;
  std::optional<oai::_3gpp::model::Arp> arp;
  std::optional<uint8_t> r5qi;
  std::optional<bool> smf_reallocation_ind;
  std::optional<oai::_3gpp::model::AreaOfValidity> area_of_validity;
  std::optional<std::string> supported_features;
  std::optional<oai::_3gpp::model::Guami> old_guami;
  std::optional<bool> ma_accepted_ind;
  std::optional<bool> ext_buf_support;
  std::optional<oai::_3gpp::model::AccessType> target_access;
  std::optional<std::string> nf_id;
  // Deferred_expiry_set removed — use deferred_expiry_at.has_value()
  // as the validity flag.  Previously both fields were written together, making
  // the bool redundant.
  std::optional<std::chrono::system_clock::time_point> deferred_expiry_at;
};

struct admission_result {
  admission_decision decision = admission_decision::REJECT;
  uint32_t http_status_code   = 400;
  oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf cause =
          oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
              eN1N2MessageTransferCause_anyOf::N1_MSG_NOT_TRANSFERRED;
  bool is_error       = true;
  bool trigger_paging = false;
  std::string problem_cause;
  std::string problem_detail;
  std::optional<int32_t> max_waiting_time;
};

}  // namespace paging
}  // namespace amf_application

#endif
