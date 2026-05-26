/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nas_procedure_manager.hpp"

// ---------------------------------------------------------------------------
// Query
// ---------------------------------------------------------------------------

bool nas_procedure_manager::is_specific_procedure_running(
    const nas_context& nc) const {
  return nc.procedure_ctx.specific_procedure != nas_procedure_type_e::NONE;
}

bool nas_procedure_manager::is_common_procedure_running(
    const nas_context& nc) const {
  return nc.procedure_ctx.common_procedure != nas_procedure_type_e::NONE;
}

nas_procedure_type_e nas_procedure_manager::get_active_specific(
    const nas_context& nc) const {
  return nc.procedure_ctx.specific_procedure;
}

nas_procedure_type_e nas_procedure_manager::get_active_common(
    const nas_context& nc) const {
  return nc.procedure_ctx.common_procedure;
}

// ---------------------------------------------------------------------------
// State transitions
// ---------------------------------------------------------------------------

void nas_procedure_manager::start_specific_procedure(
    nas_context& nc, nas_procedure_type_e type) const {
  nc.procedure_ctx.specific_procedure = type;
}

void nas_procedure_manager::start_common_procedure(
    nas_context& nc, nas_procedure_type_e type) const {
  nc.procedure_ctx.common_procedure = type;
}

nas_procedure_type_e nas_procedure_manager::complete_common_procedure(
    nas_context& nc) const {
  const nas_procedure_type_e prev   = nc.procedure_ctx.common_procedure;
  nc.procedure_ctx.common_procedure = nas_procedure_type_e::NONE;
  return prev;
}

nas_procedure_type_e nas_procedure_manager::abort_common_procedure(
    nas_context& nc) const {
  const nas_procedure_type_e prev   = nc.procedure_ctx.common_procedure;
  nc.procedure_ctx.common_procedure = nas_procedure_type_e::NONE;
  return prev;
}

void nas_procedure_manager::complete_specific_procedure(
    nas_context& nc, nas_procedure_type_e type) const {
  if (type == nas_procedure_type_e::NONE) {
    nc.procedure_ctx.specific_procedure = nas_procedure_type_e::NONE;
    nc.procedure_ctx.common_procedure   = nas_procedure_type_e::NONE;
  } else if (nc.procedure_ctx.specific_procedure == type) {
    nc.procedure_ctx.specific_procedure = nas_procedure_type_e::NONE;
    nc.procedure_ctx.common_procedure   = nas_procedure_type_e::NONE;
  }
}

void nas_procedure_manager::abort_specific_procedure(nas_context& nc) const {
  nc.procedure_ctx.specific_procedure = nas_procedure_type_e::NONE;
  nc.procedure_ctx.common_procedure   = nas_procedure_type_e::NONE;
}

// ---------------------------------------------------------------------------
// Collision resolution per 3GPP TS 24.501 V17.10.1
// ---------------------------------------------------------------------------

