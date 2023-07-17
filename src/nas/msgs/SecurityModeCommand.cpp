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

using namespace nas;

//------------------------------------------------------------------------------
SecurityModeCommand::SecurityModeCommand()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, SECURITY_MODE_COMMAND) {
  ie_imeisv_request                     = std::nullopt;
  ie_eps_nas_security_algorithms        = std::nullopt;
  ie_additional_5g_security_information = std::nullopt;
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
  ie_ng_ksi.SetTypeOfSecurityContext(tsc);
  ie_ng_ksi.SetNasKeyIdentifier(key_set_id);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetUeSecurityCapability(uint8_t ea, uint8_t ia) {
  ie_ue_security_capability.Set(ea, ia);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetUeSecurityCapability(
    uint8_t ea, uint8_t ia, uint8_t eea, uint8_t eia) {
  ie_ue_security_capability.Set(ea, ia, eea, eia);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetUeSecurityCapability(
    const UeSecurityCapability& ue_security_capability) {
  uint8_t eea = 0;
  uint8_t eia = 0;
  if (ue_security_capability.GetEea(eea) &&
      ue_security_capability.GetEia(eia)) {
    ie_ue_security_capability.Set(
        ue_security_capability.GetEa(), ue_security_capability.GetIa(), eea,
        eia);
  } else {
    ie_ue_security_capability.Set(
        ue_security_capability.GetEa(), ue_security_capability.GetIa());
  }
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
  ie_additional_5g_security_information =
      std::make_optional<Additional5gSecurityInformation>(rinmr, hdp);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetEapMessage(bstring eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetAbba(uint8_t length, uint8_t* value) {
  ie_abba = std::make_optional<Abba>(kIeiAbba, length, value);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::SetS1UeSecurityCapability(uint8_t eea, uint8_t eia) {
  ie_s1_ue_security_capability = std::make_optional<S1UeSecurityCapability>(
      kIeiS1UeSecurityCapability, eea, eia);
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
        "Encoding %s error", NasSecurityAlgorithms::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  // NAS key set identifier
  size = ie_ng_ksi.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    // 1/2 octet for ngKSI, 1/2 for Spare half octet
    // TODO:do it in NAS Key Set Identifier
    uint8_t octet    = 0;
    int decoded_size = 0;
    DECODE_U8(buf + encoded_size, octet, decoded_size);
    // clear spare half
    octet = octet & 0x0f;
    ENCODE_U8(buf + encoded_size, octet, encoded_size);
    // encoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", NasKeySetIdentifier::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  // UE security capability
  size =
      ie_ue_security_capability.Encode(buf + encoded_size, len - encoded_size);
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", UeSecurityCapability::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  // Optional IEs
  if (!ie_imeisv_request.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", ImeisvRequest::GetIeName().c_str());
  } else {
    size = ie_imeisv_request.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", ImeisvRequest::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_eps_nas_security_algorithms.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        EpsNasSecurityAlgorithms::GetIeName().c_str());
  } else {
    size = ie_eps_nas_security_algorithms.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", EpsNasSecurityAlgorithms::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_additional_5g_security_information.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        Additional5gSecurityInformation::GetIeName().c_str());
  } else {
    size = ie_additional_5g_security_information.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error",
          Additional5gSecurityInformation::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", EapMessage::GetIeName().c_str());
  } else {
    size =
        ie_eap_message.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", EapMessage::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

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

  if (!ie_s1_ue_security_capability.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", S1UeSecurityCapability::GetIeName().c_str());
  } else {
    size = ie_s1_ue_security_capability.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", S1UeSecurityCapability::GetIeName().c_str());
      return KEncodeDecodeError;
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
      ie_ng_ksi.Decode(buf + decoded_size, len - decoded_size, false, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet

  // UE security capability
  decoded_result = ie_ue_security_capability.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  bool flag = false;
  while ((octet != 0x0)) {
    Logger::nas_mm().debug("IEI 0x%x", octet);
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
        DECODE_U8_VALUE(buf + decoded_size, octet);
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
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAdditional5gSecurityInformation: {
        Logger::nas_mm().debug(
            "decoding IEI 0x%x", kIeiAdditional5gSecurityInformation);
        Additional5gSecurityInformation
            ie_additional_5g_security_information_tmp = {};
        if ((decoded_result = ie_additional_5g_security_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_additional_5g_security_information =
            std::optional<Additional5gSecurityInformation>(
                ie_additional_5g_security_information_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
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
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAbba: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiAbba);
        Abba ie_abba_tmp = {};
        if ((decoded_result = ie_abba_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_abba = std::optional<Abba>(ie_abba_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiS1UeSecurityCapability: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiS1UeSecurityCapability);
        S1UeSecurityCapability ie_s1_ue_security_capability_tmp = {};
        if ((decoded_result = ie_s1_ue_security_capability_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_s1_ue_security_capability = std::optional<S1UeSecurityCapability>(
            ie_s1_ue_security_capability_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
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
