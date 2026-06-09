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

// Free-function helpers declared here, defined in paging_controller.cpp
// to avoid ODR-bloat from static definitions in a shared header.
// (TS 23.502 §5.2.2.2.7 cause→HTTP map)
paging::admission_result make_dispatch_failure_result(
    const std::string& detail);
paging::admission_result make_no_paging_target_result();
paging::admission_result make_n2_forwarding_blocked_result(
    const std::string& detail);

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
