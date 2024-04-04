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

#include "QosFlowPerTnlInformation.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowPerTnlInformation::QosFlowPerTnlInformation() {}

//------------------------------------------------------------------------------
QosFlowPerTnlInformation::~QosFlowPerTnlInformation() {}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformation::set(
    const UpTransportLayerInformation& uPTransportLayerInformation,
    const AssociatedQosFlowList& associatedQosFlowList) {
  m_UpTransportLayerInformation = uPTransportLayerInformation;
  m_AssociatedQosFlowList       = associatedQosFlowList;
}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformation::get(
    UpTransportLayerInformation& uPTransportLayerInformation,
    AssociatedQosFlowList& associatedQosFlowList) const {
  uPTransportLayerInformation = m_UpTransportLayerInformation;
  associatedQosFlowList       = m_AssociatedQosFlowList;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformation::encode(
    Ngap_QosFlowPerTNLInformation_t& qosFlowPerTnlInformation) const {
  if (!m_UpTransportLayerInformation.encode(
          qosFlowPerTnlInformation.uPTransportLayerInformation))
    return false;

  if (!m_AssociatedQosFlowList.encode(
          qosFlowPerTnlInformation.associatedQosFlowList))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformation::decode(
    const Ngap_QosFlowPerTNLInformation_t& qosFlowPerTnlInformation) {
  if (!m_UpTransportLayerInformation.decode(
          qosFlowPerTnlInformation.uPTransportLayerInformation))
    return false;
  if (!m_AssociatedQosFlowList.decode(
          qosFlowPerTnlInformation.associatedQosFlowList))
    return false;

  return true;
}

}  // namespace ngap
