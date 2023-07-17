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

#include "RejectedNssai.hpp"

using namespace nas;

//------------------------------------------------------------------------------
RejectedNssai::RejectedNssai(uint8_t iei) : Type4NasIe(iei) {
  SetLengthIndicator(2);  // 1 for Length/Cause and 1 for SST of the 1st S-NSSAI
}

//------------------------------------------------------------------------------
RejectedNssai::~RejectedNssai() {}

//------------------------------------------------------------------------------
void RejectedNssai::SetRejectedSNssais(
    const std::vector<RejectedSNssai>& nssais) {
  uint8_t length   = 0;
  rejected_nssais_ = nssais;
  for (auto n : nssais) {
    length += n.getLength();
  }

  SetLengthIndicator(length);
}

//------------------------------------------------------------------------------
void RejectedNssai::GetRejectedSNssais(std::vector<RejectedSNssai>& nssais) {
  nssais = rejected_nssais_;
}
//------------------------------------------------------------------------------

int RejectedNssai::Encode(uint8_t* buf, int len) {
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

  for (auto n : rejected_nssais_) {
    int size = n.Encode(buf + encoded_size, len - encoded_size);
    if (size != KEncodeDecodeError) {
      encoded_size += size;
    } else {
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RejectedNssai::Decode(uint8_t* buf, int len, bool is_iei) {
  if (len < kRejectedNssaiMinimumLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kRejectedNssaiMinimumLength);
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

  int ie_len = GetLengthIndicator();
  while (ie_len > 0) {
    RejectedSNssai snssai = {};
    int size = snssai.Decode(buf + decoded_size, len - decoded_size);
    if (size != KEncodeDecodeError) {
      decoded_size += size;
      ie_len -= size;
      rejected_nssais_.push_back(snssai);
    } else {
      return KEncodeDecodeError;
    }
  }

  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
