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

#include "CriticalityDiagnostics.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
CriticalityDiagnostics::CriticalityDiagnostics() {
  m_ProcedureCodeIsSet        = false;
  m_TriggeringMessageIsSet    = false;
  m_ProcedureCriticalityIsSet = false;
}

//------------------------------------------------------------------------------
CriticalityDiagnostics::~CriticalityDiagnostics() {}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setProcedureCodeValue(
    const Ngap_ProcedureCode_t& procedureCode) {
  m_ProcedureCodeIsSet = true;
  m_ProcedureCode      = procedureCode;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setTriggeringMessageValue(
    const Ngap_TriggeringMessage_t& triggeringMessage) {
  m_TriggeringMessageIsSet = true;
  m_TriggeringMessage      = triggeringMessage;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setCriticalityValue(
    const Ngap_Criticality_t& procedureCriticality) {
  m_ProcedureCriticalityIsSet = true;
  m_ProcedureCriticality      = procedureCriticality;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setIesCriticalityDiagnosticsList(
    const std::vector<IesCriticalityDiagnostics>& iEsCriticalityDiagnostics) {
  uint8_t number_items = (iEsCriticalityDiagnostics.size() >
                          kCriticalityDiagnosticsMaxNoOfErrors) ?
                             kCriticalityDiagnosticsMaxNoOfErrors :
                             iEsCriticalityDiagnostics.size();

  for (int i = 0; i < number_items; i++) {
    m_IEsCriticalityDiagnostics.push_back(iEsCriticalityDiagnostics[i]);
  }
}

//------------------------------------------------------------------------------
int CriticalityDiagnostics::encode(
    Ngap_NGSetupFailure_t& ngSetupFailure) const {
  Ngap_NGSetupFailureIEs_t* ie =
      (Ngap_NGSetupFailureIEs_t*) calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_CriticalityDiagnostics;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupFailureIEs__value_PR_CriticalityDiagnostics;

  if (m_ProcedureCodeIsSet) {
    Ngap_ProcedureCode_t* procedureCodeIE =
        (Ngap_ProcedureCode_t*) calloc(1, sizeof(Ngap_ProcedureCode_t));
    *procedureCodeIE                                      = m_ProcedureCode;
    ie->value.choice.CriticalityDiagnostics.procedureCode = procedureCodeIE;
  }
  if (m_TriggeringMessageIsSet) {
    Ngap_TriggeringMessage_t* triggeringMessageIE =
        (Ngap_TriggeringMessage_t*) calloc(1, sizeof(Ngap_TriggeringMessage_t));
    *triggeringMessageIE = m_TriggeringMessage;
    ie->value.choice.CriticalityDiagnostics.triggeringMessage =
        triggeringMessageIE;
  }
  if (m_ProcedureCriticalityIsSet) {
    Ngap_Criticality_t* procedureCriticalityIE =
        (Ngap_Criticality_t*) calloc(1, sizeof(Ngap_Criticality_t));
    *procedureCriticalityIE = m_ProcedureCriticality;
    ie->value.choice.CriticalityDiagnostics.procedureCriticality =
        procedureCriticalityIE;
  }

  if (m_IEsCriticalityDiagnostics.size() > 0) {
    Ngap_CriticalityDiagnostics_IE_List_t* ieList =
        (Ngap_CriticalityDiagnostics_IE_List_t*) calloc(
            1, sizeof(Ngap_CriticalityDiagnostics_IE_List_t));
    for (int i = 0; i < m_IEsCriticalityDiagnostics.size(); i++) {
      Ngap_CriticalityDiagnostics_IE_Item_t* ieItem =
          (Ngap_CriticalityDiagnostics_IE_Item_t*) calloc(
              1, sizeof(Ngap_CriticalityDiagnostics_IE_Item_t));
      m_IEsCriticalityDiagnostics[i].encode(*ieItem);
      ASN_SEQUENCE_ADD(&ieList->list, ieItem);
    }
    ie->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics = ieList;
  }
  if (!m_ProcedureCodeIsSet && !m_TriggeringMessageIsSet &&
      !m_ProcedureCriticalityIsSet && !m_NumberOfIEsCriticalityDiagnostics) {
    free(ie);
    return 1;
  }
  int ret = ASN_SEQUENCE_ADD(&ngSetupFailure.protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode CriticalityDiagnostics IE error");
  return ret;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::decode(const Ngap_CriticalityDiagnostics_t& pdu) {
  if (pdu.procedureCode) {
    m_ProcedureCodeIsSet = true;
    m_ProcedureCode      = *pdu.procedureCode;
  }
  if (pdu.triggeringMessage) {
    m_TriggeringMessageIsSet = true;
    m_TriggeringMessage      = *pdu.triggeringMessage;
  }
  if (pdu.procedureCriticality) {
    m_ProcedureCriticalityIsSet = true;
    m_ProcedureCriticality      = *pdu.procedureCriticality;
  }
  if (pdu.iEsCriticalityDiagnostics) {
    m_NumberOfIEsCriticalityDiagnostics =
        pdu.iEsCriticalityDiagnostics->list.count;
    for (int i = 0; i < m_NumberOfIEsCriticalityDiagnostics; i++) {
      IesCriticalityDiagnostics item = {};
      item.decode(*pdu.iEsCriticalityDiagnostics->list.array[i]);
      m_IEsCriticalityDiagnostics.push_back(item);
    }
  }
  if (!m_ProcedureCodeIsSet && !m_TriggeringMessageIsSet &&
      !m_ProcedureCriticalityIsSet && !m_NumberOfIEsCriticalityDiagnostics) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getProcedureCodeValue(
    Ngap_ProcedureCode_t& procedureCode) const {
  procedureCode = m_ProcedureCode;
  return m_ProcedureCodeIsSet;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getTriggeringMessageValue(
    Ngap_TriggeringMessage_t& triggeringMessage) const {
  triggeringMessage = m_TriggeringMessage;
  return m_TriggeringMessageIsSet;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getCriticalityValue(
    Ngap_Criticality_t& procedureCriticality) const {
  procedureCriticality = m_ProcedureCriticality;
  return m_ProcedureCriticalityIsSet;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::getIesCriticalityDiagnosticsList(
    std::vector<IesCriticalityDiagnostics>& iEsCriticalityDiagnostics) const {
  iEsCriticalityDiagnostics = m_IEsCriticalityDiagnostics;
}
}  // namespace ngap
