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

#include "RegistrationReject.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
RegistrationReject::RegistrationReject()
    : NasMmPlainHeader(k5gsMobilityManagementMessages, kRegistrationReject) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Initiating RegistrationReject");
  ie_t3346_value_    = std::nullopt;
  ie_t3502_value_    = std::nullopt;
  ie_eap_message_    = std::nullopt;
  ie_rejected_nssai_ = std::nullopt;
}

//------------------------------------------------------------------------------
RegistrationReject::~RegistrationReject() {}

//------------------------------------------------------------------------------
void RegistrationReject::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void RegistrationReject::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause_.SetValue(value);
}

//------------------------------------------------------------------------------
void RegistrationReject::SetT3346(uint8_t value) {
  ie_t3346_value_ = std::make_optional<GprsTimer2>(kT3346Value, value);
}

//------------------------------------------------------------------------------
void RegistrationReject::SetT3502(uint8_t value) {
  ie_t3502_value_ = std::make_optional<GprsTimer2>(kT3502Value, value);
}

//------------------------------------------------------------------------------
void RegistrationReject::SetEapMessage(const bstring& eap) {
  ie_eap_message_ = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
void RegistrationReject::SetRejectedNssai(
    const std::vector<RejectedSNssai>& nssai) {
  ie_rejected_nssai_ = std::make_optional<RejectedNssai>(kIeiRejectedNssaiRr);
  ie_rejected_nssai_.value().SetRejectedSNssais(nssai);
}

//------------------------------------------------------------------------------
int RegistrationReject::Encode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding RegistrationReject message");
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

  // 5GMM Cause
  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gmm_cause_, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Timer 3346
  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3346_value_, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Timer T3502
  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3502_value_, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // EAP Message
  if ((encoded_ie_size = NasHelper::Encode(
           ie_eap_message_, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Rejected NSSAI
  if ((encoded_ie_size = NasHelper::Encode(
           ie_rejected_nssai_, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded RegistrationReject message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RegistrationReject::Decode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding RegistrationReject message");
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

  // 5GMM Cause
  if ((decoded_ie_size =
           NasHelper::Decode(ie_5gmm_cause_, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .debug("IEI 0x%x", octet);
    switch (octet) {
      case kT3346Value: {
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Decoding IEI 0x%x", kT3346Value);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3346_value_, kT3346Value, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Next IEI (0x%x)", octet);
      } break;

      case kT3502Value: {
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Decoding IEI 0x%x", kT3502Value);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3502_value_, kT3502Value, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEapMessage: {
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Decoding IEI 0x%x", kIeiEapMessage);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_eap_message_, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiRejectedNssaiRr: {
        oai::logger::logger_registry::get_logger(LOGGER_COMMON)
            .debug("Decoding IEI 0x%x", kIeiRejectedNssaiRr);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_rejected_nssai_, kIeiRejectedNssaiRr, buf, len,
                 decoded_size, true)) == KEncodeDecodeError) {
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
      .debug("Decoded RegistrationReject message len(%d)", decoded_size);
  return decoded_size;
}
