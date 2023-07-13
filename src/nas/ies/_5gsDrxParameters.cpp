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

#include "_5gsDrxParameters.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsDrxParameters::_5gsDrxParameters(uint8_t value)
    : Type4NasIe(kIei5gsDrxParameters) {
  _value = value & 0x0F;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
_5gsDrxParameters::_5gsDrxParameters() {
  _value = 0;
  SetLengthIndicator(1);
}
_5gsDrxParameters::~_5gsDrxParameters() {}

//------------------------------------------------------------------------------
void _5gsDrxParameters::SetValue(uint8_t value) {
  _value = value & 0x0F;
}

//------------------------------------------------------------------------------
uint8_t _5gsDrxParameters::GetValue() const {
  return _value;
}

//------------------------------------------------------------------------------
int _5gsDrxParameters::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  if (len < k5gsDrxParametersLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsDrxParametersLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  ENCODE_U8(buf + encoded_size, _value, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsDrxParameters::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < k5gsDrxParametersLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsDrxParametersLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, true);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  _value = octet & 0x0f;

  Logger::nas_mm().debug(
      "Decoded %s, DRX value 0x%x, len %d", GetIeName().c_str(), _value,
      decoded_size);
  return decoded_size;
}
