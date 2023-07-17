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

#include "PduSessionIdentity2.hpp"

#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
PduSessionIdentity2::PduSessionIdentity2(uint8_t iei) : Type3NasIe(iei) {
  value_ = 0;
}

//------------------------------------------------------------------------------
PduSessionIdentity2::PduSessionIdentity2(const uint8_t iei, uint8_t value)
    : Type3NasIe(iei) {
  value_ = value;
}

//------------------------------------------------------------------------------
PduSessionIdentity2::PduSessionIdentity2() : Type3NasIe() {
  value_ = 0;
}

//------------------------------------------------------------------------------
PduSessionIdentity2::~PduSessionIdentity2() {}

//------------------------------------------------------------------------------
void PduSessionIdentity2::SetValue(uint8_t value) {
  value_ = value;
}

//------------------------------------------------------------------------------
uint8_t PduSessionIdentity2::GetValue() const {
  return value_;
}

//------------------------------------------------------------------------------
int PduSessionIdentity2::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  if (len < kPduSessionIdentity2Length) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kPduSessionIdentity2Length);
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
int PduSessionIdentity2::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < kPduSessionIdentity2Length) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kPduSessionIdentity2Length);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  // IEI and Length
  decoded_size += Type3NasIe::Decode(buf + decoded_size, len, true);

  DECODE_U8(buf + decoded_size, value_, decoded_size);

  Logger::nas_mm().debug("Decoded value 0x%x", value_);
  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
