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

#include "_5gmmCause.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5gmmCause::_5gmmCause(uint8_t iei) : Type3NasIe(iei) {
  value_ = 0;
}

//------------------------------------------------------------------------------
_5gmmCause::_5gmmCause() : Type3NasIe() {
  value_ = 0;
}

//------------------------------------------------------------------------------
_5gmmCause::_5gmmCause(uint8_t iei, uint8_t value) : Type3NasIe(iei) {
  value_ = value;
}

//------------------------------------------------------------------------------
_5gmmCause::~_5gmmCause(){};

//------------------------------------------------------------------------------
void _5gmmCause::SetValue(uint8_t value) {
  value_ = value;
}

//------------------------------------------------------------------------------
uint8_t _5gmmCause::GetValue() const {
  return value_;
}

//------------------------------------------------------------------------------
void _5gmmCause::Set(uint8_t iei, uint8_t value) {
  SetIei(iei);
  value_ = value;
}

//------------------------------------------------------------------------------
int _5gmmCause::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  if (len < k5gmmCauseMaximumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gmmCauseMaximumLength);
    return KEncodeDecodeError;
  }
  int encoded_size = 0;

  // IEI
  encoded_size += Type3NasIe::Encode(buf + encoded_size, len);
  // Value
  ENCODE_U8(buf + encoded_size, value_, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gmmCause::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < k5gmmCauseMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gmmCauseMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  // IEI and Length
  decoded_size += Type3NasIe::Decode(buf + decoded_size, len, is_iei);

  DECODE_U8(buf + decoded_size, value_, decoded_size);

  Logger::nas_mm().debug("Decoded value 0x%x", value_);
  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
