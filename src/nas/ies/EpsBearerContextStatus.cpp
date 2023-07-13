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

#include "EpsBearerContextStatus.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
EpsBearerContextStatus::EpsBearerContextStatus()
    : Type4NasIe(kIeiEpsBearerContextStatus) {
  value_ = 0;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
EpsBearerContextStatus::EpsBearerContextStatus(uint16_t value) {
  value_ = value;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
EpsBearerContextStatus::~EpsBearerContextStatus() {}

//------------------------------------------------------------------------------
void EpsBearerContextStatus::SetValue(uint16_t value) {
  value_ = value;
}

//------------------------------------------------------------------------------
uint16_t EpsBearerContextStatus::GetValue() const {
  return value_;
}

//------------------------------------------------------------------------------
int EpsBearerContextStatus::Encode(uint8_t* buf, int len) {
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

  // Value
  ENCODE_U16(buf + encoded_size, value_, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int EpsBearerContextStatus::Decode(uint8_t* buf, int len, bool is_iei) {
  if (len < kEpsBearerContextStatusLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kEpsBearerContextStatusLength);
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

  // Value
  DECODE_U16(buf + decoded_size, value_, decoded_size);  // for IE

  Logger::nas_mm().debug("EPS_Bearer_Context_Status, value 0x%0x", value_);
  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
