/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_PAGING_CONTROLLER_H_
#define _AMF_PAGING_CONTROLLER_H_

#include <cstddef>
#include <memory>

#include "amf_config.hpp"

#include "NgapIesStruct.hpp"
#include "nas_context.hpp"
#include "paging_types.hpp"

extern std::unique_ptr<oai::config::amf_config> amf_cfg;

namespace amf_application {

static paging::admission_result make_dispatch_failure_result(
    const std::string& detail) {
  paging::admission_result result;
  result.decision         = paging::admission_decision::REJECT;
  result.http_status_code = 503;
  result.cause            = oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf::FAILURE_CAUSE_UNSPECIFIED;
  result.is_error       = true;
  result.problem_cause  = "SYSTEM_FAILURE";
  result.problem_detail = detail;
  return result;
}

static paging::admission_result make_no_paging_target_result() {
  paging::admission_result result;
  result.decision         = paging::admission_decision::REJECT;
  result.http_status_code = 504;
  result.cause            = oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf::AN_NOT_RESPONDING;
  result.is_error       = true;
  result.problem_cause  = "AN_NOT_RESPONDING";
  result.problem_detail = "No matching NG-RAN target was found for paging.";
  return result;
}

static paging::admission_result make_n2_forwarding_blocked_result(
    const std::string& detail) {
  paging::admission_result result;
  result.decision         = paging::admission_decision::REJECT;
  result.http_status_code = 409;
  result.cause            = oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf::N2_MSG_NOT_TRANSFERRED;
  result.is_error       = true;
  result.problem_cause  = "N2_MSG_NOT_TRANSFERRED";
  result.problem_detail = detail;
  return result;
}

static size_t get_paging_max_transactions_per_ue() {
  if (amf_cfg && amf_cfg->paging.max_transactions_per_ue > 0) {
    return amf_cfg->paging.max_transactions_per_ue;
  }
  return AMF_CONFIG_PAGING_MAX_TRANSACTIONS_PER_UE_DEFAULT_VALUE;
}

static uint32_t get_paging_registration_defer_timeout_sec() {
  if (amf_cfg && amf_cfg->paging.registration_defer_timeout_sec > 0) {
    return amf_cfg->paging.registration_defer_timeout_sec;
  }
  return AMF_CONFIG_PAGING_REGISTRATION_DEFER_TIMEOUT_SEC_DEFAULT_VALUE;
}

static uint32_t get_paging_temporary_unreachable_defer_timeout_sec() {
  if (amf_cfg && amf_cfg->paging.temporary_unreachable_defer_timeout_sec > 0) {
    return amf_cfg->paging.temporary_unreachable_defer_timeout_sec;
  }
  return AMF_CONFIG_PAGING_TEMPORARY_UNREACHABLE_DEFER_TIMEOUT_SEC_DEFAULT_VALUE;
}

class paging_controller {
 public:
  paging_controller(
      size_t max_transactions_per_ue, uint32_t registration_defer_timeout_sec,
      uint32_t temporary_unreachable_defer_timeout_sec);
  paging_controller();

  paging::admission_result admit_transfer(
      const std::shared_ptr<nas_context>& nc,
      paging::paging_transaction&& transaction) const;
  bool can_forward_n2_sm_over_3gpp_access(
      const paging::paging_transaction& transaction,
      const oai::ngap::Tai_t& current_tai,
      const std::optional<uint16_t>& allowed_pdu_session_status,
      std::string& rejection_reason) const;

 private:
  bool is_registration_in_progress(const nas_context& nc) const;
  bool enqueue_for_paging(
      const std::shared_ptr<nas_context>& nc,
      paging::paging_transaction&& transaction) const;
  bool enqueue_for_registration(
      const std::shared_ptr<nas_context>& nc,
      paging::paging_transaction&& transaction) const;
  bool enqueue_for_temporary_unreachable(
      const std::shared_ptr<nas_context>& nc,
      paging::paging_transaction&& transaction) const;
  paging::admission_result make_reject_result(
      uint32_t http_status_code,
      oai::_3gpp::model::N1N2MessageTransferCause_anyOf::
          eN1N2MessageTransferCause_anyOf cause,
      const std::string& problem_cause,
      const std::string& problem_detail) const;
  bool matches_area_of_validity(
      const oai::_3gpp::model::AreaOfValidity& area_of_validity,
      const oai::ngap::Tai_t& current_tai) const;
  bool is_pdu_session_allowed_on_3gpp_access(
      uint8_t pdu_session_id, uint16_t allowed_pdu_session_status) const;

  size_t max_transactions_per_ue_;
  uint32_t registration_defer_timeout_sec_;
  uint32_t temporary_unreachable_defer_timeout_sec_;
};

}  // namespace amf_application

#endif
