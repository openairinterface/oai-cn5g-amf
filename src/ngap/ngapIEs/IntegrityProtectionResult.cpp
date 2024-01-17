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

#include "IntegrityProtectionResult.hpp"

namespace ngap {

//------------------------------------------------------------------------------
IntegrityProtectionResult::IntegrityProtectionResult() {
  result_ = -1;
}

//------------------------------------------------------------------------------
IntegrityProtectionResult::~IntegrityProtectionResult() {}

//------------------------------------------------------------------------------
void IntegrityProtectionResult::set(e_Ngap_IntegrityProtectionResult value) {
  result_ = value;
}

//------------------------------------------------------------------------------
bool IntegrityProtectionResult::get(long& value) const {
  value = result_;

  return true;
}

//------------------------------------------------------------------------------
bool IntegrityProtectionResult::encode(
    Ngap_IntegrityProtectionResult_t& value) const {
  value = result_;

  return true;
}

//------------------------------------------------------------------------------
bool IntegrityProtectionResult::decode(Ngap_IntegrityProtectionResult_t value) {
  result_ = value;

  return true;
}
}  // namespace ngap
