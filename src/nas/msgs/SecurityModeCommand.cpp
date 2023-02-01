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

#include "SecurityModeCommand.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SecurityModeCommand::SecurityModeCommand()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, SECURITY_MODE_COMMAND) {
  ie_imeisv_request                     = std::nullopt;
  ie_eps_nas_security_algorithms        = std::nullopt;
  ie_additional_5G_security_information = std::nullopt;
  ie_eap_message                        = std::nullopt;
  ie_abba                               = std::nullopt;
  ie_s1_ue_security_capability          = std::nullopt;
}

//------------------------------------------------------------------------------
SecurityModeCommand::~SecurityModeCommand() {}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetNasSecurityAlgorithms(
    uint8_t ciphering, uint8_t integrity) {
  ie_selected_nas_security_algorithms.Set(ciphering, integrity);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetNgKsi(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI.SetTypeOfSecurityContext(tsc);
  ie_ngKSI.SetNasKeyIdentifier(key_set_id);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetUeSecurityCapability(
    uint8_t g_EASel, uint8_t g_IASel) {
  ie_ue_security_capability.Set(g_EASel, g_IASel);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetUeSecurityCapability(
    uint8_t g_EASel, uint8_t g_IASel, uint8_t eea, uint8_t eia) {
  ie_ue_security_capability.Set(g_EASel, g_IASel, eea, eia);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetImeisvRequest(uint8_t value) {
  ie_imeisv_request = std::make_optional<ImeisvRequest>(value);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetEpsNasSecurityAlgorithms(
    uint8_t ciphering, uint8_t integrity) {
  ie_eps_nas_security_algorithms =
      std::make_optional<EpsNasSecurityAlgorithms>(ciphering, integrity);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetAdditional5gSecurityInformation(
    bool rinmr, bool hdp) {
  ie_additional_5G_security_information =
      std::make_optional<Additional5gSecurityInformation>(rinmr, hdp);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetEapMessage(bstring eap) {
  ie_eap_message = std::make_optional<EapMessage>(0x78, eap);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetAbba(uint8_t length, uint8_t* value) {
  ie_abba = std::make_optional<ABBA>(0x38, length, value);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetS1UeSecurityCapability(
    uint8_t g_EEASel, uint8_t g_EIASel) {
  ie_s1_ue_security_capability =
      std::make_optional<S1UeSecurityCapability>(0x19, g_EEASel, g_EIASel);
}

//------------------------------------------------------------------------------
int SecurityModeCommand::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding SecurityModeCommand message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // NAS security algorithms
  int size = ie_selected_nas_security_algorithms.Encode(
      buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Encoding ie_selected_nas_security_algorithms error");
    return KEncodeDecodeError;
  }

  // NAS key set identifier
  size = ie_ngKSI.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet
  } else {
    Logger::nas_mm().error("Encoding ie_ngKSI error");
    return KEncodeDecodeError;
  }

  // UE security capability
  size =
      ie_ue_security_capability.Encode(buf + encoded_size, len - encoded_size);

  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding ie_ue_security_capability error");
    return KEncodeDecodeError;
  }

  // Optional IEs
  if (!ie_imeisv_request.has_value()) {
    Logger::nas_mm().warn("IE ie_imeisv_request is not available");
  } else {
    if (int size = ie_imeisv_request.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_imeisv_request error");
      return 0;
    }
  }

  if (!ie_eps_nas_security_algorithms.has_value()) {
    Logger::nas_mm().warn("IE ie_eps_nas_security_algorithms is not available");
  } else {
    if (int size = ie_eps_nas_security_algorithms.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eps_nas_security_algorithms error");
      return 0;
    }
  }

  if (!ie_additional_5G_security_information.has_value()) {
    Logger::nas_mm().warn(
        "IE ie_additional_5G_security_information is not available");
  } else {
    if (int size = ie_additional_5G_security_information.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_additional_5G_security_information error");
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
      Logger::nas_mm().error("encoding ie_eap_message error");
      return 0;
    }
  }

  if (!ie_abba.has_value()) {
    Logger::nas_mm().warn("IE ie_abba is not available");
  } else {
    if (int size =
            ie_abba.value().Encode(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_abba error");
      return 0;
    }
  }

  if (!ie_s1_ue_security_capability.has_value()) {
    Logger::nas_mm().warn("IE ie_s1_ue_security_capability is not available");
  } else {
    if (int size = ie_s1_ue_security_capability.value().Encode(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_s1_ue_security_capability error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "Encoded SecurityModeCommand message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SecurityModeCommand::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding SecurityModeCommand message");
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // NAS security algorithms
  decoded_result = ie_selected_nas_security_algorithms.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_result;

  // NAS key set identifier
  decoded_result =
      ie_ngKSI.Decode(buf + decoded_size, len - decoded_size, false, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet

  // UE security capability
  decoded_result = ie_ue_security_capability.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  bool flag = false;
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case kIeiImeisvRequest: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiImeisvRequest);
        ImeisvRequest ie_imeisv_request_tmp = {};
        if ((decoded_result = ie_imeisv_request_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_imeisv_request = std::optional<ImeisvRequest>(ie_imeisv_request_tmp);
        octet             = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      default: {
        flag = true;
      }
    }

    switch (octet) {
      case kIeiEpsNasSecurityAlgorithms: {
        Logger::nas_mm().debug(
            "decoding IEI 0x%x", kIeiEpsNasSecurityAlgorithms);
        EpsNasSecurityAlgorithms ie_eps_nas_security_algorithms_tmp = {};
        if ((decoded_result = ie_eps_nas_security_algorithms_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_eps_nas_security_algorithms =
            std::optional<EpsNasSecurityAlgorithms>(
                ie_eps_nas_security_algorithms_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAdditional5gSecurityInformation: {
        Logger::nas_mm().debug(
            "decoding IEI 0x%x", kIeiAdditional5gSecurityInformation);
        Additional5gSecurityInformation
            ie_additional_5G_security_information_tmp = {};
        if ((decoded_result = ie_additional_5G_security_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_additional_5G_security_information =
            std::optional<Additional5gSecurityInformation>(
                ie_additional_5G_security_information_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEapMessage: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEapMessage);
        EapMessage ie_eap_message_tmp = {};
        if ((decoded_result = ie_eap_message_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_eap_message = std::optional<EapMessage>(ie_eap_message_tmp);
        octet          = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAbba: {
        Logger::nas_mm().debug("decoding IEI (0x38)");
        ABBA ie_abba_tmp = {};
        if ((decoded_result = ie_abba_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_abba = std::optional<ABBA>(ie_abba_tmp);
        octet   = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case 0x19: {
        Logger::nas_mm().debug("decoding IEI (0x19)");
        S1UeSecurityCapability ie_s1_ue_security_capability_tmp = {};
        if ((decoded_result = ie_s1_ue_security_capability_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_s1_ue_security_capability = std::optional<S1UeSecurityCapability>(
            ie_s1_ue_security_capability_tmp);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        // TODO:
        if (flag) {
          Logger::nas_mm().warn("Unknown IEI 0x%x, stop decoding...", octet);
          // Stop decoding
          octet = 0x00;
        }
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded SecurityModeCommand message len (%d)", decoded_size);
  return decoded_size;
}
