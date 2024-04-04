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

#include "NasPdu.hpp"

#include "amf_conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NasPdu::NasPdu() {}

//------------------------------------------------------------------------------
NasPdu::~NasPdu() {}

//------------------------------------------------------------------------------
bool NasPdu::encode(Ngap_NAS_PDU_t& nasPdu) const {
  return amf_conv::bstring_2_octet_string(m_Pdu, nasPdu);
}

//------------------------------------------------------------------------------
bool NasPdu::decode(const Ngap_NAS_PDU_t& nasPdu) {
  if (!nasPdu.buf) return false;
  return amf_conv::octet_string_2_bstring(nasPdu, m_Pdu);
}
/*

//------------------------------------------------------------------------------
bool NasPdu::get(uint8_t*& buffer, size_t& size) const {
  if (!pdu_.buf) return false;
  if (pdu_.size < 0) return false;
  memcpy(buffer, pdu_.buf, pdu_.size);
  size = pdu_.size;
  return true;
}
*/
//------------------------------------------------------------------------------
void NasPdu::set(uint8_t* buffer, size_t size) {
  if (!buffer) return;
  m_Pdu = blk2bstr(buffer, size);
  return;
}

//------------------------------------------------------------------------------
bool NasPdu::get(OCTET_STRING_t& pdu) const {
  amf_conv::bstring_2_octet_string(m_Pdu, pdu);
  return true;
}

//------------------------------------------------------------------------------
bool NasPdu::set(const OCTET_STRING_t& pdu) {
  amf_conv::octet_string_2_bstring(pdu, m_Pdu);
  return true;
}

//------------------------------------------------------------------------------
bool NasPdu::get(NasPdu& nasPdu) const {
  return nasPdu.set(m_Pdu);
}

//------------------------------------------------------------------------------
bool NasPdu::set(const NasPdu& nasPdu) {
  bstring pdu = {};
  if (!nasPdu.get(pdu)) return false;
  return set(pdu);
}

//------------------------------------------------------------------------------
bool NasPdu::get(bstring& pdu) const {
  pdu = bstrcpy(m_Pdu);
  return true;
}

//------------------------------------------------------------------------------
bool NasPdu::set(const bstring& pdu) {
  m_Pdu = bstrcpy(pdu);
  return true;
}
}  // namespace ngap
