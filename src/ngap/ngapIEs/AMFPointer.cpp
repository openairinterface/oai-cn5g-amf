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

#include "AMFPointer.hpp"

#include "String2Value.hpp"

namespace ngap {

//------------------------------------------------------------------------------
AMFPointer::AMFPointer() {
  pointer_ = 0;
}

//------------------------------------------------------------------------------
AMFPointer::~AMFPointer() {}

//------------------------------------------------------------------------------
bool AMFPointer::set(const std::string& pointer) {
  uint8_t tmp = fromString<int>(pointer);
  if (tmp > kAmfPointerMaxValue) return false;
  pointer_ = tmp;
  return true;
}

//------------------------------------------------------------------------------
bool AMFPointer::set(const uint8_t& pointer) {
  if (pointer > kAmfPointerMaxValue) return false;
  pointer_ = pointer;
  return true;
}

//------------------------------------------------------------------------------
void AMFPointer::get(std::string& pointer) {
  pointer = std::to_string(pointer_);
}

//------------------------------------------------------------------------------
bool AMFPointer::encode(Ngap_AMFPointer_t& amf_pointer) {
  amf_pointer.size = 1;
  uint8_t* buffer  = (uint8_t*) calloc(1, sizeof(uint8_t));
  if (!buffer) return false;
  *buffer                 = ((pointer_ & 0x3f) << 2);
  amf_pointer.buf         = buffer;
  amf_pointer.bits_unused = 2;

  return true;
}

//------------------------------------------------------------------------------
bool AMFPointer::decode(Ngap_AMFPointer_t& amf_pointer) {
  if (!amf_pointer.buf) return false;
  pointer_ = (amf_pointer.buf[0] & 0xfc) >> 2;  // 1111 1100

  return true;
}
}  // namespace ngap
