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

#include "EutraCellIdentity.hpp"

namespace ngap {

//------------------------------------------------------------------------------
EutraCellIdentity::EutraCellIdentity() {
  m_EutraCellIdentity = 0;
}

//------------------------------------------------------------------------------
EutraCellIdentity::~EutraCellIdentity() {}

//------------------------------------------------------------------------------
bool EutraCellIdentity::set(const uint32_t& id) {
  if (id > kEUTRACellIdentityMaxValue) return false;
  m_EutraCellIdentity = id;
  return true;
}

//------------------------------------------------------------------------------
uint32_t EutraCellIdentity::get() const {
  return m_EutraCellIdentity;
}

//------------------------------------------------------------------------------
bool EutraCellIdentity::encode(
    Ngap_EUTRACellIdentity_t& eutraCellIdentity) const {
  eutraCellIdentity.bits_unused = 4;  // 28 = 4*8 - 4 bits
  eutraCellIdentity.size        = 4;
  eutraCellIdentity.buf         = (uint8_t*) calloc(1, sizeof(uint32_t));
  if (!eutraCellIdentity.buf) return false;
  eutraCellIdentity.buf[3] = m_EutraCellIdentity & 0x000000ff;
  eutraCellIdentity.buf[2] = (m_EutraCellIdentity & 0x0000ff00) >> 8;
  eutraCellIdentity.buf[1] = (m_EutraCellIdentity & 0x00ff0000) >> 16;
  eutraCellIdentity.buf[0] = (m_EutraCellIdentity & 0xff000000) >> 24;

  return true;
}

//------------------------------------------------------------------------------
bool EutraCellIdentity::decode(
    const Ngap_EUTRACellIdentity_t& eutraCellIdentity) {
  if (!eutraCellIdentity.buf) return false;

  m_EutraCellIdentity = eutraCellIdentity.buf[0] << 24;
  m_EutraCellIdentity |= eutraCellIdentity.buf[1] << 16;
  m_EutraCellIdentity |= eutraCellIdentity.buf[2] << 8;
  m_EutraCellIdentity |= eutraCellIdentity.buf[3];

  return true;
}
}  // namespace ngap
