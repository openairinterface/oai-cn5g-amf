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
  dl_ = 0;
  ul_ = 0;
}

//------------------------------------------------------------------------------
PduSessionAggregateMaximumBitRate::PduSessionAggregateMaximumBitRate(
    long bit_rate_downlink, long bit_rate_uplink) {
  dl_ = bit_rate_downlink;
  ul_ = bit_rate_uplink;
}
//------------------------------------------------------------------------------
PduSessionAggregateMaximumBitRate::~PduSessionAggregateMaximumBitRate() {}

//------------------------------------------------------------------------------
void PduSessionAggregateMaximumBitRate::set(
    long bit_rate_downlink, long bit_rate_uplink) {
  dl_ = bit_rate_downlink;
  ul_ = bit_rate_uplink;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::get(
    long& bit_rate_downlink, long& bit_rate_uplink) const {
  bit_rate_downlink = dl_;
  bit_rate_uplink   = ul_;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::encode(
    Ngap_PDUSessionAggregateMaximumBitRate_t& bit_rate) {
  bit_rate.pDUSessionAggregateMaximumBitRateDL.size = 6;
  bit_rate.pDUSessionAggregateMaximumBitRateDL.buf =
      (uint8_t*) calloc(1, bit_rate.pDUSessionAggregateMaximumBitRateDL.size);
  if (!bit_rate.pDUSessionAggregateMaximumBitRateDL.buf) return false;

  for (int i = 0; i < bit_rate.pDUSessionAggregateMaximumBitRateDL.size; i++) {
    bit_rate.pDUSessionAggregateMaximumBitRateDL.buf[i] =
        (dl_ & (0xff0000000000 >> i * 8)) >>
        ((bit_rate.pDUSessionAggregateMaximumBitRateDL.size - i - 1) * 8);
  }

  bit_rate.pDUSessionAggregateMaximumBitRateUL.size = 6;
  bit_rate.pDUSessionAggregateMaximumBitRateUL.buf =
      (uint8_t*) calloc(1, bit_rate.pDUSessionAggregateMaximumBitRateUL.size);
  if (!bit_rate.pDUSessionAggregateMaximumBitRateUL.buf) return false;

  for (int i = 0; i < bit_rate.pDUSessionAggregateMaximumBitRateUL.size; i++) {
    bit_rate.pDUSessionAggregateMaximumBitRateUL.buf[i] =
        (ul_ & (0xff0000000000 >> i * 8)) >>
        ((bit_rate.pDUSessionAggregateMaximumBitRateUL.size - i - 1) * 8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::decode(
    const Ngap_PDUSessionAggregateMaximumBitRate_t& bit_rate) {
  if (!bit_rate.pDUSessionAggregateMaximumBitRateDL.buf) return false;
  if (!bit_rate.pDUSessionAggregateMaximumBitRateUL.buf) return false;

  dl_ = 0;
  ul_ = 0;

  for (int i = 0; i < bit_rate.pDUSessionAggregateMaximumBitRateDL.size; i++) {
    dl_ = dl_ << 8;
    dl_ |= bit_rate.pDUSessionAggregateMaximumBitRateDL.buf[i];
  }
  for (int i = 0; i < bit_rate.pDUSessionAggregateMaximumBitRateUL.size; i++) {
    ul_ = ul_ << 8;
    ul_ |= bit_rate.pDUSessionAggregateMaximumBitRateUL.buf[i];
  }

  return true;
}

}  // namespace ngap
