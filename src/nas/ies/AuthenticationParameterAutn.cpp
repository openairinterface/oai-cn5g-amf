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

#include "AuthenticationParameterAutn.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
AuthenticationParameterAutn::AuthenticationParameterAutn(uint8_t iei)
    : Type4NasIe(iei), _value() {
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
AuthenticationParameterAutn::AuthenticationParameterAutn(
    const uint8_t iei, uint8_t value[kAuthenticationParameterAutnValueLength])
    : Type4NasIe(iei) {
  for (int i = 0; i < kAuthenticationParameterAutnValueLength; i++) {
    this->_value[i] = value[i];
  }
  SetLengthIndicator(kAuthenticationParameterAutnValueLength);
}

//------------------------------------------------------------------------------
AuthenticationParameterAutn::AuthenticationParameterAutn()
    : Type4NasIe(), _value() {
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
AuthenticationParameterAutn::~AuthenticationParameterAutn() {}

//------------------------------------------------------------------------------
int AuthenticationParameterAutn::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {
    Logger::nas_mm().error("Len is less than %d", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  if (GetLengthIndicator() != kAuthenticationParameterAutnValueLength)
    return KEncodeDecodeError;

  for (int i = 0; i < kAuthenticationParameterAutnValueLength; i++) {
    ENCODE_U8(buf + encoded_size, _value[i], encoded_size);
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationParameterAutn len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationParameterAutn::Decode(uint8_t* buf, int len, bool is_iei) {
  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  if (GetLengthIndicator() != kAuthenticationParameterAutnValueLength)
    return KEncodeDecodeError;

  for (int i = 0; i < kAuthenticationParameterAutnValueLength; i++) {
    DECODE_U8(buf + decoded_size, _value[i], decoded_size);
  }
  for (int j = 0; j < kAuthenticationParameterAutnValueLength; j++) {
    Logger::nas_mm().debug(
        "Decoded AuthenticationParameterAutn value (0x%2x)", _value[j]);
  }
  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
