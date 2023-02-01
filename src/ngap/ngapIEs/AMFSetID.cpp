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

#include "AMFSetID.hpp"

#include "String2Value.hpp"

#include <string>

namespace ngap {

//------------------------------------------------------------------------------
AMFSetID::AMFSetID() {
  id_ = 0;
}

//------------------------------------------------------------------------------
AMFSetID::~AMFSetID() {}

//------------------------------------------------------------------------------
bool AMFSetID::set(const std::string& id) {
  uint16_t tmp = fromString<uint16_t>(id);
  if (tmp > kAmfSetIdMaxValue) return false;
  id_ = tmp;
  return true;
}

//------------------------------------------------------------------------------
bool AMFSetID::set(const uint16_t& id) {
  if (id > kAmfSetIdMaxValue) return false;
  id_ = id;
  return true;
}

//------------------------------------------------------------------------------
void AMFSetID::get(std::string& id) {
  id = std::to_string(id_);
}

//------------------------------------------------------------------------------
void AMFSetID::get(uint16_t& id) {
  id = id_;
}

//------------------------------------------------------------------------------
bool AMFSetID::encode(Ngap_AMFSetID_t& amf_set_id) const {
  amf_set_id.size = 2;
  uint8_t* buffer = (uint8_t*) calloc(1, sizeof(uint16_t));
  if (!buffer) return false;
  //*(uint16_t *)buffer = id_ & 0x3ff;
  buffer[0]              = ((id_ & 0x03fc) >> 2);
  buffer[1]              = ((id_ & 0x0003) << 6);
  amf_set_id.buf         = buffer;
  amf_set_id.bits_unused = 6;

  return true;
}

//------------------------------------------------------------------------------
bool AMFSetID::decode(const Ngap_AMFSetID_t& amf_set_id) {
  if (!amf_set_id.buf) return false;
  for (int i = 0; i < amf_set_id.size; i++) {
    printf("%x ", amf_set_id.buf[i]);
  }
  printf("\n");
  uint16_t temp = 0;
  temp |= amf_set_id.buf[0] << 8;
  temp |= amf_set_id.buf[1];
  id_ = (temp & 0xffc0) >> 6;  // 1111 1111 11 00 0000

  return true;
}
}  // namespace ngap
