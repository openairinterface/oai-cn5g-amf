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

#include "Type4NasIe.hpp"

using namespace nas;
//------------------------------------------------------------------------------
Type4NasIe::Type4NasIe() : NasIe() {
  iei_ = std::nullopt;
  li_  = 0;
}

//------------------------------------------------------------------------------
Type4NasIe::Type4NasIe(const uint8_t& iei) : NasIe() {
  iei_ = std::optional<uint8_t>(iei);
}

//------------------------------------------------------------------------------
Type4NasIe::~Type4NasIe() {}

//------------------------------------------------------------------------------
void Type4NasIe::SetIei(const uint8_t& iei) {
  iei_ = std::optional<uint8_t>(iei);
}

//------------------------------------------------------------------------------
void Type4NasIe::SetLengthIndicator(const uint8_t& li) {
  li_ = li;
}

//------------------------------------------------------------------------------
void Type4NasIe::GetLengthIndicator(uint8_t& li) const {
  li = li_;
}

//------------------------------------------------------------------------------
uint8_t Type4NasIe::GetLengthIndicator() const {
  return li_;
}

//------------------------------------------------------------------------------
uint8_t Type4NasIe::GetIeLength() const {
  return (iei_.has_value() ? (li_ + 2) : (li_ + 1));  // 1 for IEI, 1 for Length
}

//------------------------------------------------------------------------------
uint8_t Type4NasIe::GetHeaderLength() const {
  return (iei_.has_value() ? 2 : 1);  // 1 for IEI, 1 for Length
}

//------------------------------------------------------------------------------
bool Type4NasIe::Validate(const int& len) const {
  int ie_len = GetIeLength();  // Length of the content + IEI/Len
  if (len < ie_len) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet(s))",
        ie_len);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool Type4NasIe::ValidateHeader(const int& len) const {
  int header_len = GetHeaderLength();  // Length of IEI/Len
  if (len < header_len) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of the header (IEI/Length) of "
        "this IE (%d octet(s))",
        header_len);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
int Type4NasIe::Encode(uint8_t* buf, const int& len) {
  if (!Validate(len)) return KEncodeDecodeError;

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (iei_.has_value()) {
    ENCODE_U8(buf + encoded_size, iei_.value(), encoded_size);
  }

  ENCODE_U8(buf + encoded_size, li_, encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Type4NasIe::Encode(uint8_t* buf, const int& len, int& len_pos) {
  if (!Validate(len)) return KEncodeDecodeError;

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (iei_.has_value()) {
    ENCODE_U8(buf + encoded_size, iei_.value(), encoded_size);
  }
  len_pos = encoded_size;
  encoded_size++;  // IE len will be encoded later
  //  ENCODE_U8(buf + encoded_size, li_, encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Type4NasIe::Decode(const uint8_t* const buf, const int& len, bool is_iei) {
  if (!ValidateHeader(len)) return KEncodeDecodeError;

  int decoded_size = 0;
  uint8_t octet    = 0;

  if (is_iei) {
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    iei_ = std::optional<uint8_t>(octet);
  }

  DECODE_U8(buf + decoded_size, li_, decoded_size);

  // after obtaining information for IEI/Length, validate the buffer size
  if (!Validate(len)) return KEncodeDecodeError;

  return decoded_size;
}
