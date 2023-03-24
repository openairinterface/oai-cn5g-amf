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

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "NasMmPlainHeader.hpp"

using namespace nas;

//------------------------------------------------------------------------------
NasMmPlainHeader::NasMmPlainHeader(uint8_t epd) : epd_(epd) {}

//------------------------------------------------------------------------------
NasMmPlainHeader::NasMmPlainHeader(uint8_t epd, uint8_t msg_type)
    : epd_(epd), msg_type_(msg_type) {}

//------------------------------------------------------------------------------
NasMmPlainHeader::~NasMmPlainHeader() {}

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetEpd(uint8_t epd) {
  epd_.Set(epd);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::GetEpd() const {
  return epd_.Get();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetSecurityHeaderType(uint8_t type) {
  secu_header_type_.Set(type);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::GetSecurityHeaderType() const {
  return secu_header_type_.Get();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetMessageType(uint8_t type) {
  msg_type_.Set(type);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::GetMessageType() const {
  return msg_type_.Get();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetHeader(
    uint8_t epd, uint8_t security_header_type, uint8_t msg_type) {
  epd_.Set(epd);
  secu_header_type_.Set(security_header_type);
  msg_type_.Set(msg_type);
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::SetMessageName(const std::string& name) {
  msg_name_ = name;
}

//------------------------------------------------------------------------------
std::string NasMmPlainHeader::GetMessageName() const {
  return msg_name_;
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::GetMessageName(std::string& name) const {
  name = msg_name_;
}

//------------------------------------------------------------------------------
int NasMmPlainHeader::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NasMmPlainHeader");
  if (len < kNasMmPlainHeaderLength) {
    Logger::nas_mm().error(
        "Buffer length is less than %d octets", kNasMmPlainHeaderLength);
    return KEncodeDecodeError;
  } else {
    uint32_t encoded_size = 0;
    uint32_t size         = 0;
    if ((size = epd_.Encode(buf + encoded_size, len - encoded_size)) ==
        KEncodeDecodeError) {
      Logger::nas_mm().error("Encode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    encoded_size += size;

    if ((size = secu_header_type_.Encode(
             buf + encoded_size, len - encoded_size)) == KEncodeDecodeError) {
      Logger::nas_mm().error("Encode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    encoded_size += size;

    if ((size = msg_type_.Encode(buf + encoded_size, len - encoded_size)) ==
        KEncodeDecodeError) {
      Logger::nas_mm().error("Encode NAS MM Header IE error");
      return KEncodeDecodeError;
    }
    encoded_size += size;

    Logger::nas_mm().debug(
        "Encoded NasMmPlainHeader (len %d octets)", encoded_size);
    return encoded_size;
  }
}

//------------------------------------------------------------------------------
int NasMmPlainHeader::Decode(const uint8_t* const buf, int len) {
  Logger::nas_mm().debug("Decoding NasMmPlainHeader");

  int decoded_size = 0;
  if (len < kNasMmPlainHeaderLength) {
    Logger::nas_mm().error(
        "Buffer length is less than %d octets", kNasMmPlainHeaderLength);
    return KEncodeDecodeError;
  }

  int size = 0;
  if ((size = epd_.Decode(buf + decoded_size, len - decoded_size)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Decode NAS MM Header IE error");
    return KEncodeDecodeError;
  }
  decoded_size += size;

  if ((size = secu_header_type_.Decode(
           buf + decoded_size, len - decoded_size)) == KEncodeDecodeError) {
    Logger::nas_mm().error("Decode NAS MM Header IE error");
    return KEncodeDecodeError;
  }
  decoded_size += size;

  if ((size = msg_type_.Decode(buf + decoded_size, len - decoded_size)) ==
      KEncodeDecodeError) {
    Logger::nas_mm().error("Decode NAS MM Header IE error");
    return KEncodeDecodeError;
  }
  decoded_size += size;

  Logger::nas_mm().debug(
      "Decoded NasMmPlainHeader len (%d octets)", decoded_size);
  return decoded_size;
}
