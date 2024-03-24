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

#include "Dynamic5qiDescriptor.hpp"

namespace ngap {

//------------------------------------------------------------------------------
Dynamic5qiDescriptor::Dynamic5qiDescriptor() {
  m_FiveQI                 = std::nullopt;
  m_DelayCritical          = std::nullopt;
  m_AveragingWindow        = std::nullopt;
  m_MaximumDataBurstVolume = std::nullopt;
}

//------------------------------------------------------------------------------
Dynamic5qiDescriptor::~Dynamic5qiDescriptor() {}

//------------------------------------------------------------------------------
void Dynamic5qiDescriptor::set(
    const PriorityLevelQos& priorityLevelQos,
    const PacketDelayBudget& packetDelayBudget,
    const PacketErrorRate& packetErrorRate, const std::optional<FiveQI>& fiveQI,
    const std::optional<DelayCritical>& delayCritical,
    const std::optional<AveragingWindow>& averagingWindow,
    const std::optional<MaximumDataBurstVolume>& maximumDataBurstVolume) {
  m_PriorityLevelQos  = priorityLevelQos;
  m_PacketDelayBudget = packetDelayBudget;
  m_PacketErrorRate   = packetErrorRate;

  m_FiveQI = fiveQI;

  if (delayCritical.has_value()) {
    m_DelayCritical = delayCritical.value();
  }
  if (averagingWindow.has_value()) {
    m_AveragingWindow = averagingWindow.value();
  }
  if (maximumDataBurstVolume.has_value()) {
    m_MaximumDataBurstVolume = maximumDataBurstVolume.value();
  }
}

//------------------------------------------------------------------------------
bool Dynamic5qiDescriptor::get(
    PriorityLevelQos& priorityLevelQos, PacketDelayBudget& packetDelayBudget,
    PacketErrorRate& packetErrorRate, std::optional<FiveQI>& fiveQI,
    std::optional<DelayCritical>& delayCritical,
    std::optional<AveragingWindow>& averagingWindow,
    std::optional<MaximumDataBurstVolume>& maximumDataBurstVolume) const {
  priorityLevelQos  = m_PriorityLevelQos;
  packetDelayBudget = m_PacketDelayBudget;
  packetErrorRate   = m_PacketErrorRate;

  fiveQI                 = m_FiveQI;
  delayCritical          = m_DelayCritical;
  averagingWindow        = m_AveragingWindow;
  maximumDataBurstVolume = m_MaximumDataBurstVolume;

  return true;
}

//------------------------------------------------------------------------------
bool Dynamic5qiDescriptor::encode(
    Ngap_Dynamic5QIDescriptor_t& dynamic5QIDescriptor) const {
  if (!m_PriorityLevelQos.encode(dynamic5QIDescriptor.priorityLevelQos))
    return false;
  if (!m_PacketDelayBudget.encode(dynamic5QIDescriptor.packetDelayBudget))
    return false;
  if (!m_PacketErrorRate.encode(dynamic5QIDescriptor.packetErrorRate))
    return false;

  if (m_FiveQI.has_value()) {
    Ngap_FiveQI_t* fq = (Ngap_FiveQI_t*) calloc(1, sizeof(Ngap_FiveQI_t));
    if (!fq) return false;
    if (!m_FiveQI.value().encode(*fq)) return false;
    dynamic5QIDescriptor.fiveQI = fq;
  }
  if (m_DelayCritical.has_value()) {
    Ngap_DelayCritical_t* dc =
        (Ngap_DelayCritical_t*) calloc(1, sizeof(Ngap_DelayCritical_t));
    if (!dc) return false;
    if (!m_DelayCritical.value().encode(*dc)) return false;
    dynamic5QIDescriptor.delayCritical = dc;
  }
  if (m_AveragingWindow.has_value()) {
    Ngap_AveragingWindow_t* aw =
        (Ngap_AveragingWindow_t*) calloc(1, sizeof(Ngap_AveragingWindow_t));
    if (!aw) return false;
    if (!m_AveragingWindow.value().encode(*aw)) return false;
    dynamic5QIDescriptor.averagingWindow = aw;
  }
  if (m_MaximumDataBurstVolume.has_value()) {
    Ngap_MaximumDataBurstVolume_t* mdbv =
        (Ngap_MaximumDataBurstVolume_t*) calloc(
            1, sizeof(Ngap_MaximumDataBurstVolume_t));
    if (!mdbv) return false;
    if (!m_MaximumDataBurstVolume.value().encode(*mdbv)) return false;
    dynamic5QIDescriptor.maximumDataBurstVolume = mdbv;
  }

  return true;
}

//------------------------------------------------------------------------------
bool Dynamic5qiDescriptor::decode(
    const Ngap_Dynamic5QIDescriptor_t& dynamic5QIDescriptor) {
  if (!m_PriorityLevelQos.decode(dynamic5QIDescriptor.priorityLevelQos))
    return false;
  if (!m_PacketDelayBudget.decode(dynamic5QIDescriptor.packetDelayBudget))
    return false;
  if (!m_PacketErrorRate.decode(dynamic5QIDescriptor.packetErrorRate))
    return false;

  if (dynamic5QIDescriptor.fiveQI) {
    FiveQI tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.fiveQI)) return false;
    m_FiveQI = std::make_optional<FiveQI>(tmp);
  }
  if (dynamic5QIDescriptor.delayCritical) {
    DelayCritical tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.delayCritical)) return false;
    m_DelayCritical = std::make_optional<DelayCritical>(tmp);
  }
  if (dynamic5QIDescriptor.averagingWindow) {
    AveragingWindow tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.averagingWindow)) return false;
    m_AveragingWindow = std::make_optional<AveragingWindow>(tmp);
  }
  if (dynamic5QIDescriptor.maximumDataBurstVolume) {
    MaximumDataBurstVolume tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.maximumDataBurstVolume)) return false;
    m_MaximumDataBurstVolume = std::make_optional<MaximumDataBurstVolume>(tmp);
  }

  return true;
}
}  // namespace ngap
