/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _AMF_PAGING_CONTROLLER_H_
#define _AMF_PAGING_CONTROLLER_H_

#include <cstddef>
#include <memory>

#include "NgapIesStruct.hpp"
#include "nas_context.hpp"
#include "paging_types.hpp"

namespace amf_application {

class paging_controller {
 public:
  explicit paging_controller(
      size_t max_transactions_per_ue, uint32_t registration_defer_timeout_sec,
      uint32_t temporary_unreachable_defer_timeout_sec);

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
