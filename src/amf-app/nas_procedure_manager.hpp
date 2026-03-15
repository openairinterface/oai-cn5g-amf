/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License.
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

#include "nas_context.hpp"

// Procedure collision resolution action per 3GPP TS 24.501 v16.14.0
// §5.4.2.7c/d, §5.4.3.6c-f, §5.4.4.6c-e, §5.5.2.3.5c-f
enum class collision_action_e {
  ALLOW,          // New procedure may proceed
  REJECT,         // New procedure rejected; old continues
  ABORT_OLD,      // Abort old procedure, start new one
  ABORT_NEW,      // Old procedure continues; new is rejected (same as REJECT)
  PROGRESS_BOTH,  // Both procedures may run simultaneously
  COMPLETE_OLD_THEN_NEW,  // Complete old procedure first, then handle new
                          // §5.4.3.6f: identification + UE-dereg (not
                          // switch-off)
};

class nas_procedure_manager {
 public:
  // Start a specific procedure (registration, deregistration, service request)
  void start_specific_procedure(
      nas_context& nc, nas_procedure_type_e type) const;

  // Start a common procedure nested within the active specific procedure
  void start_common_procedure(nas_context& nc, nas_procedure_type_e type) const;

  // Complete/abort the current common procedure (returns which common procedure
  // was active)
  nas_procedure_type_e complete_common_procedure(nas_context& nc) const;
  nas_procedure_type_e abort_common_procedure(nas_context& nc) const;

  // Complete/abort the entire specific procedure (including any nested common)
  void complete_specific_procedure(nas_context& nc) const;
  void abort_specific_procedure(nas_context& nc) const;

  // Check if a new procedure would collide with the running one, resolving per
  // spec. Uses nc.procedure_ctx.dereg_switch_off and
  // nc.procedure_ctx.dereg_cause.
  collision_action_e check_collision(
      const nas_context& nc, nas_procedure_type_e new_procedure) const;

  // Query active procedures
  bool is_specific_procedure_running(const nas_context& nc) const;
  bool is_common_procedure_running(const nas_context& nc) const;
  nas_procedure_type_e get_active_specific(const nas_context& nc) const;
  nas_procedure_type_e get_active_common(const nas_context& nc) const;
};
