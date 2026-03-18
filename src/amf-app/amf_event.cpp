/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
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
