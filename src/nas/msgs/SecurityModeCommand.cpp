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

#include "NasHelper.hpp"

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
  if ((encoded_ie_size = NasHelper::Encode(
           ie_selected_nas_security_algorithms, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // NgKSI
  if ((encoded_ie_size = NasHelper::Encode(
           ie_ng_ksi, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  if (encoded_ie_size == 0)
    encoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet

  // UE security capability
  if ((encoded_ie_size = NasHelper::Encode(
           ie_ue_security_capability, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // IMEISV Request
  if ((encoded_ie_size = NasHelper::Encode(
           ie_imeisv_request, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // EPS NAS Security Algorithms
  if ((encoded_ie_size = NasHelper::Encode(
           ie_eps_nas_security_algorithms, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Additional 5G security information
  if ((encoded_ie_size = NasHelper::Encode(
           ie_additional_5g_security_information, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // EAP message
  if ((encoded_ie_size = NasHelper::Encode(
           ie_eap_message, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // ABBA
  if ((encoded_ie_size = NasHelper::Encode(ie_abba, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Replayed S1 UE Security Capability
  if ((encoded_ie_size = NasHelper::Encode(
           ie_s1_ue_security_capability, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Encoded SecurityModeCommand message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SecurityModeCommand::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding SecurityModeCommand message");
  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // NAS security algorithms
  if ((decoded_ie_size = NasHelper::Decode(
           ie_selected_nas_security_algorithms, buf, len, decoded_size,
           false)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // NAS key set identifier
  if ((decoded_ie_size = NasHelper::Decode(
           ie_ng_ksi, buf, len, decoded_size, false, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  if (decoded_ie_size == 0)
    decoded_size++;  // 1/2 octet for ngKSI, 1/2 for Spare half octet

  // UE security capability
  if ((decoded_ie_size = NasHelper::Decode(
           ie_ue_security_capability, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

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
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_imeisv_request, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
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
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_eps_nas_security_algorithms, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAdditional5gSecurityInformation: {
        Logger::nas_mm().debug(
            "decoding IEI 0x%x", kIeiAdditional5gSecurityInformation);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_additional_5g_security_information, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiEapMessage: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiEapMessage);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_eap_message, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAbba: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiAbba);
        if ((decoded_ie_size =
                 NasHelper::Decode(ie_abba, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiS1UeSecurityCapability: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiS1UeSecurityCapability);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_s1_ue_security_capability, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
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
