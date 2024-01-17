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
#include "GbrQoSFlowInformation.hpp"
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
      const QosCharacteristics& m_qosCharacteristics,
      const AllocationAndRetentionPriority& m_allocationAndRetentionPriority,
      const std::optional<GbrQoSFlowInformation>& m_gBR_QosInformation,
      const std::optional<ReflectiveQosAttribute>& m_reflectiveQosAttribute,
      const std::optional<AdditionalQosFlowInformation>&
          m_additionalQosFlowInformation);

  void get(
      QosCharacteristics& m_qosCharacteristics,
      AllocationAndRetentionPriority& m_allocationAndRetentionPriority,
      std::optional<GbrQoSFlowInformation>& m_gBR_QosInformation,
      std::optional<ReflectiveQosAttribute>& m_reflectiveQosAttribute,
      std::optional<AdditionalQosFlowInformation>&
          m_additionalQosFlowInformation) const;

  bool encode(Ngap_QosFlowLevelQosParameters_t&);
  bool decode(const Ngap_QosFlowLevelQosParameters_t&);

 private:
  QosCharacteristics qosCharacteristics;                          // Mandatory
  AllocationAndRetentionPriority allocationAndRetentionPriority;  // Mandatory
  std::optional<GbrQoSFlowInformation> gBR_QosInformation;        // Optional
  std::optional<ReflectiveQosAttribute> reflectiveQosAttribute;   // Optional
  std::optional<AdditionalQosFlowInformation>
      additionalQosFlowInformation;  // Optional
};

}  // namespace ngap

#endif
