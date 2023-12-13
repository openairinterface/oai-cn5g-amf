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
  buffer_ = nullptr;
  size_   = 0;
}

//------------------------------------------------------------------------------
SecurityKey::~SecurityKey() {}

//------------------------------------------------------------------------------
bool SecurityKey::encode(Ngap_SecurityKey_t& security_key) {
  security_key.bits_unused = 0;
  security_key.size        = 32;
  uint8_t* buffer          = (uint8_t*) calloc(1, 32);
  if (!buffer) return false;
  memcpy(buffer, buffer_, 32);
  security_key.buf = buffer;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::decode(const Ngap_SecurityKey_t& security_key) {
  buffer_ = security_key.buf;
  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::getSecurityKey(uint8_t*& buffer, size_t& size) {
  if (!buffer_) return false;
  buffer = (uint8_t*) buffer_;
  size   = size_;
  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::getSecurityKey(uint8_t*& buffer) {
  if (!buffer_) return false;
  buffer = (uint8_t*) buffer_;
  return true;
}

//------------------------------------------------------------------------------
void SecurityKey::setSecurityKey(uint8_t* buffer, const size_t& size) {
  buffer_ = buffer;
  size_   = size;
}
}  // namespace ngap
