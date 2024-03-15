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

#include "FiveGSTmsi.hpp"

#include "amf_conversions.hpp"

using namespace ngap;

//------------------------------------------------------------------------------
FiveGSTmsi::FiveGSTmsi() {}

//------------------------------------------------------------------------------
FiveGSTmsi::~FiveGSTmsi() {}

//------------------------------------------------------------------------------
void FiveGSTmsi::getTmsi(std::string& tmsi) {
  tmsi = _5g_s_tmsi_;
}

//------------------------------------------------------------------------------
void FiveGSTmsi::get(
    std::string& set_id, std::string& pointer, std::string& tmsi) const {
  amf_set_id_.get(set_id);
  amf_pointer_.get(pointer);
  tmsi = tmsi_value_;
}

//------------------------------------------------------------------------------
bool FiveGSTmsi::set(
    const std::string& set_id, const std::string& pointer,
    const std::string& tmsi) {
  if (!amf_set_id_.set(set_id)) return false;
  if (!amf_pointer_.set(pointer)) return false;
  tmsi_value_ = tmsi;
  return true;
}

//------------------------------------------------------------------------------
bool FiveGSTmsi::encode(Ngap_FiveG_S_TMSI_t& pdu) {
  amf_set_id_.encode(pdu.aMFSetID);
  amf_pointer_.encode(pdu.aMFPointer);

  uint32_t tmsi       = (uint32_t) std::stol(tmsi_value_);
  uint8_t* buf        = (uint8_t*) malloc(sizeof(uint32_t));
  *(uint32_t*) buf    = htonl(tmsi);
  pdu.fiveG_TMSI.buf  = buf;
  pdu.fiveG_TMSI.size = sizeof(uint32_t);

  return true;
}

//------------------------------------------------------------------------------
bool FiveGSTmsi::decode(const Ngap_FiveG_S_TMSI_t& pdu) {
  amf_set_id_.decode(pdu.aMFSetID);
  amf_pointer_.decode(pdu.aMFPointer);

  uint32_t tmsi = ntohl(*(uint32_t*) pdu.fiveG_TMSI.buf);
  int size      = pdu.fiveG_TMSI.size;

  // AMF Set ID: 10 bits, AMF pointer ID: 6 bits
  uint16_t amf_set_id_value = 0;
  amf_set_id_.get(amf_set_id_value);
  uint8_t amf_pointer_value = 0;
  amf_pointer_.get(amf_pointer_value);
  uint16_t first_part_tmsi = 0xffff & (((amf_set_id_value & 0x03ff) << 6) |
                                       (amf_pointer_value & 0x3f));
  std::string first_part_tmsi_str = {};
  amf_conv::int_to_string_hex(first_part_tmsi, first_part_tmsi_str, 2);

  tmsi_value_ = amf_conv::tmsi_to_string(tmsi);
  _5g_s_tmsi_ = first_part_tmsi_str + tmsi_value_;

  return true;
}
