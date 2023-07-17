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

#include "_5gsRegistrationResult.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsRegistrationResult::_5gsRegistrationResult() : Type4NasIe() {
  emergency_registered_ = false;
  nssaa_performed_      = false;
  sms_allowed_          = false;
  value_                = 0;
  SetLengthIndicator(k5gsRegistrationResultContentLength);
}

//------------------------------------------------------------------------------
_5gsRegistrationResult::_5gsRegistrationResult(uint8_t iei) : Type4NasIe(iei) {
  emergency_registered_ = false;
  nssaa_performed_      = false;
  sms_allowed_          = false;
  value_                = 0;
  SetLengthIndicator(k5gsRegistrationResultContentLength);
}

//------------------------------------------------------------------------------
_5gsRegistrationResult::_5gsRegistrationResult(
    bool emergency, bool nssaa, bool sms, uint8_t value)
    : Type4NasIe() {
  emergency_registered_ = emergency;
  nssaa_performed_      = nssaa;
  sms_allowed_          = sms;
  value_                = value & 0x07;
  SetLengthIndicator(k5gsRegistrationResultContentLength);
}

//------------------------------------------------------------------------------
_5gsRegistrationResult::_5gsRegistrationResult(
    uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value)
    : Type4NasIe(iei) {
  emergency_registered_ = emergency;
  nssaa_performed_      = nssaa;
  sms_allowed_          = sms;
  value_                = value & 0x07;
  SetLengthIndicator(k5gsRegistrationResultContentLength);
}

//------------------------------------------------------------------------------
_5gsRegistrationResult::~_5gsRegistrationResult() {}

//------------------------------------------------------------------------------
void _5gsRegistrationResult::setValue(uint8_t value) {
  value_ = value & 0x07;
}

//------------------------------------------------------------------------------
uint8_t _5gsRegistrationResult::getValue() const {
  return value_;
}

//------------------------------------------------------------------------------
void _5gsRegistrationResult::set(
    uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value) {
  emergency_registered_ = emergency;
  nssaa_performed_      = nssaa;
  sms_allowed_          = sms;
  value_                = value & 0x07;
  SetIei(iei);
}

//------------------------------------------------------------------------------
void _5gsRegistrationResult::set(
    bool emergency, bool nssaa, bool sms, uint8_t value) {
  emergency_registered_ = emergency;
  nssaa_performed_      = nssaa;
  sms_allowed_          = sms;
  value_                = value & 0x07;
}

//------------------------------------------------------------------------------
int _5gsRegistrationResult::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  if (len < k5gsRegistrationResultLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsRegistrationResultLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Octet 3
  uint8_t octet = 0;
  octet = 0x00 | (emergency_registered_ << 5) | (nssaa_performed_ << 4) |
          (sms_allowed_ << 3) | (value_ & 0x07);
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsRegistrationResult::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < k5gsRegistrationResultLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsRegistrationResultLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, true);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  // Octet 3
  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  emergency_registered_ = (octet & 0x20) >> 5;
  nssaa_performed_      = (octet & 0x10) >> 4;
  sms_allowed_          = (octet & 0x08) >> 3;
  value_                = octet & 0x07;

  Logger::nas_mm().debug(
      "Decoded _5gsRegistrationResult, Emergency Registered 0x%x, NSSAA "
      "Performed 0x%x, SMS Allowed 0x%x, "
      "Value 0x%x",
      emergency_registered_, nssaa_performed_, sms_allowed_, value_);

  Logger::nas_mm().debug(
      "Decoded %s, DRX value 0x%x, len %d", GetIeName().c_str(), value_,
      decoded_size);
  return decoded_size;
}
