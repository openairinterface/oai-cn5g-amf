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

#include "UEIdentityIndexValue.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UEIdentityIndexValue::UEIdentityIndexValue() {
  index_   = 0;
  present_ = Ngap_UEIdentityIndexValue_PR_NOTHING;
}

//------------------------------------------------------------------------------
UEIdentityIndexValue::~UEIdentityIndexValue() {}

//------------------------------------------------------------------------------
void UEIdentityIndexValue::set(const uint16_t& index) {
  index_   = index;
  present_ = Ngap_UEIdentityIndexValue_PR_indexLength10;
}

//------------------------------------------------------------------------------
bool UEIdentityIndexValue::encode(
    Ngap_UEIdentityIndexValue_t& ue_identity_index_value) {
  ue_identity_index_value.present = Ngap_UEIdentityIndexValue_PR_indexLength10;
  ue_identity_index_value.choice.indexLength10.size        = sizeof(uint16_t);
  ue_identity_index_value.choice.indexLength10.bits_unused = 6;
  ue_identity_index_value.choice.indexLength10.buf =
      (uint8_t*) calloc(1, ue_identity_index_value.choice.indexLength10.size);
  if (!ue_identity_index_value.choice.indexLength10.buf) return false;
  ue_identity_index_value.choice.indexLength10.buf[0] = (index_ >> 8) & 0x03;
  ue_identity_index_value.choice.indexLength10.buf[1] = index_ & 0xff;

  return true;
}

//------------------------------------------------------------------------------
bool UEIdentityIndexValue::decode(
    const Ngap_UEIdentityIndexValue_t& ue_identity_index_value) {
  if (ue_identity_index_value.present !=
      Ngap_UEIdentityIndexValue_PR_indexLength10)
    return false;
  if (!ue_identity_index_value.choice.indexLength10.buf) return false;
  index_ = ue_identity_index_value.choice.indexLength10.buf[0];
  index_ = index_ << 8;
  index_ |= ue_identity_index_value.choice.indexLength10.buf[1];

  return true;
}

//------------------------------------------------------------------------------
bool UEIdentityIndexValue::get(uint16_t& index) const {
  if (present_ == Ngap_UEIdentityIndexValue_PR_indexLength10) {
    index = index_;
    return true;
  }
  return false;
}

}  // namespace ngap
