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

#include "Type3NasIe.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;
//------------------------------------------------------------------------------
Type3NasIe::Type3NasIe() : NasIe() {
  iei_ = std::nullopt;
}

//------------------------------------------------------------------------------
Type3NasIe::Type3NasIe(const uint8_t& iei) : NasIe() {
  iei_ = std::optional<uint8_t>(iei);
}

//------------------------------------------------------------------------------
Type3NasIe::~Type3NasIe() {}

//------------------------------------------------------------------------------
void Type3NasIe::SetIei(const uint8_t& iei) {
  iei_ = std::optional<uint8_t>(iei);
}

//------------------------------------------------------------------------------
bool Type3NasIe::Validate(const int& len) const {
  if (iei_.has_value() and (len < kType3NasIeFormatTvLength)) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kType3NasIeFormatTvLength);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
int Type3NasIe::Encode(uint8_t* buf, const int& len) {
  if (!Validate(len)) return KEncodeDecodeError;

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (iei_.has_value()) {
    ENCODE_U8(buf + encoded_size, iei_.value(), encoded_size);
  }
  return encoded_size;
}

//------------------------------------------------------------------------------
int Type3NasIe::Decode(const uint8_t* const buf, const int& len, bool is_iei) {
  if (!Validate(len)) return KEncodeDecodeError;

  int decoded_size = 0;
  uint8_t octet    = 0;

  if (is_iei) {
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    iei_ = std::optional<uint8_t>(octet);
  }
  return decoded_size;
}
