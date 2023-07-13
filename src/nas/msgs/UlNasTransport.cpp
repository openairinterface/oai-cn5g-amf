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

#include "UlNasTransport.hpp"

using namespace nas;

//------------------------------------------------------------------------------
UlNasTransport::UlNasTransport()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, UL_NAS_TRANSPORT) {
  ie_pdu_session_id                = std::nullopt;
  ie_old_pdu_session_id            = std::nullopt;
  ie_request_type                  = std::nullopt;
  ie_s_nssai                       = std::nullopt;
  ie_dnn                           = std::nullopt;
  ie_additional_information        = std::nullopt;
  ie_ma_pdu_session_information    = std::nullopt;
  ie_release_assistance_indication = std::nullopt;
}

//------------------------------------------------------------------------------
UlNasTransport::~UlNasTransport() {}

//------------------------------------------------------------------------------
void UlNasTransport::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void UlNasTransport::SetPayloadContainerType(uint8_t value) {
  ie_payload_container_type.SetValue(value);
}

//------------------------------------------------------------------------------
uint8_t UlNasTransport::GetPayloadContainerType() const {
  return ie_payload_container_type.GetValue();
}

//------------------------------------------------------------------------------
void UlNasTransport::SetPayloadContainer(
    const std::vector<PayloadContainerEntry>& content) {
  ie_payload_container.SetValue(content);
}

//------------------------------------------------------------------------------
void UlNasTransport::GetPayloadContainer(bstring& content) const {
  ie_payload_container.GetValue(content);
}

//------------------------------------------------------------------------------
void UlNasTransport::GetPayloadContainer(
    std::vector<PayloadContainerEntry>& content) const {
  ie_payload_container.GetValue(content);
}

//------------------------------------------------------------------------------
void UlNasTransport::SetPduSessionId(uint8_t value) {
  ie_pdu_session_id =
      std::make_optional<PduSessionIdentity2>(kIeiPduSessionId, value);
}

