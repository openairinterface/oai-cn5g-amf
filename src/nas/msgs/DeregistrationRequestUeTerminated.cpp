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

#include "NasHelper.hpp"
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
  encoded_ie_size =
      NasHelper::Encode(ie_deregistration_type, buf, len, encoded_size);
  // only 1/2 octet
  if ((encoded_ie_size == KEncodeDecodeError) or (encoded_ie_size != 0)) {
    return KEncodeDecodeError;
  }
  if (encoded_ie_size == 0)
    encoded_size++;  // 1/2 octet for Deregistration Type, 1/2 for Spare half
                     // octet

  // 5GMM Cause
  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gmm_cause, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // T3346 value
  if ((encoded_ie_size = NasHelper::Encode(
           ie_t3346_value, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Rejected NSSAI
  if ((encoded_ie_size = NasHelper::Encode(
           ie_rejected_nssai, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
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

  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // De-registration Type +  Spare half octet
  if ((decoded_ie_size = NasHelper::Decode(
           ie_deregistration_type, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  if (decoded_ie_size == 0)
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
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_5gmm_cause, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer2T3346: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiGprsTimer2T3346);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_t3346_value, kIeiGprsTimer2T3346, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiRejectedNssaiDr: {
        Logger::nas_mm().debug("Decoding IEI 0x%x", kIeiRejectedNssaiDr);
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_rejected_nssai, kIeiRejectedNssaiDr, buf, len, decoded_size,
                 true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
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
