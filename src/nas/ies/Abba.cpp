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
#include "Abba.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
Abba::Abba() : Type4NasIe(), value_() {
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
Abba::Abba(uint8_t iei) : Type4NasIe(iei), value_() {
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
Abba::Abba(uint8_t length, uint8_t* value) : Type4NasIe() {
  for (int i = 0; i < length; i++) {
    this->value_[i] = value[i];
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
Abba::Abba(uint8_t iei, uint8_t length, uint8_t* value) : Type4NasIe(iei) {
  for (int i = 0; i < length; i++) {
    this->value_[i] = value[i];
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
Abba::~Abba() {}

//------------------------------------------------------------------------------
void Abba::Set(uint8_t length, uint8_t* value) {
  for (int i = 0; i < length; i++) {
    this->value_[i] = value[i];
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
void Abba::Set(uint8_t iei, uint8_t length, uint8_t* value) {
  SetIei(iei);
  for (int i = 0; i < length; i++) {
    this->value_[i] = value[i];
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
int Abba::Encode(uint8_t* buf, int len) {
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
  int length = GetLengthIndicator();
  int i      = 0;
  while (length != 0) {
    ENCODE_U8(buf + encoded_size, value_[i], encoded_size);
    length--;
    i++;
  }

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Abba::Decode(uint8_t* buf, int len, bool is_iei) {
  if (len < kAbbaMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kAbbaMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  int i          = 0;
  uint8_t length = GetLengthIndicator();
  while (length != 0) {
    DECODE_U8(buf + decoded_size, value_[i], decoded_size);
    length--;
    i++;
  }

  for (int j = 0; j < GetLengthIndicator(); j++) {
    Logger::nas_mm().debug("Decoded ABBA value (0x%4x)", value_[j]);
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
