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

#include "UEAggregateMaxBitRate.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UEAggregateMaxBitRate::UEAggregateMaxBitRate() {
  dl_ = 0;
  ul_ = 0;
}

//------------------------------------------------------------------------------
UEAggregateMaxBitRate::~UEAggregateMaxBitRate() {}

//------------------------------------------------------------------------------
void UEAggregateMaxBitRate::set(const uint64_t& dl, const uint64_t& ul) {
  dl_ = dl;
  ul_ = ul;
}

//------------------------------------------------------------------------------
bool UEAggregateMaxBitRate::get(uint64_t& dl, uint64_t& ul) const {
  dl = dl_;
  ul = ul_;
  return true;
}

//------------------------------------------------------------------------------
bool UEAggregateMaxBitRate::encode(Ngap_UEAggregateMaximumBitRate_t& bit_rate) {
  bit_rate.uEAggregateMaximumBitRateDL.size = 4;  // TODO: 6 bytes
  bit_rate.uEAggregateMaximumBitRateDL.buf =
      (uint8_t*) calloc(1, bit_rate.uEAggregateMaximumBitRateDL.size);
  if (!bit_rate.uEAggregateMaximumBitRateDL.buf) return false;

  for (int i = 0; i < bit_rate.uEAggregateMaximumBitRateDL.size; i++) {
    bit_rate.uEAggregateMaximumBitRateDL.buf[i] =
        (dl_ & (0xff000000 >> i * 8)) >>
        ((bit_rate.uEAggregateMaximumBitRateDL.size - i - 1) * 8);
  }

  bit_rate.uEAggregateMaximumBitRateUL.size = 4;  // TODO: 6 bytes
  bit_rate.uEAggregateMaximumBitRateUL.buf =
      (uint8_t*) calloc(1, bit_rate.uEAggregateMaximumBitRateUL.size);
  if (!bit_rate.uEAggregateMaximumBitRateUL.buf) return false;

  for (int i = 0; i < bit_rate.uEAggregateMaximumBitRateUL.size; i++) {
    bit_rate.uEAggregateMaximumBitRateUL.buf[i] =
        (ul_ & (0xff000000 >> i * 8)) >>
        ((bit_rate.uEAggregateMaximumBitRateUL.size - i - 1) * 8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool UEAggregateMaxBitRate::decode(
    const Ngap_UEAggregateMaximumBitRate_t& bit_rate) {
  if (!bit_rate.uEAggregateMaximumBitRateDL.buf) return false;
  if (!bit_rate.uEAggregateMaximumBitRateUL.buf) return false;

  dl_ = 0;
  ul_ = 0;

  for (int i = 0; i < bit_rate.uEAggregateMaximumBitRateDL.size; i++) {
    dl_ = dl_ << 8;
    dl_ |= bit_rate.uEAggregateMaximumBitRateDL.buf[i];
  }
  for (int i = 0; i < bit_rate.uEAggregateMaximumBitRateUL.size; i++) {
    ul_ = ul_ << 8;
    ul_ |= bit_rate.uEAggregateMaximumBitRateUL.buf[i];
  }

  return true;
}

}  // namespace ngap
