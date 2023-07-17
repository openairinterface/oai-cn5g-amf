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

#include "SorTransparentContainer.hpp"

#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SorTransparentContainer::SorTransparentContainer()
    : Type6NasIe(kIeiSorTransparentContainer), header_(), sor_mac_i_() {
  SetLengthIndicator(kSorTransparentContainerIeMinimumLength);
}

//------------------------------------------------------------------------------
SorTransparentContainer::SorTransparentContainer(
    uint8_t header,
    const uint8_t (&value)[kSorTransparentContainerIeMacLength]) {
  header_ = header;
  for (int i = 0; i < 16; i++) {
    this->sor_mac_i_[i] = value[i];
  }
  SetLengthIndicator(kSorTransparentContainerIeMinimumLength);
}

//------------------------------------------------------------------------------
SorTransparentContainer::~SorTransparentContainer() {}

//------------------------------------------------------------------------------
void SorTransparentContainer::getValue(
    uint8_t (&value)[kSorTransparentContainerIeMacLength]) const {
  for (int i = 0; i < 16; i++) {
    value[i] = sor_mac_i_[i];
  }
}

//------------------------------------------------------------------------------
int SorTransparentContainer::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
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

  // Header
  ENCODE_U8(buf + encoded_size, header_, encoded_size);
  // SOR-MAC-I
  for (int i = 0; i < 16; i++) {
    ENCODE_U8(buf + encoded_size, sor_mac_i_[i], encoded_size);
  }

  // TODO: Encode spare for the rest
  uint8_t spare = 0;
  int spare_len = ie_len - encoded_size;
  for (int i = 0; i < spare_len; i++) {
    ENCODE_U8(buf + encoded_size, spare, encoded_size);
  }

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SorTransparentContainer::Decode(uint8_t* buf, int len, bool is_iei) {
  if (len < kSorTransparentContainerMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kSorTransparentContainerMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  DECODE_U8(buf + decoded_size, header_, decoded_size);
  // Decode SOR-MAC-I
  for (int i = 0; i < 16; i++) {
    DECODE_U8(buf + decoded_size, sor_mac_i_[i], decoded_size);
  }

  // TODO: decode the rest as spare for now
  uint8_t spare = 0;
  for (int i = 0;
       i < (GetLengthIndicator() - kSorTransparentContainerIeMinimumLength);
       i++) {
    DECODE_U8(buf + decoded_size, spare, decoded_size);
  }

  Logger::nas_mm().debug("Decoded SOR-MAC-I");

  for (int j = 0; j < 16; j++) {
    Logger::nas_mm().debug("Value 0x%2x", sor_mac_i_[j]);
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
