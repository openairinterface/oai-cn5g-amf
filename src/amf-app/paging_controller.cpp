/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "paging_controller.hpp"

#include <bitset>
#include <chrono>
#include <mutex>
#include <utility>

#include "amf_statistics.hpp"
#include "logger.hpp"

using namespace oai::_3gpp::model;

namespace amf_application {

extern statistics stacs;

// ---------------------------------------------------------------------------
// B4 — config accessors (moved from header to avoid ODR-bloat)
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// B4 — free-function result helpers (moved from header; TS 23.502 §5.2.2.2.7)
// ---------------------------------------------------------------------------
paging::admission_result make_dispatch_failure_result(
    const std::string& detail) {
  paging::admission_result result;
  result.decision         = paging::admission_decision::REJECT;
  result.http_status_code = 503;
  result.cause            = N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf::FAILURE_CAUSE_UNSPECIFIED;
  result.is_error       = true;
  result.problem_cause  = "SYSTEM_FAILURE";
  result.problem_detail = detail;
  return result;
}

paging::admission_result make_no_paging_target_result() {
  paging::admission_result result;
  result.decision         = paging::admission_decision::REJECT;
  result.http_status_code = 504;
  result.cause            = N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf::AN_NOT_RESPONDING;
  result.is_error       = true;
  result.problem_cause  = "AN_NOT_RESPONDING";
  result.problem_detail = "No matching NG-RAN target was found for paging.";
  return result;
}

paging::admission_result make_n2_forwarding_blocked_result(
    const std::string& detail) {
  paging::admission_result result;
  result.decision         = paging::admission_decision::REJECT;
  result.http_status_code = 409;
  result.cause            = N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf::N2_MSG_NOT_TRANSFERRED;
  result.is_error       = true;
  result.problem_cause  = "N2_MSG_NOT_TRANSFERRED";
  result.problem_detail = detail;
  return result;
}

paging_controller::paging_controller(
    size_t max_transactions_per_ue, uint32_t registration_defer_timeout_sec,
    uint32_t temporary_unreachable_defer_timeout_sec)
    : max_transactions_per_ue_(max_transactions_per_ue),
      registration_defer_timeout_sec_(registration_defer_timeout_sec),
      temporary_unreachable_defer_timeout_sec_(
          temporary_unreachable_defer_timeout_sec) {}

paging_controller::paging_controller() {
  max_transactions_per_ue_        = get_paging_max_transactions_per_ue();
  registration_defer_timeout_sec_ = get_paging_registration_defer_timeout_sec();
  temporary_unreachable_defer_timeout_sec_ =
      get_paging_temporary_unreachable_defer_timeout_sec();
}

paging::admission_result paging_controller::admit_transfer(
    const std::shared_ptr<nas_context>& nc,
    paging::paging_transaction&& transaction) const {
  // B4 — cause→HTTP map per TS 23.502 §5.2.2.2.7 plan table.
  if (!nc) {
    // REJECT (no UE): 404 / CONTEXT_NOT_FOUND
    return make_reject_result(
        404,
        N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf::
            UE_NOT_REACHABLE_FOR_SESSION,
        "CONTEXT_NOT_FOUND",
        "No NAS context is available for the requested UE.");
  }

  if (nc->_5gmm_state != _5GMM_REGISTERED) {
    if (!is_registration_in_progress(*nc)) {
      // REJECT (no UE / not registered and no reg in progress): 404
      return make_reject_result(
          404,
          N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf::
              UE_NOT_REACHABLE_FOR_SESSION,
          "CONTEXT_NOT_FOUND",
          "UE context exists but registration is not in progress.");
    }

    // Track G3: set optional directly (deferred_expiry_set removed).
    transaction.deferred_expiry_at =
        std::chrono::system_clock::now() +
        std::chrono::seconds(registration_defer_timeout_sec_);
    {
      // Track G1: protect awaiting_registration_messages push.
      std::lock_guard<std::mutex> lk(nc->paging_queues_mutex);
      if (!enqueue_for_registration(nc, std::move(transaction))) {
        // Queue full — 503 / SYSTEM_FAILURE per plan "REJECT (other)" row
        return make_reject_result(
            503,
            N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf::
                FAILURE_CAUSE_UNSPECIFIED,
            "SYSTEM_FAILURE",
            "Awaiting-registration transfer queue is full for this UE.");
      }
    }  // release paging_queues_mutex

    // DEFER_AWAITING_REGISTRATION: 202 / TEMPORARY_REJECT_REGISTRATION_ONGOING
    paging::admission_result result;
    result.decision = paging::admission_decision::DEFER_AWAITING_REGISTRATION;
    result.http_status_code = 202;
    result.cause            = N1N2MessageTransferCause_anyOf::
        eN1N2MessageTransferCause_anyOf::TEMPORARY_REJECT_REGISTRATION_ONGOING;
    result.is_error = false;
    result.max_waiting_time =
        static_cast<int32_t>(registration_defer_timeout_sec_);
    return result;
  }

  if (nc->nas_status == CM_CONNECTED) {
    // DIRECT_DELIVERY: 200 / N1_N2_TRANSFER_INITIATED
    paging::admission_result result;
    result.decision         = paging::admission_decision::DIRECT_DELIVERY;
    result.http_status_code = 200;
    result.cause            = N1N2MessageTransferCause_anyOf::
        eN1N2MessageTransferCause_anyOf::N1_N2_TRANSFER_INITIATED;
    result.is_error = false;
    return result;
  }

  if (!nc->ppf_3gpp || nc->is_mobile_reachable_timer_timeout) {
    // REJECT (PPF=false): 403 / UE_NOT_REACHABLE
    return make_reject_result(
        403,
        N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf::
            UE_NOT_REACHABLE_FOR_SESSION,
        "UE_NOT_REACHABLE",
        "UE is not currently reachable for paging (PPF=false).");
  }

  if (nc->is_mico_mode) {
    if (transaction.ext_buf_support.value_or(false)) {
      // Track G3: set optional directly (deferred_expiry_set removed).
      transaction.deferred_expiry_at =
          std::chrono::system_clock::now() +
          std::chrono::seconds(temporary_unreachable_defer_timeout_sec_);
      {
        // Track G1: protect temporarily_unreachable_messages push.
        std::lock_guard<std::mutex> lk(nc->paging_queues_mutex);
        if (!enqueue_for_temporary_unreachable(nc, std::move(transaction))) {
          // Queue full — 503 / SYSTEM_FAILURE
          return make_reject_result(
              503,
              N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf::
                  FAILURE_CAUSE_UNSPECIFIED,
              "SYSTEM_FAILURE",
              "Temporary-unreachable transfer queue is full for this UE.");
        }
      }  // release paging_queues_mutex

      // DEFER_TEMP_UNREACHABLE: 202 / WAITING_FOR_ASYNCHRONOUS_TRANSFER
      paging::admission_result result;
      result.decision = paging::admission_decision::DEFER_TEMPORARY_UNREACHABLE;
      result.http_status_code = 202;
      result.cause            = N1N2MessageTransferCause_anyOf::
          eN1N2MessageTransferCause_anyOf::WAITING_FOR_ASYNCHRONOUS_TRANSFER;
      result.is_error = false;
      result.max_waiting_time =
          static_cast<int32_t>(temporary_unreachable_defer_timeout_sec_);
      return result;
    }

    // MICO mode, no ext_buf_support — not in the table; treat as
    // N2_MSG_NOT_TRANSFERRED (paging restriction) per the closest row: REJECT
    // (N2 SM blocked) 409.
    return make_reject_result(
        409,
        N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf::
            REJECTION_DUE_TO_PAGING_RESTRICTION,
        "N2_MSG_NOT_TRANSFERRED",
        "UE is subject to MICO-mode paging restriction and extended buffering "
        "is not supported.");
  }

  paging::admission_result result;
  result.decision         = paging::admission_decision::PAGING;
  result.http_status_code = 202;
  result.cause            = N1N2MessageTransferCause_anyOf::
      eN1N2MessageTransferCause_anyOf::ATTEMPTING_TO_REACH_UE;
  result.is_error = false;

  {
    // Track G1: protect pending_paging_messages push + subsequent .back()
    // read as a single critical section.  The .back() read and the priority
    // update must be atomic with the push to avoid a drain on TASK_AMF_N1
    // popping the entry between push and read.
    std::lock_guard<std::mutex> lk(nc->paging_queues_mutex);

    if (!enqueue_for_paging(nc, std::move(transaction))) {
      // Queue full — 503 / SYSTEM_FAILURE
      return make_reject_result(
          503,
          N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf::
              FAILURE_CAUSE_UNSPECIFIED,
          "SYSTEM_FAILURE", "Paging transfer queue is full for this UE.");
    }

    if (nc->is_paging_ongoing) {
      if (!nc->pending_paging_messages.empty()) {
        const auto& queued = nc->pending_paging_messages.back();
        if (queued.ppi.has_value() &&
            (!nc->paging_priority_present ||
             queued.ppi.value() < nc->paging_effective_ppi)) {
          nc->paging_effective_ppi    = queued.ppi.value();
          nc->paging_priority_present = true;
          // Carry the ARP alongside PPI for threshold gating (TS 23.502
          // §4.2.3.3 NOTE 4)
          nc->paging_effective_arp = queued.arp;
          result.trigger_paging    = true;
        }
      }
      return result;
    }

    nc->is_paging_ongoing       = true;
    nc->paging_completed        = false;
    nc->paging_priority_present = false;
    nc->paging_effective_ppi    = 0;
    nc->paging_effective_arp    = std::nullopt;
    if (!nc->pending_paging_messages.empty()) {
      const auto& queued = nc->pending_paging_messages.back();
      if (queued.ppi.has_value()) {
        nc->paging_priority_present = true;
        nc->paging_effective_ppi    = queued.ppi.value();
      }
      // Carry the ARP alongside PPI for threshold gating (TS 23.502 §4.2.3.3
      // NOTE 4)
      nc->paging_effective_arp = queued.arp;
    }
  }  // release paging_queues_mutex

  result.trigger_paging = true;
  return result;
}

bool paging_controller::is_registration_in_progress(
    const nas_context& nc) const {
  switch (nc.procedure_ctx.specific_procedure) {
    case nas_procedure_type_e::REGISTRATION_INITIAL:
    case nas_procedure_type_e::REGISTRATION_MOBILITY:
    case nas_procedure_type_e::REGISTRATION_PERIODIC:
      return true;
    default:
      break;
  }

  return nc._5gmm_state == _5GMM_COMMON_PROCEDURE_INITIATED;
}

bool paging_controller::enqueue_for_paging(
    const std::shared_ptr<nas_context>& nc,
    paging::paging_transaction&& transaction) const {
  // NOTE: caller (admit_transfer) holds nc->paging_queues_mutex for the
  // entire enqueue + subsequent .back() read block (Track G1).
  if (nc->pending_paging_messages.size() >= max_transactions_per_ue_) {
    Logger::amf_app().warn(
        "Pending paging queue full for UE %s - rejecting new transfer",
        transaction.supi.c_str());
    stacs.increment_paging_queue_full();
    return false;
  }

  nc->pending_paging_messages.push_back(std::move(transaction));
  return true;
}

bool paging_controller::enqueue_for_registration(
    const std::shared_ptr<nas_context>& nc,
    paging::paging_transaction&& transaction) const {
  // NOTE: caller (admit_transfer) holds nc->paging_queues_mutex (Track G1).
  if (nc->awaiting_registration_messages.size() >= max_transactions_per_ue_) {
    Logger::amf_app().warn(
        "Awaiting-registration queue full for UE %s - rejecting new transfer",
        transaction.supi.c_str());
    stacs.increment_awaiting_registration_queue_full();
    return false;
  }

  nc->awaiting_registration_messages.push_back(std::move(transaction));
  return true;
}

bool paging_controller::enqueue_for_temporary_unreachable(
    const std::shared_ptr<nas_context>& nc,
    paging::paging_transaction&& transaction) const {
  // NOTE: caller (admit_transfer) holds nc->paging_queues_mutex (Track G1).
  if (nc->temporarily_unreachable_messages.size() >= max_transactions_per_ue_) {
    Logger::amf_app().warn(
        "Temporary-unreachable queue full for UE %s - rejecting new transfer",
        transaction.supi.c_str());
    stacs.increment_temporary_unreachable_queue_full();
    return false;
  }

  nc->temporarily_unreachable_messages.push_back(std::move(transaction));
  return true;
}

bool paging_controller::can_forward_n2_sm_over_3gpp_access(
    const paging::paging_transaction& transaction,
    const oai::ngap::Tai_t& current_tai,
    const std::optional<uint16_t>& allowed_pdu_session_status,
    std::string& rejection_reason) const {
  if (!transaction.has_n2sm) {
    return true;
  }

  if (transaction.target_access.has_value() &&
      transaction.target_access.value().getValue() ==
          AccessType::eAccessType::NON_3GPP_ACCESS) {
    if (!allowed_pdu_session_status.has_value() ||
        !is_pdu_session_allowed_on_3gpp_access(
            transaction.pdu_session_id, allowed_pdu_session_status.value())) {
      rejection_reason =
          "Requested N2 SM targets a non-3GPP-associated PDU session that is "
          "not allowed on the current 3GPP access.";
      return false;
    }
  }

  if (transaction.area_of_validity.has_value() &&
      !matches_area_of_validity(
          transaction.area_of_validity.value(), current_tai)) {
    rejection_reason =
        "UE current TAI is outside the areaOfValidity for the N2 SM "
        "forwarding request.";
    return false;
  }

  rejection_reason.clear();
  return true;
}

paging::admission_result paging_controller::make_reject_result(
    uint32_t http_status_code,
    N1N2MessageTransferCause_anyOf::eN1N2MessageTransferCause_anyOf cause,
    const std::string& problem_cause, const std::string& problem_detail) const {
  paging::admission_result result;
  result.decision         = paging::admission_decision::REJECT;
  result.http_status_code = http_status_code;
  result.cause            = cause;
  result.is_error         = true;
  result.problem_cause    = problem_cause;
  result.problem_detail   = problem_detail;
  return result;
}

bool paging_controller::matches_area_of_validity(
    const AreaOfValidity& area_of_validity,
    const oai::ngap::Tai_t& current_tai) const {
  for (const auto& tai : area_of_validity.getTaiList()) {
    const auto plmn = tai.getPlmnId();
    try {
      const auto tac =
          static_cast<uint32_t>(std::stoul(tai.getTac(), nullptr, 16));
      if (plmn.getMcc() == current_tai.mcc &&
          plmn.getMnc() == current_tai.mnc && tac == current_tai.tac) {
        return true;
      }
    } catch (const std::exception& e) {
      Logger::amf_app().warn(
          "Ignoring invalid areaOfValidity TAC %s for UE TAI matching: %s",
          tai.getTac().c_str(), e.what());
    }
  }

  return area_of_validity.getTaiList().empty();
}

bool paging_controller::is_pdu_session_allowed_on_3gpp_access(
    uint8_t pdu_session_id, uint16_t allowed_pdu_session_status) const {
  if (pdu_session_id == 0 || pdu_session_id > 15) {
    return false;
  }

  std::bitset<16> status_bits(allowed_pdu_session_status);
  if (pdu_session_id <= 7) {
    return status_bits.test(pdu_session_id + 8);
  }

  return status_bits.test(pdu_session_id - 8);
}

}  // namespace amf_application
