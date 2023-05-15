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

#include "Type1NasIe.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;
//------------------------------------------------------------------------------
Type1NasIe::Type1NasIe() : NasIe(), high_pos_(false), value_(0) {
  iei_ = std::nullopt;
}

//------------------------------------------------------------------------------
Type1NasIe::Type1NasIe(const bool& high_pos, const uint8_t& value) {
  iei_      = std::nullopt;
  high_pos_ = high_pos;
  value_    = value & 0x0f;
}

//------------------------------------------------------------------------------
Type1NasIe::Type1NasIe(const bool& high_pos) : value_() {
  iei_      = std::nullopt;
  high_pos_ = high_pos;
}

//------------------------------------------------------------------------------
Type1NasIe::Type1NasIe(const uint8_t& iei, const uint8_t& value) : NasIe() {
  iei_      = std::optional<uint8_t>(iei & 0x0f);
  high_pos_ = false;
  value_    = value & 0x0f;
}

//------------------------------------------------------------------------------
Type1NasIe::Type1NasIe(const uint8_t& iei) : NasIe(), value_() {
  iei_      = std::optional<uint8_t>(iei & 0x0f);
  high_pos_ = false;
}

//------------------------------------------------------------------------------
Type1NasIe::~Type1NasIe() {}

/*
//------------------------------------------------------------------------------
void Type1NasIe::SetIei(const uint8_t& iei) {
  iei_      = std::optional<uint8_t>(iei & 0x0f);
  high_pos_ = false;
}
*/

//------------------------------------------------------------------------------
void Type1NasIe::Set(const bool& high_pos, const uint8_t& value) {
  high_pos_ = high_pos;
  value_    = value & 0x0f;
}

//------------------------------------------------------------------------------
void Type1NasIe::Set(const bool& high_pos) {
  high_pos_ = high_pos;
}

//------------------------------------------------------------------------------
bool Type1NasIe::Validate(const int& len) const {
  if (len < kType1NasIeLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kType1NasIeLength);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void Type1NasIe::SetValue(const uint8_t& value) {
  value_ = value & 0x0f;  // 4 lower bits
}

//------------------------------------------------------------------------------
int Type1NasIe::Encode(uint8_t* buf, const int& len) {
  // Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  if (!Validate(len)) return KEncodeDecodeError;

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (iei_.has_value()) {
    octet = (iei_.value() << 4) | value_;
  } else {
    int decoded_size = 0;
    // First get value of this octet
    DECODE_U8(buf + encoded_size, octet, decoded_size);
    if (high_pos_) {
      octet =
          (octet & 0x0f) | (value_ << 4);  // Keep 4 less significant bits and
                                           // update 4 most significant bits
    } else {
      octet = (octet & 0xf0) | (value_);  // Keep 4 most significant bits and
                                          // update 4 less significant bits
    }
  }

  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  // Logger::nas_mm().debug(
  //     "Encoded %s (len %d)", GetIeName().c_str(), encoded_size);
  if (iei_.has_value()) {
    return encoded_size;  // 1 octet
  } else {
    return 0;  // 1/2 octet
  }
}

//------------------------------------------------------------------------------
int Type1NasIe::Decode(const uint8_t* const buf, const int& len, bool is_iei) {
  return Decode(buf, len, false, is_iei);
}

//------------------------------------------------------------------------------
int Type1NasIe::Decode(
    const uint8_t* const buf, const int& len, const bool& high_pos,
    bool is_iei) {
  // Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (!Validate(len)) return KEncodeDecodeError;

  high_pos_        = high_pos;
  int decoded_size = 0;
  uint8_t octet    = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  if (is_iei) {
    iei_   = std::optional<uint8_t>((octet & 0xf0) >> 4);
    value_ = octet & 0x0f;
  } else {
    if (high_pos_) {
      value_ = (octet & 0xf0) >> 4;
    } else {
      value_ = (octet & 0x0f);
    }
  }

  // Logger::nas_mm().debug(
  //     "Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  if (is_iei) {
    return decoded_size;  // 1 octet
  } else {
    return 0;  // 1/2 octet
  }
}
