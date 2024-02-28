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

#include "Type1NasIeFormatV.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace nas;

//------------------------------------------------------------------------------
Type1NasIeFormatV::Type1NasIeFormatV() : NasIe(), high_pos_(false), value_(0) {}

//------------------------------------------------------------------------------
Type1NasIeFormatV::Type1NasIeFormatV(bool high_pos) : NasIe(), value_(0) {
  high_pos_ = high_pos;
}
//------------------------------------------------------------------------------
Type1NasIeFormatV::Type1NasIeFormatV(bool high_pos, uint8_t value) : NasIe() {
  high_pos_ = high_pos;
  value_    = value & 0x0f;
}

//------------------------------------------------------------------------------
Type1NasIeFormatV::~Type1NasIeFormatV() {}

//------------------------------------------------------------------------------
bool Type1NasIeFormatV::Validate(const int& len) const {
  if (len < kType1NasIeFormatVLength) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Buffer length is less than the minimum length of this IE (%d "
            "octet)",
            kType1NasIeFormatVLength);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void Type1NasIeFormatV::Set(bool high_pos, uint8_t value) {
  high_pos_ = high_pos;
  value_    = value & 0x0f;  // 4 bits
}

//------------------------------------------------------------------------------
void Type1NasIeFormatV::Set(uint8_t value) {
  value_ = value & 0x0f;  // 4 bits
}
//------------------------------------------------------------------------------
int Type1NasIeFormatV::Encode(uint8_t* buf, const int& len) {
  // oai::logger::logger_registry::get_logger(LOGGER_COMMON).debug("Encoding
  // %s", GetIeName().c_str());
  if (!Validate(len)) return KEncodeDecodeError;

  int encoded_size = 0;
  int decoded_size = 0;
  uint8_t octet    = 0;

  // First get value of this octet
  DECODE_U8(buf + encoded_size, octet, decoded_size);
  if (high_pos_) {
    octet = (octet & 0x0f) | (value_ << 4);  // Keep 4 less significant bits and
                                             // update 4 most significant bits
  } else {
    octet = (octet & 0xf0) | (value_);  // Keep 4 most significant bits and
                                        // update 4 less significant bits
  }

  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  // oai::logger::logger_registry::get_logger(LOGGER_COMMON).debug(
  //    "Encoded %s (len %d)", GetIeName().c_str(), encoded_size);
  return 0;  // 1/2 octet
}

//------------------------------------------------------------------------------
int Type1NasIeFormatV::Decode(
    const uint8_t* const buf, const int& len, bool high_pos) {
  // oai::logger::logger_registry::get_logger(LOGGER_COMMON).debug("Decoding
  // %s", GetIeName().c_str());

  if (!Validate(len)) return KEncodeDecodeError;
  high_pos_        = high_pos;
  int decoded_size = 0;
  uint8_t octet    = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  if (high_pos_) {
    value_ = (octet & 0xf0) >> 4;
  } else {
    value_ = (octet & 0x0f);
  }
  // oai::logger::logger_registry::get_logger(LOGGER_COMMON).debug(
  //     "Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return 0;  // 1/2 octet
}
