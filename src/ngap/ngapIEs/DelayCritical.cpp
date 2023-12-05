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

#include "DelayCritical.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DelayCritical::DelayCritical() {
  delay_critical_ = 0;
}

//------------------------------------------------------------------------------
DelayCritical::~DelayCritical() {}

//------------------------------------------------------------------------------
void DelayCritical::setDelayCritical(e_Ngap_DelayCritical value) {
  delay_critical_ = value;
}

//------------------------------------------------------------------------------
bool DelayCritical::getDelayCritical(e_Ngap_DelayCritical& value) const {
  if ((delay_critical_ >= 0) && (delay_critical_ <= 1))
    value = (e_Ngap_DelayCritical) delay_critical_;
  else
    false;

  return true;
}

//------------------------------------------------------------------------------
bool DelayCritical::encode(Ngap_DelayCritical_t& value) const {
  value = delay_critical_;

  return true;
}

//------------------------------------------------------------------------------
bool DelayCritical::decode(Ngap_DelayCritical_t value) {
  delay_critical_ = value;

  return true;
}
}  // namespace ngap
