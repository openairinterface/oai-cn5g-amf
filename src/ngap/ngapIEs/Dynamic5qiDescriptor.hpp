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

#ifndef _DYNAMIC_5QI_DESCRIPTOR_H_
#define _DYNAMIC_5QI_DESCRIPTOR_H_

#include <optional>

#include "AveragingWindow.hpp"
#include "DelayCritical.hpp"
#include "FiveQI.hpp"
#include "MaximumDataBurstVolume.hpp"
#include "PacketDelayBudget.hpp"
#include "PacketErrorRate.hpp"
#include "PriorityLevelQos.hpp"

extern "C" {
#include "Ngap_Dynamic5QIDescriptor.h"
}

namespace ngap {

class Dynamic5qiDescriptor {
 public:
  Dynamic5qiDescriptor();
  virtual ~Dynamic5qiDescriptor();

  void set(
      const PriorityLevelQos& priorityLevelQos,
      const PacketDelayBudget& packetDelayBudget,
      const PacketErrorRate& packetErrorRate,
      const std::optional<FiveQI>& fiveQI,
      const std::optional<DelayCritical>& delayCritical,
      const std::optional<AveragingWindow>& averagingWindow,
      const std::optional<MaximumDataBurstVolume>& maximumDataBurstVolume);

  bool get(
      PriorityLevelQos& priorityLevelQos, PacketDelayBudget& packetDelayBudget,
      PacketErrorRate& packetErrorRate, std::optional<FiveQI>& fiveQI,
      std::optional<DelayCritical>& delayCritical,
      std::optional<AveragingWindow>& averagingWindow,
      std::optional<MaximumDataBurstVolume>& maximumDataBurstVolume) const;

  bool encode(Ngap_Dynamic5QIDescriptor_t&) const;
  bool decode(const Ngap_Dynamic5QIDescriptor_t&);

 private:
  PriorityLevelQos m_PriorityLevelQos;    // Mandatory
  PacketDelayBudget m_PacketDelayBudget;  // Mandatory
  PacketErrorRate m_PacketErrorRate;      // Mandatory

  std::optional<FiveQI> m_FiveQI;                    // Optional
  std::optional<DelayCritical> m_DelayCritical;      // Conditional
  std::optional<AveragingWindow> m_AveragingWindow;  // Conditional
  std::optional<MaximumDataBurstVolume> m_MaximumDataBurstVolume;  // Optional
};
}  // namespace ngap

#endif
