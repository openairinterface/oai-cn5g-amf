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

#include "GnbId.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GnbId::GnbId() {
  m_GnbId   = std::nullopt;
  m_Present = Ngap_GNB_ID_PR_NOTHING;
}

//------------------------------------------------------------------------------
GnbId::~GnbId() {}

//------------------------------------------------------------------------------
void GnbId::set(const gNBId_t& gnbId) {
  m_GnbId   = std::optional<gNBId_t>(gnbId);
  m_Present = Ngap_GNB_ID_PR_gNB_ID;
}

//------------------------------------------------------------------------------
bool GnbId::set(const uint32_t& id, const uint8_t& bitLength) {
  if (!((bitLength >= NGAP_GNB_ID_SIZE_MIN) &&
        (bitLength <= NGAP_GNB_ID_SIZE_MAX))) {
    Logger::ngap().warn("gNBID length out of range!");
    return false;
  }

  gNBId_t tmp   = {};
  tmp.id        = id;
  tmp.bitLength = bitLength;

  m_GnbId   = std::optional<gNBId_t>(tmp);
  m_Present = Ngap_GNB_ID_PR_gNB_ID;
  return true;
}

//------------------------------------------------------------------------------
bool GnbId::get(gNBId_t& gnbId) const {
  if (m_GnbId.has_value()) {
    gnbId = m_GnbId.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool GnbId::get(uint32_t& id) const {
  if (m_GnbId.has_value()) {
    id = m_GnbId.value().id;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool GnbId::encode(Ngap_GNB_ID_t& gnbId) const {
  if (!m_GnbId.has_value()) {
    gnbId.present = Ngap_GNB_ID_PR_NOTHING;
    return true;
  }

  gnbId.present                   = Ngap_GNB_ID_PR_gNB_ID;
  gnbId.choice.gNB_ID.size        = 4;  // TODO: to be vefified
  gnbId.choice.gNB_ID.bits_unused = 32 - m_GnbId.value().bitLength;
  gnbId.choice.gNB_ID.buf         = (uint8_t*) calloc(1, 4 * sizeof(uint8_t));
  if (!gnbId.choice.gNB_ID.buf) return false;
  gnbId.choice.gNB_ID.buf[3] = m_GnbId.value().id & 0x000000ff;
  gnbId.choice.gNB_ID.buf[2] = (m_GnbId.value().id & 0x0000ff00) >> 8;
  gnbId.choice.gNB_ID.buf[1] = (m_GnbId.value().id & 0x00ff0000) >> 16;
  gnbId.choice.gNB_ID.buf[0] = (m_GnbId.value().id & 0xff000000) >> 24;

  return true;
}

//------------------------------------------------------------------------------
bool GnbId::decode(const Ngap_GNB_ID_t& gnbId) {
  if (gnbId.present != Ngap_GNB_ID_PR_gNB_ID) return false;
  if (!gnbId.choice.gNB_ID.buf) return false;

  gNBId_t tmp = {};
  tmp.id      = gnbId.choice.gNB_ID.buf[0] << 24;
  tmp.id |= gnbId.choice.gNB_ID.buf[1] << 16;
  tmp.id |= gnbId.choice.gNB_ID.buf[2] << 8;
  tmp.id |= gnbId.choice.gNB_ID.buf[3];
  tmp.bitLength = 32 - gnbId.choice.gNB_ID.bits_unused;

  m_GnbId   = std::optional<gNBId_t>(tmp);
  m_Present = Ngap_GNB_ID_PR_gNB_ID;

  return true;
}

}  // namespace ngap
