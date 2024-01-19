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

#include "DeregistrationAccept.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
DeregistrationAccept::DeregistrationAccept(bool is_ue_originating)
    : NasMmPlainHeader() {
  NasMmPlainHeader::SetEpd(EPD_5GS_MM_MSG);
  if (is_ue_originating) {
    NasMmPlainHeader::SetMessageType(DEREGISTRATION_ACCEPT_UE_ORIGINATING);
  } else {
    NasMmPlainHeader::SetMessageType(DEREGISTRATION_ACCEPT_UE_TERMINATED);
  }
}

//------------------------------------------------------------------------------
DeregistrationAccept::~DeregistrationAccept() {}

//------------------------------------------------------------------------------
void DeregistrationAccept::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
int DeregistrationAccept::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding De-registration Accept message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  Logger::nas_mm().debug(
      "Encoded De-registration Accept message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DeregistrationAccept::Decode(uint8_t* buf, int len) {
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  Logger::nas_mm().debug(
      "Decoded De-registrationReject message len (%d)", decoded_size);
  return decoded_size;
}
