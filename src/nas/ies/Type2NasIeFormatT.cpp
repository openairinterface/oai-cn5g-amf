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

#include "Type2NasIeFormatT.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;
//------------------------------------------------------------------------------
Type2NasIeFormatT::Type2NasIeFormatT() : NasIe() {
  iei_ = 0;
}

//------------------------------------------------------------------------------
Type2NasIeFormatT::Type2NasIeFormatT(const uint8_t& iei) : NasIe() {
  iei_ = iei;
}

//------------------------------------------------------------------------------
Type2NasIeFormatT::~Type2NasIeFormatT() {}

//------------------------------------------------------------------------------
bool Type2NasIeFormatT::Validate(const int& len) const {
  if (len < kType2NasIeFormatTLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kType2NasIeFormatTLength);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
int Type2NasIeFormatT::Encode(uint8_t* buf, const int& len) {
  // Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  if (!Validate(len)) return KEncodeDecodeError;

  int encoded_size = 0;
  ENCODE_U8(buf + encoded_size, iei_, encoded_size);

  //  Logger::nas_mm().debug(
  //     "Encoded %s (len %d)", GetIeName().c_str(), encoded_size);
  return encoded_size;  // 1 octet
}

//------------------------------------------------------------------------------
int Type2NasIeFormatT::Decode(
    const uint8_t* const buf, const int& len, bool is_iei) {
  // Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  if (!Validate(len)) return KEncodeDecodeError;

  int decoded_size = 0;
  DECODE_U8(buf + decoded_size, iei_, decoded_size);

  //  Logger::nas_mm().debug(
  //     "Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return decoded_size;  // 1 octet
}
