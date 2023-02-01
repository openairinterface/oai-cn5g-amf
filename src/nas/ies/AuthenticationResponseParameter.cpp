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

#include "AuthenticationResponseParameter.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
AuthenticationResponseParameter::AuthenticationResponseParameter()
    : Type4NasIe(kIeiAuthenticationResponseParameter), res_or_res_star_() {
  SetLengthIndicator(4);  // Minimum length for the content (RES or RES*)
}

//------------------------------------------------------------------------------
AuthenticationResponseParameter::AuthenticationResponseParameter(bstring para)
    : Type4NasIe(kIeiAuthenticationResponseParameter) {
  res_or_res_star_ = bstrcpy(para);
  SetLengthIndicator(blength(res_or_res_star_));
}

//------------------------------------------------------------------------------
AuthenticationResponseParameter::~AuthenticationResponseParameter() {}

//------------------------------------------------------------------------------
void AuthenticationResponseParameter::SetValue(const bstring& para) {
  res_or_res_star_ = bstrcpy(para);
}

//------------------------------------------------------------------------------
void AuthenticationResponseParameter::GetValue(bstring& para) const {
  para = bstrcpy(res_or_res_star_);
}

//------------------------------------------------------------------------------
int AuthenticationResponseParameter::Encode(uint8_t* buf, int len) {
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

  // Value
  int size = encode_bstring(
      res_or_res_star_, (buf + encoded_size), len - encoded_size);
  encoded_size += size;

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int AuthenticationResponseParameter::Decode(
    uint8_t* buf, int len, bool is_iei) {
  if (len < kAuthenticationResponseParameterMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kAuthenticationResponseParameterMinimumLength);
    return KEncodeDecodeError;
  }

  uint8_t decoded_size = 0;
  uint8_t octet        = 0;
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());

  // IEI and Length
  int decoded_header_size = Type4NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;

  // RES or RES*
  uint8_t ie_len = GetLengthIndicator();
  decode_bstring(
      &res_or_res_star_, ie_len, (buf + decoded_size), len - decoded_size);
  decoded_size += ie_len;

  for (int i = 0; i < ie_len; i++) {
    Logger::nas_mm().debug(
        "Decoded value 0x%x", (uint8_t) res_or_res_star_->data[i]);
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
