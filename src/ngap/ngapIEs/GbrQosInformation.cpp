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

#include "GbrQosInformation.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GbrQosInformation::GbrQosInformation() {
  m_MaximumFlowBitRateDl    = 0;
  m_MaximumFlowBitRateUl    = 0;
  m_GuaranteedFlowBitRateDl = 0;
  m_GuaranteedFlowBitRateUl = 0;
  m_NotificationControl     = std::nullopt;
  m_MaximumPacketLossRateDl = std::nullopt;
  m_MaximumPacketLossRateUl = std::nullopt;
}

//------------------------------------------------------------------------------
GbrQosInformation::~GbrQosInformation() {}

//------------------------------------------------------------------------------
void GbrQosInformation::set(
    const long& maximumFlowBitRateDl, const long& maximumFlowBitRateUl,
    const long& guaranteedFlowBitRateDl, const long& guaranteedFlowBitRateUl,
    const std::optional<NotificationControl>& notificationControl,
    const std::optional<PacketLossRate>& maximumPacketLossRateDl,
    const std::optional<PacketLossRate>& maximumPacketLossRateUl) {
  m_MaximumFlowBitRateDl    = maximumFlowBitRateDl;
  m_MaximumFlowBitRateUl    = maximumFlowBitRateUl;
  m_GuaranteedFlowBitRateDl = guaranteedFlowBitRateDl;
  m_GuaranteedFlowBitRateUl = guaranteedFlowBitRateUl;

  m_NotificationControl     = notificationControl;
  m_MaximumPacketLossRateDl = maximumPacketLossRateDl;
  m_MaximumPacketLossRateUl = maximumPacketLossRateUl;
}

//------------------------------------------------------------------------------
bool GbrQosInformation::get(
    long& maximumFlowBitRateDl, long& maximumFlowBitRateUl,
    long& guaranteedFlowBitRateDl, long& guaranteedFlowBitRateUl,
    std::optional<NotificationControl>& notificationControl,
    std::optional<PacketLossRate>& maximumPacketLossRateDl,
    std::optional<PacketLossRate>& maximumPacketLossRateUl) {
  maximumFlowBitRateDl    = m_MaximumFlowBitRateDl;
  maximumFlowBitRateUl    = m_MaximumFlowBitRateUl;
  guaranteedFlowBitRateDl = m_GuaranteedFlowBitRateDl;
  guaranteedFlowBitRateUl = m_GuaranteedFlowBitRateUl;

  notificationControl     = m_NotificationControl;
  maximumPacketLossRateDl = m_MaximumPacketLossRateDl;
  maximumPacketLossRateUl = m_MaximumPacketLossRateUl;

  return true;
}

//------------------------------------------------------------------------------
bool GbrQosInformation::encode(
    Ngap_GBR_QosInformation_t& gbrQosInformation) const {
  gbrQosInformation.maximumFlowBitRateDL.size = 6;
  gbrQosInformation.maximumFlowBitRateDL.buf =
      (uint8_t*) calloc(1, gbrQosInformation.maximumFlowBitRateDL.size);
  if (!gbrQosInformation.maximumFlowBitRateDL.buf) return false;

  for (int i = 0; i < gbrQosInformation.maximumFlowBitRateDL.size; i++) {
    gbrQosInformation.maximumFlowBitRateDL.buf[i] =
        (m_MaximumFlowBitRateDl & (0xff0000000000 >> i * 8)) >>
        ((gbrQosInformation.maximumFlowBitRateDL.size - i - 1) * 8);
  }

  gbrQosInformation.maximumFlowBitRateUL.size = 6;
  gbrQosInformation.maximumFlowBitRateUL.buf =
      (uint8_t*) calloc(1, gbrQosInformation.maximumFlowBitRateUL.size);
  if (!gbrQosInformation.maximumFlowBitRateUL.buf) return false;

  for (int i = 0; i < gbrQosInformation.maximumFlowBitRateUL.size; i++) {
    gbrQosInformation.maximumFlowBitRateUL.buf[i] =
        (m_MaximumFlowBitRateUl & (0xff0000000000 >> i * 8)) >>
        ((gbrQosInformation.maximumFlowBitRateUL.size - i - 1) * 8);
  }

  gbrQosInformation.guaranteedFlowBitRateDL.size = 6;
  gbrQosInformation.guaranteedFlowBitRateDL.buf =
      (uint8_t*) calloc(1, gbrQosInformation.guaranteedFlowBitRateDL.size);
  if (!gbrQosInformation.guaranteedFlowBitRateDL.buf) return false;

  for (int i = 0; i < gbrQosInformation.guaranteedFlowBitRateDL.size; i++) {
    gbrQosInformation.guaranteedFlowBitRateDL.buf[i] =
        (m_GuaranteedFlowBitRateDl & (0xff0000000000 >> i * 8)) >>
        ((gbrQosInformation.guaranteedFlowBitRateDL.size - i - 1) * 8);
  }

  gbrQosInformation.guaranteedFlowBitRateUL.size = 6;
  gbrQosInformation.guaranteedFlowBitRateUL.buf =
      (uint8_t*) calloc(1, gbrQosInformation.guaranteedFlowBitRateUL.size);
  if (!gbrQosInformation.guaranteedFlowBitRateUL.buf) return false;

  for (int i = 0; i < gbrQosInformation.guaranteedFlowBitRateUL.size; i++) {
    gbrQosInformation.guaranteedFlowBitRateUL.buf[i] =
        (m_GuaranteedFlowBitRateUl & (0xff0000000000 >> i * 8)) >>
        ((gbrQosInformation.guaranteedFlowBitRateUL.size - i - 1) * 8);
  }

  if (m_NotificationControl.has_value()) {
    Ngap_NotificationControl_t* nc = (Ngap_NotificationControl_t*) calloc(
        1, sizeof(Ngap_NotificationControl_t));
    if (!nc) return false;
    if (!m_NotificationControl.value().encode(*nc)) return false;
    gbrQosInformation.notificationControl = nc;
  }
  if (m_MaximumPacketLossRateDl.has_value()) {
    Ngap_PacketLossRate_t* mplrd =
        (Ngap_PacketLossRate_t*) calloc(1, sizeof(Ngap_PacketLossRate_t));
    if (!mplrd) return false;
    if (!m_MaximumPacketLossRateDl.value().encode(*mplrd)) return false;
    gbrQosInformation.maximumPacketLossRateDL = mplrd;
  }
  if (m_MaximumPacketLossRateUl.has_value()) {
    Ngap_PacketLossRate_t* mplru =
        (Ngap_PacketLossRate_t*) calloc(1, sizeof(Ngap_PacketLossRate_t));
    if (!mplru) return false;
    if (!m_MaximumPacketLossRateUl.value().encode(*mplru)) return false;
    gbrQosInformation.maximumPacketLossRateUL = mplru;
  }

  return true;
}

