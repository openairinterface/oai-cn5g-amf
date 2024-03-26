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

#include "QosFlowSetupRequestItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowSetupRequestItem::QosFlowSetupRequestItem() {}

//------------------------------------------------------------------------------
QosFlowSetupRequestItem::~QosFlowSetupRequestItem() {}

//------------------------------------------------------------------------------
void QosFlowSetupRequestItem::set(
    const QosFlowIdentifier& qosFlowIdentifier,
    const QosFlowLevelQosParameters& qosFlowLevelQosParameters) {
  m_QosFlowIdentifier         = qosFlowIdentifier;
  m_QosFlowLevelQosParameters = qosFlowLevelQosParameters;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestItem::get(
    QosFlowIdentifier& qosFlowIdentifier,
    QosFlowLevelQosParameters& qosFlowLevelQosParameters) const {
  qosFlowIdentifier         = m_QosFlowIdentifier;
  qosFlowLevelQosParameters = m_QosFlowLevelQosParameters;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestItem::encode(
    Ngap_QosFlowSetupRequestItem_t& qosFlowSetupRequestItem) const {
  if (!m_QosFlowIdentifier.encode(qosFlowSetupRequestItem.qosFlowIdentifier))
    return false;
  if (!m_QosFlowLevelQosParameters.encode(
          qosFlowSetupRequestItem.qosFlowLevelQosParameters))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestItem::decode(
    const Ngap_QosFlowSetupRequestItem_t& qosFlowSetupRequestItem) {
  if (!m_QosFlowIdentifier.decode(qosFlowSetupRequestItem.qosFlowIdentifier))
    return false;
  if (!m_QosFlowLevelQosParameters.decode(
          qosFlowSetupRequestItem.qosFlowLevelQosParameters))
    return false;

  return true;
}
}  // namespace ngap
