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

#include <optional>

#include "NotificationControl.hpp"
#include "PacketLossRate.hpp"

extern "C" {
#include "Ngap_GBR-QosInformation.h"
}

namespace ngap {

class GbrQosInformation {
 public:
  GbrQosInformation();
  virtual ~GbrQosInformation();

  void set(
      const long& maximumFlowBitRateDl, const long& maximumFlowBitRateUl,
      const long& guaranteedFlowBitRateDl, const long& guaranteedFlowBitRateUl,
      const std::optional<NotificationControl>& notificationControl,
      const std::optional<PacketLossRate>& maximumPacketLossRateDl,
      const std::optional<PacketLossRate>& maximumPacketLossRateUl);
  bool get(
      long& maximumFlowBitRateDl, long& maximumFlowBitRateUl,
      long& guaranteedFlowBitRateDl, long& guaranteedFlowBitRateUl,
      std::optional<NotificationControl>& notificationControl,
      std::optional<PacketLossRate>& maximumPacketLossRateDl,
      std::optional<PacketLossRate>& maximumPacketLossRateUl);

  bool encode(Ngap_GBR_QosInformation_t&) const;
  bool decode(const Ngap_GBR_QosInformation_t&);

 private:
  long m_MaximumFlowBitRateDl;
  long m_MaximumFlowBitRateUl;
  long m_GuaranteedFlowBitRateDl;
  long m_GuaranteedFlowBitRateUl;
  std::optional<NotificationControl> m_NotificationControl;  // Optional
  std::optional<PacketLossRate> m_MaximumPacketLossRateDl;   // Optional
  std::optional<PacketLossRate> m_MaximumPacketLossRateUl;   // Optional
  // TODO: Alternative QoS Parameters Set List (Optional, Rel 16.14.0)
};
}  // namespace ngap

#endif
