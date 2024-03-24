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

#include "PduSessionAggregateMaximumBitRate.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionAggregateMaximumBitRate::PduSessionAggregateMaximumBitRate() {
  m_Dl = 0;
  m_Ul = 0;
}

//------------------------------------------------------------------------------
PduSessionAggregateMaximumBitRate::PduSessionAggregateMaximumBitRate(
    const long& bitRateDl, const long& bitRateUl) {
  m_Dl = bitRateDl;
  m_Ul = bitRateUl;
}
//------------------------------------------------------------------------------
PduSessionAggregateMaximumBitRate::~PduSessionAggregateMaximumBitRate() {}

//------------------------------------------------------------------------------
void PduSessionAggregateMaximumBitRate::set(
    const long& bitRateDl, const long& bitRateUl) {
  m_Dl = bitRateDl;
  m_Ul = bitRateUl;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::get(
    long& bitRateDl, long& bitRateUl) const {
  bitRateDl = m_Dl;
  bitRateUl = m_Ul;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::encode(
    Ngap_PDUSessionAggregateMaximumBitRate_t& bitRate) const {
  bitRate.pDUSessionAggregateMaximumBitRateDL.size = 6;
  bitRate.pDUSessionAggregateMaximumBitRateDL.buf =
      (uint8_t*) calloc(1, bitRate.pDUSessionAggregateMaximumBitRateDL.size);
  if (!bitRate.pDUSessionAggregateMaximumBitRateDL.buf) return false;

  for (int i = 0; i < bitRate.pDUSessionAggregateMaximumBitRateDL.size; i++) {
    bitRate.pDUSessionAggregateMaximumBitRateDL.buf[i] =
        (m_Dl & (0xff0000000000 >> i * 8)) >>
        ((bitRate.pDUSessionAggregateMaximumBitRateDL.size - i - 1) * 8);
  }

  bitRate.pDUSessionAggregateMaximumBitRateUL.size = 6;
  bitRate.pDUSessionAggregateMaximumBitRateUL.buf =
      (uint8_t*) calloc(1, bitRate.pDUSessionAggregateMaximumBitRateUL.size);
  if (!bitRate.pDUSessionAggregateMaximumBitRateUL.buf) return false;

  for (int i = 0; i < bitRate.pDUSessionAggregateMaximumBitRateUL.size; i++) {
    bitRate.pDUSessionAggregateMaximumBitRateUL.buf[i] =
        (m_Ul & (0xff0000000000 >> i * 8)) >>
        ((bitRate.pDUSessionAggregateMaximumBitRateUL.size - i - 1) * 8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::decode(
    const Ngap_PDUSessionAggregateMaximumBitRate_t& bitRate) {
  if (!bitRate.pDUSessionAggregateMaximumBitRateDL.buf) return false;
  if (!bitRate.pDUSessionAggregateMaximumBitRateUL.buf) return false;

  m_Dl = 0;
  m_Ul = 0;

  for (int i = 0; i < bitRate.pDUSessionAggregateMaximumBitRateDL.size; i++) {
    m_Dl = m_Dl << 8;
    m_Dl |= bitRate.pDUSessionAggregateMaximumBitRateDL.buf[i];
  }
  for (int i = 0; i < bitRate.pDUSessionAggregateMaximumBitRateUL.size; i++) {
    m_Ul = m_Ul << 8;
    m_Ul |= bitRate.pDUSessionAggregateMaximumBitRateUL.buf[i];
  }

  return true;
}

}  // namespace ngap
