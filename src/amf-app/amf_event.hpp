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

#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

#include "amf.hpp"
#include "amf_event_sig.hpp"

namespace amf_application {
class amf_event {
 public:
  amf_event(){};
  amf_event(amf_event const&) = delete;
  void operator=(amf_event const&) = delete;

  static amf_event& get_instance() {
    static amf_event instance;
    return instance;
  }

  // class register/handle event
  friend class amf_app;
  friend class amf_n1;
  friend class amf_profile;

  /*
   * Subscribe to Location Report signal
   * @param [const ue_location_report_sig_t::slot_type&] sig: slot_type
   * parameter
   * @return boost::signals2::connection: the connection between the signal and
   * the slot
   */
  bs2::connection subscribe_ue_location_report(
      const ue_location_report_sig_t::slot_type& sig);

  /*
   * Subscribe to UE Reachability Status Notification signal
   * @param [const ue_reachability_status_sig_t::slot_type&] sig: slot_type
   * parameter
   * @return boost::signals2::connection: the connection between the signal and
   * the slot
   */
  bs2::connection subscribe_ue_reachability_status(
      const ue_reachability_status_sig_t::slot_type& sig);

  /*
   * Subscribe to UE Registration State Notification signal
   * @param [const ue_registration_state_sig_t::slot_type&] sig: slot_type
   * parameter
   * @return boost::signals2::connection: the connection between the signal and
   * the slot
   */
  bs2::connection subscribe_ue_registration_state(
      const ue_registration_state_sig_t::slot_type& sig);

  /*
   * Subscribe to UE Connectivity State Notification signal
   * @param [const ue_connectivity_state_sig_t::slot_type&] sig: slot_type
   * parameter
   * @return boost::signals2::connection: the connection between the signal and
   * the slot
   */
  bs2::connection subscribe_ue_connectivity_state(
      const ue_connectivity_state_sig_t::slot_type& sig);

  /*
   * Subscribe to UE Loss of Connectivity Notification signal
   * @param [const ue_loss_of_connectivity_sig_t::slot_type&] sig: slot_type*/

  bs2::connection subscribe_ue_loss_of_connectivity(
      const ue_loss_of_connectivity_sig_t::slot_type& sig);
  /*
   * Subscribe to UE Communication Failure Notification signal
   * @param [const ue_communication_failure_sig_t::slot_type&] sig: slot_type
   * parameter
   * @return boost::signals2::connection: the connection between the signal and
   * the slot
   */
  bs2::connection subscribe_ue_communication_failure(
      const ue_communication_failure_sig_t::slot_type& sig);

  ue_location_report_sig_t ue_location_report;  // Signal for UE Location Report
  ue_reachability_status_sig_t
      ue_reachability_status;  // Signal for UE Reachability Report
  ue_registration_state_sig_t
      ue_registration_state;  // Signal for UE Registration State Report
  ue_connectivity_state_sig_t
      ue_connectivity_state;  // Signal for UE Connectivity State Report
  ue_loss_of_connectivity_sig_t
      ue_loss_of_connectivity;  // Signal for UE Loss of Connectivity
  ue_communication_failure_sig_t
      ue_communication_failure;  // Signal for UE Communication Failure Report
};
}  // namespace amf_application
