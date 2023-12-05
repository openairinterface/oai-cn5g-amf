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

#include "NonDynamic5QIDescriptor.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NonDynamic5QIDescriptor::NonDynamic5QIDescriptor() {
  priorityLevelQos       = std::nullopt;
  averagingWindow        = std::nullopt;
  maximumDataBurstVolume = std::nullopt;
}

//------------------------------------------------------------------------------
NonDynamic5QIDescriptor::NonDynamic5QIDescriptor(
    const FiveQI& m_fiveQI,
    const std::optional<PriorityLevelQos>& m_priorityLevelQos,
    const std::optional<AveragingWindow>& m_averagingWindow,
    const std::optional<MaximumDataBurstVolume>& m_maximumDataBurstVolume) {
  fiveQI                 = m_fiveQI;
  priorityLevelQos       = m_priorityLevelQos;
  averagingWindow        = m_averagingWindow;
  maximumDataBurstVolume = m_maximumDataBurstVolume;
}

//------------------------------------------------------------------------------
NonDynamic5QIDescriptor::~NonDynamic5QIDescriptor() {}

//------------------------------------------------------------------------------
void NonDynamic5QIDescriptor::set(
    const FiveQI& m_fiveQI,
    const std::optional<PriorityLevelQos>& m_priorityLevelQos,
    const std::optional<AveragingWindow>& m_averagingWindow,
    const std::optional<MaximumDataBurstVolume>& m_maximumDataBurstVolume) {
  fiveQI                 = m_fiveQI;
  priorityLevelQos       = m_priorityLevelQos;
  averagingWindow        = m_averagingWindow;
  maximumDataBurstVolume = m_maximumDataBurstVolume;
}

//------------------------------------------------------------------------------
void NonDynamic5QIDescriptor::get(
    FiveQI& m_fiveQI, std::optional<PriorityLevelQos>& m_priorityLevelQos,
    std::optional<AveragingWindow>& m_averagingWindow,
    std::optional<MaximumDataBurstVolume>& m_maximumDataBurstVolume) const {
  m_fiveQI                 = fiveQI;
  m_priorityLevelQos       = priorityLevelQos;
  m_averagingWindow        = averagingWindow;
  m_maximumDataBurstVolume = maximumDataBurstVolume;
}

//------------------------------------------------------------------------------
bool NonDynamic5QIDescriptor::encode(
    Ngap_NonDynamic5QIDescriptor_t& nonDynamic5QIDescriptor) {
  if (!fiveQI.encode(nonDynamic5QIDescriptor.fiveQI)) return false;
  if (priorityLevelQos.has_value()) {
    Ngap_PriorityLevelQos_t* plq =
        (Ngap_PriorityLevelQos_t*) calloc(1, sizeof(Ngap_PriorityLevelQos_t));
    if (!plq) return false;
    if (!priorityLevelQos.value().encode(*plq)) return false;
    nonDynamic5QIDescriptor.priorityLevelQos = plq;
  }
  if (averagingWindow.has_value()) {
    Ngap_AveragingWindow_t* aw =
        (Ngap_AveragingWindow_t*) calloc(1, sizeof(Ngap_AveragingWindow_t));
    if (!aw) return false;
    if (!averagingWindow.value().encode(*aw)) return false;
    nonDynamic5QIDescriptor.averagingWindow = aw;
  }
  if (maximumDataBurstVolume.has_value()) {
    Ngap_MaximumDataBurstVolume_t* mdbv =
        (Ngap_MaximumDataBurstVolume_t*) calloc(
            1, sizeof(Ngap_MaximumDataBurstVolume_t));
    if (!mdbv) return false;
    if (!maximumDataBurstVolume.value().encode(*mdbv)) return false;
    nonDynamic5QIDescriptor.maximumDataBurstVolume = mdbv;
  }

  return true;
}

//------------------------------------------------------------------------------
bool NonDynamic5QIDescriptor::decode(
    const Ngap_NonDynamic5QIDescriptor_t& nonDynamic5QIDescriptor) {
  if (!fiveQI.decode(nonDynamic5QIDescriptor.fiveQI)) return false;
  if (nonDynamic5QIDescriptor.priorityLevelQos) {
    PriorityLevelQos tmp = {};
    if (!tmp.decode(*nonDynamic5QIDescriptor.priorityLevelQos)) return false;
    priorityLevelQos = std::make_optional<PriorityLevelQos>(tmp);
  }
  if (nonDynamic5QIDescriptor.averagingWindow) {
    AveragingWindow tmp = {};
    if (!tmp.decode(*nonDynamic5QIDescriptor.averagingWindow)) return false;
    averagingWindow = std::make_optional<AveragingWindow>(tmp);
  }
  if (nonDynamic5QIDescriptor.maximumDataBurstVolume) {
    MaximumDataBurstVolume tmp = {};
    if (!tmp.decode(*nonDynamic5QIDescriptor.maximumDataBurstVolume))
      return false;
    maximumDataBurstVolume = std::make_optional<MaximumDataBurstVolume>(tmp);
  }

  return true;
}
}  // namespace ngap
