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

#include "EpsNasMessageContainer.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
EpsNasMessageContainer::EpsNasMessageContainer()
    : Type6NasIe(kIeiEpsNasMessageContainer), value_() {}

//------------------------------------------------------------------------------
EpsNasMessageContainer::EpsNasMessageContainer(const bstring& value)
    : Type6NasIe(kIeiEpsNasMessageContainer) {
  value_ = bstrcpy(value);
  SetLengthIndicator(blength(value_));
}

//------------------------------------------------------------------------------
EpsNasMessageContainer::~EpsNasMessageContainer() {}

//------------------------------------------------------------------------------
void EpsNasMessageContainer::GetValue(bstring& value) const {
  value = bstrcpy(value_);
}

//------------------------------------------------------------------------------
int EpsNasMessageContainer::Encode(uint8_t* buf, int len) {
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

  int size = encode_bstring(value_, (buf + encoded_size), len - encoded_size);
  encoded_size += size;

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int EpsNasMessageContainer::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding EpsNasMessageContainer");
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();

  int result =
      decode_bstring(&value_, ie_len, (buf + decoded_size), len - decoded_size);
  decoded_size += ie_len;
  if (result == ie_len) {
    for (int i = 0; i < ie_len; i++) {
      Logger::nas_mm().debug(
          "Decoded EpsNasMessageContainer value 0x%x",
          (uint8_t) value_->data[i]);
    }
  }

  Logger::nas_mm().debug(
      "Decoded EpsNasMessageContainer (len %d)", decoded_size);
  return decoded_size;
}
