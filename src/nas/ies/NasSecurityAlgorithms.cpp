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

#include "NasSecurityAlgorithms.hpp"

#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
NasSecurityAlgorithms::NasSecurityAlgorithms()
    : Type3NasIe(),
      type_of_ciphering_algorithm_(),
      type_of_integrity_protection_algorithm_() {}

//------------------------------------------------------------------------------
NasSecurityAlgorithms::NasSecurityAlgorithms(uint8_t iei)
    : Type3NasIe(iei),
      type_of_ciphering_algorithm_(),
      type_of_integrity_protection_algorithm_() {}

//------------------------------------------------------------------------------
NasSecurityAlgorithms::~NasSecurityAlgorithms() {}

//------------------------------------------------------------------------------
NasSecurityAlgorithms::NasSecurityAlgorithms(
    uint8_t ciphering, uint8_t integrity_protection)
    : Type3NasIe() {
  type_of_ciphering_algorithm_            = ciphering & 0x0f;
  type_of_integrity_protection_algorithm_ = integrity_protection & 0x0f;
}

//------------------------------------------------------------------------------
void NasSecurityAlgorithms::SetTypeOfCipheringAlgorithm(uint8_t value) {
  type_of_ciphering_algorithm_ = value & 0x0f;
}

//------------------------------------------------------------------------------
void NasSecurityAlgorithms::SetTypeOfIntegrityProtectionAlgorithm(
    uint8_t value) {
  type_of_integrity_protection_algorithm_ = value & 0x0f;
}

//------------------------------------------------------------------------------
uint8_t NasSecurityAlgorithms::GetTypeOfCipheringAlgorithm() const {
  return type_of_ciphering_algorithm_;
}

//------------------------------------------------------------------------------
uint8_t NasSecurityAlgorithms::GetTypeOfIntegrityProtectionAlgorithm() const {
  return type_of_integrity_protection_algorithm_;
}

//------------------------------------------------------------------------------
void NasSecurityAlgorithms::Set(
    uint8_t ciphering, uint8_t integrity_protection) {
  type_of_ciphering_algorithm_            = ciphering & 0x0f;
  type_of_integrity_protection_algorithm_ = integrity_protection & 0x0f;
}
//------------------------------------------------------------------------------
void NasSecurityAlgorithms::Get(
    uint8_t& ciphering, uint8_t& integrity_protection) const {
  ciphering            = type_of_ciphering_algorithm_;
  integrity_protection = type_of_integrity_protection_algorithm_;
}

//------------------------------------------------------------------------------
int NasSecurityAlgorithms::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  if (len < kNasSecurityAlgorithmsLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNasSecurityAlgorithmsLength);
    return KEncodeDecodeError;
  }
  int encoded_size = 0;

  // IEI
  encoded_size += Type3NasIe::Encode(buf + encoded_size, len);

  uint8_t octet = 0;
  octet         = ((type_of_ciphering_algorithm_ & 0x0f) << 4) |
          (type_of_integrity_protection_algorithm_ & 0x0f);

  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NasSecurityAlgorithms::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < kNasSecurityAlgorithmsLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNasSecurityAlgorithmsLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;

  // IEI and Length
  decoded_size += Type3NasIe::Decode(buf + decoded_size, len, is_iei);

  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  type_of_ciphering_algorithm_            = (octet & 0xf0) >> 4;
  type_of_integrity_protection_algorithm_ = octet & 0x0f;

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
