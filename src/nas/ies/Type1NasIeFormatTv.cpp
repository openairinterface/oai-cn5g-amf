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

#include "Type1NasIeFormatTv.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;
//------------------------------------------------------------------------------
Type1NasIeFormatTv::Type1NasIeFormatTv() : NasIe(), value_(0) {
  iei_ = std::nullopt;
}

//------------------------------------------------------------------------------
Type1NasIeFormatTv::Type1NasIeFormatTv(const uint8_t& iei)
    : NasIe(), value_(0) {
  iei_ = std::optional<uint8_t>(iei & 0x0f);
}

//------------------------------------------------------------------------------
Type1NasIeFormatTv::~Type1NasIeFormatTv() {}

//------------------------------------------------------------------------------
void Type1NasIeFormatTv::SetIei(const uint8_t& iei) {
  iei_ = std::optional<uint8_t>(iei & 0x0f);
}

//------------------------------------------------------------------------------
bool Type1NasIeFormatTv::Validate(const int& len) const {
  if (len < kType1NasIeFormatTvLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kType1NasIeFormatTvLength);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
uint8_t Type1NasIeFormatTv::GetIeLength() const {
  return kType1NasIeFormatTvLength;
}

//------------------------------------------------------------------------------
void Type1NasIeFormatTv::SetValue(const uint8_t& value) {
  value_ = value & 0x0f;  // 4 lower bits
}

//------------------------------------------------------------------------------
uint8_t Type1NasIeFormatTv::GetValue() const {
  return value_;
}
//------------------------------------------------------------------------------
int Type1NasIeFormatTv::Encode(uint8_t* buf, const int& len) {
  // Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  if (!Validate(len)) return KEncodeDecodeError;

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (iei_.has_value()) {
    octet = (iei_.value() << 4) | value_;
  } else {
    octet = value_ & 0x0f;
  }
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  //  Logger::nas_mm().debug(
  //     "Encoded %s (len %d)", GetIeName().c_str(), encoded_size);
  if (iei_.has_value()) {
    return encoded_size;  // 1 octet
  } else {
    return 0;  // 1/2 octet
  }
}

//------------------------------------------------------------------------------
int Type1NasIeFormatTv::Decode(
    const uint8_t* const buf, const int& len, bool is_iei) {
  // Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (!Validate(len)) return KEncodeDecodeError;

  int decoded_size = 0;
  uint8_t octet    = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  if (is_iei) {
    iei_ = std::optional<uint8_t>((octet & 0xf0) >> 4);
  }
  value_ = octet & 0x0f;

  // Logger::nas_mm().debug(
  //      "Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  if (is_iei) {
    return decoded_size;  // 1 octet
  } else {
    return 0;  // 1/2 octet
  }
}
