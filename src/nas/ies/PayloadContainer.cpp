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

#include "PayloadContainer.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace nas;

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer() : Type6NasIe() {
  content_  = std::nullopt;
  contents_ = std::nullopt;
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(uint8_t iei) : Type6NasIe(iei) {
  content_  = std::nullopt;
  contents_ = std::nullopt;
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(const bstring& b) : Type6NasIe() {
  content_  = std::optional<bstring>(b);
  contents_ = std::nullopt;
  SetLengthIndicator(blength(b));
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(uint8_t iei, const bstring& b)
    : Type6NasIe(iei) {
  content_  = std::optional<bstring>(b);
  contents_ = std::nullopt;
  SetLengthIndicator(blength(b));
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(
    const std::vector<PayloadContainerEntry>& contents)
    : Type6NasIe() {
  content_ = std::nullopt;

  int length = 1;  // for number of entries
  // contents_.assign(content_.begin(), content_.end());
  contents_ = std::optional<std::vector<PayloadContainerEntry>>(contents);
  for (int i = 0; i < contents.size(); i++) {
    length = length + 2 +
             contents.at(i).length;  // 2 for Length of Payload container entry
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
PayloadContainer::PayloadContainer(
    uint8_t iei, const std::vector<PayloadContainerEntry>& contents)
    : Type6NasIe(iei) {
  content_ = std::nullopt;

  int length = 1;  // for number of entries
  // contents_.assign(content_.begin(), content_.end());
  contents_ = std::optional<std::vector<PayloadContainerEntry>>(contents);
  for (int i = 0; i < contents.size(); i++) {
    length = length + 2 +
             contents.at(i).length;  // 2 for Length of Payload container entry
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
PayloadContainer::~PayloadContainer() {}

//------------------------------------------------------------------------------
void PayloadContainer::SetValue(const bstring& cnt) {
  content_ = std::optional<bstring>(cnt);
  SetLengthIndicator(blength(cnt));
}

//------------------------------------------------------------------------------
void PayloadContainer::SetValue(
    const std::vector<PayloadContainerEntry>& content) {
  int length = 1;  // for number of entries
  // contents_.assign(content.begin(), content.end());
  contents_ = std::optional<std::vector<PayloadContainerEntry>>(content);
  for (int i = 0; i < content.size(); i++) {
    length = length + 2 +
             content.at(i).length;  // 2 for Length of Payload container entry
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
bool PayloadContainer::GetValue(
    std::vector<PayloadContainerEntry>& content) const {
  if (contents_.has_value()) {
    content.assign(contents_.value().begin(), contents_.value().end());
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool PayloadContainer::GetValue(bstring& cnt) const {
  if (content_.has_value()) {
    cnt = content_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
int PayloadContainer::Encode(uint8_t* buf, int len, uint8_t type) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding %s", GetIeName().c_str());

  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Size of the buffer is not enough to store this IE (IE len %d)",
            ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length (later)
  int len_pos = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  if (content_.has_value()) {
    uint8_t* buf_tmp = (uint8_t*) bdata(content_.value());
    if (buf_tmp != nullptr)
      memcpy(buf + encoded_size, buf_tmp, blength(content_.value()));
    encoded_size += blength(content_.value());
  } else if (contents_.has_value()) {
    // Number of entries
    ENCODE_U8(buf + encoded_size, contents_.value().size(), encoded_size);

    for (int i = 0; i < contents_.value().size(); i++) {
      // Length of Payload container entry
      ENCODE_U8(
          buf + encoded_size, contents_.value().at(i).length, encoded_size);
      // Number of optional IEs and Payload container type
      uint8_t octet = 0;
      octet = ((contents_.value().at(i).optionalIE.size() & 0x0f) << 4) |
              contents_.value().at(i).payloadContainerType;
      ENCODE_U8(buf + encoded_size, octet, encoded_size);
      // Optional IEs
      for (int j = 0; j < contents_.value().at(i).optionalIE.size(); j++) {
        // Type
        ENCODE_U8(
            buf + encoded_size,
            contents_.value().at(i).optionalIE.at(j).ie_type, encoded_size);
        // Length
        ENCODE_U8(
            buf + encoded_size, contents_.value().at(i).optionalIE.at(j).ie_len,
            encoded_size);
        // Value
        int size = encode_bstring(
            contents_.value().at(i).optionalIE.at(j).ie_value,
            (buf + encoded_size), len - encoded_size);
        encoded_size += size;
      }
    }
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PayloadContainer::Decode(uint8_t* buf, int len, bool is_iei, uint8_t type) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding %s", GetIeName().c_str());
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();

  if (type != kMultiplePayloads) {  // not multiple payloads
    uint8_t octet       = 0;
    bstring content_tmp = {};
    content_tmp         = blk2bstr(buf + decoded_size, ie_len);
    content_            = std::optional<bstring>(content_tmp);
    decoded_size += ie_len;
    return decoded_size;
  }

  // Multiple payloads
  uint8_t num_entries  = {};
  uint8_t num_optional = {};
  std::vector<PayloadContainerEntry> CONTENT_tmp;

  PayloadContainerEntry payloadcontainerentry = {};
  DECODE_U8(buf + decoded_size, num_entries, decoded_size);

  while (num_entries) {
    uint8_t length_entry = 0;
    DECODE_U8(buf + decoded_size, length_entry, decoded_size);
    uint8_t octet = 0;
    // Number of optional IEs and Payload container type
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    payloadcontainerentry.payloadContainerType = octet & 0x0f;
    num_optional                               = (octet & 0xf0) >> 4;

    IE_t value;
    while (num_optional) {
      DECODE_U8(buf + decoded_size, value.ie_type, decoded_size);
      DECODE_U8(buf + decoded_size, value.ie_len, decoded_size);

      decode_bstring(
          &value.ie_value, value.ie_len, (buf + decoded_size),
          len - decoded_size);
      decoded_size += value.ie_len;
      payloadcontainerentry.optionalIE.insert(
          payloadcontainerentry.optionalIE.end(), value);
      num_optional--;
    }
    CONTENT_tmp.insert(CONTENT_tmp.end(), payloadcontainerentry);
    num_entries--;
  }
  contents_ = std::optional<std::vector<PayloadContainerEntry>>(contents_);
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
