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

#include "SecurityKey.hpp"

namespace ngap {

//------------------------------------------------------------------------------
SecurityKey::SecurityKey() {
  m_Buffer = nullptr;
  m_Size   = 0;
}

//------------------------------------------------------------------------------
SecurityKey::~SecurityKey() {}

//------------------------------------------------------------------------------
bool SecurityKey::encode(Ngap_SecurityKey_t& security_key) const {
  security_key.bits_unused = 0;
  security_key.size        = 32;
  security_key.buf         = (uint8_t*) calloc(1, 32);
  if (!security_key.buf) return false;
  memcpy(security_key.buf, m_Buffer, 32);
  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::decode(const Ngap_SecurityKey_t& security_key) {
  m_Buffer = (uint8_t*) calloc(1, security_key.size);
  memcpy(m_Buffer, security_key.buf, security_key.size);
  m_Size = security_key.size;
  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::get(uint8_t*& buffer, size_t& size) const {
  if (!m_Buffer) return false;
  if (!buffer) buffer = (uint8_t*) calloc(1, m_Size);
  memcpy(buffer, m_Buffer, m_Size);
  size = m_Size;
  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::get(uint8_t*& buffer) const {
  if (!m_Buffer) return false;
  if (!buffer) buffer = (uint8_t*) calloc(1, m_Size);
  memcpy(buffer, m_Buffer, m_Size);
  return true;
}

//------------------------------------------------------------------------------
void SecurityKey::set(uint8_t* buffer, const size_t& size) {
  if (!m_Buffer) m_Buffer = (uint8_t*) calloc(1, size);
  memcpy(m_Buffer, buffer, size);
  m_Size = size;
}
}  // namespace ngap
