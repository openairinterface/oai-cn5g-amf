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

#include "NasHelper.hpp"
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
  if ((encoded_ie_size = NasHelper::Encode(
           ie_payload_container_type, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if (encoded_ie_size == 0)
    // Spare half octet
    encoded_size++;  // 1/2 octet + 1/2 octet for Payload container type

  // Payload container
  encoded_ie_size = ie_payload_container.Encode(
      buf + encoded_size, len - encoded_size,
      ie_payload_container_type.GetValue());
  if (encoded_ie_size != KEncodeDecodeError) {
    encoded_size += encoded_ie_size;
  } else {
    Logger::nas_mm().error(
        "Encoding %s error", PayloadContainer::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  /*
    if ((encoded_ie_size = NasHelper::Encode(
                    ie_payload_container, buf, len, encoded_size)) ==
        KEncodeDecodeError) {
      return KEncodeDecodeError;
    }
  */

  // PDU session ID
  if ((encoded_ie_size = NasHelper::Encode(
           ie_pdu_session_identity_2, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Additional information
  if ((encoded_ie_size = NasHelper::Encode(
           ie_additional_information, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // 5GMM cause
  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gmm_cause, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  // Back-off timer value
  if ((encoded_ie_size = NasHelper::Encode(
           ie_back_off_timer_value, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Encoded DlNasTransport message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DlNasTransport::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding DlNasTransport message");

  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // Payload container type
  if ((decoded_ie_size = NasHelper::Decode(
           ie_payload_container_type, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  if (decoded_ie_size == 0)
    decoded_size++;  // 1/2 octet for PayloadContainerType, 1/2 octet for spare

  // Payload container
  decoded_ie_size = ie_payload_container.Decode(
      buf + decoded_size, len - decoded_size, false,
      N1_SM_INFORMATION);  // TODO: verified Type of Payload Container
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error(
        "Decoding %s error", PayloadContainer::GetIeName().c_str());
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);

  // Decode other IEs
  uint8_t octet = 0x00;
  DECODE_U8_VALUE(buf + decoded_size, octet);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    Logger::nas_mm().debug("Decoding IEI 0x%x", octet);
    switch (octet) {
      case kIeiPduSessionId: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_pdu_session_identity_2, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiAdditionalInformation: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_additional_information, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIei5gmmCause: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_5gmm_cause, buf, len, decoded_size, true)) ==
            KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
        DECODE_U8_VALUE(buf + decoded_size, octet);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;

      case kIeiGprsTimer3BackOffTimer: {
        if ((decoded_ie_size = NasHelper::Decode(
                 ie_5gmm_cause, kIeiGprsTimer3BackOffTimer, buf, len,
                 decoded_size, true)) == KEncodeDecodeError) {
          return KEncodeDecodeError;
        }
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
