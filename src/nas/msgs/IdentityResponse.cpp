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

#include "IdentityResponse.hpp"

using namespace nas;

//------------------------------------------------------------------------------
IdentityResponse::IdentityResponse()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, IDENTITY_RESPONSE) {}

//------------------------------------------------------------------------------
IdentityResponse::~IdentityResponse() {}

//------------------------------------------------------------------------------
void IdentityResponse::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void IdentityResponse::Get5gsMobileIdentity(
    _5gsMobileIdentity& mobile_identity) const {
  mobile_identity = ie_mobile_identity;
}
//------------------------------------------------------------------------------
void IdentityResponse::SetSuciSupiFormatImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routing_ind, uint8_t protection_sch_id,
    const std::string& msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "Encoding suci and supi format for imsi error, please choose right "
        "interface");
    return;
  } else {
    ie_mobile_identity.SetSuciWithSupiImsi(
        mcc, mnc, routing_ind, protection_sch_id, msin);
  }
}

//------------------------------------------------------------------------------
void IdentityResponse::SetSuciSupiFormatImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routingInd, uint8_t protection_sch_id, uint8_t hnpki,
    const std::string& msin) {
  // TODO:
}

//------------------------------------------------------------------------------
void IdentityResponse::Set5gGuti() {
  // TODO:
}

//------------------------------------------------------------------------------
void IdentityResponse::SetImeiImeisv() {
  // TODO:
}

//------------------------------------------------------------------------------
void IdentityResponse::Set5gSTmsi() {
  // TODO:
}

//------------------------------------------------------------------------------
int IdentityResponse::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding IdentityResponse message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // Mobile Identity
  int size = ie_mobile_identity.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", _5gsMobileIdentity::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Encoded IdentityResponse message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int IdentityResponse::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding IdentityResponse message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // Mobile Identity
  decoded_result =
      ie_mobile_identity.Decode(buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", _5gsMobileIdentity::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  Logger::nas_mm().debug(
      "Decoded IdentityResponse message len (%d)", decoded_size);
  return decoded_size;
}
