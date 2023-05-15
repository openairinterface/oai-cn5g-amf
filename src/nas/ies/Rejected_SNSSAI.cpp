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

#include "Rejected_SNSSAI.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Rejected_SNSSAI::Rejected_SNSSAI() {
  length_ = 1;  // only SST
  cause_  = 0;
  sst_    = 0;
  sd_     = std::nullopt;
}

//------------------------------------------------------------------------------
Rejected_SNSSAI::Rejected_SNSSAI(
    const uint8_t& cause, const uint8_t& sst, const uint32_t& sd) {
  cause_  = cause;
  sst_    = sst;
  sd_     = std::optional<uint32_t>(sd);
  length_ = 4;  // SST and SD
}

//------------------------------------------------------------------------------
Rejected_SNSSAI::~Rejected_SNSSAI() {
  sd_ = std::nullopt;
}

//------------------------------------------------------------------------------
uint8_t Rejected_SNSSAI::getLength() {
  return (length_ + 1);  // 1 for length + cause
}

//------------------------------------------------------------------------------
void Rejected_SNSSAI::setSST(const uint8_t& sst) {
  sst_ = sst;
  // length_ = 1;
}

//------------------------------------------------------------------------------
void Rejected_SNSSAI::getSST(uint8_t& sst) {
  sst = sst_;
}

//------------------------------------------------------------------------------
uint8_t Rejected_SNSSAI::getSST() {
  return sst_;
}

//------------------------------------------------------------------------------
void Rejected_SNSSAI::setSd(const uint32_t& sd) {
  sd_.emplace(sd);
  length_ = 4;
}

//------------------------------------------------------------------------------
bool Rejected_SNSSAI::getSd(uint32_t& sd) {
  if (sd_.has_value()) {
    sd = sd_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------

void Rejected_SNSSAI::getSd(std::optional<uint32_t>& sd) {
  sd = sd_;
}

//------------------------------------------------------------------------------
void Rejected_SNSSAI::setCause(const uint8_t& cause) {
  cause_ = cause;
}

//------------------------------------------------------------------------------
uint8_t Rejected_SNSSAI::getCause() {
  return cause_;
}

//------------------------------------------------------------------------------

void Rejected_SNSSAI::getCause(uint8_t& cause) {
  cause = cause_;
}

//------------------------------------------------------------------------------
int Rejected_SNSSAI::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Rejected_SNSSAI");
  if (len < length_ + 1) {
    Logger::nas_mm().error("len is less than %d", length_);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  uint8_t octet    = 0;
  if (sd_.has_value()) {
    length_ = 4;
  } else {
    length_ = 1;
  }

  // Length + Cause
  octet = (length_ << 4) | (0x0f & cause_);
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  // SST
  ENCODE_U8(buf + encoded_size, sst_, encoded_size);
  Logger::nas_mm().debug("SST %d", sst_);

  // SD
  if (sd_.has_value()) {
    ENCODE_U24(buf + encoded_size, sd_.value(), encoded_size);
    /*ENCODE_U8(
        buf + encoded_size, (sd_.value() & 0x00ff0000) >> 16, encoded_size);
    ENCODE_U8(
        buf + encoded_size, (sd_.value() & 0x0000ff00) >> 8, encoded_size);
    ENCODE_U8(buf + encoded_size, (sd_.value() & 0x000000ff), encoded_size);
    */
    Logger::nas_mm().debug("SD 0x%x", sd_.value());
  }

  Logger::nas_mm().debug("Encoded Rejected_SNSSAI (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Rejected_SNSSAI::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding Rejected_SNSSAI");
  int decoded_size = 0;
  uint8_t octet    = 0;

  // Length and Cause
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  length_ = (octet >> 4) & 0x0f;
  cause_  = octet & 0x0f;

  // SST
  DECODE_U8(buf + decoded_size, sst_, decoded_size);
  if (length_ == 1) {
    Logger::nas_mm().debug(
        "Decoded Rejected_SNSSAI length 0x%x,cause 0x%x, SST 0x%x", length_,
        cause_, sst_);
  } else if (length_ == 4) {
    // SD
    uint32_t sd = 0;
    DECODE_U24(buf + decoded_size, sd, decoded_size);
    sd_ = std::optional<uint32_t>(sd);
    Logger::nas_mm().debug(
        "Decoded Rejected_SNSSAI length 0x%x, cause 0x%x, SST 0x%x, SD 0x%x",
        length_, cause_, sst_, sd);
  } else {
    return KEncodeDecodeError;  // invalid value
  }

  Logger::nas_mm().debug("Decoded Rejected_SNSSAI (len %d)", decoded_size);
  return decoded_size;
}
