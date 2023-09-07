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

#include "utils.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

//------------------------------------------------------------------------------
int utils::encodeMccMnc2Buffer(
    const std::string& mcc_str, const std::string& mnc_str, uint8_t* buf,
    int len) {
  int encoded_size = 0;
  uint8_t value    = 0;
  int mcc          = utils::fromString<int>(mcc_str);
  int mnc          = utils::fromString<int>(mnc_str);

  value = (0x0f & (mcc / 100)) | ((0x0f & ((mcc % 100) / 10)) << 4);
  ENCODE_U8(buf + encoded_size, value, encoded_size);

  Logger::nas_mm().debug("MNC digit 1: %d", mnc / 100);
  if (!(mnc / 100)) {
    Logger::nas_mm().debug("Encoding MNC 2 digits");
    value = (0x0f & (mcc % 10)) | 0xf0;
    ENCODE_U8(buf + encoded_size, value, encoded_size);

    value = (0x0f & ((mnc % 100) / 10)) | ((0x0f & (mnc % 10)) << 4);
    ENCODE_U8(buf + encoded_size, value, encoded_size);

  } else {
    Logger::nas_mm().debug("Encoding MNC 3 digits");
    value = (0x0f & (mcc % 10)) | ((0x0f & (mnc % 10)) << 4);
    ENCODE_U8(buf + encoded_size, value, encoded_size);

    value = ((0x0f & ((mnc % 100) / 10)) << 4) | (0x0f & (mnc / 100));
    ENCODE_U8(buf + encoded_size, value, encoded_size);
  }
  Logger::nas_mm().debug("MCC %s, MNC %s", mcc_str.c_str(), mnc_str.c_str());
  return encoded_size;
}

//------------------------------------------------------------------------------
int utils::decodeMccMncFromBuffer(
    std::string& mcc_str, std::string& mnc_str, uint8_t* buf, int len) {
  if (len < kMccMncLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kMccMncLength);
    return KEncodeDecodeError;
  }
  int decoded_size = 0;
  uint8_t octet    = 0;

  DECODE_U8(buf + decoded_size, octet, decoded_size);
  int mcc = 0;
  int mnc = 0;
  mcc += ((octet & 0x0f) * 100 + ((octet & 0xf0) >> 4) * 10);

  DECODE_U8(buf + decoded_size, octet, decoded_size);
  mcc += (octet & 0x0f);

  if ((octet & 0xf0) == 0xf0) {
    DECODE_U8(buf + decoded_size, octet, decoded_size);
    mnc += ((octet & 0x0f) * 10 + ((octet & 0xf0) >> 4));
  } else {
    mnc += ((octet & 0xf0) >> 4);
    DECODE_U8(buf + decoded_size, octet, decoded_size);

    mnc += ((octet & 0x0f) * 100 + ((octet & 0xf0) >> 4) * 10);
  }

  mnc_str = std::to_string(mnc);
  if (mnc < 10) {
    mnc_str = "0" + mnc_str;
  }

  mcc_str = std::to_string(mcc);
  if (mcc < 10) {
    mcc_str = "00" + mcc_str;
  } else if (mcc < 100) {
    mcc_str = "0" + mcc_str;
  }

  Logger::nas_mm().debug("MCC %s, MNC %s", mcc_str.c_str(), mnc_str.c_str());
  return decoded_size;
}
