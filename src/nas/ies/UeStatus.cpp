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

#include "UeStatus.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
UeStatus::UeStatus() : Type4NasIe(kIeiUeStatus) {
  s1_ = false;
  n1_ = false;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
UeStatus::UeStatus(bool n1, bool s1) : Type4NasIe(kIeiUeStatus) {
  s1_ = s1;
  n1_ = n1;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
UeStatus::~UeStatus() {}

//------------------------------------------------------------------------------
void UeStatus::SetS1(bool value) {
  s1_ = value;
}

//------------------------------------------------------------------------------
bool UeStatus::GetS1() const {
  return s1_;
}

//------------------------------------------------------------------------------
void UeStatus::SetN1(bool value) {
  n1_ = value;
}

//------------------------------------------------------------------------------
bool UeStatus::GetN1() const {
  return n1_;
}

//------------------------------------------------------------------------------
int UeStatus::Encode(uint8_t* buf, int len) {
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

  uint8_t octet = 0x03 & (s1_ | (n1_ << 1));
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UeStatus::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (len < kUeStatusIeLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kUeStatusIeLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  // decoded_size += Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  uint8_t octet = 0;
  DECODE_U8(buf + decoded_size, octet, decoded_size);

  n1_ = octet & 0x02;
  s1_ = octet & 0x01;

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);

  Logger::nas_mm().debug("N1 0x%x, S1 0x%x", n1_, s1_);
  return decoded_size;
}
