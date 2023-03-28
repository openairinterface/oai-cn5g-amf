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

#include "AuthenticationResponse.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationResponse::AuthenticationResponse()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, AUTHENTICATION_RESPONSE) {
  ie_authentication_response_parameter = std::nullopt;
  ie_eap_message                       = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationResponse::~AuthenticationResponse() {}

//------------------------------------------------------------------------------
void AuthenticationResponse::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationResponse::SetAuthenticationResponseParameter(
    const bstring& para) {
  ie_authentication_response_parameter =
      std::make_optional<AuthenticationResponseParameter>(para);
}

//------------------------------------------------------------------------------
bool AuthenticationResponse::GetAuthenticationResponseParameter(
    bstring& para) const {
  if (ie_authentication_response_parameter.has_value()) {
    ie_authentication_response_parameter.value().GetValue(para);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void AuthenticationResponse::SetEapMessage(const bstring& eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
bool AuthenticationResponse::GetEapMessage(bstring& eap) const {
  if (ie_eap_message.has_value()) {
    ie_eap_message.value().GetValue(eap);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
int AuthenticationResponse::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationResponse message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if (!ie_authentication_response_parameter.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        AuthenticationResponseParameter::GetIeName().c_str());
  } else {
    int size = ie_authentication_response_parameter.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error",
          AuthenticationResponseParameter::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", EapMessage::GetIeName().c_str());
  } else {
    int size =
        ie_eap_message.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", EapMessage::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationResponse message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationResponse::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding AuthenticationResponse message");
  int decoded_size   = 0;
  int decoded_result = 0;
  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiAuthenticationResponseParameter: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiAuthenticationResponseParameter);
        AuthenticationResponseParameter
            ie_authentication_response_parameter_tmp = {};
        if ((decoded_result = ie_authentication_response_parameter_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_authentication_response_parameter =
            std::optional<AuthenticationResponseParameter>(
                ie_authentication_response_parameter_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEapMessage: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEapMessage);
        EapMessage ie_eap_message_tmp = {};
        if ((decoded_result = ie_eap_message_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_eap_message = std::optional<EapMessage>(ie_eap_message_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        Logger::nas_mm().warn("Unknown IEI 0x%x, stop decoding...", octet);
        // Stop decoding
        octet = 0x00;
      } break;
    }
  }

  Logger::nas_mm().debug(
      "Decoded AuthenticationResponse message len (%d)", decoded_size);
  return decoded_size;
}
