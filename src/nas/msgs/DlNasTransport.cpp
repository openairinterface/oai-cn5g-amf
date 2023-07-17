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

#include "DlNasTransport.hpp"

#include "bstrlib.h"

using namespace nas;

//------------------------------------------------------------------------------
DlNasTransport::DlNasTransport()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, DL_NAS_TRANSPORT) {
  ie_pdu_session_identity_2 = std::nullopt;
  ie_additional_information = std::nullopt;
  ie_5gmm_cause             = std::nullopt;
  ie_back_off_timer_value   = std::nullopt;
}

//------------------------------------------------------------------------------
DlNasTransport::~DlNasTransport() {}

//------------------------------------------------------------------------------
void DlNasTransport::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void DlNasTransport::SetPayloadContainerType(uint8_t value) {
  ie_payload_container_type.SetValue(value);
}

//------------------------------------------------------------------------------
void DlNasTransport::SetPayloadContainer(
    const std::vector<PayloadContainerEntry>& content) {
  ie_payload_container.SetValue(content);
}

//------------------------------------------------------------------------------
void DlNasTransport::SetPayloadContainer(uint8_t* buf, int len) {
  bstring b = blk2bstr(buf, len);
  ie_payload_container.SetValue(b);
}

//------------------------------------------------------------------------------
void DlNasTransport::SetPduSessionId(uint8_t value) {
  ie_pdu_session_identity_2 =
      std::make_optional<PduSessionIdentity2>(kIeiPduSessionId, value);
}

//------------------------------------------------------------------------------
void DlNasTransport::SetAdditionalInformation(const bstring& value) {
  ie_additional_information = std::make_optional<AdditionalInformation>(value);
}

//------------------------------------------------------------------------------
void DlNasTransport::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause = std::make_optional<_5gmmCause>(kIei5gmmCause, value);
}

//------------------------------------------------------------------------------
void DlNasTransport::SetBackOffTimerValue(uint8_t unit, uint8_t value) {
  ie_back_off_timer_value =
      std::make_optional<GprsTimer3>(kIeiGprsTimer3BackOffTimer, unit, value);
}

//------------------------------------------------------------------------------
int DlNasTransport::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding DlNasTransport message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // Payload container type
  int size =
      ie_payload_container_type.Encode(buf + encoded_size, len - encoded_size);
  if (size == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Encoding %s error", PayloadContainerType::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  if (size == 0)
    size++;  // 1/2 octet for  ie_payload_container_type, 1/2 octet for spare
  encoded_size += size;

  // Payload container
  size = ie_payload_container.Encode(
      buf + encoded_size, len - encoded_size,
      ie_payload_container_type.GetValue());
  if (size != KEncodeDecodeError) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", PayloadContainer::GetIeName().c_str());
    return KEncodeDecodeError;
  }

  // PDU session ID
  if (!ie_pdu_session_identity_2.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", PduSessionIdentity2::GetIeName().c_str());
  } else {
    size = ie_pdu_session_identity_2->Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding IE %s", PduSessionIdentity2::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  // Additional information
  if (!ie_additional_information.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", AdditionalInformation::GetIeName().c_str());
  } else {
    size = ie_additional_information->Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", AdditionalInformation::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  // 5GMM cause
  if (!ie_5gmm_cause.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", _5gmmCause::GetIeName().c_str());
  } else {
    size = ie_5gmm_cause->Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", _5gmmCause::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  // Back-off timer value
  if (!ie_back_off_timer_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer3::GetIeName().c_str());
  } else {
    size =
        ie_back_off_timer_value->Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer3::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded DlNasTransport message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DlNasTransport::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding DlNasTransport message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // Payload container type
  decoded_result = ie_payload_container_type.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", PayloadContainerType::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size++;  // 1/2 octet for PayloadContainerType, 1/2 octet for spare

  // Payload container
  decoded_result = ie_payload_container.Decode(
      buf + decoded_size, len - decoded_size, false,
      N1_SM_INFORMATION);  // TODO: verified Type of Payload Container
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
  while ((octet != 0x0)) {
    switch (octet) {
      case kIeiPduSessionId: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionId);
        PduSessionIdentity2 ie_pdu_session_identity_2_tmp = {};
        if ((decoded_result = ie_pdu_session_identity_2_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", PduSessionIdentity2::GetIeName().c_str());
          return decoded_result;
        }
        decoded_size += decoded_result;
        ie_pdu_session_identity_2 =
            std::optional<PduSessionIdentity2>(ie_pdu_session_identity_2_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAdditionalInformation: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiAdditionalInformation);
        AdditionalInformation ie_additional_information_tmp = {};
        if ((decoded_result = ie_additional_information_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", AdditionalInformation::GetIeName().c_str());
          return decoded_result;
        }
        decoded_size += decoded_result;
        ie_additional_information =
            std::optional<AdditionalInformation>(ie_additional_information_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gmmCause: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIei5gmmCause);
        _5gmmCause ie_5gmm_cause_tmp = {};
        if ((decoded_result = ie_5gmm_cause_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", _5gmmCause::GetIeName().c_str());
          return decoded_result;
        }
        decoded_size += decoded_result;
        ie_5gmm_cause = std::optional<_5gmmCause>(ie_5gmm_cause_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3BackOffTimer: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3BackOffTimer);
        GprsTimer3 ie_back_off_timer_value_tmp(kIeiGprsTimer3BackOffTimer);
        if ((decoded_result = ie_back_off_timer_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer3::GetIeName().c_str());
          return decoded_result;
        }
        decoded_size += decoded_result;
        ie_back_off_timer_value =
            std::optional<GprsTimer3>(ie_back_off_timer_value_tmp);
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
      "Decoded DlNasTransport message len (%d)", decoded_size);
  return decoded_size;
}
