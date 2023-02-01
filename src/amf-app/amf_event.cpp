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

#include "amf_event.hpp"
using namespace amf_application;

//------------------------------------------------------------------------------
bs2::connection amf_event::subscribe_ue_location_report(
    const ue_location_report_sig_t::slot_type& sig) {
  return ue_location_report.connect(sig);
}

//------------------------------------------------------------------------------
bs2::connection amf_event::subscribe_ue_reachability_status(
    const ue_reachability_status_sig_t::slot_type& sig) {
  return ue_reachability_status.connect(sig);
}

//------------------------------------------------------------------------------
bs2::connection amf_event::subscribe_ue_registration_state(
    const ue_registration_state_sig_t::slot_type& sig) {
  return ue_registration_state.connect(sig);
}

bs2::connection amf_event::subscribe_ue_connectivity_state(
    const ue_connectivity_state_sig_t::slot_type& sig) {
  return ue_connectivity_state.connect(sig);
}

bs2::connection amf_event::subscribe_ue_loss_of_connectivity(
    const ue_loss_of_connectivity_sig_t::slot_type& sig) {
  return ue_loss_of_connectivity.connect(sig);
}
//------------------------------------------------------------------------------
bs2::connection amf_event::subscribe_ue_communication_failure(
    const ue_communication_failure_sig_t::slot_type& sig) {
  return ue_communication_failure.connect(sig);
}
