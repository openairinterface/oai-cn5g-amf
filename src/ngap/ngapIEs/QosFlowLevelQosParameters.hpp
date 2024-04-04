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

#ifndef _QOS_FLOW_LEVEL_QOS_PARAMETERS_H_
#define _QOS_FLOW_LEVEL_QOS_PARAMETERS_H_

#include "AdditionalQosFlowInformation.hpp"
#include "AllocationAndRetentionPriority.hpp"
#include "GbrQosFlowInformation.hpp"
#include "QosCharacteristics.hpp"
#include "ReflectiveQosAttribute.hpp"

extern "C" {
#include "Ngap_QosFlowLevelQosParameters.h"
}

namespace ngap {

class QosFlowLevelQosParameters {
 public:
  QosFlowLevelQosParameters();
  virtual ~QosFlowLevelQosParameters();

  void set(
      const QosCharacteristics& qosCharacteristics,
      const AllocationAndRetentionPriority& allocationAndRetentionPriority,
      const std::optional<GbrQosFlowInformation>& gbrQosFlowInformation,
      const std::optional<ReflectiveQosAttribute>& reflectiveQosAttribute,
      const std::optional<AdditionalQosFlowInformation>&
          additionalQosFlowInformation);

  void get(
      QosCharacteristics& qosCharacteristics,
      AllocationAndRetentionPriority& allocationAndRetentionPriority,
      std::optional<GbrQosFlowInformation>& gbrQosFlowInformation,
      std::optional<ReflectiveQosAttribute>& reflectiveQosAttribute,
      std::optional<AdditionalQosFlowInformation>& additionalQosFlowInformation)
      const;

  bool encode(Ngap_QosFlowLevelQosParameters_t&) const;
  bool decode(const Ngap_QosFlowLevelQosParameters_t&);

 private:
  QosCharacteristics m_QosCharacteristics;                          // Mandatory
  AllocationAndRetentionPriority m_AllocationAndRetentionPriority;  // Mandatory
  std::optional<GbrQosFlowInformation> m_GbrQosInformation;         // Optional
  std::optional<ReflectiveQosAttribute> m_ReflectiveQosAttribute;   // Optional
  std::optional<AdditionalQosFlowInformation>
      m_AdditionalQosFlowInformation;  // Optional
};

}  // namespace ngap

#endif
