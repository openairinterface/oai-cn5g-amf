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

#include "EUtraCellIdentity.hpp"

namespace ngap {

//------------------------------------------------------------------------------
EUtraCellIdentity::EUtraCellIdentity() {
  id_ = 0;
}

//------------------------------------------------------------------------------
EUtraCellIdentity::~EUtraCellIdentity() {}

//------------------------------------------------------------------------------
bool EUtraCellIdentity::set(const uint32_t& id) {
  if (id > kEUTRACellIdentityMaxValue) return false;
  id_ = id;
  return true;
}

//------------------------------------------------------------------------------
uint32_t EUtraCellIdentity::get() const {
  return id_;
}

//------------------------------------------------------------------------------
bool EUtraCellIdentity::encode(Ngap_EUTRACellIdentity_t& eUTRA_cell_identity) {
  eUTRA_cell_identity.bits_unused = 4;  // 28 = 4*8 - 4 bits
  eUTRA_cell_identity.size        = 4;
  eUTRA_cell_identity.buf         = (uint8_t*) calloc(1, sizeof(uint32_t));
  if (!eUTRA_cell_identity.buf) return false;
  eUTRA_cell_identity.buf[3] = id_ & 0x000000ff;
  eUTRA_cell_identity.buf[2] = (id_ & 0x0000ff00) >> 8;
  eUTRA_cell_identity.buf[1] = (id_ & 0x00ff0000) >> 16;
  eUTRA_cell_identity.buf[0] = (id_ & 0xff000000) >> 24;

  return true;
}

//------------------------------------------------------------------------------
bool EUtraCellIdentity::decode(
    const Ngap_EUTRACellIdentity_t& eUTRA_cell_identity) {
  if (!eUTRA_cell_identity.buf) return false;

  id_ = eUTRA_cell_identity.buf[0] << 24;
  id_ |= eUTRA_cell_identity.buf[1] << 16;
  id_ |= eUTRA_cell_identity.buf[2] << 8;
  id_ |= eUTRA_cell_identity.buf[3];

  return true;
}
}  // namespace ngap
