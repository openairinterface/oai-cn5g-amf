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

#include "SNssai.hpp"

#include "amf.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SNssai::SNssai(std::optional<uint8_t> iei) : Type4NasIe() {
  if (iei_.has_value()) {
    SetIei(iei.value());
  }
  sst_              = {0};
  sd_               = std::nullopt;
  mapped_hplmn_sst_ = std::nullopt;
  mapped_hplmn_sd_  = std::nullopt;
  SetLengthIndicator(1);  // SST
}

//------------------------------------------------------------------------------
SNssai::SNssai(std::optional<uint8_t> iei, SNSSAI_s snssai) {
  // IEI
  if (iei_.has_value()) {
    SetIei(iei.value());
  }

  // SST
  sst_ = snssai.sst;

  uint8_t len = snssai.length;
  len -= 1;

  // SD
  if (len == SD_LENGTH + SST_LENGTH + SD_LENGTH) {
    mapped_hplmn_sd_ = std::optional<uint32_t>{snssai.mHplmnSd};
    len -= SD_LENGTH;
  } else {
    mapped_hplmn_sd_ = std::nullopt;
  }

  // mappedHPLMN SST
  if (len == SD_LENGTH + SST_LENGTH) {
    mapped_hplmn_sst_ = std::optional<uint8_t>{snssai.mHplmnSst};
    len -= SST_LENGTH;
  } else {
    mapped_hplmn_sst_ = std::nullopt;
  }

  // mappedHPLMN SD
  if (len == SD_LENGTH) {
    sd_ = std::optional<uint32_t>{snssai.sd};
    len -= SD_LENGTH;
  } else {
    sd_ = std::nullopt;
  }

  SetLengthIndicator(snssai.length);
}

//------------------------------------------------------------------------------
SNssai::~SNssai() {}

//------------------------------------------------------------------------------
void SNssai::getValue(SNSSAI_t& snssai) const {
  uint8_t len = {0};
  // SST
  snssai.sst = sst_;
  len += SST_LENGTH;

  // SD
  if (sd_.has_value()) {
    snssai.sd = sd_.value();
    len += SD_LENGTH;
  } else {
    snssai.sd = SD_NO_VALUE;
  }

  // mappedHPLMN SST
  if (mapped_hplmn_sst_.has_value()) {
    snssai.mHplmnSd = mapped_hplmn_sst_.value();
    len += SST_LENGTH;
  } else {
    snssai.mHplmnSst = 0;  // TODO
  }

  // mappedHPLMN SD
  if (mapped_hplmn_sd_.has_value()) {
    snssai.mHplmnSd = mapped_hplmn_sd_.value();
    len += SD_LENGTH;
  } else {
    snssai.mHplmnSd = SD_NO_VALUE;
  }

  // Length
  snssai.length = len;
}

//------------------------------------------------------------------------------
void SNssai::SetSNSSAI(
    std::optional<int8_t> iei, uint8_t sst, std::optional<int32_t> sd,
    std::optional<int8_t> mapped_hplmn_sst,
    std::optional<int32_t> mapped_hplmn_sd) {
  // IEI
  if (iei_.has_value()) {
    SetIei(iei.value());
  }
  uint8_t length = 0;
  // SST
  sst_ = sst;
  length += SST_LENGTH;

  // SD
  sd_ = sd;
  if (sd_.has_value()) {
    length += SD_LENGTH;
  }

  // mappedHPLMN SST
  mapped_hplmn_sst_ = mapped_hplmn_sst;
  if (mapped_hplmn_sst_.has_value()) {
    length += SST_LENGTH;
  }

  // mappedHPLMN SD
  mapped_hplmn_sd_ = mapped_hplmn_sd;
  if (mapped_hplmn_sd_.has_value()) {
    length += SD_LENGTH;
  }

  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
std::string SNssai::ToString() {
  std::string s;
  s.append(fmt::format("SST {:#x}", sst_));

  if (sd_.has_value()) {
    s.append(fmt::format(" SD {:#x}", sd_.value()));
  }

  if (mapped_hplmn_sst_.has_value()) {
    s.append(fmt::format(" M-HPLMN SST {:#x}", mapped_hplmn_sst_.value()));
  }

  if (mapped_hplmn_sd_.has_value()) {
    s.append(fmt::format(" M-HPLMN SD {:#x}", mapped_hplmn_sd_.value()));
  }
  return s;
}

//------------------------------------------------------------------------------
int SNssai::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {
    Logger::nas_mm().error("Len is less than %d", ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // SST
  ENCODE_U8(buf + encoded_size, sst_, encoded_size);

  // SD
  if (sd_.has_value())
    ENCODE_U24(buf + encoded_size, sd_.value(), encoded_size);

  // mappedHPLMN SST
  if (mapped_hplmn_sst_.has_value())
    ENCODE_U8(buf + encoded_size, mapped_hplmn_sst_.value(), encoded_size);

  // mappedHPLMN SD
  if (mapped_hplmn_sd_.has_value())
    ENCODE_U24(buf + encoded_size, mapped_hplmn_sd_.value(), encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SNssai::Decode(uint8_t* buf, int len, const bool is_iei) {
  if (len < kSNssaiMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kSNssaiMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  switch (GetLengthIndicator()) {
    case 1: {
      DECODE_U8(buf + decoded_size, sst_, decoded_size);
      sd_               = std::nullopt;
      mapped_hplmn_sst_ = std::nullopt;
      mapped_hplmn_sd_  = std::nullopt;
    } break;
    case 4: {
      // SST
      DECODE_U8(buf + decoded_size, sst_, decoded_size);
      // SSD
      uint32_t sd_value = {0};
      DECODE_U24(buf + decoded_size, sd_value, decoded_size);
      sd_ = std::optional<uint32_t>{sd_value};
      // mapped HPLMN SST/SD
      mapped_hplmn_sst_ = std::nullopt;
      mapped_hplmn_sd_  = std::nullopt;

    } break;
    case 5: {
      // SST
      DECODE_U8(buf + decoded_size, sst_, decoded_size);
      // SD
      uint32_t sd_value = {0};
      DECODE_U24(buf + decoded_size, sd_value, decoded_size);
      sd_ = std::optional<uint32_t>{sd_value};
      // Mapped HPLMN SST
      uint8_t mapped_hplmn_sst_value = {0};
      DECODE_U8(buf + decoded_size, mapped_hplmn_sst_value, decoded_size);
      mapped_hplmn_sst_ = std::optional<uint8_t>{mapped_hplmn_sst_value};
      // Mapped HPLMN SD
      mapped_hplmn_sd_ = std::nullopt;
    } break;
    case 8: {
      // SST
      DECODE_U8(buf + decoded_size, sst_, decoded_size);
      // SD
      uint32_t sd_value = {0};
      DECODE_U24(buf + decoded_size, sd_value, decoded_size);
      sd_ = std::optional<uint32_t>{sd_value};
      // Mapped HPLMN SST
      uint8_t mapped_hplmn_sst_value = {0};
      DECODE_U8(buf + decoded_size, mapped_hplmn_sst_value, decoded_size);
      mapped_hplmn_sst_ = std::optional<uint8_t>{mapped_hplmn_sst_value};
      // Mapped HPLMN SD
      uint32_t mapped_hplmn_sd_value = {0};
      DECODE_U24(buf + decoded_size, mapped_hplmn_sd_value, decoded_size);
      mapped_hplmn_sd_ = std::optional<uint32_t>{mapped_hplmn_sd_value};
    } break;
  }

  Logger::nas_mm().debug("Decoded S-NSSAI %s", ToString().c_str());
  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
