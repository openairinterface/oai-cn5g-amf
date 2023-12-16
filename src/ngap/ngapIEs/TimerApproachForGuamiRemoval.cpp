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

#include "TimerApproachForGuamiRemoval.hpp"

namespace ngap {

//------------------------------------------------------------------------------
TimerApproachForGuamiRemoval::TimerApproachForGuamiRemoval() {
  value_ = -1;
}

//------------------------------------------------------------------------------
TimerApproachForGuamiRemoval::~TimerApproachForGuamiRemoval() {}

//------------------------------------------------------------------------------
void TimerApproachForGuamiRemoval::set(
    e_Ngap_TimerApproachForGUAMIRemoval value) {
  value_ = value;
}

//------------------------------------------------------------------------------
bool TimerApproachForGuamiRemoval::get(long& value) const {
  value = value_;

  return true;
}

//------------------------------------------------------------------------------
bool TimerApproachForGuamiRemoval::encode(
    Ngap_TimerApproachForGUAMIRemoval_t& value) const {
  value = value_;

  return true;
}

//------------------------------------------------------------------------------
bool TimerApproachForGuamiRemoval::decode(
    Ngap_TimerApproachForGUAMIRemoval_t value) {
  value_ = value;

  return true;
}
}  // namespace ngap
