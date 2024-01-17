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

#include "RelativeAmfCapacity.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RelativeAmfCapacity::RelativeAmfCapacity() {
  amf_capacity_ = 0;
}

//------------------------------------------------------------------------------
RelativeAmfCapacity::~RelativeAmfCapacity() {}

//------------------------------------------------------------------------------
void RelativeAmfCapacity::setValue(uint8_t amf_capacity) {
  amf_capacity_ = amf_capacity;
}

//------------------------------------------------------------------------------
uint8_t RelativeAmfCapacity::getValue() const {
  return amf_capacity_;
}

//------------------------------------------------------------------------------
bool RelativeAmfCapacity::encode(
    Ngap_RelativeAMFCapacity_t& amf_capacity_ie) const {
  amf_capacity_ie = amf_capacity_;
  return true;
}

//------------------------------------------------------------------------------
bool RelativeAmfCapacity::decode(Ngap_RelativeAMFCapacity_t amf_capacity_ie) {
  amf_capacity_ = (uint8_t) amf_capacity_ie;
  return true;
}

}  // namespace ngap
