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

#include "UeIdentityIndexValue.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UeIdentityIndexValue::UeIdentityIndexValue() {
  m_Index   = 0;
  m_Present = Ngap_UEIdentityIndexValue_PR_NOTHING;
}

//------------------------------------------------------------------------------
UeIdentityIndexValue::~UeIdentityIndexValue() {}

//------------------------------------------------------------------------------
void UeIdentityIndexValue::set(const uint16_t& index) {
  m_Index   = index;
  m_Present = Ngap_UEIdentityIndexValue_PR_indexLength10;
}

//------------------------------------------------------------------------------
bool UeIdentityIndexValue::encode(Ngap_UEIdentityIndexValue_t& index) const {
  index.present                   = Ngap_UEIdentityIndexValue_PR_indexLength10;
  index.choice.indexLength10.size = sizeof(uint16_t);
  index.choice.indexLength10.bits_unused = 6;
  index.choice.indexLength10.buf =
      (uint8_t*) calloc(1, index.choice.indexLength10.size);
  if (!index.choice.indexLength10.buf) return false;
  index.choice.indexLength10.buf[0] = (m_Index >> 8) & 0x03;
  index.choice.indexLength10.buf[1] = m_Index & 0xff;

  return true;
}

//------------------------------------------------------------------------------
bool UeIdentityIndexValue::decode(const Ngap_UEIdentityIndexValue_t& index) {
  if (index.present != Ngap_UEIdentityIndexValue_PR_indexLength10) return false;
  if (!index.choice.indexLength10.buf) return false;
  m_Index = index.choice.indexLength10.buf[0];
  m_Index = m_Index << 8;
  m_Index |= index.choice.indexLength10.buf[1];

  return true;
}

//------------------------------------------------------------------------------
bool UeIdentityIndexValue::get(uint16_t& index) const {
  if (m_Present == Ngap_UEIdentityIndexValue_PR_indexLength10) {
    index = m_Index;
    return true;
  }
  return false;
}

}  // namespace ngap
