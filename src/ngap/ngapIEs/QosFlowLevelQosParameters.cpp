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

#include "QosFlowLevelQosParameters.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowLevelQosParameters::QosFlowLevelQosParameters() {
  gBR_QosInformation           = std::nullopt;
  reflectiveQosAttribute       = std::nullopt;
  additionalQosFlowInformation = std::nullopt;
}

//------------------------------------------------------------------------------
QosFlowLevelQosParameters::~QosFlowLevelQosParameters() {}

//------------------------------------------------------------------------------
void QosFlowLevelQosParameters::set(
    const QosCharacteristics& m_qosCharacteristics,
    const AllocationAndRetentionPriority& m_allocationAndRetentionPriority,
    const std::optional<GbrQoSFlowInformation>& m_gBR_QosInformation,
    const std::optional<ReflectiveQosAttribute>& m_reflectiveQosAttribute,
    const std::optional<AdditionalQosFlowInformation>&
        m_additionalQosFlowInformation) {
  qosCharacteristics             = m_qosCharacteristics;
  allocationAndRetentionPriority = m_allocationAndRetentionPriority;
  gBR_QosInformation             = m_gBR_QosInformation;
  reflectiveQosAttribute         = m_reflectiveQosAttribute;
  additionalQosFlowInformation   = m_additionalQosFlowInformation;
}

//------------------------------------------------------------------------------
void QosFlowLevelQosParameters::get(
    QosCharacteristics& m_qosCharacteristics,
    AllocationAndRetentionPriority& m_allocationAndRetentionPriority,
    std::optional<GbrQoSFlowInformation>& m_gBR_QosInformation,
    std::optional<ReflectiveQosAttribute>& m_reflectiveQosAttribute,
    std::optional<AdditionalQosFlowInformation>& m_additionalQosFlowInformation)
    const {
  m_qosCharacteristics             = qosCharacteristics;
  m_allocationAndRetentionPriority = allocationAndRetentionPriority;
  m_gBR_QosInformation             = gBR_QosInformation;
  m_reflectiveQosAttribute         = reflectiveQosAttribute;
  m_additionalQosFlowInformation   = additionalQosFlowInformation;
}

//------------------------------------------------------------------------------
bool QosFlowLevelQosParameters::encode(
    Ngap_QosFlowLevelQosParameters_t& qosFlowLevelQosParameters) {
  if (!qosCharacteristics.encode(qosFlowLevelQosParameters.qosCharacteristics))
    return false;
  if (!allocationAndRetentionPriority.encode(
          qosFlowLevelQosParameters.allocationAndRetentionPriority))
    return false;

  if (gBR_QosInformation.has_value()) {
    qosFlowLevelQosParameters.gBR_QosInformation =
        (Ngap_GBR_QosInformation_t*) calloc(
            1, sizeof(Ngap_GBR_QosInformation_t));
    if (!qosFlowLevelQosParameters.gBR_QosInformation) return false;
    if (!gBR_QosInformation.value().encode(
            *qosFlowLevelQosParameters.gBR_QosInformation))
      return false;
  }

  if (reflectiveQosAttribute.has_value()) {
    qosFlowLevelQosParameters.reflectiveQosAttribute =
        (Ngap_ReflectiveQosAttribute_t*) calloc(
            1, sizeof(Ngap_ReflectiveQosAttribute_t));
    if (!qosFlowLevelQosParameters.reflectiveQosAttribute) return false;
    if (!reflectiveQosAttribute.value().encode(
            *qosFlowLevelQosParameters.reflectiveQosAttribute))
      return false;
  }

  if (additionalQosFlowInformation.has_value()) {
    qosFlowLevelQosParameters.additionalQosFlowInformation =
        (Ngap_AdditionalQosFlowInformation_t*) calloc(
            1, sizeof(Ngap_AdditionalQosFlowInformation_t));
    if (!qosFlowLevelQosParameters.additionalQosFlowInformation) return false;
    if (!additionalQosFlowInformation.value().encode(
            *qosFlowLevelQosParameters.additionalQosFlowInformation))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowLevelQosParameters::decode(
    const Ngap_QosFlowLevelQosParameters_t& qosFlowLevelQosParameters) {
  if (!qosCharacteristics.decode(qosFlowLevelQosParameters.qosCharacteristics))
    return false;
  if (!allocationAndRetentionPriority.decode(
          qosFlowLevelQosParameters.allocationAndRetentionPriority))
    return false;

  if (qosFlowLevelQosParameters.gBR_QosInformation) {
    GbrQoSFlowInformation tmp = {};
    if (!tmp.decode(*qosFlowLevelQosParameters.gBR_QosInformation))
      return false;
    gBR_QosInformation = std::make_optional<GbrQoSFlowInformation>(tmp);
  }
  if (qosFlowLevelQosParameters.reflectiveQosAttribute) {
    ReflectiveQosAttribute tmp = {};
    if (!tmp.decode(*qosFlowLevelQosParameters.reflectiveQosAttribute))
      return false;
    reflectiveQosAttribute = std::make_optional<ReflectiveQosAttribute>(tmp);
  }
  if (qosFlowLevelQosParameters.additionalQosFlowInformation) {
    AdditionalQosFlowInformation tmp = {};
    if (!tmp.decode(*qosFlowLevelQosParameters.additionalQosFlowInformation))
      return false;
    additionalQosFlowInformation =
        std::make_optional<AdditionalQosFlowInformation>(tmp);
  }

  return true;
}
}  // namespace ngap
