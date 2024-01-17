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
void QosFlowPerTnlInformation::setQoSFlowPerTNLInformation(
    const UpTransportLayerInformation& m_uPTransportLayerInformation,
    const AssociatedQosFlowList& m_associatedQosFlowList) {
  uPTransportLayerInformation = m_uPTransportLayerInformation;
  associatedQosFlowList       = m_associatedQosFlowList;
}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformation::getQoSFlowPerTNLInformation(
    UpTransportLayerInformation& m_uPTransportLayerInformation,
    AssociatedQosFlowList& m_associatedQosFlowList) const {
  m_uPTransportLayerInformation = uPTransportLayerInformation;
  m_associatedQosFlowList       = associatedQosFlowList;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformation::encode(
    Ngap_QosFlowPerTNLInformation_t& qosFlowPerTNLInformation) {
  if (!uPTransportLayerInformation.encode(
          qosFlowPerTNLInformation.uPTransportLayerInformation))
    return false;

  if (!associatedQosFlowList.encode(
          qosFlowPerTNLInformation.associatedQosFlowList))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformation::decode(
    const Ngap_QosFlowPerTNLInformation_t& qosFlowPerTNLInformation) {
  if (!uPTransportLayerInformation.decode(
          qosFlowPerTNLInformation.uPTransportLayerInformation))
    return false;
  if (!associatedQosFlowList.decode(
          qosFlowPerTNLInformation.associatedQosFlowList))
    return false;

  return true;
}

}  // namespace ngap