//------------------------------------------------------------------------------
bool UlNasTransport::GetPduSessionId(uint8_t& value) const {
  if (ie_pdu_session_id.has_value()) {
    value = ie_pdu_session_id.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void UlNasTransport::SetOldPduSessionId(uint8_t value) {
  ie_old_pdu_session_id =
      std::make_optional<PduSessionIdentity2>(kIeiOldPduSessionId, value);
}

//------------------------------------------------------------------------------
bool UlNasTransport::GetOldPduSessionId(uint8_t& value) const {
  if (ie_old_pdu_session_id.has_value()) {
    value = ie_old_pdu_session_id.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void UlNasTransport::SetRequestType(uint8_t value) {
  ie_request_type = std::make_optional<RequestType>(value);
}

//------------------------------------------------------------------------------
bool UlNasTransport::GetRequestType(uint8_t& value) const {
  if (ie_request_type.has_value()) {
    value = ie_request_type.value().GetValue();
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void UlNasTransport::SetSNssai(const SNSSAI_s& snssai) {
  ie_s_nssai =
      std::make_optional<SNssai>(std::optional<uint8_t>{kIeiSNssai}, snssai);
}

//------------------------------------------------------------------------------
bool UlNasTransport::GetSNssai(SNSSAI_s& snssai) const {
  if (ie_s_nssai.has_value()) {
    ie_s_nssai.value().getValue(snssai);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void UlNasTransport::SetDnn(const bstring& dnn) {
  ie_dnn = std::make_optional<Dnn>(dnn);
}

//------------------------------------------------------------------------------
bool UlNasTransport::GetDnn(bstring& dnn) const {
  if (ie_dnn.has_value()) {
    ie_dnn.value().GetValue(dnn);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void UlNasTransport::SetAdditionalInformation(const bstring& value) {
  ie_additional_information = std::make_optional<AdditionalInformation>(value);
}

//------------------------------------------------------------------------------
void UlNasTransport::SetMaPduSessionInformation(uint8_t value) {
  ie_ma_pdu_session_information =
      std::make_optional<MaPduSessionInformation>(value);
}

//------------------------------------------------------------------------------
void UlNasTransport::SetReleaseAssistanceIndication(uint8_t value) {
  ie_release_assistance_indication =
      std::make_optional<ReleaseAssistanceIndication>(value);
}

//------------------------------------------------------------------------------
int UlNasTransport::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding UL NAS Transport message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  int size =
      ie_payload_container_type.Encode(buf + encoded_size, len - encoded_size);
  if (size == KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding ie_payload_container_type error");
    return KEncodeDecodeError;
  }
  if (size == 0)
    size++;  // 1/2 octet for  ie_payload_container_type, 1/2 octet for spare
  encoded_size += size;

  size = ie_payload_container.Encode(
      buf + encoded_size, len - encoded_size,
      ie_payload_container_type.GetValue());
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Decoding %s error", PayloadContainer::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  if (!ie_pdu_session_id.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", PduSessionIdentity2::GetIeName().c_str());
  } else {
    size = ie_pdu_session_id.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", PduSessionIdentity2::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_old_pdu_session_id.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", PduSessionIdentity2::GetIeName().c_str());
  } else {
    size = ie_old_pdu_session_id.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", PduSessionIdentity2::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_request_type.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", RequestType::GetIeName().c_str());
  } else {
    size =
        ie_request_type.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", RequestType::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_s_nssai.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", SNssai::GetIeName().c_str());
  } else {
    size = ie_s_nssai.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding %s error", SNssai::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_dnn.has_value()) {
    Logger::nas_mm().debug("IE %s is not available", Dnn::GetIeName().c_str());
  } else {
    size = ie_dnn.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding %s error", Dnn::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_additional_information.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", AdditionalInformation::GetIeName().c_str());
  } else {
    size = ie_additional_information.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", AdditionalInformation::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_ma_pdu_session_information.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", MaPduSessionInformation::GetIeName().c_str());
  } else {
    size = ie_ma_pdu_session_information.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", MaPduSessionInformation::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_release_assistance_indication.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        ReleaseAssistanceIndication::GetIeName().c_str());
  } else {
    size = ie_release_assistance_indication.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error",
          ReleaseAssistanceIndication::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded UL NAS Transport message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UlNasTransport::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding UlNasTransport message");
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // Payload Container Type
  decoded_result = ie_payload_container_type.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", PayloadContainerType::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;
  decoded_size++;  // 1/2 octet for PayloadContainerType, 1/2 octet for spare

  // Payload Container
  decoded_result = ie_payload_container.Decode(
      buf + decoded_size, len - decoded_size, false,
      ie_payload_container_type.GetValue());
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", PayloadContainer::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  bool flag = false;

  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case kIeiRequestType: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiRequestType);
        RequestType ie_request_type_tmp = {};
        if ((decoded_result = ie_request_type_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_request_type = std::optional<RequestType>(ie_request_type_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiMaPduSessionInformation: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiMaPduSessionInformation);
        MaPduSessionInformation ie_ma_pdu_session_information_tmp = {};
        if ((decoded_result = ie_ma_pdu_session_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_ma_pdu_session_information = std::optional<MaPduSessionInformation>(
            ie_ma_pdu_session_information_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiReleaseAssistanceIndication: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiReleaseAssistanceIndication);
        ReleaseAssistanceIndication ie_release_assistance_indication_tmp = {};
        if ((decoded_result = ie_release_assistance_indication_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_release_assistance_indication =
            std::optional<ReleaseAssistanceIndication>(
                ie_release_assistance_indication_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      default: {
        flag = true;
      }
    }

    switch (octet) {
      case kIeiPduSessionId: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionId);
        PduSessionIdentity2 ie_pdu_session_id_tmp = {};
        if ((decoded_result = ie_pdu_session_id_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_pdu_session_id =
            std::optional<PduSessionIdentity2>(ie_pdu_session_id_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiOldPduSessionId: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiOldPduSessionId);
        PduSessionIdentity2 ie_old_pdu_session_id_tmp = {};
        if ((decoded_result = ie_old_pdu_session_id_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_old_pdu_session_id =
            std::optional<PduSessionIdentity2>(ie_old_pdu_session_id_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiSNssai: {
        Logger::nas_mm().debug("Decoding IEI (0x22)");
        auto s_nssai_ie = std::optional<uint8_t>(kIeiSNssai);
        SNssai ie_s_nssai_tmp(s_nssai_ie);
        if ((decoded_result = ie_s_nssai_tmp.Decode(
                 buf + decoded_size, len - decoded_size, kIeIsOptional)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_s_nssai = std::optional<SNssai>(ie_s_nssai_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiDnn: {
        Logger::nas_mm().debug("Decoding IEI (0x25)");
        Dnn ie_dnn_tmp = {};
        if ((decoded_result = ie_dnn_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_dnn = std::optional<Dnn>(ie_dnn_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAdditionalInformation: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiAdditionalInformation);
        AdditionalInformation ie_additional_information_tmp = {};
        if ((decoded_result = ie_additional_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return KEncodeDecodeError;
        decoded_size += decoded_result;
        ie_additional_information =
            std::optional<AdditionalInformation>(ie_additional_information_tmp);
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
    flag = false;
  }

  Logger::nas_mm().debug(
      "Decoded UlNasTransport message len (%d)", decoded_size);
  return decoded_size;
}
