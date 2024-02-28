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
#include "logger_base.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsDrxParameters::_5gsDrxParameters(uint8_t value)
    : Type4NasIe(kIei5gsDrxParameters) {
  value_ = value & 0x0F;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
_5gsDrxParameters::_5gsDrxParameters() {
  value_ = 0;
  SetLengthIndicator(1);
}
_5gsDrxParameters::~_5gsDrxParameters() {}

//------------------------------------------------------------------------------
void _5gsDrxParameters::SetValue(uint8_t value) {
  value_ = value & 0x0F;
}

//------------------------------------------------------------------------------
uint8_t _5gsDrxParameters::GetValue() const {
  return value_;
}

//------------------------------------------------------------------------------
int _5gsDrxParameters::Encode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding %s", GetIeName().c_str());

  if (len < k5gsDrxParametersLength) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Buffer length is less than the minimum length of this IE (%d "
            "octet)",
            k5gsDrxParametersLength);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  ENCODE_U8(buf + encoded_size, value_, encoded_size);

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsDrxParameters::Decode(uint8_t* buf, int len, bool is_iei) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding %s", GetIeName().c_str());

  if (len < k5gsDrxParametersLength) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Buffer length is less than the minimum length of this IE (%d "
            "octet)",
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
  value_ = octet & 0x0f;

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug(
          "Decoded %s, DRX value 0x%x, len %d", GetIeName().c_str(), value_,
          decoded_size);
  return decoded_size;
}
