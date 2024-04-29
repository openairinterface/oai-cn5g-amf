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

#include "nas_context.hpp"

#include "amf.hpp"

//------------------------------------------------------------------------------
nas_context::nas_context() : _5g_he_av(), _5g_av(), kamf(), _5gmm_capability() {
  is_imsi_present                                    = false;
  is_auth_vectors_present                            = false;
  auts                                               = nullptr;
  ctx_avaliability_ind                               = false;
  amf_ue_ngap_id                                     = INVALID_AMF_UE_NGAP_ID;
  ran_ue_ngap_id                                     = 0;
  old_amf_ue_ngap_id                                 = INVALID_AMF_UE_NGAP_ID;
  old_ran_ue_ngap_id                                 = 0;
  _5gmm_state                                        = {};
  registration_type                                  = 0;
  follow_on_req_pending_ind                          = false;
  ngksi                                              = 0;
  ue_security_capability                             = {};
  is_specific_procedure_for_registration_running     = false;
  is_specific_procedure_for_deregistration_running   = false;
  is_specific_procedure_for_eCell_inactivity_running = false;
  is_common_procedure_for_authentication_running     = false;
  is_common_procedure_for_identification_running     = false;
  is_common_procedure_for_security_mode_control_running = false;
  is_common_procedure_for_nas_transport_running         = false;
  security_ctx                                          = std::nullopt;
  is_current_security_available                         = false;
  registration_attempt_counter                          = 0;
  is_imsi_present                                       = false;
  is_5g_guti_present                                    = false;
  is_auth_vectors_present                               = false;
  to_be_register_by_new_suci                            = false;
  registration_request_is_set                           = false;
  registration_request                                  = nullptr;
  nas_status                                            = CM_IDLE;
  is_mobile_reachable_timer_timeout                     = false;
  mobile_reachable_timer                                = ITTI_INVALID_TIMER_ID;
  implicit_deregistration_timer                         = ITTI_INVALID_TIMER_ID;
  href                                                  = {};
  imeisv                                                = std::nullopt;
  guti                                                  = std::nullopt;
}

//------------------------------------------------------------------------------
nas_context::~nas_context() {}

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
