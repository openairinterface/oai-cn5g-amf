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

#include "DeregistrationRequestUeTerminated.hpp"

#include "conversions.hpp"

using namespace nas;

//------------------------------------------------------------------------------
DeregistrationRequestUeTerminated::DeregistrationRequestUeTerminated()
    : NasMmPlainHeader(EPD_5GS_MM_MSG) {
  NasMmPlainHeader::SetMessageType(DEREGISTRATION_REQUEST_UE_TERMINATED);
}

//------------------------------------------------------------------------------
DeregistrationRequestUeTerminated::~DeregistrationRequestUeTerminated() {}

//------------------------------------------------------------------------------
void DeregistrationRequestUeTerminated::SetHeader(
    uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void DeregistrationRequestUeTerminated::SetDeregistrationType(
    uint8_t dereg_type) {
  ie_deregistration_type.set(dereg_type);
}

//------------------------------------------------------------------------------
void DeregistrationRequestUeTerminated::SetDeregistrationType(
    const _5gs_deregistration_type_t& type) {
  ie_deregistration_type.set(type);
}

//------------------------------------------------------------------------------
void DeregistrationRequestUeTerminated::Set5gmmCause(uint8_t value) {
  ie_5gmm_cause = std::make_optional<_5gmmCause>(kIei5gmmCause, value);
}

//------------------------------------------------------------------------------
std::optional<_5gmmCause> DeregistrationRequestUeTerminated::Get5gmmCause()
    const {
  return ie_5gmm_cause;
}

//------------------------------------------------------------------------------
void DeregistrationRequestUeTerminated::SetT3346Value(uint8_t value) {
  ie_t3346_value = std::make_optional<GprsTimer2>(kT3346Value, value);
}

//------------------------------------------------------------------------------
std::optional<GprsTimer2> DeregistrationRequestUeTerminated::GetT3346Value()
    const {
  return ie_t3346_value;
}

//------------------------------------------------------------------------------
void DeregistrationRequestUeTerminated::SetRejectedNssai(
    const std::vector<RejectedSNssai>& nssai) {
  if (nssai.size() > 0) {
    ie_rejected_nssai = std::make_optional<RejectedNssai>(kIeiRejectedNssaiDr);
    ie_rejected_nssai.value().SetRejectedSNssais(nssai);
  }
}

std::optional<RejectedNssai>
DeregistrationRequestUeTerminated::GetRejectedNssai() const {
  return ie_rejected_nssai;
}

//------------------------------------------------------------------------------
int DeregistrationRequestUeTerminated::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding DeregistrationRequestUeTerminated message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // De-registration Type and Spare half octet
  int size =
      ie_deregistration_type.Encode(buf + encoded_size, len - encoded_size);
  if (size == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Encoding %s error", _5gsDeregistrationType::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  // only 1/2 octet
  if (size != 0) {
    Logger::nas_mm().error(
        "Encoding %s error", _5gsDeregistrationType::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  encoded_size++;  // 1/2 octet for Deregistration Type, 1/2 for Spare half
                   // octet

  // 5GMM Cause
  if (!ie_5gmm_cause.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", _5gmmCause::GetIeName().c_str());
  } else {
    size = ie_5gmm_cause.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", _5gmmCause::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  // T3346 value
  if (!ie_t3346_value.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", GprsTimer2::GetIeName().c_str());
  } else {
    size =
        ie_t3346_value.value().Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", GprsTimer2::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  // Rejected NSSAI
  if (!ie_rejected_nssai.has_value()) {
    Logger::nas_mm().debug(
        "IE %s is not available", RejectedNssai::GetIeName().c_str());
  } else {
    size = ie_rejected_nssai.value().Encode(
        buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding %s error", RejectedNssai::GetIeName().c_str());
      return KEncodeDecodeError;
    }
  }

  // TODO: CagInformationList

  Logger::nas_mm().debug(
      "Encoded DeregistrationRequestUeTerminated message len (%d)",
      encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DeregistrationRequestUeTerminated::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding DeregistrationRequestUeTerminated message");

  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  // De-registration Type +  Spare half octet
  decoded_result = ie_deregistration_type.Decode(
      buf + decoded_size, len - decoded_size, false);
  if (decoded_result == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", _5gsDeregistrationType::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size++;  // 1/2 octet for De-registration Type, 1/2 for Spare half
                   // octet

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
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

      case kIeiGprsTimer2T3346: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer2T3346);
        GprsTimer2 ie_t3346_value_tmp(kIeiGprsTimer2T3346);
        if ((decoded_result = ie_t3346_value_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", GprsTimer2::GetIeName().c_str());
          return decoded_result;
        }
        decoded_size += decoded_result;
        ie_t3346_value = std::optional<GprsTimer2>(ie_t3346_value_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiRejectedNssaiDr: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiRejectedNssaiDr);
        RejectedNssai ie_rejected_nssai_tmp(kIeiRejectedNssaiDr);
        if ((decoded_result = ie_rejected_nssai_tmp.Decode(
                 buf + decoded_size, len - decoded_size, true)) ==
            KEncodeDecodeError) {
          Logger::nas_mm().error(
              "Decoding %s error", RejectedNssai::GetIeName().c_str());
          return KEncodeDecodeError;
        }
        decoded_size += decoded_result;
        ie_rejected_nssai = std::optional<RejectedNssai>(ie_rejected_nssai_tmp);
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

        // TODO: CagInformationList ie_cag_information_list ; //Optional

      default: {
        Logger::nas_mm().warn("Unknown IEI 0x%x, stop decoding...", octet);
        // Stop decoding
        octet = 0x00;
      } break;
    }
  }

  Logger::nas_mm().debug(
      "Decoded DeregistrationRequestUeTerminated message (len %d)",
      decoded_size);
  return decoded_size;
}
