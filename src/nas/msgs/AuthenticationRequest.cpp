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

#include "AuthenticationRequest.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationRequest::AuthenticationRequest()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, AUTHENTICATION_REQUEST) {
  ie_authentication_parameter_rand = std::nullopt;
  ie_authentication_parameter_autn = std::nullopt;
  ie_eap_message                   = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationRequest::~AuthenticationRequest() {}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetNgKsi(uint8_t tsc, uint8_t key_set_id) {
  ie_ng_ksi.Set(false);  // 4 lower bits
  ie_ng_ksi.SetNasKeyIdentifier(key_set_id);
  ie_ng_ksi.SetTypeOfSecurityContext(tsc);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetAbba(uint8_t length, uint8_t* value) {
  ie_abba.Set(length, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetAuthenticationParameterRand(
    uint8_t value[kAuthenticationParameterRandValueLength]) {
  ie_authentication_parameter_rand =
      std::make_optional<AuthenticationParameterRand>(
          kIeiAuthenticationParameterRand, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetAuthenticationParameterAutn(
    uint8_t value[kAuthenticationParameterAutnValueLength]) {
  ie_authentication_parameter_autn =
      std::make_optional<AuthenticationParameterAutn>(
          kIeiAuthenticationParameterAutn, value);
}

//------------------------------------------------------------------------------
void AuthenticationRequest::SetEapMessage(const bstring& eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
int AuthenticationRequest::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationRequest message");
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
           ie_ng_ksi, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  // Spare half octet
  if (encoded_ie_size == 0)
    encoded_size++;  // 1/2 octet + 1/2 octet from ie_ng_ksi

  // ABBA
  if ((encoded_ie_size = NasHelper::Encode(ie_abba, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Authentication parameter RAND
  if ((encoded_ie_size = NasHelper::Encode(
           ie_authentication_parameter_rand, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Authentication parameter AUTN
  if ((encoded_ie_size = NasHelper::Encode(
           ie_authentication_parameter_autn, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // EAP message
  if ((encoded_ie_size = NasHelper::Encode(
           ie_eap_message, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationRequest message (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationRequest::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationReject message");
  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // NgKSI
  if ((decoded_ie_size = NasHelper::Decode(
           ie_ng_ksi, buf, len, decoded_size, false, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  if (decoded_ie_size == 0)
    decoded_size++;  // 1/2 octet from ie_ng_ksi, 1/2 from Spare half octet

  // ABBA
  if ((decoded_ie_size = NasHelper::Decode(
           ie_abba, buf, len, decoded_size, false)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  Logger::nas_mm().debug("Decoded_size %d", decoded_size);

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI 0x%x", octet);
  while ((octet != 0x0)) {
    Logger::nas_mm().debug("Decoding IEI 0x%x", octet);
    switch (octet) {
      case kIeiAuthenticationParameterRand: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_authentication_parameter_rand, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;

      case kIeiAuthenticationParameterAutn: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_authentication_parameter_autn, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;

      case kIeiEapMessage: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_eap_message, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;

      default: {
        Logger::nas_mm().warn("Unknown IEI 0x%x, stop decoding...", octet);
        // Stop decoding
        octet = 0x00;
      } break;
    }
  }

  Logger::nas_mm().debug(
      "Decoded AuthenticationRequest message (len %d)", decoded_size);
  return decoded_size;
}
