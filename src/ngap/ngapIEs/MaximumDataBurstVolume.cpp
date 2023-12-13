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

#include "MaximumDataBurstVolume.hpp"

namespace ngap {

//------------------------------------------------------------------------------
MaximumDataBurstVolume::MaximumDataBurstVolume() {
  volume_ = 0;
}

//------------------------------------------------------------------------------
MaximumDataBurstVolume::~MaximumDataBurstVolume() {}

//------------------------------------------------------------------------------
void MaximumDataBurstVolume::set(long value) {
  volume_ = value;
}

//------------------------------------------------------------------------------
void MaximumDataBurstVolume::get(long& value) const {
  value = volume_;
}

//------------------------------------------------------------------------------
bool MaximumDataBurstVolume::encode(
    Ngap_MaximumDataBurstVolume_t& value) const {
  value = volume_;

  return true;
}

//------------------------------------------------------------------------------
bool MaximumDataBurstVolume::decode(Ngap_MaximumDataBurstVolume_t value) {
  volume_ = value;

  return true;
}
}  // namespace ngap
