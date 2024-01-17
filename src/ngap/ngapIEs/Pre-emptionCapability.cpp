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

#include "Pre-emptionCapability.hpp"

namespace ngap {

//------------------------------------------------------------------------------
Pre_emptionCapability::Pre_emptionCapability() {
  pre_emption_capability_ = 0;
}

//------------------------------------------------------------------------------
Pre_emptionCapability::~Pre_emptionCapability() {}

//------------------------------------------------------------------------------
void Pre_emptionCapability::set(e_Ngap_Pre_emptionCapability value) {
  pre_emption_capability_ = value;
}

//------------------------------------------------------------------------------
bool Pre_emptionCapability::get(e_Ngap_Pre_emptionCapability& value) {
  if ((pre_emption_capability_ == 0) || (pre_emption_capability_ == 1))
    value = (e_Ngap_Pre_emptionCapability) pre_emption_capability_;
  else
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool Pre_emptionCapability::encode(
    Ngap_Pre_emptionCapability_t& pre_emptionCapability) const {
  pre_emptionCapability = pre_emption_capability_;

  return true;
}

//------------------------------------------------------------------------------
bool Pre_emptionCapability::decode(
    Ngap_Pre_emptionCapability_t pre_emptionCapability) {
  pre_emption_capability_ = pre_emptionCapability;

  return true;
}
}  // namespace ngap
