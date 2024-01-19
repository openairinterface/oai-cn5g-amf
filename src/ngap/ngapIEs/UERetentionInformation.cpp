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

#include "UERetentionInformation.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UERetentionInformation::UERetentionInformation() {
  value_ = Ngap_UERetentionInformation_ues_retained;
}

//------------------------------------------------------------------------------
UERetentionInformation::~UERetentionInformation() {}

//------------------------------------------------------------------------------
void UERetentionInformation::set(const long value) {
  value_ = value;
}

//------------------------------------------------------------------------------
void UERetentionInformation::get(long& value) const {
  value = value_;
}

//------------------------------------------------------------------------------
void UERetentionInformation::set(const e_Ngap_UERetentionInformation& value) {
  value_ = static_cast<long>(value);
}

//------------------------------------------------------------------------------
void UERetentionInformation::get(e_Ngap_UERetentionInformation& value) const {
  value = static_cast<e_Ngap_UERetentionInformation>(value_);
}

//------------------------------------------------------------------------------
e_Ngap_UERetentionInformation UERetentionInformation::get() const {
  return static_cast<e_Ngap_UERetentionInformation>(value_);
}

//------------------------------------------------------------------------------
bool UERetentionInformation::encode(
    Ngap_UERetentionInformation_t& value) const {
  value = value_;
  return true;
}

//------------------------------------------------------------------------------
bool UERetentionInformation::decode(Ngap_UERetentionInformation_t value) {
  value_ = value;
  return true;
}

}  // namespace ngap
