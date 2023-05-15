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

#include "RegistrationComplete.hpp"

using namespace nas;

//------------------------------------------------------------------------------
RegistrationComplete::RegistrationComplete()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, REGISTRATION_COMPLETE) {
  ie_sor_transparent_container = std::nullopt;
}

//------------------------------------------------------------------------------
RegistrationComplete::~RegistrationComplete() {}

//------------------------------------------------------------------------------
void RegistrationComplete::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void RegistrationComplete::SetSorTransparentContainer(
    uint8_t header,
    const uint8_t (&value)[kSorTransparentContainerIeMacLength]) {
  ie_sor_transparent_container =
      std::make_optional<SorTransparentContainer>(header, value);
}

//------------------------------------------------------------------------------
int RegistrationComplete::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding RegistrationComplete message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if (!ie_sor_transparent_container.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", SorTransparentContainer::GetIeName().c_str());
  } else {
    encoded_ie_size = ie_sor_transparent_container.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", SorTransparentContainer::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded RegistrationComplete message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RegistrationComplete::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationComplete message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiSorTransparentContainer: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiSorTransparentContainer);
        SorTransparentContainer ie_sor_transparent_container_tmp = {};
        if ((decoded_result = ie_sor_transparent_container_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_sor_transparent_container = std::optional<SorTransparentContainer>(
            ie_sor_transparent_container_tmp);
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
      "Decoded RegistrationComplete message (len %d)", decoded_size);
  return decoded_size;
}
