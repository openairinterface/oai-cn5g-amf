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

#include "UplinkDataStatus.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
UplinkDataStatus::UplinkDataStatus() : Type4NasIe(kIeiUplinkDataStatus) {
  _value = 0;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
UplinkDataStatus::UplinkDataStatus(const uint16_t& value)
    : Type4NasIe(kIeiUplinkDataStatus) {
  _value = value;
  SetLengthIndicator(2);
}

//-----------------------------------------------------------------------------
UplinkDataStatus::~UplinkDataStatus() {}

//------------------------------------------------------------------------------
void UplinkDataStatus::SetValue(uint16_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint16_t UplinkDataStatus::GetValue() const {
  return _value;
}

//------------------------------------------------------------------------------
int UplinkDataStatus::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Value
  ENCODE_U16(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UplinkDataStatus::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  int decoded_size = 0;

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  DECODE_U16(buf + decoded_size, _value, decoded_size);

  Logger::nas_mm().debug(
      "Decoded %s, value 0x%x len %d", GetIeName().c_str(), _value,
      decoded_size);
  return decoded_size;
}
