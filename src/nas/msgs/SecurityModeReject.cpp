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

#include "SecurityModeReject.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SecurityModeReject::SecurityModeReject()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, SECURITY_MODE_REJECT) {}

//------------------------------------------------------------------------------
SecurityModeReject::~SecurityModeReject() {}

//------------------------------------------------------------------------------
void SecurityModeReject::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void SecurityModeReject::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause.SetValue(value);
}

//------------------------------------------------------------------------------
int SecurityModeReject::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding SecurityModeReject message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;
  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // 5GMM Cause
  encoded_ie_size =
      ie_5gmm_cause.Encode(buf + encoded_size, len - encoded_size);
  if (encoded_ie_size != KEncodeDecodeError) {
    encoded_size += encoded_ie_size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", _5gmmCause::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Encoded SecurityModeReject message len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SecurityModeReject::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding SecurityModeReject message");
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // 5GMM Cause
  decoded_result =
      ie_5gmm_cause.Decode(buf + decoded_size, len - decoded_size, false);
  if (decoded_result != KEncodeDecodeError) {
    decoded_size += decoded_result;
  } else {
    Logger::nas_mm().error(
        "Decoding %s error", _5gmmCause::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Decoded SecurityModeReject message len(%d)", decoded_size);
  return decoded_size;
}
