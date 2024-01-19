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

#include "LadnInformation.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
LadnInformation::LadnInformation() : Type6NasIe(kIeiLadnInformation) {
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
LadnInformation::~LadnInformation() {}

//------------------------------------------------------------------------------
void LadnInformation::Set(const std::vector<Ladn>& value) {
  ladn_list.assign(value.begin(), value.end());
}

//------------------------------------------------------------------------------
void LadnInformation::Add(const Ladn& value) {
  // TODO: Check maximum items  - 8
  ladn_list.push_back(value);
  int ie_len = GetIeLength();
  ie_len += value.GetLength();
}

//------------------------------------------------------------------------------
int LadnInformation::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
        "Size of the buffer is not enough to store this IE (IE len %d)",
        ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  /*
    // IEI and Length (later)
    int len_pos = 0;
    int encoded_header_size =
        Type6NasIe::Encode(buf + encoded_size, len, len_pos);
    if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
    encoded_size += encoded_header_size;

    for (int i = 0; i < LADN.size(); i++) {
      ENCODE_U8(buf + encoded_size, blength(LADN.at(i)), encoded_size);
      encoded_size +=
          encode_bstring(LADN.at(i), (buf + encoded_size), len - encoded_size);
    }

    // Encode length
    int encoded_len_ie = 0;
    ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);
  */
  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int LadnInformation::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding EPS_NAS_Message_Container");
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();
  /*
    uint8_t dnn_len = 0;
    bstring dnn     = {};
    while (ie_len) {
      DECODE_U8(buf + decoded_size, dnn_len, decoded_size);
      ie_len--;
      decode_bstring(&dnn, dnn_len, (buf + decoded_size), len - decoded_size);
      decoded_size += dnn_len;
      ie_len -= dnn_len;
      LADN.insert(LADN.end(), dnn);
    }

    for (int i = 0; i < LADN.size(); i++) {
      for (int j = 0; j < blength(LADN.at(i)); j++) {
        Logger::nas_mm().debug(
            "Decoded LadnInformation value (0x%x)",
            (uint8_t) LADN.at(i)->data[j]);
      }
    }
  */
  Logger::nas_mm().debug(
      "Decoded EPS_NAS_Message_Container (len %d)", decoded_size);
  return decoded_size;
}
