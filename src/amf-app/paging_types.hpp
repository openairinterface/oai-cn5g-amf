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
  NOTIFICATION,
  NOTIFICATION_RESPONSE,
  NGAP_RESUME
};

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
  bool deferred_expiry_set = false;
  std::chrono::system_clock::time_point deferred_expiry_at =
      std::chrono::system_clock::time_point::min();
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
