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

#include "Dynamic5QIDescriptor.hpp"

namespace ngap {

//------------------------------------------------------------------------------
Dynamic5QIDescriptor::Dynamic5QIDescriptor() {
  fiveQI                 = std::nullopt;
  delayCritical          = std::nullopt;
  averagingWindow        = std::nullopt;
  maximumDataBurstVolume = std::nullopt;
}

//------------------------------------------------------------------------------
Dynamic5QIDescriptor::~Dynamic5QIDescriptor() {}

//------------------------------------------------------------------------------
void Dynamic5QIDescriptor::set(
    const PriorityLevelQos& m_priorityLevelQos,
    const PacketDelayBudget& m_packetDelayBudget,
    const PacketErrorRate& m_packetErrorRate,
    const std::optional<FiveQI>& m_fiveQI,
    const std::optional<DelayCritical>& m_delayCritical,
    const std::optional<AveragingWindow>& m_averagingWindow,
    const std::optional<MaximumDataBurstVolume>& m_maximumDataBurstVolume) {
  priorityLevelQos  = m_priorityLevelQos;
  packetDelayBudget = m_packetDelayBudget;
  packetErrorRate   = m_packetErrorRate;

  fiveQI = m_fiveQI;

  if (m_delayCritical.has_value()) {
    delayCritical = m_delayCritical.value();
  }
  if (m_averagingWindow.has_value()) {
    averagingWindow = m_averagingWindow.value();
  }
  if (m_maximumDataBurstVolume.has_value()) {
    maximumDataBurstVolume = m_maximumDataBurstVolume.value();
  }
}

//------------------------------------------------------------------------------
bool Dynamic5QIDescriptor::get(
    PriorityLevelQos& m_priorityLevelQos,
    PacketDelayBudget& m_packetDelayBudget, PacketErrorRate& m_packetErrorRate,
    std::optional<FiveQI>& m_fiveQI,
    std::optional<DelayCritical>& m_delayCritical,
    std::optional<AveragingWindow>& m_averagingWindow,
    std::optional<MaximumDataBurstVolume>& m_maximumDataBurstVolume) const {
  m_priorityLevelQos  = priorityLevelQos;
  m_packetDelayBudget = packetDelayBudget;
  m_packetErrorRate   = packetErrorRate;

  m_fiveQI                 = fiveQI;
  m_delayCritical          = delayCritical;
  m_averagingWindow        = averagingWindow;
  m_maximumDataBurstVolume = maximumDataBurstVolume;

  return true;
}

//------------------------------------------------------------------------------
bool Dynamic5QIDescriptor::encode(
    Ngap_Dynamic5QIDescriptor_t& dynamic5QIDescriptor) {
  if (!priorityLevelQos.encode(dynamic5QIDescriptor.priorityLevelQos))
    return false;
  if (!packetDelayBudget.encode(dynamic5QIDescriptor.packetDelayBudget))
    return false;
  if (!packetErrorRate.encode(dynamic5QIDescriptor.packetErrorRate))
    return false;

  if (fiveQI.has_value()) {
    Ngap_FiveQI_t* fq = (Ngap_FiveQI_t*) calloc(1, sizeof(Ngap_FiveQI_t));
    if (!fq) return false;
    if (!fiveQI.value().encode(*fq)) return false;
    dynamic5QIDescriptor.fiveQI = fq;
  }
  if (delayCritical.has_value()) {
    Ngap_DelayCritical_t* dc =
        (Ngap_DelayCritical_t*) calloc(1, sizeof(Ngap_DelayCritical_t));
    if (!dc) return false;
    if (!delayCritical.value().encode(*dc)) return false;
    dynamic5QIDescriptor.delayCritical = dc;
  }
  if (averagingWindow.has_value()) {
    Ngap_AveragingWindow_t* aw =
        (Ngap_AveragingWindow_t*) calloc(1, sizeof(Ngap_AveragingWindow_t));
    if (!aw) return false;
    if (!averagingWindow.value().encode(*aw)) return false;
    dynamic5QIDescriptor.averagingWindow = aw;
  }
  if (maximumDataBurstVolume.has_value()) {
    Ngap_MaximumDataBurstVolume_t* mdbv =
        (Ngap_MaximumDataBurstVolume_t*) calloc(
            1, sizeof(Ngap_MaximumDataBurstVolume_t));
    if (!mdbv) return false;
    if (!maximumDataBurstVolume.value().encode(*mdbv)) return false;
    dynamic5QIDescriptor.maximumDataBurstVolume = mdbv;
  }

  return true;
}

//------------------------------------------------------------------------------
bool Dynamic5QIDescriptor::decode(
    const Ngap_Dynamic5QIDescriptor_t& dynamic5QIDescriptor) {
  if (!priorityLevelQos.decode(dynamic5QIDescriptor.priorityLevelQos))
    return false;
  if (!packetDelayBudget.decode(dynamic5QIDescriptor.packetDelayBudget))
    return false;
  if (!packetErrorRate.decode(dynamic5QIDescriptor.packetErrorRate))
    return false;

  if (dynamic5QIDescriptor.fiveQI) {
    FiveQI tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.fiveQI)) return false;
    fiveQI = std::make_optional<FiveQI>(tmp);
  }
  if (dynamic5QIDescriptor.delayCritical) {
    DelayCritical tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.delayCritical)) return false;
    delayCritical = std::make_optional<DelayCritical>(tmp);
  }
  if (dynamic5QIDescriptor.averagingWindow) {
    AveragingWindow tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.averagingWindow)) return false;
    averagingWindow = std::make_optional<AveragingWindow>(tmp);
  }
  if (dynamic5QIDescriptor.maximumDataBurstVolume) {
    MaximumDataBurstVolume tmp = {};
    if (!tmp.decode(*dynamic5QIDescriptor.maximumDataBurstVolume)) return false;
    maximumDataBurstVolume = std::make_optional<MaximumDataBurstVolume>(tmp);
  }

  return true;
}
}  // namespace ngap
