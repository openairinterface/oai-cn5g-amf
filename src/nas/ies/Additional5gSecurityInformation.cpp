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

#include "Additional5gSecurityInformation.hpp"

using namespace nas;

//------------------------------------------------------------------------------
Additional5gSecurityInformation::Additional5gSecurityInformation()
    : Type4NasIe(kIeiAdditional5gSecurityInformation) {
  rinmr_ = false;
  hdp_   = false;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
Additional5gSecurityInformation::Additional5gSecurityInformation(
    bool rinmr, bool hdp)
    : Type4NasIe(kIeiAdditional5gSecurityInformation) {
  rinmr_ = rinmr;
  hdp_   = hdp;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
Additional5gSecurityInformation::~Additional5gSecurityInformation() {}

//------------------------------------------------------------------------------
void Additional5gSecurityInformation::SetRinmr(bool value) {
  rinmr_ = value;
}

//------------------------------------------------------------------------------
void Additional5gSecurityInformation::SetHdp(bool value) {
  hdp_ = value;
}

//------------------------------------------------------------------------------
bool Additional5gSecurityInformation::GetRinmr() const {
  return rinmr_;
}

//------------------------------------------------------------------------------
bool Additional5gSecurityInformation::GetHdp() const {
  return hdp_;
}

//------------------------------------------------------------------------------
int Additional5gSecurityInformation::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {
    Logger::nas_mm().error("Len is less than %d", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Octet 3
  uint8_t octet = 0;
  if (rinmr_) octet |= 0x02;
  if (hdp_) octet |= 0x01;

  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Additional5gSecurityInformation::Decode(
    uint8_t* buf, int len, bool is_iei) {
  if (len < kAdditional5gSecurityInformationLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kAdditional5gSecurityInformationLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  // Octet 3
  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  rinmr_ = octet & 0x02;
  hdp_   = octet & 0x01;

  Logger::nas_mm().debug(
      "Decoded %s, RINMR 0x%x, HDP 0x%x", GetIeName().c_str(), rinmr_, hdp_);

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
