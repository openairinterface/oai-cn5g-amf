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

#include "RejectedSNssai.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger_base.hpp"
using namespace nas;

//------------------------------------------------------------------------------
RejectedSNssai::RejectedSNssai() {
  length_ = 1;  // only SST
  cause_  = 0;
  sst_    = 0;
  sd_     = std::nullopt;
}

//------------------------------------------------------------------------------
RejectedSNssai::RejectedSNssai(uint8_t cause, uint8_t sst, uint32_t sd) {
  cause_  = cause;
  sst_    = sst;
  sd_     = std::optional<uint32_t>(sd);
  length_ = 4;  // SST and SD
}

//------------------------------------------------------------------------------
RejectedSNssai::~RejectedSNssai() {
  sd_ = std::nullopt;
}

//------------------------------------------------------------------------------
uint8_t RejectedSNssai::GetLength() const {
  return (length_ + 1);  // 1 for length + cause
}

//------------------------------------------------------------------------------
void RejectedSNssai::SetSST(uint8_t sst) {
  sst_ = sst;
  // length_ = 1;
}

//------------------------------------------------------------------------------
void RejectedSNssai::GetSST(uint8_t& sst) const {
  sst = sst_;
}

//------------------------------------------------------------------------------
uint8_t RejectedSNssai::GetSST() const {
  return sst_;
}

//------------------------------------------------------------------------------
void RejectedSNssai::SetSd(uint32_t sd) {
  sd_.emplace(sd);
  length_ = 4;
}

//------------------------------------------------------------------------------
bool RejectedSNssai::GetSd(uint32_t& sd) const {
  if (sd_.has_value()) {
    sd = sd_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------

void RejectedSNssai::GetSd(std::optional<uint32_t>& sd) const {
  sd = sd_;
}

//------------------------------------------------------------------------------
void RejectedSNssai::SetCause(uint8_t cause) {
  cause_ = cause;
}

//------------------------------------------------------------------------------
uint8_t RejectedSNssai::GetCause() const {
  return cause_;
}

//------------------------------------------------------------------------------

void RejectedSNssai::GetCause(uint8_t& cause) const {
  cause = cause_;
}

//------------------------------------------------------------------------------
int RejectedSNssai::Encode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding RejectedSNssai");
  if (len < length_ + 1) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error("len is less than %d", length_);
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
  oai::logger::logger_registry::get_logger(LOGGER_COMMON).debug("SST %d", sst_);

  // SD
  if (sd_.has_value()) {
    ENCODE_U24(buf + encoded_size, sd_.value(), encoded_size);
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .debug("SD 0x%x", sd_.value());
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded RejectedSNssai (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RejectedSNssai::Decode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding RejectedSNssai");
  int decoded_size = 0;
  uint8_t octet    = 0;

  // Length and Cause
  DECODE_U8(buf + decoded_size, octet, decoded_size);
  length_ = (octet >> 4) & 0x0f;
  cause_  = octet & 0x0f;

  // SST
  DECODE_U8(buf + decoded_size, sst_, decoded_size);
  if (length_ == 1) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .debug(
            "Decoded RejectedSNssai length 0x%x,cause 0x%x, SST 0x%x", length_,
            cause_, sst_);
  } else if (length_ == 4) {
    // SD
    uint32_t sd = 0;
    DECODE_U24(buf + decoded_size, sd, decoded_size);
    sd_ = std::optional<uint32_t>(sd);
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .debug(
            "Decoded RejectedSNssai length 0x%x, cause 0x%x, SST 0x%x, SD 0x%x",
            length_, cause_, sst_, sd);
  } else {
    return KEncodeDecodeError;  // invalid value
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded RejectedSNssai (len %d)", decoded_size);
  return decoded_size;
}
