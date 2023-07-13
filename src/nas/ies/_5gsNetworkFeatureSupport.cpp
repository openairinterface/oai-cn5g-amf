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

#include "_5gsNetworkFeatureSupport.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsNetworkFeatureSupport::_5gsNetworkFeatureSupport()
    : Type4NasIe(kIei5gsNetworkFeatureSupport) {
  _value  = 0;
  _value2 = 0;
  SetLengthIndicator(1);  // With mimimum length of 3
}

//------------------------------------------------------------------------------
_5gsNetworkFeatureSupport::_5gsNetworkFeatureSupport(uint8_t value)
    : Type4NasIe(kIei5gsNetworkFeatureSupport) {
  _value  = value;
  _value2 = 0;
  SetLengthIndicator(1);  // With mimimum length of 3
}

//------------------------------------------------------------------------------
_5gsNetworkFeatureSupport::_5gsNetworkFeatureSupport(
    uint8_t value, uint8_t value2)
    : Type4NasIe(kIei5gsNetworkFeatureSupport) {
  _value  = value;
  _value2 = value2;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
_5gsNetworkFeatureSupport::~_5gsNetworkFeatureSupport() {}

//------------------------------------------------------------------------------
void _5gsNetworkFeatureSupport::setValue(uint8_t value, uint8_t value2) {
  _value  = value;
  _value2 = value2;
}

/*
//------------------------------------------------------------------------------
uint8_t _5gsNetworkFeatureSupport::getValue() {
  return _value;
}
*/

//------------------------------------------------------------------------------
int _5gsNetworkFeatureSupport::Encode(uint8_t* buf, int len) {
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
  ENCODE_U8(buf + encoded_size, _value, encoded_size);

  // Octet 4
  if (GetIeLength() > encoded_size)
    ENCODE_U8(buf + encoded_size, _value2, encoded_size);

  // Spare
  if (GetIeLength() > encoded_size) {
    uint8_t spare = 0;
    ENCODE_U8(buf + encoded_size, spare, encoded_size);
  }

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsNetworkFeatureSupport::Decode(uint8_t* buf, int len, bool is_iei) {
  if (len < k5gsNetworkFeatureSupportMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsNetworkFeatureSupportMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  // Octet 3
  DECODE_U8(buf + decoded_size, _value, decoded_size);
  // Octet 4
  if (GetLengthIndicator() > 1) {
    DECODE_U8(buf + decoded_size, _value2, decoded_size);
  }
  // Spare
  if (GetLengthIndicator() > 2) {
    uint8_t spare = 0;
    DECODE_U8(buf + decoded_size, spare, decoded_size);
  }

  Logger::nas_mm().debug(
      "Decoded %s, value (0x%x)", GetIeName().c_str(), _value);

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
