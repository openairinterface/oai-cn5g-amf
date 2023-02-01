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

#include "ExtendedProtocolDiscriminator.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ExtendedProtocolDiscriminator::ExtendedProtocolDiscriminator(const uint8_t& epd)
    : NasIe(), epd_(epd) {}

//------------------------------------------------------------------------------
ExtendedProtocolDiscriminator::~ExtendedProtocolDiscriminator() {}

//------------------------------------------------------------------------------
bool ExtendedProtocolDiscriminator::Validate(const int& len) const {
  if (len < kExtendedProtocolDiscriminatorLength) {
    Logger::nas_mm().error(
        "Buffer length is less than the minimum length of this IE (%d octet)",
        kExtendedProtocolDiscriminatorLength);
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void ExtendedProtocolDiscriminator::Set(const uint8_t& epd) {
  epd_ = epd;
}

//------------------------------------------------------------------------------
void ExtendedProtocolDiscriminator::Get(uint8_t& epd) const {
  epd = epd_;
}

//------------------------------------------------------------------------------
uint8_t ExtendedProtocolDiscriminator::Get() const {
  return epd_;
}

//------------------------------------------------------------------------------
int ExtendedProtocolDiscriminator::Encode(uint8_t* buf, const int& len) {
  if (!Validate(len)) return KEncodeDecodeError;
  uint32_t encoded_size = 0;
  ENCODE_U8(buf, epd_, encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ExtendedProtocolDiscriminator::Decode(
    const uint8_t* const buf, const int& len, bool is_iei) {
  if (!Validate(len)) return KEncodeDecodeError;
  uint32_t decoded_size = 0;
  DECODE_U8(buf, epd_, decoded_size);
  return decoded_size;
}
