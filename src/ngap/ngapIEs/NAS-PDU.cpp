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

#include "NAS-PDU.hpp"
#include "conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NAS_PDU::NAS_PDU() {}

//------------------------------------------------------------------------------
NAS_PDU::~NAS_PDU() {}

//------------------------------------------------------------------------------
bool NAS_PDU::encode(Ngap_NAS_PDU_t& nas_pdu) {
  return conv::bstring_2_octet_string(pdu_bstring, nas_pdu);
}

//------------------------------------------------------------------------------
bool NAS_PDU::decode(Ngap_NAS_PDU_t& nas_pdu) {
  if (!nas_pdu.buf) return false;
  return conv::octet_string_2_bstring(nas_pdu, pdu_bstring);
}
/*

//------------------------------------------------------------------------------
bool NAS_PDU::get(uint8_t*& buffer, size_t& size) const {
  if (!pdu_.buf) return false;
  if (pdu_.size < 0) return false;
  memcpy(buffer, pdu_.buf, pdu_.size);
  size = pdu_.size;
  return true;
}
*/
//------------------------------------------------------------------------------
void NAS_PDU::set(uint8_t* buffer, size_t size) {
  if (!buffer) return;
  pdu_bstring = blk2bstr(buffer, size);
  return;
}

//------------------------------------------------------------------------------
bool NAS_PDU::get(OCTET_STRING_t& pdu) const {
  conv::bstring_2_octet_string(pdu_bstring, pdu);
  return true;
}

//------------------------------------------------------------------------------
bool NAS_PDU::set(const OCTET_STRING_t& pdu) {
  conv::octet_string_2_bstring(pdu, pdu_bstring);
  return true;
}

//------------------------------------------------------------------------------
bool NAS_PDU::get(NAS_PDU& nas_pdu) const {
  return nas_pdu.set(pdu_bstring);
}

//------------------------------------------------------------------------------
bool NAS_PDU::set(const NAS_PDU& nas_pdu) {
  bstring pdu = {};
  if (!nas_pdu.get(pdu)) return false;
  return set(pdu);
}

//------------------------------------------------------------------------------
bool NAS_PDU::get(bstring& pdu) const {
  pdu = bstrcpy(pdu_bstring);
  return true;
}

//------------------------------------------------------------------------------
bool NAS_PDU::set(const bstring& pdu) {
  pdu_bstring = bstrcpy(pdu);
  return true;
}
}  // namespace ngap
