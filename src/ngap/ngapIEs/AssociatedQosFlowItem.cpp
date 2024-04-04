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

#include "AssociatedQosFlowItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
AssociatedQosFlowItem::AssociatedQosFlowItem() {
  m_QosFlowMappingIndication = -1;
}

//------------------------------------------------------------------------------
AssociatedQosFlowItem::~AssociatedQosFlowItem() {}

//------------------------------------------------------------------------------
void AssociatedQosFlowItem::set(
    const e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication&
        qosFlowMappingIndication,
    const QosFlowIdentifier& qosFlowIdentifier) {
  m_QosFlowMappingIndication = qosFlowMappingIndication;
  m_QosFlowIdentifier        = qosFlowIdentifier;
}

//------------------------------------------------------------------------------
void AssociatedQosFlowItem::set(const QosFlowIdentifier& qosFlowIdentifier) {
  m_QosFlowIdentifier = qosFlowIdentifier;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowItem::get(
    long& qosFlowMappingIndication,
    QosFlowIdentifier& qosFlowIdentifier) const {
  qosFlowMappingIndication = m_QosFlowMappingIndication;
  qosFlowIdentifier        = m_QosFlowIdentifier;

  return true;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowItem::encode(
    Ngap_AssociatedQosFlowItem_t& associatedQosFlowItem) const {
  if (m_QosFlowMappingIndication >= 0) {
    associatedQosFlowItem.qosFlowMappingIndication =
        (long*) calloc(1, sizeof(long));
    *associatedQosFlowItem.qosFlowMappingIndication =
        m_QosFlowMappingIndication;
  }

  if (!m_QosFlowIdentifier.encode(associatedQosFlowItem.qosFlowIdentifier))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowItem::decode(
    const Ngap_AssociatedQosFlowItem_t& associatedQosFlowItem) {
  if (!m_QosFlowIdentifier.decode(associatedQosFlowItem.qosFlowIdentifier))
    return false;

  if (associatedQosFlowItem.qosFlowMappingIndication) {
    m_QosFlowMappingIndication =
        *associatedQosFlowItem.qosFlowMappingIndication;
  }

  return true;
}

}  // namespace ngap
