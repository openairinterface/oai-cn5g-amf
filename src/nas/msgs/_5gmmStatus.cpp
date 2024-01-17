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

#include "_5gmmStatus.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gmmStatus::_5gmmStatus() : NasMmPlainHeader(EPD_5GS_MM_MSG, _5GMM_STATUS) {}

//------------------------------------------------------------------------------
_5gmmStatus::~_5gmmStatus() {}

//------------------------------------------------------------------------------
void _5gmmStatus::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void _5gmmStatus::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause.SetValue(value);
}

//------------------------------------------------------------------------------
uint8_t _5gmmStatus::Get5gmmCause() const {
  return ie_5gmm_cause.GetValue();
}

//------------------------------------------------------------------------------
int _5gmmStatus::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding _5gmmStatus message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gmm_cause, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug("Encoded _5gmmStatus message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gmmStatus::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding _5gmmStatus message");

  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  if ((decoded_ie_size =
           NasHelper::Decode(ie_5gmm_cause, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug("Decoded _5gmmStatus message len (%d)", decoded_size);
  return decoded_size;
}
