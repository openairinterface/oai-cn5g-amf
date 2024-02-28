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

#include "IdentityRequest.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
IdentityRequest::IdentityRequest()
    : NasMmPlainHeader(k5gsMobilityManagementMessages, kIdentityRequest) {}

//------------------------------------------------------------------------------
IdentityRequest::~IdentityRequest() {}

//------------------------------------------------------------------------------
void IdentityRequest::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
void IdentityRequest::Set5gsIdentityType(uint8_t value) {
  ie_5gs_identity_type_.SetValue(value);
}

//------------------------------------------------------------------------------
int IdentityRequest::Encode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding IdentityRequest message");
  int encoded_size    = 0;
  int encoded_ie_size = 0;

  // Header
  if ((encoded_ie_size = NasMmPlainHeader::Encode(buf, len)) ==
      KEncodeDecodeError) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error("Encoding NAS Header error");
    return KEncodeDecodeError;
  }
  encoded_size += encoded_ie_size;

  if ((encoded_ie_size =
           NasHelper::Encode(ie_5gs_identity_type_, buf, len, encoded_size)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }

  if (encoded_ie_size == 0)
    encoded_size++;  // 1/2 for 5GS Identity Type and 1/2 for spare mode

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded IdentityRequest message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int IdentityRequest::Decode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding IdentityRequest message");

  int decoded_size    = 0;
  int decoded_ie_size = 0;

  // Header
  decoded_ie_size = NasMmPlainHeader::Decode(buf, len);
  if (decoded_ie_size == KEncodeDecodeError) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_ie_size;

  if ((decoded_ie_size = NasHelper::Decode(
           ie_5gs_identity_type_, buf, len, decoded_size, false)) ==
      KEncodeDecodeError) {
    return KEncodeDecodeError;
  }
  if (decoded_ie_size == 0) {
    decoded_size++;  // including 1/2 octet for Spare
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded IdentityRequest message len (%d)", decoded_size);
  return decoded_size;
}
