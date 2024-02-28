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

#include "AuthenticationReject.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationReject::AuthenticationReject()
    : NasMmPlainHeader(k5gsMobilityManagementMessages, kAuthenticationReject) {
  ie_eap_message_ = std::nullopt;
}

//------------------------------------------------------------------------------
AuthenticationReject::~AuthenticationReject() {}

//------------------------------------------------------------------------------
void AuthenticationReject::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void AuthenticationReject::SetEapMessage(const bstring& eap) {
  ie_eap_message_ = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
int AuthenticationReject::Encode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding AuthenticationReject message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if ((encoded_ie_size = NasHelper::Encode(
           ie_eap_message_, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded AuthenticationReject message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationReject::Decode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding AuthenticationReject message");

  int decoded_size    = 0;
  int decoded_ie_size = 0;
  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // IEIs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  while ((octet != 0x0)) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .debug("Decoding IEI (0x%x)", octet);
    switch (octet) {
      case kIeiEapMessage: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_eap_message_, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .warn("Unknown IEI 0x%x, stop decoding...", octet);
        // Stop decoding
        octet = 0x00;
      } break;
    }
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded AuthenticationReject message len (%d)", decoded_size);
  return decoded_size;
}
