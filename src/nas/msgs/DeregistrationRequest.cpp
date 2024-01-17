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

#include "DeregistrationRequest.hpp"

#include "NasHelper.hpp"
#include "amf_conversions.hpp"

using namespace nas;

//------------------------------------------------------------------------------
DeregistrationRequest::DeregistrationRequest()
    : NasMmPlainHeader(EPD_5GS_MM_MSG) {
  NasMmPlainHeader::SetMessageType(DEREGISTRATION_REQUEST_UE_ORIGINATING);
}

//------------------------------------------------------------------------------
DeregistrationRequest::~DeregistrationRequest() {}

//------------------------------------------------------------------------------
void DeregistrationRequest::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::SetDeregistrationType(uint8_t dereg_type) {
  ie_deregistrationtype.set(dereg_type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::SetDeregistrationType(
    const _5gs_deregistration_type_t& type) {
  ie_deregistrationtype.set(type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::SetNgKsi(uint8_t tsc, uint8_t key_set_id) {
  ie_ng_ksi.Set(true);  // high position
  ie_ng_ksi.SetTypeOfSecurityContext(tsc);
  ie_ng_ksi.SetNasKeyIdentifier(key_set_id);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::GetDeregistrationType(uint8_t& dereg_type) const {
  ie_deregistrationtype.get(dereg_type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::GetDeregistrationType(
    _5gs_deregistration_type_t& type) const {
  ie_deregistrationtype.get(type);
}

//------------------------------------------------------------------------------
bool DeregistrationRequest::GetNgKsi(uint8_t& ng_ksi) const {
  ng_ksi =
      (ie_ng_ksi.GetTypeOfSecurityContext()) | ie_ng_ksi.GetNasKeyIdentifier();
  return true;
}

//------------------------------------------------------------------------------
void DeregistrationRequest::SetSuciSupiFormatImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routing_ind, uint8_t protection_sch_id,
    const std::string& msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "Encoding SUCI and SUPI format for IMSI error, please choose correct "
        "protection scheme");
    return;
  } else {
    ie_5gs_mobility_id.SetSuciWithSupiImsi(
        mcc, mnc, routing_ind, protection_sch_id, msin);
  }
}

//------------------------------------------------------------------------------
void DeregistrationRequest::GetMobilityIdentityType(uint8_t& type) const {
  type = ie_5gs_mobility_id.GetTypeOfIdentity();
}

//------------------------------------------------------------------------------
bool DeregistrationRequest::GetSuciSupiFormatImsi(
    nas::SUCI_imsi_t& imsi) const {
  ie_5gs_mobility_id.GetSuciWithSupiImsi(imsi);
  return true;
}

//------------------------------------------------------------------------------
std::string DeregistrationRequest::Get5gGuti() const {
  std::optional<nas::_5G_GUTI_t> guti = std::nullopt;
  ie_5gs_mobility_id.Get5gGuti(guti);
  if (!guti.has_value()) return {};

  std::string guti_str = guti.value().mcc + guti.value().mnc +
                         std::to_string(guti.value().amf_region_id) +
                         std::to_string(guti.value().amf_set_id) +
                         std::to_string(guti.value().amf_pointer) +
                         amf_conv::tmsi_to_string(guti.value()._5g_tmsi);
  Logger::nas_mm().debug("5G GUTI %s", guti_str.c_str());
  return guti_str;
}

//------------------------------------------------------------------------------
void DeregistrationRequest::SetSuciSupiFormatImsi(
    const std::string& mcc, const std::string& mnc,
    const std::string& routing_ind, uint8_t protection_sch_id, uint8_t hnpki,
    const std::string& msin) {
  // TODO:
}

//------------------------------------------------------------------------------
void DeregistrationRequest::Set5gGuti() {}

//------------------------------------------------------------------------------
void DeregistrationRequest::SetImeiImeisv() {}

//------------------------------------------------------------------------------
void DeregistrationRequest::Set5gSTmsi() {}

//------------------------------------------------------------------------------
int DeregistrationRequest::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding DeregistrationRequest message");

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  // De-registration Type and ngKSI
  encoded_ie_size = NasHelper::Encode(ie_ng_ksi, buf, len, encoded_size);
  // only 1/2 octet
  if ((encoded_ie_size == KEncodeDecodeError) or (encoded_ie_size != 0)) {
    return KEncodeDecodeError;
  }

  if ((encoded_ie_size = NasHelper::Encode(
           ie_ng_ksi, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  encoded_size++;  // 1/2 octet for Deregistration Type, 1/2 for ngKSI

  // 5GS mobile identity
  if ((encoded_ie_size = NasHelper::Encode(
           ie_5gs_mobility_id, buf, len, encoded_size)) == KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Encoded DeregistrationRequest message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DeregistrationRequest::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding DeregistrationRequest message");

  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    Logger::nas_mm().error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  // De-registration Type + ngKSI
  if ((decoded_ie_size = NasHelper::Decode(
           ie_deregistrationtype, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  if ((decoded_ie_size = NasHelper::Decode(
           ie_ng_ksi, buf, len, decoded_size, true, false)) ==  // 4 higher bits
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  decoded_size++;  // 1/2 octet for De-registration Type, 1/2 ngKSI

  if ((decoded_ie_size = NasHelper::Decode(
           ie_5gs_mobility_id, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  Logger::nas_mm().debug(
      "Decoded DeregistrationRequest message (len %d)", decoded_size);
  return decoded_size;
}
