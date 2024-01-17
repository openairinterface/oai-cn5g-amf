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

#include "NetworkName.hpp"

#include "common_defs.h"
#include "conversions.hpp"
#include "logger.hpp"
#include "string.hpp"
#include "utils.hpp"

extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
}

using namespace nas;

//------------------------------------------------------------------------------
NetworkName::NetworkName() {
  iei_                 = 0;
  length               = kNetworkNameMinimumLength;
  coding_scheme        = 0;
  add_CI               = false;
  number_of_spare_bits = 0;
  text_string          = nullptr;
}
//------------------------------------------------------------------------------
NetworkName::NetworkName(const uint8_t& iei) {
  iei_                 = iei;
  length               = kNetworkNameMinimumLength;
  coding_scheme        = 0;
  add_CI               = false;
  number_of_spare_bits = 0;
  text_string          = nullptr;
}

//------------------------------------------------------------------------------
NetworkName::~NetworkName() {
  text_string = nullptr;
}

//------------------------------------------------------------------------------
void NetworkName::setIEI(const uint8_t& iei) {
  iei_ = iei;
}
//------------------------------------------------------------------------------
void NetworkName::NetworkName::setCodingScheme(const uint8_t& value) {
  coding_scheme = value & 0x07;
}

//------------------------------------------------------------------------------
void NetworkName::setAddCI(const uint8_t& value) {
  add_CI = value & 0x01;
}

//------------------------------------------------------------------------------
void NetworkName::setNumberOfSpareBits(const uint8_t& value) {
  number_of_spare_bits = value & 0x07;
}

//------------------------------------------------------------------------------
void NetworkName::setTextString(const std::string& str) {
  // TODO: Temporary for now
  // SMS Packing using "Seven characters in seven octets" (3GPP TS 23.038 )
  // str = "Testing";
  // std::string packed_str;
  // util::sms_packing(str, packed_str);
  // amf_conv::string_2_bstring(packed_str, text_string);

  uint8_t* packed_str = (uint8_t*) calloc(7, sizeof(uint8_t));
  if (!packed_str) {
    utils::free_wrapper((void**) &packed_str);
    return;
  }
  // Text string = "Testing"
  packed_str[0] = 0xd4;
  packed_str[1] = 0xf2;
  packed_str[2] = 0x9c;
  packed_str[3] = 0x9e;
  packed_str[4] = 0x76;
  packed_str[5] = 0x9f;
  packed_str[6] = 0x01;

  text_string = blk2bstr(packed_str, 7);
  utils::free_wrapper((void**) &packed_str);
  length = 1 + blength(text_string);
}

//------------------------------------------------------------------------------
void NetworkName::setTextString(const bstring& str) {
  text_string = bstrcpy(str);
  length      = 1 + blength(text_string);
}

//------------------------------------------------------------------------------
int NetworkName::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NetworkName");

  if (len < kNetworkNameMinimumLength) {
    Logger::nas_mm().error("len is less than %d", length);
    return -1;
  }

  int encoded_size = 0;
  if (iei_) {
    ENCODE_U8(buf + encoded_size, iei_, encoded_size);  // IEI
  }
  ENCODE_U8(buf + encoded_size, length, encoded_size);  // length

  // Octet 3
  uint8_t octet = 0;
  // TODO: Extension (0x00 for now)
  octet = 0x00 | (coding_scheme << 4) | (add_CI << 3) | number_of_spare_bits;
  ENCODE_U8(buf + encoded_size, octet, encoded_size);
  // Text String
  int size =
      encode_bstring(text_string, (buf + encoded_size), len - encoded_size);
  encoded_size += size;

  Logger::nas_mm().debug("Encoded NetworkName (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NetworkName::Decode(uint8_t* buf, int len, bool is_option) {
  // TODO
  return -1;
}
