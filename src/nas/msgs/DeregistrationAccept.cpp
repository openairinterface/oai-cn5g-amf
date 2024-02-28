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

#include "DeregistrationAccept.hpp"

#include "NasHelper.hpp"

using namespace nas;

//------------------------------------------------------------------------------
DeregistrationAccept::DeregistrationAccept(bool is_ue_originating)
    : NasMmPlainHeader() {
  NasMmPlainHeader::SetEpd(k5gsMobilityManagementMessages);
  if (is_ue_originating) {
    NasMmPlainHeader::SetMessageType(kDeregistrationAcceptUeOriginating);
  } else {
    NasMmPlainHeader::SetMessageType(kDeregistrationAcceptUeTerminated);
  }
}

//------------------------------------------------------------------------------
DeregistrationAccept::~DeregistrationAccept() {}

//------------------------------------------------------------------------------
void DeregistrationAccept::SetHeader(uint8_t security_header_type) {
  NasMmPlainHeader::SetSecurityHeaderType(security_header_type);
}

//------------------------------------------------------------------------------
int DeregistrationAccept::Encode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding De-registration Accept message");

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

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded De-registration Accept message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DeregistrationAccept::Decode(uint8_t* buf, int len) {
  int decoded_size   = 0;
  int decoded_result = 0;

  // Header
  decoded_result = NasMmPlainHeader::Decode(buf, len);
  if (decoded_result == KEncodeDecodeError) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error("Decoding NAS Header error");
    return KEncodeDecodeError;
  }
  decoded_size += decoded_result;

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded De-registrationReject message len (%d)", decoded_size);
  return decoded_size;
}
