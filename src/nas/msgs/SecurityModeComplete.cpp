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

#include "SecurityModeComplete.hpp"

#include "3gpp_24.501.hpp"
#include "Ie_Const.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SecurityModeComplete::SecurityModeComplete()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, SECURITY_MODE_COMPLETE) {
  ie_imeisv                = std::nullopt;
  ie_nas_message_container = std::nullopt;
  ie_non_imeisvpei         = std::nullopt;
};

//------------------------------------------------------------------------------
SecurityModeComplete::~SecurityModeComplete() {}

//------------------------------------------------------------------------------
void SecurityModeComplete::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void SecurityModeComplete::SetImeisv(IMEISV_t imeisv) {
  ie_imeisv =
      std::make_optional<_5GSMobileIdentity>(kIei5gsMobileIdentityImeiSv);
  // ie_imeisv->SetIei(kIei5gsMobileIdentityImeiSv);
  ie_imeisv.value().SetImeisv(imeisv);
}

//------------------------------------------------------------------------------
void SecurityModeComplete::SetNasMessageContainer(bstring value) {
  ie_nas_message_container = std::make_optional<NasMessageContainer>(value);
}

//------------------------------------------------------------------------------
void SecurityModeComplete::SetNonImeisv(IMEISV_t imeisv) {
  ie_non_imeisvpei =
      std::make_optional<_5GSMobileIdentity>(kIei5gsMobileIdentityNonImeiSvPei);
  // ie_non_imeisvpei->SetIei(kIei5gsMobileIdentityNonImeiSvPei);
  ie_non_imeisvpei.value().SetImeisv(imeisv);
}

//------------------------------------------------------------------------------
bool SecurityModeComplete::GetImeisv(IMEISV_t& imeisv) const {
  if (ie_imeisv.has_value()) {
    ie_imeisv.value().GetImeisv(imeisv);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
bool SecurityModeComplete::GetNasMessageContainer(bstring& nas) const {
  if (ie_nas_message_container.has_value()) {
    ie_nas_message_container.value().GetValue(nas);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
bool SecurityModeComplete::GetNonImeisv(IMEISV_t& imeisv) const {
  if (ie_non_imeisvpei.has_value()) {
    ie_non_imeisvpei.value().GetImeisv(imeisv);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
int SecurityModeComplete::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding SecurityModeComplete message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if (!ie_imeisv.has_value()) {
    Logger::nas_mm().warn("IE ie_imeisv is not available");
  } else {
    int size = ie_imeisv.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_imeisv error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_nas_message_container.has_value()) {
    Logger::nas_mm().warn("IE ie_nas_message_container is not available");
  } else {
    int size = ie_nas_message_container.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_nas_message_container error");
      return KEncodeDecodeError;
    }
  }

  if (!ie_non_imeisvpei.has_value()) {
    Logger::nas_mm().warn("IE ie_non_imeisvpei is not available");
  } else {
    int size =
        ie_non_imeisvpei.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_non_imeisvpei error");
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded SecurityModeComplete message len (%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int SecurityModeComplete::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding SecurityModeComplete message");

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

  // while ((octet != 0x0)) {
  while (len - decoded_size > 0) {
    uint8_t octet = *(buf + decoded_size);
    Logger::nas_mm().debug("Optional IEI (0x%x)", octet);
    switch (octet) {
      case kIei5gsMobileIdentityImeiSv: {
        Logger::nas_mm().debug("Decoding IEI (0x77)");
        _5GSMobileIdentity ie_imeisv_tmp = {};
        if ((decoded_result = ie_imeisv_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_imeisv = std::optional<_5GSMobileIdentity>(ie_imeisv_tmp);
      } break;

      case kIeiNasMessageContainer: {
        Logger::nas_mm().debug("Decoding IEI (0x71)");
        NasMessageContainer ie_nas_message_container_tmp = {};
        if ((decoded_result = ie_nas_message_container_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_nas_message_container =
            std::optional<NasMessageContainer>(ie_nas_message_container_tmp);
      } break;

      case kIei5gsMobileIdentityNonImeiSvPei: {
        Logger::nas_mm().debug("Decoding IEI (0x78)");
        _5GSMobileIdentity ie_non_imeisvpei_tmp = {};
        if ((decoded_result = ie_non_imeisvpei_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_non_imeisvpei =
            std::optional<_5GSMobileIdentity>(ie_non_imeisvpei_tmp);
      } break;

      default: {
        Logger::nas_mm().warn("Unexpected IEI (0x%x)", octet);
        return decoded_size;
      }
    }
  }

  Logger::nas_mm().debug(
      "Decoded SecurityModeComplete message len (%d)", decoded_size);
  return decoded_size;
}
