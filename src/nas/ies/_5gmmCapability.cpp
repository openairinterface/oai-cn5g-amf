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

#include "_5gmmCapability.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gmmCapability::_5gmmCapability(const uint8_t iei, uint8_t octet3)
    : Type4NasIe(kIei5gmmCapability) {
  octet3_ = octet3;
  octet4_ = std::nullopt;
  octet5_ = std::nullopt;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
_5gmmCapability::_5gmmCapability() : Type4NasIe(kIei5gmmCapability) {
  octet4_ = std::nullopt;
  octet5_ = std::nullopt;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
_5gmmCapability::~_5gmmCapability() {}

//------------------------------------------------------------------------------
void _5gmmCapability::SetOctet3(const uint8_t iei, uint8_t octet3) {
  SetIei(iei);
  SetLengthIndicator(1);
  octet3_ = octet3;
}

//------------------------------------------------------------------------------
uint8_t _5gmmCapability::GetOctet3() const {
  return octet3_;
}

//------------------------------------------------------------------------------
int _5gmmCapability::Encode(uint8_t* buf, int len) {
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

  // Octet 3
  ENCODE_U8(buf + encoded_size, octet3_, encoded_size);
  // TODO: Encode spare for the rest
  uint8_t spare = 0;
  int spare_len = ie_len - encoded_size;
  for (int i = 0; i < spare_len; i++) {
    ENCODE_U8(buf + encoded_size, spare, encoded_size);
  }

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gmmCapability::Decode(uint8_t* buf, int len, bool is_iei) {
  if (len < k5gmmCapabilityMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gmmCapabilityMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  // decoded_size += Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  DECODE_U8(buf + decoded_size, octet3_, decoded_size);
  // TODO: decode the rest as spare for now
  uint8_t spare = 0;
  for (int i = 0; i < (GetLengthIndicator() - 1); i++) {
    DECODE_U8(buf + decoded_size, spare, decoded_size);
  }

  Logger::nas_mm().debug(
      "Decoded %s, Octet3 value (0x%x)", GetIeName().c_str(), octet3_);
  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
