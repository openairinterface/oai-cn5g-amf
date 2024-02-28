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

#include "Ladn.hpp"

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace nas;

//------------------------------------------------------------------------------
Ladn::Ladn() : dnn_(false), ta_list_(false) {}

//------------------------------------------------------------------------------
Ladn::~Ladn() {}

//------------------------------------------------------------------------------
void Ladn::Set(const Dnn& value) {
  dnn_ = value;
}

//------------------------------------------------------------------------------
void Ladn::Set(const _5gsTrackingAreaIdList& value) {
  ta_list_ = value;
}

//------------------------------------------------------------------------------
uint32_t Ladn::GetLength() const {
  return (dnn_.GetIeLength() + ta_list_.GetIeLength());
}
//------------------------------------------------------------------------------
int Ladn::Encode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding LADN");

  int ie_len = dnn_.GetIeLength();
  ie_len += ta_list_.GetIeLength();

  if (len < ie_len) {  // Length of the content + IEI/Len
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error(
            "Size of the buffer is not enough to store this IE (IE len %d)",
            ie_len);
    return KEncodeDecodeError;
  }

  int encoded_size    = 0;
  int encoded_ie_size = 0;

  encoded_ie_size = dnn_.Encode(buf + encoded_size, len);
  if (encoded_ie_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_ie_size;

  encoded_ie_size = ta_list_.Encode(buf + encoded_size, len);
  if (encoded_ie_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_ie_size;

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded LADN, len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Ladn::Decode(uint8_t* buf, int len) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding LADN");
  int decoded_size = 0;
  // TODO:

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded LADN (len %d)", decoded_size);
  return decoded_size;
}
