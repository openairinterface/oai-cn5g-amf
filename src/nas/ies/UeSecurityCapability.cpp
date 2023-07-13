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

#include "UeSecurityCapability.hpp"

using namespace nas;

//------------------------------------------------------------------------------
UeSecurityCapability::UeSecurityCapability() : Type4NasIe() {
  _5g_ea_ = 0;
  _5g_ia_ = 0;
  eea_    = std::nullopt;
  eia_    = std::nullopt;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
UeSecurityCapability::UeSecurityCapability(uint8_t iei) : Type4NasIe(iei) {
  _5g_ea_ = 0;
  _5g_ia_ = 0;
  eea_    = std::nullopt;
  eia_    = std::nullopt;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
UeSecurityCapability::UeSecurityCapability(uint8_t _5g_ea, uint8_t _5g_ia)
    : Type4NasIe() {
  _5g_ea_ = _5g_ea;
  _5g_ia_ = _5g_ia;
  eea_    = std::nullopt;
  eia_    = std::nullopt;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
UeSecurityCapability::UeSecurityCapability(
    uint8_t iei, uint8_t _5g_ea, uint8_t _5g_ia)
    : Type4NasIe(iei) {
  _5g_ea_ = _5g_ea;
  _5g_ia_ = _5g_ia;
  eea_    = std::nullopt;
  eia_    = std::nullopt;
  SetLengthIndicator(2);
}

//------------------------------------------------------------------------------
UeSecurityCapability::UeSecurityCapability(
    uint8_t iei, uint8_t _5g_ea, uint8_t _5g_ia, uint8_t eea, uint8_t eia)
    : Type4NasIe(iei) {
  _5g_ea_ = _5g_ea;
  _5g_ia_ = _5g_ia;
  eea_    = std::optional<uint8_t>(eea);
  eia_    = std::optional<uint8_t>(eia);
  SetLengthIndicator(4);
}

//------------------------------------------------------------------------------
UeSecurityCapability::UeSecurityCapability(
    uint8_t _5g_ea, uint8_t _5g_ia, uint8_t eea, uint8_t eia)
    : Type4NasIe() {
  _5g_ea_ = _5g_ea;
  _5g_ia_ = _5g_ia;
  eea_    = std::optional<uint8_t>(eea);
  eia_    = std::optional<uint8_t>(eia);
  SetLengthIndicator(4);
}

//------------------------------------------------------------------------------
UeSecurityCapability::~UeSecurityCapability() {}

void UeSecurityCapability::operator=(
    const UeSecurityCapability& ue_security_capability) {
  UeSecurityCapability m_ue_security_capability;
  _5g_ea_     = ue_security_capability.GetEa();
  _5g_ia_     = ue_security_capability.GetIa();
  uint8_t eea = 0;
  uint8_t eia = 0;

  if (ue_security_capability.GetEea(eea)) {
    eea_ = std::make_optional<uint8_t>(eea);
  }

  if (ue_security_capability.GetEia(eia)) {
    eia_ = std::make_optional<uint8_t>(eia);
  }
}

//------------------------------------------------------------------------------
void UeSecurityCapability::SetEa(uint8_t value) {
  _5g_ea_ = value;
}

//------------------------------------------------------------------------------
uint8_t UeSecurityCapability::GetEa() const {
  return _5g_ea_;
}

//------------------------------------------------------------------------------
void UeSecurityCapability::SetIa(uint8_t value) {
  _5g_ia_ = value;
}

//------------------------------------------------------------------------------
uint8_t UeSecurityCapability::GetIa() const {
  return _5g_ia_;
}

//------------------------------------------------------------------------------
void UeSecurityCapability::SetEea(uint8_t value) {
  eea_ = std::make_optional<uint8_t>(value);
}

//------------------------------------------------------------------------------
bool UeSecurityCapability::GetEea(uint8_t& value) const {
  if (eea_.has_value()) {
    value = eea_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void UeSecurityCapability::SetEia(uint8_t value) {
  eia_ = std::make_optional<uint8_t>(value);
}

//------------------------------------------------------------------------------
bool UeSecurityCapability::GetEia(uint8_t& value) const {
  if (eia_.has_value()) {
    value = eia_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void UeSecurityCapability::Set(uint8_t _5g_ea, uint8_t _5g_ia) {
  _5g_ea_ = _5g_ea;
  _5g_ia_ = _5g_ia;
}

//------------------------------------------------------------------------------
void UeSecurityCapability::Set(
    uint8_t _5g_ea, uint8_t _5g_ia, uint8_t eea, uint8_t eia) {
  _5g_ea_ = _5g_ea;
  _5g_ia_ = _5g_ia;
  eea_    = std::optional<uint8_t>(eea);
  eia_    = std::optional<uint8_t>(eia);
  SetLengthIndicator(4);
}

//------------------------------------------------------------------------------
int UeSecurityCapability::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
        "Size of the buffer is not enough to store this IE (IE len %d)",
        ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // EA
  ENCODE_U8(buf + encoded_size, _5g_ea_, encoded_size);
  // IA
  ENCODE_U8(buf + encoded_size, _5g_ia_, encoded_size);

  if (eea_.has_value()) {
    // EEA
    ENCODE_U8(buf + encoded_size, eea_.value(), encoded_size);
  }
  if (eia_.has_value()) {
    // EIA
    ENCODE_U8(buf + encoded_size, eia_.value(), encoded_size);
  }

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UeSecurityCapability::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < kUeSecurityCapabilityMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kUeSecurityCapabilityMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  uint8_t octet    = 0;

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  // decoded_size += Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  // EA
  DECODE_U8(buf + decoded_size, _5g_ea_, decoded_size);
  // IA
  DECODE_U8(buf + decoded_size, _5g_ia_, decoded_size);

  int ie_len = GetIeLength();
  if (ie_len > decoded_size) {
    // EEA
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    eea_ = std::make_optional<uint8_t>(octet);
  }

  if (ie_len > decoded_size) {
    // EIA
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    eia_ = std::make_optional<uint8_t>(octet);
  }

  // TODO: decode the rest as spare for now
  uint8_t spare = 0;
  int spare_len = ie_len - decoded_size;
  for (int i = 0; i < spare_len; i++) {
    DECODE_U8(buf + decoded_size, spare, decoded_size);
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);

  Logger::nas_mm().debug("5G EA 0x%x, 5G IA 0x%x", _5g_ea_, _5g_ia_);
  if (eea_.has_value()) {
    Logger::nas_mm().debug("EEA 0x%x", eea_.value());
  }

  if (eia_.has_value()) {
    Logger::nas_mm().debug("EIA 0x%x", eia_.value());
  }

  return decoded_size;
}
