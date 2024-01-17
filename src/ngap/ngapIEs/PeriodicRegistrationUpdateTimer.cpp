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

#include "PeriodicRegistrationUpdateTimer.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PeriodicRegistrationUpdateTimer::PeriodicRegistrationUpdateTimer() {
  update_timer_ = 0;
}

//------------------------------------------------------------------------------
PeriodicRegistrationUpdateTimer::~PeriodicRegistrationUpdateTimer() {}

//------------------------------------------------------------------------------
void PeriodicRegistrationUpdateTimer::set(const uint8_t& update_timer) {
  update_timer_ = update_timer;
}

//------------------------------------------------------------------------------
bool PeriodicRegistrationUpdateTimer::encode(
    Ngap_PeriodicRegistrationUpdateTimer_t& periodic_registration_update_timer)
    const {
  periodic_registration_update_timer.size        = sizeof(uint8_t);
  periodic_registration_update_timer.bits_unused = 0;
  periodic_registration_update_timer.buf =
      (uint8_t*) calloc(1, periodic_registration_update_timer.size);
  if (!periodic_registration_update_timer.buf) return false;
  periodic_registration_update_timer.buf[0] = update_timer_;

  return true;
}

//------------------------------------------------------------------------------
bool PeriodicRegistrationUpdateTimer::decode(
    Ngap_PeriodicRegistrationUpdateTimer_t periodic_registration_update_timer) {
  if (!periodic_registration_update_timer.buf) return false;
  update_timer_ = periodic_registration_update_timer.buf[0];

  return true;
}

//------------------------------------------------------------------------------
void PeriodicRegistrationUpdateTimer::get(uint8_t& update_timer) const {
  update_timer = update_timer_;
}

}  // namespace ngap
