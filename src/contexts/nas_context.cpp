/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "nas_context.hpp"

#include "amf.hpp"
#include "utils.hpp"
#include "bstrlib.h"

const char* nas_procedure_type_to_string(nas_procedure_type_e type) {
  switch (type) {
    case nas_procedure_type_e::AUTHENTICATION:
      return "AUTHENTICATION";
    case nas_procedure_type_e::SECURITY_MODE_CONTROL:
      return "SECURITY_MODE_CONTROL";
    case nas_procedure_type_e::IDENTIFICATION:
      return "IDENTIFICATION";
    case nas_procedure_type_e::CONFIGURATION_UPDATE:
      return "CONFIGURATION_UPDATE";
    case nas_procedure_type_e::NAS_TRANSPORT:
      return "NAS_TRANSPORT";
    case nas_procedure_type_e::_5GMM_STATUS:
      return "_5GMM_STATUS";
    case nas_procedure_type_e::REGISTRATION_INITIAL:
      return "REGISTRATION_INITIAL";
    case nas_procedure_type_e::REGISTRATION_MOBILITY:
      return "REGISTRATION_MOBILITY";
    case nas_procedure_type_e::REGISTRATION_PERIODIC:
      return "REGISTRATION_PERIODIC";
    case nas_procedure_type_e::DEREGISTRATION_UE:
      return "DEREGISTRATION_UE";
    case nas_procedure_type_e::DEREGISTRATION_NETWORK:
      return "DEREGISTRATION_NETWORK";
    case nas_procedure_type_e::SERVICE_REQUEST:
      return "SERVICE_REQUEST";
    case nas_procedure_type_e::PAGING:
      return "PAGING";
    case nas_procedure_type_e::NOTIFICATION:
      return "NOTIFICATION";
    default:
      return "UNKNOWN_PROCEDURE";
  }
}

//------------------------------------------------------------------------------
nas_context::nas_context()
    : _5g_he_av(), _5g_av(), kamf(), kgNB(), _5gmm_capability() {
  is_imsi_present                    = false;
  is_auth_vectors_present            = false;
  auts                               = nullptr;
  ctx_avaliability_ind               = false;
  amf_ue_ngap_id                     = INVALID_AMF_UE_NGAP_ID;
  ran_ue_ngap_id                     = 0;
  old_amf_ue_ngap_id                 = INVALID_AMF_UE_NGAP_ID;
  old_ran_ue_ngap_id                 = 0;
  _5gmm_state                        = _5GMM_DEREGISTERED;
  registration_type                  = 0;
  follow_on_req_pending_ind          = false;
  ngksi                              = 0;
  ue_security_capability             = {};
  security_ctx                       = std::nullopt;
  is_current_security_available      = false;
  registration_attempt_counter       = 0;
  is_imsi_present                    = false;
  is_5g_suci_present                 = false;
  is_5g_guti_present                 = false;
  is_auth_vectors_present            = false;
  to_be_register_by_new_suci         = false;
  registration_request_is_set        = false;
  registration_request               = nullptr;
  nas_status                         = CM_IDLE;
  is_mobile_reachable_timer_timeout  = false;
  mobile_reachable_timer             = ITTI_INVALID_TIMER_ID;
  implicit_deregistration_timer      = ITTI_INVALID_TIMER_ID;
  awaiting_registration_timer        = ITTI_INVALID_TIMER_ID;
  temporary_unreachable_timer        = ITTI_INVALID_TIMER_ID;
  procedure_ctx.specific_procedure   = nas_procedure_type_e::NONE;
  procedure_ctx.common_procedure     = nas_procedure_type_e::NONE;
  procedure_ctx.prior_state          = _5GMM_DEREGISTERED;
  procedure_ctx.dereg_switch_off     = false;
  procedure_ctx.dereg_cause          = 0;
  procedure_ctx.retransmission_count = 0;
  for (size_t i = 0; i < kNasTimerCount; ++i) {
    nas_timers[i].itti_timer_id        = 0;
    nas_timers[i].retransmission_count = 0;
    nas_timers[i].is_running           = false;
  }
  href        = {};
  imeisv      = std::nullopt;
  guti        = std::nullopt;
  is_kgNB_set = false;
}

//------------------------------------------------------------------------------
nas_context::~nas_context() {
  oai::utils::utils::bdestroy_wrapper(&registration_request);
  oai::utils::utils::bdestroy_wrapper(&auts);
}

//------------------------------------------------------------------------------
bool nas_context::get_kamf(
    uint8_t index, uint8_t (&k)[AUTH_VECTOR_LENGTH_OCTETS]) const {
  if (index >= MAX_5GS_AUTH_VECTORS) return false;
  for (uint8_t i = 0; i < AUTH_VECTOR_LENGTH_OCTETS; i++) {
    k[i] = kamf[index][i];
  }
  return true;
}

//------------------------------------------------------------------------------
std::string nas_context::fivegmm_state_to_string(const _5gmm_state_t& state) {
  switch (state) {
    case _5GMM_DEREGISTERED: {
      return "5GMM-DEREGISTERED";
    } break;
    case _5GMM_REGISTERED: {
      return "5GMM-REGISTERED";
    } break;
    case _5GMM_DEREGISTERED_INITIATED: {
      return "5GMM-DEREG_INIT";
    } break;
    case _5GMM_COMMON_PROCEDURE_INITIATED: {
      return "COMM-PROC-INIT";
    } break;
    default:
      return "STATE-INVALID";
  }
  return "STATE-INVALID";
}

//------------------------------------------------------------------------------
std::string nas_context::cm_state_to_string(const cm_state_t& state) {
  switch (state) {
    case CM_IDLE: {
      return "5GMM-IDLE";
    } break;
    case CM_CONNECTED: {
      return "5GMM-CONNECTED";
    } break;
    default:
      return "STATE-INVALID";
  }
  return "STATE-INVALID";
}
