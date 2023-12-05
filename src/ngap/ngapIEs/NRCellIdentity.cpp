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

#include "NRCellIdentity.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NRCellIdentity::NRCellIdentity() {
  nr_cell_identity_ = 0;
}

//------------------------------------------------------------------------------
NRCellIdentity::~NRCellIdentity() {}

//------------------------------------------------------------------------------
void NRCellIdentity::setNRCellIdentity(unsigned long m_nrcellidentity) {
  nr_cell_identity_ = m_nrcellidentity;
}

//------------------------------------------------------------------------------
bool NRCellIdentity::encode(Ngap_NRCellIdentity_t& nRCellIdentity) {
  nRCellIdentity.bits_unused = 4;
  nRCellIdentity.size        = 5;
  nRCellIdentity.buf = (uint8_t*) calloc(1, sizeof(uint32_t) + sizeof(uint8_t));
  if (!nRCellIdentity.buf) return false;
  nRCellIdentity.buf[4] = nr_cell_identity_ & 0x00000000ff;
  nRCellIdentity.buf[3] = (nr_cell_identity_ & 0x000000ff00) >> 8;
  nRCellIdentity.buf[2] = (nr_cell_identity_ & 0x0000ff0000) >> 16;
  nRCellIdentity.buf[1] = (nr_cell_identity_ & 0x00ff000000) >> 24;
  nRCellIdentity.buf[0] = (nr_cell_identity_ & 0xff00000000) >> 32;

  return true;
}

//------------------------------------------------------------------------------
bool NRCellIdentity::decode(Ngap_NRCellIdentity_t nRCellIdentity) {
  if (!nRCellIdentity.buf) return false;

  nr_cell_identity_ = nRCellIdentity.buf[0];
  nr_cell_identity_ = nr_cell_identity_ << 32;
  nr_cell_identity_ |= nRCellIdentity.buf[1] << 24;
  nr_cell_identity_ |= nRCellIdentity.buf[2] << 16;
  nr_cell_identity_ |= nRCellIdentity.buf[3] << 8;
  nr_cell_identity_ |= nRCellIdentity.buf[4];

  return true;
}

//------------------------------------------------------------------------------
unsigned long NRCellIdentity::getNRCellIdentity() const {
  return nr_cell_identity_;
}
}  // namespace ngap
