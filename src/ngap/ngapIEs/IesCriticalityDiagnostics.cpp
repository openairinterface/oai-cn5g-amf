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

#include "IesCriticalityDiagnostics.hpp"

namespace ngap {

//------------------------------------------------------------------------------
IesCriticalityDiagnostics::IesCriticalityDiagnostics() {}

//------------------------------------------------------------------------------
IesCriticalityDiagnostics::~IesCriticalityDiagnostics() {}

//------------------------------------------------------------------------------
void IesCriticalityDiagnostics::setIeCriticality(
    const Ngap_Criticality_t& criticality) {
  m_Criticality = criticality;
}

//------------------------------------------------------------------------------
void IesCriticalityDiagnostics::setIeId(
    const Ngap_ProtocolIE_ID_t& protocolIeId) {
  m_ProtocolIeId = protocolIeId;
}

//------------------------------------------------------------------------------
void IesCriticalityDiagnostics::setTypeOfError(
    const Ngap_TypeOfError_t& typeOfError) {
  m_TypeOfError = typeOfError;
}

//------------------------------------------------------------------------------
void IesCriticalityDiagnostics::encode(
    Ngap_CriticalityDiagnostics_IE_Item_t& IE_Item) const {
  IE_Item.iECriticality = m_Criticality;
  IE_Item.iE_ID         = m_ProtocolIeId;
  IE_Item.typeOfError   = m_TypeOfError;
}

//------------------------------------------------------------------------------
void IesCriticalityDiagnostics::decode(
    const Ngap_CriticalityDiagnostics_IE_Item_t& pdu) {
  m_Criticality  = pdu.iECriticality;
  m_ProtocolIeId = pdu.iE_ID;
  m_TypeOfError  = pdu.typeOfError;
}

//------------------------------------------------------------------------------
Ngap_Criticality_t IesCriticalityDiagnostics::getIeCriticality() const {
  return m_Criticality;
}

//------------------------------------------------------------------------------
Ngap_ProtocolIE_ID_t IesCriticalityDiagnostics::getIeId() const {
  return m_ProtocolIeId;
}

//------------------------------------------------------------------------------
Ngap_TypeOfError_t IesCriticalityDiagnostics::getTypeOfError() const {
  return m_TypeOfError;
}
}  // namespace ngap