//------------------------------------------------------------------------------
bool GbrQosInformation::decode(
    const Ngap_GBR_QosInformation_t& gbrQosInformation) {
  if (!gbrQosInformation.maximumFlowBitRateDL.buf) return false;
  if (!gbrQosInformation.maximumFlowBitRateUL.buf) return false;
  if (!gbrQosInformation.guaranteedFlowBitRateDL.buf) return false;
  if (!gbrQosInformation.guaranteedFlowBitRateUL.buf) return false;

  m_MaximumFlowBitRateDl    = 0;
  m_MaximumFlowBitRateUl    = 0;
  m_GuaranteedFlowBitRateDl = 0;
  m_GuaranteedFlowBitRateUl = 0;

  for (int i = 0; i < gbrQosInformation.maximumFlowBitRateDL.size; i++) {
    m_MaximumFlowBitRateDl = m_MaximumFlowBitRateDl << 8;
    m_MaximumFlowBitRateDl |= gbrQosInformation.maximumFlowBitRateDL.buf[i];
  }
  for (int i = 0; i < gbrQosInformation.maximumFlowBitRateUL.size; i++) {
    m_MaximumFlowBitRateUl = m_MaximumFlowBitRateUl << 8;
    m_MaximumFlowBitRateUl |= gbrQosInformation.maximumFlowBitRateUL.buf[i];
  }
  for (int i = 0; i < gbrQosInformation.guaranteedFlowBitRateDL.size; i++) {
    m_GuaranteedFlowBitRateDl = m_GuaranteedFlowBitRateDl << 8;
    m_GuaranteedFlowBitRateDl |=
        gbrQosInformation.guaranteedFlowBitRateDL.buf[i];
  }
  for (int i = 0; i < gbrQosInformation.guaranteedFlowBitRateUL.size; i++) {
    m_GuaranteedFlowBitRateUl = m_GuaranteedFlowBitRateUl << 8;
    m_GuaranteedFlowBitRateUl |=
        gbrQosInformation.guaranteedFlowBitRateUL.buf[i];
  }

  if (gbrQosInformation.notificationControl) {
    NotificationControl tmp = {};
    if (!tmp.decode(*gbrQosInformation.notificationControl)) return false;
    m_NotificationControl = std::make_optional<NotificationControl>(tmp);
  }
  if (gbrQosInformation.maximumPacketLossRateDL) {
    PacketLossRate tmp = {};
    if (!tmp.decode(*gbrQosInformation.maximumPacketLossRateDL)) return false;
    m_MaximumPacketLossRateDl = std::make_optional<PacketLossRate>(tmp);
  }
  if (gbrQosInformation.maximumPacketLossRateUL) {
    PacketLossRate tmp = {};
    if (!tmp.decode(*gbrQosInformation.maximumPacketLossRateUL)) return false;
    m_MaximumPacketLossRateUl = std::make_optional<PacketLossRate>(tmp);
  }

  return true;
}
}  // namespace ngap
