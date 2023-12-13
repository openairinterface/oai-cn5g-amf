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

#include "S-NSSAI.hpp"

#include "amf.hpp"
#include "conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
S_NSSAI::S_NSSAI() {
  //  sdIsSet = false;
  sst_ = 0;
  //  sd      = SD_NO_VALUE;
  sd_ = std::nullopt;
}

//------------------------------------------------------------------------------
S_NSSAI::~S_NSSAI() {}

//------------------------------------------------------------------------------
bool S_NSSAI::encodeSD(Ngap_SD_t& m_sd) {
  if (!sd_.has_value()) {
    return false;
  }
  m_sd.size = 3;
  m_sd.buf  = (uint8_t*) calloc(3, sizeof(uint8_t));
  if (!m_sd.buf) return false;
  m_sd.buf[0] = (sd_.value() & 0x00ff0000) >> 16;
  m_sd.buf[1] = (sd_.value() & 0x0000ff00) >> 8;
  m_sd.buf[2] = (sd_.value() & 0x000000ff) >> 0;
  return true;
}

//------------------------------------------------------------------------------
bool S_NSSAI::decodeSD(const Ngap_SD_t& m_sd) {
  if (!m_sd.buf) return false;

  uint32_t value = SD_NO_VALUE;
  if (m_sd.size == 3) {
    value = ((m_sd.buf[0] << 16) + (m_sd.buf[1] << 8) + m_sd.buf[2]);
    sd_   = std::optional<uint32_t>(value);
    return true;
  } else if (m_sd.size == 4) {
    value = ((m_sd.buf[1] << 16) + (m_sd.buf[2] << 8) + m_sd.buf[3]);
    sd_   = std::optional<uint32_t>(value);
    return true;
  }

  return false;
}

//------------------------------------------------------------------------------
void S_NSSAI::setSst(const std::string& sst) {
  conv::string_to_int8(sst, sst_);
}

//------------------------------------------------------------------------------
void S_NSSAI::setSst(const uint8_t& sst) {
  sst_ = sst;
}
//------------------------------------------------------------------------------
void S_NSSAI::getSst(std::string& sst) const {
  sst = std::to_string(sst_);
}

//------------------------------------------------------------------------------
std::string S_NSSAI::getSst() const {
  return std::to_string(sst_);
}

//------------------------------------------------------------------------------
void S_NSSAI::setSd(const std::string& sd_str) {
  uint32_t sd = SD_NO_VALUE;
  if (conv::sd_string_to_int(sd_str, sd)) {
    sd_ = std::optional<uint32_t>(sd);
  }
}

//------------------------------------------------------------------------------
void S_NSSAI::setSd(const uint32_t& sd) {
  sd_ = std::optional<uint32_t>(sd);
}

//------------------------------------------------------------------------------
bool S_NSSAI::getSd(std::string& sd) const {
  if (sd_.has_value()) {
    sd = std::to_string(sd_.value());
    return true;
  }
  sd = std::to_string(SD_NO_VALUE);
  return false;
}

//------------------------------------------------------------------------------
bool S_NSSAI::getSd(uint32_t& sd) const {
  if (sd_.has_value()) {
    sd = sd_.value();
    return true;
  }
  sd = SD_NO_VALUE;
  return false;
}

//------------------------------------------------------------------------------
std::string S_NSSAI::getSd() const {
  if (sd_.has_value()) {
    return std::to_string(sd_.value());
  }
  return std::to_string(SD_NO_VALUE);
}

//------------------------------------------------------------------------------
bool S_NSSAI::encode(Ngap_S_NSSAI_t& s_NSSAI) {
  conv::int8_2_octet_string(sst_, s_NSSAI.sST);
  if (sd_.has_value() && (sd_.value() != SD_NO_VALUE)) {
    s_NSSAI.sD = (Ngap_SD_t*) calloc(1, sizeof(Ngap_SD_t));
    if (!s_NSSAI.sD) return false;
    if (!encodeSD(*s_NSSAI.sD)) {
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool S_NSSAI::decode(const Ngap_S_NSSAI_t& s_NSSAI) {
  if (!conv::octet_string_2_int8(s_NSSAI.sST, sst_)) return false;
  if (s_NSSAI.sD) {
    if (!decodeSD(*s_NSSAI.sD)) return false;
  }
  return true;
}

}  // namespace ngap
