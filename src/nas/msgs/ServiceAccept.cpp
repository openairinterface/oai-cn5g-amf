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

#include "ServiceAccept.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ServiceAccept::ServiceAccept()
    : NasMmPlainHeader(EPD_5GS_MM_MSG, SERVICE_ACCEPT) {
  ie_pdu_session_status                          = std::nullopt;
  ie_pdu_session_reactivation_result             = std::nullopt;
  ie_pdu_session_reactivation_result_error_cause = std::nullopt;
  ie_eap_message                                 = std::nullopt;
  ie_t3448_value                                 = std::nullopt;
}

ServiceAccept::~ServiceAccept() {}
//------------------------------------------------------------------------------
void ServiceAccept::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void ServiceAccept::SetPduSessionStatus(uint16_t value) {
  ie_pdu_session_status = std::make_optional<PduSessionStatus>(value);
}

//------------------------------------------------------------------------------
void ServiceAccept::SetPduSessionReactivationResult(uint16_t value) {
  ie_pdu_session_reactivation_result =
      std::make_optional<PduSessionReactivationResult>(value);
}

//------------------------------------------------------------------------------
void ServiceAccept::SetPduSessionReactivationResultErrorCause(
    uint8_t session_id, uint8_t value) {
  ie_pdu_session_reactivation_result_error_cause =
      std::make_optional<PduSessionReactivationResultErrorCause>(
          session_id, value);
}

//------------------------------------------------------------------------------
void ServiceAccept::SetEapMessage(const bstring& eap) {
  ie_eap_message = std::make_optional<EapMessage>(kIeiEapMessage, eap);
}

//------------------------------------------------------------------------------
void ServiceAccept::SetT3448Value(uint8_t unit, uint8_t value) {
  ie_t3448_value =
      std::make_optional<GprsTimer3>(kIeiGprsTimer3T3448, unit, value);
}

//------------------------------------------------------------------------------
int ServiceAccept::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Service Accept message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;
  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if (!ie_pdu_session_status.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", PduSessionStatus::GetIeName().c_str());
  } else {
    encoded_ie_size = ie_pdu_session_status.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().debug(
          "Encoding %s error", PduSessionStatus::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_pdu_session_reactivation_result.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        PduSessionReactivationResult::GetIeName().c_str());
  } else {
    encoded_ie_size = ie_pdu_session_reactivation_result.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().debug(
          "Encoding %s error",
          PduSessionReactivationResult::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_pdu_session_reactivation_result_error_cause.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available",
        PduSessionReactivationResultErrorCause::GetIeName().c_str());
  } else {
    encoded_ie_size =
        ie_pdu_session_reactivation_result_error_cause.value().Encode(
            buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().debug(
          "Encoding %s error",
          PduSessionReactivationResultErrorCause::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_eap_message.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", EapMessage::GetIeName().c_str());
  } else {
    encoded_ie_size =
        ie_eap_message.value().Encode(buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().debug(
          "Encoding %s error", EapMessage::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  if (!ie_t3448_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer3::GetIeName().c_str());
  } else {
    encoded_ie_size =
        ie_t3448_value.value().Encode(buf + encoded_size, len - encoded_size);
    if (encoded_ie_size != KEncodeDecodeError) {
      encoded_size += encoded_ie_size;
    } else {
      Logger::nas_mm().debug(
          "Encoding %s error", GprsTimer3::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded Service Accept message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ServiceAccept::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationAccept message");
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
    Logger::nas_mm().debug("Decoding IEI 0x%x", octet);
    switch (octet) {
      case kIeiPduSessionStatus: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiPduSessionStatus);
        PduSessionStatus ie_pdu_session_status_tmp = {};
        if ((decoded_result = ie_pdu_session_status_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_pdu_session_status =
            std::optional<PduSessionStatus>(ie_pdu_session_status_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionReactivationResult: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiPduSessionReactivationResult);
        PduSessionReactivationResult ie_pdu_session_reactivation_result_tmp =
            {};
        if ((decoded_result = ie_pdu_session_reactivation_result_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_pdu_session_reactivation_result =
            std::optional<PduSessionReactivationResult>(
                ie_pdu_session_reactivation_result_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiPduSessionReactivationResultErrorCause: {
        Logger::nas_mm().debug(
            "Decoding IEI 0x%x", kIeiPduSessionReactivationResultErrorCause);
        PduSessionReactivationResultErrorCause
            ie_pdu_session_reactivation_result_error_cause_tmp = {};
        if ((decoded_result =
                 ie_pdu_session_reactivation_result_error_cause_tmp.Decode(
                     buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_pdu_session_reactivation_result_error_cause =
            std::optional<PduSessionReactivationResultErrorCause>(
                ie_pdu_session_reactivation_result_error_cause_tmp);
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

      case kIeiGprsTimer3T3448: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer3T3448);
        GprsTimer3 ie_t3448_value_tmp(kIeiGprsTimer3T3448);
        if ((decoded_result = ie_t3448_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError)
          return decoded_result;
        decoded_size += decoded_result;
        ie_t3448_value = std::optional<GprsTimer3>(ie_t3448_value_tmp);
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
      "Decoded Service Accept message len (%d)", decoded_size);
  return decoded_size;
}
