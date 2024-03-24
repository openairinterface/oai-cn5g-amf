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

#include "UeAggregateMaxBitRate.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UeAggregateMaxBitRate::UeAggregateMaxBitRate() {
  m_Dl = 0;
  m_Ul = 0;
}

//------------------------------------------------------------------------------
UeAggregateMaxBitRate::~UeAggregateMaxBitRate() {}

//------------------------------------------------------------------------------
void UeAggregateMaxBitRate::set(const uint64_t& dl, const uint64_t& ul) {
  m_Dl = dl;
  m_Ul = ul;
}

//------------------------------------------------------------------------------
bool UeAggregateMaxBitRate::get(uint64_t& dl, uint64_t& ul) const {
  dl = m_Dl;
  ul = m_Ul;
  return true;
}

//------------------------------------------------------------------------------
bool UeAggregateMaxBitRate::encode(
    Ngap_UEAggregateMaximumBitRate_t& bitRate) const {
  bitRate.uEAggregateMaximumBitRateDL.size = 4;  // TODO: 6 bytes
  bitRate.uEAggregateMaximumBitRateDL.buf =
      (uint8_t*) calloc(1, bitRate.uEAggregateMaximumBitRateDL.size);
  if (!bitRate.uEAggregateMaximumBitRateDL.buf) return false;

  for (int i = 0; i < bitRate.uEAggregateMaximumBitRateDL.size; i++) {
    bitRate.uEAggregateMaximumBitRateDL.buf[i] =
        (m_Dl & (0xff000000 >> i * 8)) >>
        ((bitRate.uEAggregateMaximumBitRateDL.size - i - 1) * 8);
  }

  bitRate.uEAggregateMaximumBitRateUL.size = 4;  // TODO: 6 bytes
  bitRate.uEAggregateMaximumBitRateUL.buf =
      (uint8_t*) calloc(1, bitRate.uEAggregateMaximumBitRateUL.size);
  if (!bitRate.uEAggregateMaximumBitRateUL.buf) return false;

  for (int i = 0; i < bitRate.uEAggregateMaximumBitRateUL.size; i++) {
    bitRate.uEAggregateMaximumBitRateUL.buf[i] =
        (m_Ul & (0xff000000 >> i * 8)) >>
        ((bitRate.uEAggregateMaximumBitRateUL.size - i - 1) * 8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool UeAggregateMaxBitRate::decode(
    const Ngap_UEAggregateMaximumBitRate_t& bitRate) {
  if (!bitRate.uEAggregateMaximumBitRateDL.buf) return false;
  if (!bitRate.uEAggregateMaximumBitRateUL.buf) return false;

  m_Dl = 0;
  m_Ul = 0;

  for (int i = 0; i < bitRate.uEAggregateMaximumBitRateDL.size; i++) {
    m_Dl = m_Dl << 8;
    m_Dl |= bitRate.uEAggregateMaximumBitRateDL.buf[i];
  }
  for (int i = 0; i < bitRate.uEAggregateMaximumBitRateUL.size; i++) {
    m_Ul = m_Ul << 8;
    m_Ul |= bitRate.uEAggregateMaximumBitRateUL.buf[i];
  }

  return true;
}

}  // namespace ngap
