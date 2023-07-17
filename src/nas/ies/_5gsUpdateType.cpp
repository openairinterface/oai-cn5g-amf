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

#include "_5gsUpdateType.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsUpdateType::_5gsUpdateType() : Type4NasIe(kIei5gsUpdateType) {
  eps_pnb_ciot_  = 0;
  _5gs_pnb_ciot_ = 0;
  ng_ran_        = false;
  sms_           = false;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
_5gsUpdateType::_5gsUpdateType(
    uint8_t eps_PNB_CIoT, uint8_t _5gs_PNB_CIoT, bool ng_RAN, bool sms)
    : Type4NasIe(kIei5gsUpdateType) {
  eps_pnb_ciot_  = eps_PNB_CIoT;
  _5gs_pnb_ciot_ = _5gs_PNB_CIoT;
  ng_ran_        = ng_RAN;
  sms_           = sms;
  SetLengthIndicator(1);
}

//------------------------------------------------------------------------------
_5gsUpdateType::~_5gsUpdateType() {}

//------------------------------------------------------------------------------
void _5gsUpdateType::SetEpsPnbCiot(uint8_t value) {
  eps_pnb_ciot_ = value & 0x03;  // 2 bits
}

//------------------------------------------------------------------------------
void _5gsUpdateType::Set5gsPnbCiot(uint8_t value) {
  _5gs_pnb_ciot_ = value & 0x03;  // 2 bits
}

//------------------------------------------------------------------------------
void _5gsUpdateType::SetNgRan(uint8_t value) {
  ng_ran_ = value & 0x01;  // 1 bit
}

//------------------------------------------------------------------------------
void _5gsUpdateType::SetSms(uint8_t value) {
  sms_ = value & 0x01;  // 1 bit
}

//------------------------------------------------------------------------------
uint8_t _5gsUpdateType::GetEpsPnbCiot() const {
  return eps_pnb_ciot_;
}

//------------------------------------------------------------------------------
uint8_t _5gsUpdateType::Get5gsPnbCiot() const {
  return _5gs_pnb_ciot_;
}

//------------------------------------------------------------------------------
bool _5gsUpdateType::GetNgRan() const {
  return ng_ran_;
}

//------------------------------------------------------------------------------
bool _5gsUpdateType::GetSms() const {
  return sms_;
}

//------------------------------------------------------------------------------
int _5gsUpdateType::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());
  int ie_len = GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    Logger::nas_mm().error(
        "Size of the buffer is not enough to store this IE (IE len %d)",
        ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size = 0;
  // IEI and Length
  int encoded_header_size = Type4NasIe::Encode(buf + encoded_size, len);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  uint8_t octet = 0;
  octet = (eps_pnb_ciot_ << 4) | (_5gs_pnb_ciot_ << 2) | (ng_ran_ << 1) | sms_;
  ENCODE_U8(buf + encoded_size, octet, encoded_size);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5gsUpdateType::Decode(uint8_t* buf, int len, bool is_iei) {
  if (len < k5gsUpdateTypeLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        k5gsUpdateTypeLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  // decoded_size += Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  DECODE_U8(buf + decoded_size, octet, decoded_size);

  eps_pnb_ciot_  = (octet & 0x30) >> 4;
  _5gs_pnb_ciot_ = (octet & 0x0c) >> 2;
  ng_ran_        = (octet & 0x02) >> 1;
  sms_           = (octet & 0x01);

  Logger::nas_mm().debug(
      "EPS_PNB_CIoT 0x%x, _5GS_PNB_CIoT 0x%x, NG RAN 0x%x, SMS 0x%x",
      eps_pnb_ciot_, _5gs_pnb_ciot_, ng_ran_, sms_);

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
