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
  m_UpdateTimer = 0;
}

//------------------------------------------------------------------------------
PeriodicRegistrationUpdateTimer::~PeriodicRegistrationUpdateTimer() {}

//------------------------------------------------------------------------------
void PeriodicRegistrationUpdateTimer::set(uint8_t updateTimer) {
  m_UpdateTimer = updateTimer;
}

//------------------------------------------------------------------------------
bool PeriodicRegistrationUpdateTimer::encode(
    Ngap_PeriodicRegistrationUpdateTimer_t& periodicRegistrationUpdateTimer)
    const {
  periodicRegistrationUpdateTimer.size        = sizeof(uint8_t);
  periodicRegistrationUpdateTimer.bits_unused = 0;
  periodicRegistrationUpdateTimer.buf =
      (uint8_t*) calloc(1, periodicRegistrationUpdateTimer.size);
  if (!periodicRegistrationUpdateTimer.buf) return false;
  periodicRegistrationUpdateTimer.buf[0] = m_UpdateTimer;

  return true;
}

//------------------------------------------------------------------------------
bool PeriodicRegistrationUpdateTimer::decode(
    Ngap_PeriodicRegistrationUpdateTimer_t periodicRegistrationUpdateTimer) {
  if (!periodicRegistrationUpdateTimer.buf) return false;
  m_UpdateTimer = periodicRegistrationUpdateTimer.buf[0];

  return true;
}

//------------------------------------------------------------------------------
void PeriodicRegistrationUpdateTimer::get(uint8_t& updateTimer) const {
  updateTimer = m_UpdateTimer;
}

}  // namespace ngap
