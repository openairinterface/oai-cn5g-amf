/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nas_state_machine.hpp"

#include <string>

namespace oai::amf::nas {

// Timer array indices — must match nas_timer_type_e order when that enum
// exists.
static constexpr size_t kT3550_idx = 0;
static constexpr size_t kT3560_idx = 1;
static constexpr size_t kT3570_idx = 2;
// kT3522_idx = 3, kT3555_idx = 4, kT3513_idx = 5, kT3565_idx = 6

// ============================================================
// Guard functions
// ============================================================

// Guard: T3550 is currently running (Registration Accept was sent with T3550)
static bool guard_t3550_running(const nas_context& nc) {
  return nc.nas_timers[kT3550_idx].is_running;
}

// Guard: prior state (before entering CPI) was REGISTERED
static bool guard_prior_state_registered(const nas_context& nc) {
  return nc.procedure_ctx.prior_state == _5GMM_REGISTERED;
}

// Guard: Registration Request is an initial registration
static bool guard_initial_registration(const nas_context& nc) {
  return nc.procedure_ctx.specific_procedure ==
         nas_procedure_type_e::REGISTRATION_INITIAL;
}

// ============================================================
// nas_event_to_string
// ============================================================
const char* nas_event_to_string(nas_event_e event) {
  switch (event) {
    case nas_event_e::REGISTRATION_REQUEST_RECEIVED:
      return "REGISTRATION_REQUEST_RECEIVED";
    case nas_event_e::REGISTRATION_ACCEPT_SENT_NO_T3550:
      return "REGISTRATION_ACCEPT_SENT_NO_T3550";
    case nas_event_e::REGISTRATION_ACCEPT_SENT_WITH_T3550:
      return "REGISTRATION_ACCEPT_SENT_WITH_T3550";
    case nas_event_e::REGISTRATION_COMPLETE_RECEIVED:
      return "REGISTRATION_COMPLETE_RECEIVED";
    case nas_event_e::REGISTRATION_REJECT_SENT:
      return "REGISTRATION_REJECT_SENT";
    case nas_event_e::UE_DEREGISTRATION_REQUEST_RECEIVED:
      return "UE_DEREGISTRATION_REQUEST_RECEIVED";
    case nas_event_e::NW_DEREGISTRATION_INITIATED:
      return "NW_DEREGISTRATION_INITIATED";
    case nas_event_e::DEREGISTRATION_ACCEPT_RECEIVED:
      return "DEREGISTRATION_ACCEPT_RECEIVED";
    case nas_event_e::AUTHENTICATION_REQUEST_SENT:
      return "AUTHENTICATION_REQUEST_SENT";
    case nas_event_e::AUTHENTICATION_RESPONSE_RECEIVED:
      return "AUTHENTICATION_RESPONSE_RECEIVED";
    case nas_event_e::AUTHENTICATION_FAILURE_RECEIVED:
      return "AUTHENTICATION_FAILURE_RECEIVED";
    case nas_event_e::AUTHENTICATION_REJECT_SENT:
      return "AUTHENTICATION_REJECT_SENT";
    case nas_event_e::SECURITY_MODE_COMMAND_SENT:
      return "SECURITY_MODE_COMMAND_SENT";
    case nas_event_e::SECURITY_MODE_COMPLETE_RECEIVED:
      return "SECURITY_MODE_COMPLETE_RECEIVED";
    case nas_event_e::SECURITY_MODE_REJECT_RECEIVED:
      return "SECURITY_MODE_REJECT_RECEIVED";
    case nas_event_e::IDENTIFICATION_REQUEST_SENT:
      return "IDENTIFICATION_REQUEST_SENT";
    case nas_event_e::IDENTIFICATION_RESPONSE_RECEIVED:
      return "IDENTIFICATION_RESPONSE_RECEIVED";
    case nas_event_e::SERVICE_REQUEST_RECEIVED:
      return "SERVICE_REQUEST_RECEIVED";
    case nas_event_e::SERVICE_ACCEPT_SENT:
      return "SERVICE_ACCEPT_SENT";
    case nas_event_e::SERVICE_REJECT_SENT:
      return "SERVICE_REJECT_SENT";
    case nas_event_e::T3550_FINAL_EXPIRY:
      return "T3550_FINAL_EXPIRY";
    case nas_event_e::T3560_FINAL_EXPIRY_AUTH:
      return "T3560_FINAL_EXPIRY_AUTH";
    case nas_event_e::T3560_FINAL_EXPIRY_SMC:
      return "T3560_FINAL_EXPIRY_SMC";
    case nas_event_e::T3570_FINAL_EXPIRY:
      return "T3570_FINAL_EXPIRY";
    case nas_event_e::T3522_FINAL_EXPIRY:
      return "T3522_FINAL_EXPIRY";
    case nas_event_e::T3555_FINAL_EXPIRY:
      return "T3555_FINAL_EXPIRY";
    case nas_event_e::LOWER_LAYER_FAILURE:
      return "LOWER_LAYER_FAILURE";
    case nas_event_e::IMPLICIT_DEREGISTRATION:
      return "IMPLICIT_DEREGISTRATION";
    default:
      return "UNKNOWN_EVENT";
  }
}

// ============================================================
// nas_state_machine constructor
// ============================================================
nas_state_machine::nas_state_machine() {
  build_transition_table();
}

// ============================================================
// build_transition_table — full 3GPP TS 24.501 v16.14.0 transition table
// ============================================================
void nas_state_machine::build_transition_table() {
  using E = nas_event_e;

  transition_table_ = {
      // ======================================================
      // FROM: DEREGISTERED (§5.1.3.2.3.2)
      // ======================================================
      {_5GMM_DEREGISTERED, E::REGISTRATION_REQUEST_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.5.1.2.2"},
      {_5GMM_DEREGISTERED, E::AUTHENTICATION_REQUEST_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.1.3.2"},
      {_5GMM_DEREGISTERED, E::SECURITY_MODE_COMMAND_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.2.2"},
      {_5GMM_DEREGISTERED, E::IDENTIFICATION_REQUEST_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.3.2"},
      {_5GMM_DEREGISTERED, E::REGISTRATION_ACCEPT_SENT_NO_T3550,
       _5GMM_REGISTERED, nullptr, "§5.5.1.2.4"},

      // ======================================================
      // FROM: COMMON-PROCEDURE-INITIATED (§5.1.3.2.3.3)
      // ======================================================
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::AUTHENTICATION_REQUEST_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.1.3.2"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::AUTHENTICATION_RESPONSE_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.1.3.3"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::AUTHENTICATION_FAILURE_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.1.3.6"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::SECURITY_MODE_COMMAND_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.2.2"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::SECURITY_MODE_COMPLETE_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.2.4"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::IDENTIFICATION_REQUEST_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.3.2"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::IDENTIFICATION_RESPONSE_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.3.4"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::REGISTRATION_REQUEST_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.3.6c/d"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::REGISTRATION_ACCEPT_SENT_NO_T3550,
       _5GMM_REGISTERED, nullptr, "§5.5.1.2.4"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::REGISTRATION_ACCEPT_SENT_WITH_T3550,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.5.1.2.4"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::REGISTRATION_COMPLETE_RECEIVED,
       _5GMM_REGISTERED, nullptr, "§5.5.1.2.4"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::T3550_FINAL_EXPIRY,
       _5GMM_REGISTERED, nullptr, "§5.5.1.2.8c"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::AUTHENTICATION_REJECT_SENT,
       _5GMM_DEREGISTERED, nullptr, "§5.4.1.3.5"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::REGISTRATION_REJECT_SENT,
       _5GMM_DEREGISTERED, nullptr, "§5.5.1.2.5"},
      // T3560 auth 5th expiry: prior_state determines target (B-2 fix)
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::T3560_FINAL_EXPIRY_AUTH,
       _5GMM_REGISTERED, guard_prior_state_registered,
       "§5.4.1.3.7b (prior=REGISTERED)"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::T3560_FINAL_EXPIRY_AUTH,
       _5GMM_DEREGISTERED, nullptr, "§5.4.1.3.7b (prior=DEREGISTERED)"},
      // T3570 5th expiry: prior_state determines target (B-2 fix)
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::T3570_FINAL_EXPIRY,
       _5GMM_REGISTERED, guard_prior_state_registered,
       "§5.4.3.6b (prior=REGISTERED)"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::T3570_FINAL_EXPIRY,
       _5GMM_DEREGISTERED, nullptr, "§5.4.3.6b (prior=DEREGISTERED)"},
      // Lower layer failure (B-1 fix: 3 guarded entries, first match wins)
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::LOWER_LAYER_FAILURE,
       _5GMM_REGISTERED, guard_t3550_running, "§5.5.1.2.8a (T3550 running)"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::LOWER_LAYER_FAILURE,
       _5GMM_REGISTERED, guard_prior_state_registered,
       "§5.4.1.3.7a (prior=REGISTERED)"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::LOWER_LAYER_FAILURE,
       _5GMM_DEREGISTERED, nullptr, "§5.4.1.3.7a (prior=DEREGISTERED)"},
      // SMC 5th expiry: abort SMC only, stay in CPI (§5.4.2.7b)
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::T3560_FINAL_EXPIRY_SMC,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.2.7b"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::UE_DEREGISTRATION_REQUEST_RECEIVED,
       _5GMM_DEREGISTERED, nullptr, "§5.5.2.2.2"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::SERVICE_REJECT_SENT,
       _5GMM_DEREGISTERED, nullptr, "§5.6.1"},
      {_5GMM_COMMON_PROCEDURE_INITIATED, E::IMPLICIT_DEREGISTRATION,
       _5GMM_DEREGISTERED, nullptr, "§5.3.7"},

      // ======================================================
      // FROM: REGISTERED (§5.1.3.2.3.4)
      // ======================================================
      {_5GMM_REGISTERED, E::REGISTRATION_REQUEST_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.5.1.3.2"},
      {_5GMM_REGISTERED, E::AUTHENTICATION_REQUEST_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.1.3.2"},
      {_5GMM_REGISTERED, E::SECURITY_MODE_COMMAND_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.2.2"},
      {_5GMM_REGISTERED, E::IDENTIFICATION_REQUEST_SENT,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.4.3.2"},
      {_5GMM_REGISTERED, E::REGISTRATION_ACCEPT_SENT_WITH_T3550,
       _5GMM_COMMON_PROCEDURE_INITIATED, nullptr, "§5.5.1.2.4"},
      {_5GMM_REGISTERED, E::REGISTRATION_COMPLETE_RECEIVED, _5GMM_REGISTERED,
       nullptr, "§5.5.1.2.4"},
      {_5GMM_REGISTERED, E::UE_DEREGISTRATION_REQUEST_RECEIVED,
       _5GMM_DEREGISTERED, nullptr, "§5.5.2.2.3"},
      {_5GMM_REGISTERED, E::NW_DEREGISTRATION_INITIATED,
       _5GMM_DEREGISTERED_INITIATED, nullptr, "§5.5.2.3.1"},
      {_5GMM_REGISTERED, E::SERVICE_REQUEST_RECEIVED, _5GMM_REGISTERED, nullptr,
       "§5.6.1"},
      {_5GMM_REGISTERED, E::SERVICE_ACCEPT_SENT, _5GMM_REGISTERED, nullptr,
       "§5.6.1"},
      {_5GMM_REGISTERED, E::SERVICE_REJECT_SENT, _5GMM_DEREGISTERED, nullptr,
       "§5.6.1"},
      {_5GMM_REGISTERED, E::T3555_FINAL_EXPIRY, _5GMM_REGISTERED, nullptr,
       "§5.4.4.6a"},
      {_5GMM_REGISTERED, E::IMPLICIT_DEREGISTRATION, _5GMM_DEREGISTERED,
       nullptr, "§5.3.7"},

      // ======================================================
      // FROM: DEREGISTERED-INITIATED (§5.1.3.2.3.5)
      // ======================================================
      {_5GMM_DEREGISTERED_INITIATED, E::DEREGISTRATION_ACCEPT_RECEIVED,
       _5GMM_DEREGISTERED, nullptr, "§5.5.2.3.3"},
      {_5GMM_DEREGISTERED_INITIATED, E::T3522_FINAL_EXPIRY, _5GMM_DEREGISTERED,
       nullptr, "§5.5.2.3.5a"},
      {_5GMM_DEREGISTERED_INITIATED, E::LOWER_LAYER_FAILURE, _5GMM_DEREGISTERED,
       nullptr, "§5.5.2.3.5b"},
      {_5GMM_DEREGISTERED_INITIATED, E::UE_DEREGISTRATION_REQUEST_RECEIVED,
       _5GMM_DEREGISTERED, nullptr, "§5.5.2.3.5c"},
      {_5GMM_DEREGISTERED_INITIATED, E::REGISTRATION_REQUEST_RECEIVED,
       _5GMM_COMMON_PROCEDURE_INITIATED, guard_initial_registration,
       "§5.5.2.3.5d"},
      {_5GMM_DEREGISTERED_INITIATED, E::IMPLICIT_DEREGISTRATION,
       _5GMM_DEREGISTERED, nullptr, "§5.3.7"},
  };
}

// ============================================================
// find_transition
// ============================================================
const nas_transition_t* nas_state_machine::find_transition(
    _5gmm_state_t state, nas_event_e event, const nas_context& nc) const {
  for (const auto& t : transition_table_) {
    if (t.from_state == state && t.event == event) {
      if (!t.guard || t.guard(nc)) return &t;
    }
  }
  return nullptr;
}

// ============================================================
// handle_event
// ============================================================
transition_result_t nas_state_machine::handle_event(
    nas_context& nc, nas_event_e event) const {
  _5gmm_state_t old_state = nc._5gmm_state;

  const nas_transition_t* t = find_transition(old_state, event, nc);
  if (!t) {
    return {
        false, old_state, old_state,
        std::string("No transition: state=") +
            std::to_string(static_cast<int>(old_state)) +
            " event=" + nas_event_to_string(event)};
  }

  // Update prior_state whenever entering CPI from a different state (B-2 fix)
  if (t->to_state == _5GMM_COMMON_PROCEDURE_INITIATED &&
      old_state != _5GMM_COMMON_PROCEDURE_INITIATED) {
    nc.procedure_ctx.prior_state = old_state;
  }

  nc._5gmm_state = t->to_state;
  return {true, old_state, t->to_state, std::string()};
}

// ============================================================
// Check nas event transition possibility without state change
// ============================================================

transition_result_t nas_state_machine::check_nas_event(
    nas_context& nc, nas_event_e event) const {
  _5gmm_state_t old_state   = nc._5gmm_state;
  const nas_transition_t* t = find_transition(old_state, event, nc);
  if (!t) {
    return {
        false, old_state, old_state,
        std::string("No transition: state=") +
            std::to_string(static_cast<int>(old_state)) +
            " event=" + nas_event_to_string(event)};
  }
  return {true, old_state, t->to_state, std::string()};
}
}  // namespace oai::amf::nas
