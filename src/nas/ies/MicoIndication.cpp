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

#include "MicoIndication.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
MicoIndication::MicoIndication(bool sprti, bool raai)
    : Type1NasIeFormatTv(kIeiMicoIndication) {
  raai_  = raai;
  sprti_ = sprti;
}

//------------------------------------------------------------------------------
MicoIndication::MicoIndication() : Type1NasIeFormatTv(kIeiMicoIndication) {
  raai_  = false;
  sprti_ = false;
}

//------------------------------------------------------------------------------
MicoIndication::~MicoIndication(){};

//------------------------------------------------------------------------------
void MicoIndication::SetValue() {
  uint8_t octet = (kIeiMicoIndication << 4) | (sprti_ << 1) | raai_;
  Type1NasIeFormatTv::SetValue(octet);
}

//------------------------------------------------------------------------------
void MicoIndication::SetSprti(bool value) {
  sprti_ = value;
}

//------------------------------------------------------------------------------
bool MicoIndication::GetSprti() const {
  return sprti_;
}

//------------------------------------------------------------------------------
void MicoIndication::SetRaai(bool value) {
  raai_ = value;
}

//------------------------------------------------------------------------------
bool MicoIndication::GetRaai() const {
  return raai_;
}

//------------------------------------------------------------------------------
int MicoIndication::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
        "Size of the buffer is not enough to store this IE (IE len %d)",
        ie_len);
    return KEncodeDecodeError;
  }

  uint8_t octet    = 0;
  int encoded_size = 0;

  octet = (kIeiMicoIndication << 4) | (sprti_ << 1) | raai_;
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);

  return encoded_size;
}

//------------------------------------------------------------------------------
int MicoIndication::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());
  if (len < kMicoIndicationIELength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kMicoIndicationIELength);
    return KEncodeDecodeError;
  }

  uint8_t octet    = 0;
  int decoded_size = 0;

  DECODE_U8(buf + decoded_size, octet, decoded_size);
  // TODO: validate IEI

  sprti_ = octet & 0x02;
  raai_  = octet & 0x01;

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);

  Logger::nas_mm().debug("SPRTI 0x%x, RAAI 0x%x", sprti_, raai_);
  return decoded_size;
}
