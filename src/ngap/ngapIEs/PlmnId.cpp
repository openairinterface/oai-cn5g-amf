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

#include "PlmnId.hpp"

#include "utils.hpp"

extern "C" {
#include <math.h>
}

namespace ngap {

//------------------------------------------------------------------------------
PlmnId::PlmnId() {
  mcc_digit1 = 0;
  mcc_digit2 = 0;
  mcc_digit3 = 0;

  mnc_digit1 = 0;
  mnc_digit2 = 0;
  mnc_digit3 = 0;
}

//------------------------------------------------------------------------------
PlmnId::~PlmnId() {}

//------------------------------------------------------------------------------
void PlmnId::set(const std::string& mcc, const std::string& mnc) {
  int mcc_value = utils::fromString<int>(mcc);
  int mnc_value = utils::fromString<int>(mnc);

  mcc_digit1 = mcc_value / 100;
  mcc_digit2 = (mcc_value - mcc_digit1 * 100) / 10;
  mcc_digit3 = mcc_value % 10;

  if (mnc_value > 99) {
    mnc_digit3 = mnc_value / 100;
  } else {
    mnc_digit3 = 0xf;
  }
  mnc_digit1 = (uint8_t) std::floor((double) (mnc_value % 100) / 10);
  mnc_digit2 = mnc_value % 10;
}

//------------------------------------------------------------------------------
void PlmnId::get(std::string& mcc, std::string& mnc) const {
  getMcc(mcc);
  getMnc(mnc);
}

//------------------------------------------------------------------------------
void PlmnId::getMcc(std::string& mcc) const {
  int m_mcc = mcc_digit1 * 100 + mcc_digit2 * 10 + mcc_digit3;
  mcc       = std::to_string(m_mcc);
  if ((mcc_digit2 == 0) and (mcc_digit1 == 0)) {
    mcc = "00" + mcc;
  } else if (mcc_digit1 == 0) {
    mcc = "0" + mcc;
  }
}

//------------------------------------------------------------------------------
void PlmnId::getMnc(std::string& mnc) const {
  int m_mnc = 0;
  if (mnc_digit3 == 0xf) {
    m_mnc = mnc_digit1 * 10 + mnc_digit2;
    if (mnc_digit1 == 0) {
      mnc = "0" + std::to_string(m_mnc);
      return;
    }
  } else {
    m_mnc = mnc_digit3 * 100 + mnc_digit1 * 10 + mnc_digit2;
  }
  mnc = std::to_string(m_mnc);
}

//------------------------------------------------------------------------------
bool PlmnId::encode(Ngap_PLMNIdentity_t& plmn) {
  plmn.size = 3;  // OCTET_STRING(SIZE(3))  9.3.3.5, 3gpp ts 38.413 V15.4.0
  plmn.buf  = (uint8_t*) calloc(1, 3 * sizeof(uint8_t));
  if (!plmn.buf) return false;

  plmn.buf[0] = 0x00 | ((mcc_digit2 & 0x0f) << 4) | (mcc_digit1 & 0x0f);
  plmn.buf[1] = 0x00 | ((mnc_digit3 & 0x0f) << 4) | (mcc_digit3 & 0x0f);
  plmn.buf[2] = 0x00 | ((mnc_digit2 & 0x0f) << 4) | (mnc_digit1 & 0x0f);
  return true;
}

//------------------------------------------------------------------------------
bool PlmnId::decode(const Ngap_PLMNIdentity_t& plmn) {
  if (!plmn.buf) return false;
  if (plmn.size < 3) return false;

  mcc_digit1 = plmn.buf[0] & 0x0f;
  mcc_digit2 = plmn.buf[0] >> 4;

  mcc_digit3 = plmn.buf[1] & 0x0f;
  mnc_digit3 = plmn.buf[1] >> 4;
  mnc_digit1 = plmn.buf[2] & 0x0f;
  mnc_digit2 = plmn.buf[2] >> 4;

  return true;
}

}  // namespace ngap
