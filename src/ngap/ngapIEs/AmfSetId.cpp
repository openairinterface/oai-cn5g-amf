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

#include "AmfSetId.hpp"

#include "output_wrapper.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
AmfSetId::AmfSetId() {
  m_Id = 0;
}

//------------------------------------------------------------------------------
AmfSetId::~AmfSetId() {}

//------------------------------------------------------------------------------
bool AmfSetId::set(const std::string& id) {
  uint16_t tmp = utils::fromString<uint16_t>(id);
  if (tmp > kAmfSetIdMaxValue) return false;
  m_Id = tmp;
  return true;
}

//------------------------------------------------------------------------------
bool AmfSetId::set(const uint16_t& id) {
  if (id > kAmfSetIdMaxValue) return false;
  m_Id = id;
  return true;
}

//------------------------------------------------------------------------------
void AmfSetId::get(std::string& id) const {
  id = std::to_string(m_Id);
}

//------------------------------------------------------------------------------
void AmfSetId::get(uint16_t& id) const {
  id = m_Id;
}

//------------------------------------------------------------------------------
bool AmfSetId::encode(Ngap_AMFSetID_t& amfSetId) const {
  amfSetId.size   = 2;
  uint8_t* buffer = (uint8_t*) calloc(1, sizeof(uint16_t));
  if (!buffer) return false;
  //*(uint16_t *)buffer = m_Id & 0x3ff;
  buffer[0]            = ((m_Id & 0x03fc) >> 2);
  buffer[1]            = ((m_Id & 0x0003) << 6);
  amfSetId.buf         = buffer;
  amfSetId.bits_unused = 6;

  return true;
}

//------------------------------------------------------------------------------
bool AmfSetId::decode(const Ngap_AMFSetID_t& amfSetId) {
  if (!amfSetId.buf) return false;
  output_wrapper::print_buffer("ngap", "AMFSetID", amfSetId.buf, amfSetId.size);

  uint16_t temp = 0;
  temp |= amfSetId.buf[0] << 8;
  temp |= amfSetId.buf[1];
  m_Id = (temp & 0xffc0) >> 6;  // 1111 1111 11 00 0000

  return true;
}
}  // namespace ngap
