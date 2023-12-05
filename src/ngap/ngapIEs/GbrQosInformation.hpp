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

#ifndef _GBR_QOS_INFORMATION_H_
#define _GBR_QOS_INFORMATION_H_

#include "NotificationControl.hpp"
#include "PacketLossRate.hpp"

#include <optional>

extern "C" {
#include "Ngap_GBR-QosInformation.h"
}

namespace ngap {

class GbrQosInformation {
 public:
  GbrQosInformation();
  virtual ~GbrQosInformation();

  void set(
      long m_maximumFlowBitRateDL, long m_maximumFlowBitRateUL,
      long m_guaranteedFlowBitRateDL, long m_guaranteedFlowBitRateUL,
      const std::optional<NotificationControl>& m_notificationControl,
      const std::optional<PacketLossRate>& m_maximumPacketLossRateDL,
      const std::optional<PacketLossRate>& m_maximumPacketLossRateUL);
  bool get(
      long& m_maximumFlowBitRateDL, long& m_maximumFlowBitRateUL,
      long& m_guaranteedFlowBitRateDL, long& m_guaranteedFlowBitRateUL,
      std::optional<NotificationControl>& m_notificationControl,
      std::optional<PacketLossRate>& m_maximumPacketLossRateDL,
      std::optional<PacketLossRate>& m_maximumPacketLossRateUL);

  bool encode(Ngap_GBR_QosInformation_t&);
  bool decode(const Ngap_GBR_QosInformation_t&);

 private:
  long maximumFlowBitRateDL;
  long maximumFlowBitRateUL;
  long guaranteedFlowBitRateDL;
  long guaranteedFlowBitRateUL;
  std::optional<NotificationControl> notificationControl;  // Optional
  std::optional<PacketLossRate> maximumPacketLossRateDL;   // Optional
  std::optional<PacketLossRate> maximumPacketLossRateUL;   // Optional
  // TODO: Alternative QoS Parameters Set List (Optional, Rel 16.14.0)
};
}  // namespace ngap

#endif
