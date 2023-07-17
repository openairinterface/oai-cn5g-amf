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

#include "Nssai.hpp"

#include <vector>

#include "3gpp_24.501.hpp"
#include "amf.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
Nssai::Nssai(uint8_t iei) : Type4NasIe(iei) {
  SetLengthIndicator(0);
}

//------------------------------------------------------------------------------
Nssai::Nssai(uint8_t iei, const std::vector<struct SNSSAI_s>& nssai)
    : Type4NasIe(iei) {
  int length = 0;
  s_nssais.assign(nssai.begin(), nssai.end());
  for (int i = 0; i < nssai.size(); i++) {
    length += (1 + nssai[i].length);  // 1 for length IE
  }
  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
Nssai::Nssai() : Type4NasIe(), s_nssais() {}

//------------------------------------------------------------------------------
Nssai::~Nssai() {}

//------------------------------------------------------------------------------
void Nssai::GetValue(std::vector<struct SNSSAI_s>& nssai) const {
  nssai.assign(s_nssais.begin(), s_nssais.end());
}

//------------------------------------------------------------------------------
int Nssai::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {
    Logger::nas_mm().error(
        "Buffer length is less than the length of this IE (%d octet)", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  for (int i = 0; i < s_nssais.size(); i++) {
    // TODO: Define encode for SNSSAI_s
    int len_s_nssai = SST_LENGTH;
    encoded_size++;  // Store the length of S-Nssai contents later

    ENCODE_U8(buf + encoded_size, s_nssais.at(i).sst, encoded_size);

    if (s_nssais.at(i).sd != SD_NO_VALUE) {
      len_s_nssai += SD_LENGTH;
      ENCODE_U24(buf + encoded_size, s_nssais.at(i).sd, encoded_size);
      Logger::nas_mm().debug("Encoded Nssai SD (0x%x)", s_nssais.at(i).sd);
    }
    if (s_nssais.at(i).length > (SST_LENGTH + SD_LENGTH)) {
      if (s_nssais.at(i).mHplmnSst != -1) {
        len_s_nssai += SST_LENGTH;
        *(buf + encoded_size) = s_nssais.at(i).mHplmnSst;
        encoded_size++;
      }
      if (s_nssais.at(i).mHplmnSd != SD_NO_VALUE) {
        len_s_nssai += SD_LENGTH;
        ENCODE_U24(buf + encoded_size, s_nssais.at(i).mHplmnSd, encoded_size);
      }
    }

    int encoded_size_tmp = 0;
    ENCODE_U8(
        buf + encoded_size - len_s_nssai - 1, len_s_nssai, encoded_size_tmp);
  }

  Logger::nas_mm().debug("Encoded Nssai len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Nssai::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());
  if (len < kNssaiMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kNssaiMinimumLength);
    return KEncodeDecodeError;
  }

  int decoded_size = 0;
  SNSSAI_s a       = {0, 0, 0, 0};

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  // decoded_size += Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  int length_tmp = GetLengthIndicator();

  while (length_tmp) {
    // Decode length of SNSSAI
    uint8_t len_snssai = 0;
    DECODE_U8(buf + decoded_size, len_snssai, decoded_size);
    length_tmp--;

    switch (len_snssai) {
      case 1: {  // Only SST
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        a.sd        = SD_NO_VALUE;
        a.mHplmnSst = 0;
        a.mHplmnSd  = 0;
      } break;
      case 4: {  // SST and SD
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        // SD
        DECODE_U24(buf + decoded_size, a.sd, decoded_size);
        length_tmp -= 3;
        a.mHplmnSst = 0;
        a.mHplmnSd  = 0;
      } break;
      case 5: {  // SST, SD and HPLMN SST
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        // SD
        DECODE_U24(buf + decoded_size, a.sd, decoded_size);
        length_tmp -= 3;
        // HPLMN SST
        DECODE_U8(buf + decoded_size, a.mHplmnSst, decoded_size);
        length_tmp--;
        a.mHplmnSd = SD_NO_VALUE;
      } break;
      case 8: {
        // SST
        DECODE_U8(buf + decoded_size, a.sst, decoded_size);
        length_tmp--;
        // SD
        DECODE_U24(buf + decoded_size, a.sd, decoded_size);
        length_tmp -= 3;
        // HPLMN SST
        DECODE_U8(buf + decoded_size, a.mHplmnSst, decoded_size);
        length_tmp--;
        // HPLMN SD
        DECODE_U24(buf + decoded_size, a.mHplmnSd, decoded_size);
        length_tmp -= 3;
      } break;
    }

    s_nssais.insert(s_nssais.end(), a);
    a = {0, 0, 0, 0};
  }

  for (int i = 0; i < s_nssais.size(); i++) {
    Logger::nas_mm().debug(
        "Decoded Nssai %s", s_nssais.at(i).ToString().c_str());
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
