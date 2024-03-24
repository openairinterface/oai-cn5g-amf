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

#include "QosFlowPerTnlInformationItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowPerTnlInformationItem::QosFlowPerTnlInformationItem() {}

//------------------------------------------------------------------------------
QosFlowPerTnlInformationItem::~QosFlowPerTnlInformationItem() {}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformationItem::set(
    const QosFlowPerTnlInformation& qosFlowPerTNLInformation) {
  m_QosFlowPerTnlInformation = qosFlowPerTNLInformation;
}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformationItem::get(
    QosFlowPerTnlInformation& qosFlowPerTNLInformation) const {
  qosFlowPerTNLInformation = m_QosFlowPerTnlInformation;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformationItem::encode(
    Ngap_QosFlowPerTNLInformationItem_t& qosFlowPerTnlInformationItem) const {
  if (!m_QosFlowPerTnlInformation.encode(
          qosFlowPerTnlInformationItem.qosFlowPerTNLInformation))
    return false;
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformationItem::decode(
    const Ngap_QosFlowPerTNLInformationItem_t& qosFlowPerTnlInformationItem) {
  if (!m_QosFlowPerTnlInformation.decode(
          qosFlowPerTnlInformationItem.qosFlowPerTNLInformation))
    return false;

  return true;
}

}  // namespace ngap
