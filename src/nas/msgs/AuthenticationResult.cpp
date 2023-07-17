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

#include "AuthenticationResult.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationResult::AuthenticationResult()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, AUTHENTICATION_RESULT) {
  ie_abba = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationResult::~AuthenticationResult() {}

//------------------------------------------------------------------------------
void AuthenticationResult::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationResult::SetNgKsi(uint8_t tsc, uint8_t key_set_id) {
  ie_ng_ksi.Set(false);  // 4 lower bits
  ie_ng_ksi.SetTypeOfSecurityContext(tsc);
  ie_ng_ksi.SetNasKeyIdentifier(key_set_id);
}

//------------------------------------------------------------------------------
void AuthenticationResult::SetAbba(uint8_t length, uint8_t* value) {
  ie_abba = std::make_optional<Abba>(kIeiAbba, length, value);
}

//------------------------------------------------------------------------------
void AuthenticationResult::SetEapMessage(const bstring& eap) {
  ie_eap_message.SetValue(eap);
}

//------------------------------------------------------------------------------
int AuthenticationResult::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationResult message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // ngKSI
  int size = ie_ng_ksi.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", NasKeySetIdentifier::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  // Spare half octet
  encoded_size++;  // 1/2 octet + 1/2 octet from ie_ng_ksi

  // EAP message
  size = ie_eap_message.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", EapMessage::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  // ABBA
  if (!ie_abba.has_value()) {
    Logger::nas_mm().debug("IE %s is not available", Abba::GetIeName().c_str());
  } else {
    size = ie_abba.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding %s error", Abba::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationResult message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationResult::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding AuthenticationResult message");
  int decoded_size   = 0;
  int decoded_result = 0;
  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // NAS key set identifier
  decoded_result =
      ie_ng_ksi.Decode(buf + decoded_size, len - decoded_size, false, false);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", NasKeySetIdentifier::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet

  // EAP message
  decoded_result =
      ie_eap_message.Decode(buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", EapMessage::GetIeName().c_str());
    return decoded_result;
  }

  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiAbba: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiAbba);
        Abba ie_abba_tmp = {};
        if ((decoded_result = ie_abba_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", Abba::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_abba = std::optional<Abba>(ie_abba_tmp);
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
      "Decoded AuthenticationResult message len (%d)", decoded_size);
  return decoded_size;
}
