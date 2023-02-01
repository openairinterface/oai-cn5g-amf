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

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
RegistrationReject::RegistrationReject()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, REGISTRATION_REJECT) {
  Logger::nas_mm().debug("Initiating RegistrationReject");
  ie_T3346_value    = std::nullopt;
  ie_T3502_value    = std::nullopt;
  ie_eap_message    = std::nullopt;
  ie_rejected_nssai = std::nullopt;
}

//------------------------------------------------------------------------------
RegistrationReject::~RegistrationReject() {}

//------------------------------------------------------------------------------
void RegistrationReject::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void RegistrationReject::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause.SetValue(value);
}

//------------------------------------------------------------------------------
void RegistrationReject::setGPRS_Timer_2_3346(uint8_t value) {
  ie_T3346_value = std::make_optional<GprsTimer2>(kT3346Value, value);
}

//------------------------------------------------------------------------------
void RegistrationReject::setGPRS_Timer_2_3502(uint8_t value) {
  ie_T3502_value = std::make_optional<GprsTimer2>(kT3502Value, value);
}

//------------------------------------------------------------------------------
void RegistrationReject::SetEapMessage(bstring eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
void RegistrationReject::SetRejectedNssai(
    const std::vector<Rejected_SNSSAI>& nssai) {
  ie_rejected_nssai = std::make_optional<Rejected_NSSAI>(kIeiRejectedNssaiRr);
  ie_rejected_nssai.value().SetRejectedSNssais(nssai);
}

//------------------------------------------------------------------------------
int RegistrationReject::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding RegistrationReject message");
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
  if (int size = ie_5gmm_cause.Encode(buf + encoded_size, len - encoded_size)) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_5gmm_cause error");
  }

  if (!ie_T3346_value.has_value()) {
    Logger::nas_mm().warn("IE ie_T3346_value is not available");
  } else {
    if (int size = ie_T3346_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3346_value error");
      return 0;
    }
  }
  if (!ie_T3502_value.has_value()) {
    Logger::nas_mm().warn("IE ie_T3502_value is not available");
  } else {
    if (int size = ie_T3502_value.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3502_value error");
      return 0;
    }
  }
  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().warn("IE ie_eap_message is not available");
  } else {
    if (int size = ie_eap_message.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eap_message error");
      return 0;
    }
  }
  if (!ie_rejected_nssai.has_value()) {
    Logger::nas_mm().warn("IE ie_rejected_nssai is not available");
  } else {
    if (int size = ie_rejected_nssai.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_rejected_nssai error");
    }
  }
  Logger::nas_mm().debug(
      "Encoded RegistrationReject message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RegistrationReject::Decode(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationReject message");
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
  decoded_size +=
      ie_5gmm_cause.Decode(buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kT3346Value: {
        Logger::nas_mm().debug("Decoding IEI 0x5F: T3346 Value");
        GprsTimer2 ie_T3346_value_tmp(kT3346Value);
        decoded_size += ie_T3346_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_T3346_value = std::optional<GprsTimer2>(ie_T3346_value_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kT3502Value: {
        Logger::nas_mm().debug("Decoding IEI 0x16: T3502 Value");
        GprsTimer2 ie_T3502_value_tmp(kT3502Value);
        decoded_size += ie_T3502_value_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_T3502_value = std::optional<GprsTimer2>(ie_T3502_value_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEapMessage: {
        Logger::nas_mm().debug("Decoding IEI 0x78: EAP Message");
        EapMessage ie_eap_message_tmp = {};
        decoded_size += ie_eap_message_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_eap_message = std::optional<EapMessage>(ie_eap_message_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiRejectedNssaiRr: {
        Logger::nas_mm().debug("Decoding IEI 0x69: Rejected NSSAI");
        Rejected_NSSAI ie_rejected_nssai_tmp(kIeiRejectedNssaiRr);
        decoded_size += ie_rejected_nssai_tmp.Decode(
            buf + decoded_size, len - decoded_size, true);
        ie_rejected_nssai =
            std::optional<Rejected_NSSAI>(ie_rejected_nssai_tmp);
        octet = *(buf + decoded_size);
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
      "Decoded RegistrationReject message len(%d)", decoded_size);
  return decoded_size;
}
