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

#include "NrCellIdentity.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NrCellIdentity::NrCellIdentity() {
  m_NrCellIdentity = 0;
}

//------------------------------------------------------------------------------
NrCellIdentity::~NrCellIdentity() {}

//------------------------------------------------------------------------------
void NrCellIdentity::set(const unsigned long& nrCellIdentity) {
  m_NrCellIdentity = nrCellIdentity;
}

//------------------------------------------------------------------------------
bool NrCellIdentity::encode(Ngap_NRCellIdentity_t& nrCellIdentity) const {
  nrCellIdentity.bits_unused = 4;
  nrCellIdentity.size        = 5;
  nrCellIdentity.buf = (uint8_t*) calloc(1, sizeof(uint32_t) + sizeof(uint8_t));
  if (!nrCellIdentity.buf) return false;
  nrCellIdentity.buf[4] = m_NrCellIdentity & 0x00000000ff;
  nrCellIdentity.buf[3] = (m_NrCellIdentity & 0x000000ff00) >> 8;
  nrCellIdentity.buf[2] = (m_NrCellIdentity & 0x0000ff0000) >> 16;
  nrCellIdentity.buf[1] = (m_NrCellIdentity & 0x00ff000000) >> 24;
  nrCellIdentity.buf[0] = (m_NrCellIdentity & 0xff00000000) >> 32;

  return true;
}

//------------------------------------------------------------------------------
bool NrCellIdentity::decode(const Ngap_NRCellIdentity_t& nrCellIdentity) {
  if (!nrCellIdentity.buf) return false;

  m_NrCellIdentity = nrCellIdentity.buf[0];
  m_NrCellIdentity = m_NrCellIdentity << 32;
  m_NrCellIdentity |= nrCellIdentity.buf[1] << 24;
  m_NrCellIdentity |= nrCellIdentity.buf[2] << 16;
  m_NrCellIdentity |= nrCellIdentity.buf[3] << 8;
  m_NrCellIdentity |= nrCellIdentity.buf[4];

  return true;
}

//------------------------------------------------------------------------------
unsigned long NrCellIdentity::get() const {
  return m_NrCellIdentity;
}
}  // namespace ngap
