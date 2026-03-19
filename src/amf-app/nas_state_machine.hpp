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

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "nas_context.hpp"

namespace oai::amf::nas {

// ============================================================
// NAS Events
// Corresponding to NAS messages and timer expiries per 3GPP TS 24.501 v16.14.0
// ============================================================
enum class nas_event_e : uint8_t {
  // Registration (§5.5.1)
  REGISTRATION_REQUEST_RECEIVED,        // §5.5.1.2.2 / §5.5.1.3.2
  REGISTRATION_ACCEPT_SENT_NO_T3550,    // §5.5.1.2.4: Reg Accept without T3550
                                        // trigger
  REGISTRATION_ACCEPT_SENT_WITH_T3550,  // §5.5.1.2.4: Reg Accept with T3550
                                        // (GUTI/SOR/NSSAI)
  REGISTRATION_COMPLETE_RECEIVED,       // §5.5.1.2.4 / §5.5.1.3.4
  REGISTRATION_REJECT_SENT,             // §5.5.1.2.5

  // Deregistration (§5.5.2)
  UE_DEREGISTRATION_REQUEST_RECEIVED,  // §5.5.2.2.2: UE-initiated
  NW_DEREGISTRATION_INITIATED,         // §5.5.2.3.1: Network-initiated
  DEREGISTRATION_ACCEPT_RECEIVED,      // §5.5.2.3.3: from UE for NW-initiated

  // Authentication (§5.4.1)
  AUTHENTICATION_REQUEST_SENT,       // §5.4.1.3.2: starts T3560
  AUTHENTICATION_RESPONSE_RECEIVED,  // §5.4.1.3.3: stops T3560
  AUTHENTICATION_FAILURE_RECEIVED,   // §5.4.1.3.6
  AUTHENTICATION_REJECT_SENT,        // §5.4.1.3.5

  // Security Mode Control (§5.4.2)
  SECURITY_MODE_COMMAND_SENT,       // §5.4.2.2: starts T3560
  SECURITY_MODE_COMPLETE_RECEIVED,  // §5.4.2.4: stops T3560
  SECURITY_MODE_REJECT_RECEIVED,    // §5.4.2.5

  // Identification (§5.4.3)
  IDENTIFICATION_REQUEST_SENT,       // §5.4.3.2: starts T3570
  IDENTIFICATION_RESPONSE_RECEIVED,  // §5.4.3.4: stops T3570

  // Service Request (§5.6.1)
  SERVICE_REQUEST_RECEIVED,  // §5.6.1
  SERVICE_ACCEPT_SENT,       // §5.6.1
  SERVICE_REJECT_SENT,       // §5.6.1

  // Timer expiries (Table 10.2.2)
  T3550_FINAL_EXPIRY,       // §5.5.1.2.8c: 5th expiry of T3550 → REGISTERED
  T3560_FINAL_EXPIRY_AUTH,  // §5.4.1.3.7b: 5th expiry during auth → abort+N1
                            // release
  T3560_FINAL_EXPIRY_SMC,  // §5.4.2.7b: 5th expiry during SMC → abort SMC only
  T3570_FINAL_EXPIRY,      // §5.4.3.6b: 5th expiry of T3570 → abort
  T3522_FINAL_EXPIRY,      // §5.5.2.3.5a: 5th expiry of T3522 → DEREGISTERED
  T3555_FINAL_EXPIRY,      // §5.4.4.6a: 5th expiry of T3555 → abort

  // Other
  LOWER_LAYER_FAILURE,  // §5.4.1.3.7a, §5.4.2.7a, §5.4.3.6a, §5.5.2.3.5b
  IMPLICIT_DEREGISTRATION,  // §5.3.7: Mobile Reachable + Implicit Dereg timer
                            // cascade
};

// Human-readable event name for logging
const char* nas_event_to_string(nas_event_e event);

// ============================================================
// Transition table types
// ============================================================

// Guard function type: raw function pointer (avoids std::function overhead)
using guard_func_t = bool (*)(const nas_context& nc);

// A single row in the transition table
struct nas_transition_t {
  _5gmm_state_t from_state;
  nas_event_e event;
  _5gmm_state_t to_state;
  guard_func_t guard;    // nullptr = unconditional
  const char* spec_ref;  // for reference, e.g. "§5.5.1.2.2"
};

// Result of handle_event()
struct transition_result_t {
  bool allowed;  // false if no transition found
  _5gmm_state_t old_state;
  _5gmm_state_t new_state;
  std::string reject_reason;  // Only set when allowed == false
};

// ============================================================
// State Machine Engine
// ============================================================
class nas_state_machine {
 public:
  nas_state_machine();

  // Process an event for a given context. Does NOT acquire any lock.
  // Caller MUST hold m_nas_context if thread safety is needed.
  // Updates nc._5gmm_state on successful transition.
  transition_result_t handle_event(nas_context& nc, nas_event_e event) const;

  // Find the first transition matching (state, event) whose guard passes.
  // Returns nullptr if no matching transition found.
  const nas_transition_t* find_transition(
      _5gmm_state_t state, nas_event_e event, const nas_context& nc) const;

  // Check nas event transition possibility without state change
  transition_result_t check_nas_event(nas_context& nc, nas_event_e event) const;

 private:
  void build_transition_table();

  std::vector<nas_transition_t> transition_table_;
};

}  // namespace oai::amf::nas