collision_action_e nas_procedure_manager::check_collision(
    const nas_context& nc, nas_procedure_type_e new_proc) const {
  const auto& ctx = nc.procedure_ctx;

  if (ctx.common_procedure == nas_procedure_type_e::NONE &&
      ctx.specific_procedure == nas_procedure_type_e::NONE)
    return collision_action_e::ALLOW;

  // SMC collisions §5.4.2.7c/d
  if (ctx.common_procedure == nas_procedure_type_e::SECURITY_MODE_CONTROL) {
    if (new_proc == nas_procedure_type_e::REGISTRATION_INITIAL ||
        new_proc == nas_procedure_type_e::REGISTRATION_MOBILITY ||
        new_proc == nas_procedure_type_e::REGISTRATION_PERIODIC ||
        new_proc == nas_procedure_type_e::SERVICE_REQUEST ||
        new_proc == nas_procedure_type_e::DEREGISTRATION_UE)
      return collision_action_e::ABORT_OLD;    // §5.4.2.7c
    return collision_action_e::PROGRESS_BOTH;  // §5.4.2.7d
  }

  // Authentication collisions (analogous to SMC per §5.4.1.3.7)
  if (ctx.common_procedure == nas_procedure_type_e::AUTHENTICATION) {
    if (new_proc == nas_procedure_type_e::REGISTRATION_INITIAL ||
        new_proc == nas_procedure_type_e::REGISTRATION_MOBILITY ||
        new_proc == nas_procedure_type_e::REGISTRATION_PERIODIC ||
        new_proc == nas_procedure_type_e::SERVICE_REQUEST ||
        new_proc == nas_procedure_type_e::DEREGISTRATION_UE)
      return collision_action_e::ABORT_OLD;
    return collision_action_e::PROGRESS_BOTH;
  }

  // Identification collisions §5.4.3.6c-f
  if (ctx.common_procedure == nas_procedure_type_e::IDENTIFICATION) {
    if (new_proc == nas_procedure_type_e::REGISTRATION_INITIAL)
      return collision_action_e::ABORT_OLD;  // §5.4.3.6c/d
    if (new_proc == nas_procedure_type_e::REGISTRATION_MOBILITY ||
        new_proc == nas_procedure_type_e::REGISTRATION_PERIODIC)
      return collision_action_e::PROGRESS_BOTH;  // §5.4.3.6e
    if (new_proc == nas_procedure_type_e::DEREGISTRATION_UE) {
      // §5.4.3.6f: switch-off → abort identification; otherwise complete first
      return ctx.dereg_switch_off ? collision_action_e::ABORT_OLD :
                                    collision_action_e::COMPLETE_OLD_THEN_NEW;
    }
    return collision_action_e::PROGRESS_BOTH;
  }

  // Configuration Update collisions §5.4.4.6c-e
  if (ctx.common_procedure == nas_procedure_type_e::CONFIGURATION_UPDATE) {
    if (new_proc == nas_procedure_type_e::DEREGISTRATION_UE ||
        new_proc == nas_procedure_type_e::DEREGISTRATION_NETWORK)
      return collision_action_e::ABORT_OLD;  // §5.4.4.6c
    if (new_proc == nas_procedure_type_e::REGISTRATION_INITIAL ||
        new_proc == nas_procedure_type_e::REGISTRATION_MOBILITY ||
        new_proc == nas_procedure_type_e::REGISTRATION_PERIODIC)
      return collision_action_e::ABORT_OLD;  // §5.4.4.6d
    if (new_proc == nas_procedure_type_e::SERVICE_REQUEST)
      return collision_action_e::PROGRESS_BOTH;  // §5.4.4.6e
  }

  // Network-initiated deregistration collisions §5.5.2.3.5c-f
  if (ctx.specific_procedure == nas_procedure_type_e::DEREGISTRATION_NETWORK) {
    if (new_proc == nas_procedure_type_e::DEREGISTRATION_UE)
      return collision_action_e::ABORT_OLD;  // §5.5.2.3.5c: both considered
                                             // completed
    if (new_proc == nas_procedure_type_e::REGISTRATION_INITIAL)
      return collision_action_e::ABORT_OLD;  // §5.5.2.3.5d
    if (new_proc == nas_procedure_type_e::SERVICE_REQUEST)
      return collision_action_e::REJECT;  // §5.5.2.3.5f
    if (new_proc == nas_procedure_type_e::REGISTRATION_MOBILITY ||
        new_proc == nas_procedure_type_e::REGISTRATION_PERIODIC) {
      // §5.5.2.3.5e: 5GMM cause #11/#12/#13/#15 → abort NW-dereg; otherwise
      // reject
      if (ctx.dereg_cause == 11 || ctx.dereg_cause == 12 ||
          ctx.dereg_cause == 13 || ctx.dereg_cause == 15)
        return collision_action_e::ABORT_OLD;
      return collision_action_e::REJECT;
    }
    return collision_action_e::REJECT;
  }

  return collision_action_e::ALLOW;  // default: allow nesting
}
