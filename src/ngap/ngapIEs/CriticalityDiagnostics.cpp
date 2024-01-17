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
  procedureCodeIsSet        = false;
  triggeringMessageIsSet    = false;
  procedureCriticalityIsSet = false;
}

//------------------------------------------------------------------------------
CriticalityDiagnostics::~CriticalityDiagnostics() {}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setProcedureCodeValue(
    Ngap_ProcedureCode_t m_procedureCode) {
  procedureCodeIsSet = true;
  procedureCode      = m_procedureCode;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setTriggeringMessageValue(
    Ngap_TriggeringMessage_t m_triggeringMessage) {
  triggeringMessageIsSet = true;
  triggeringMessage      = m_triggeringMessage;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setCriticalityValue(
    Ngap_Criticality_t m_procedureCriticality) {
  procedureCriticalityIsSet = true;
  procedureCriticality      = m_procedureCriticality;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setIEsCriticalityDiagnosticsList(
    const std::vector<IEsCriticalityDiagnostics>& m_iEsCriticalityDiagnostics) {
  uint8_t number_items = (m_iEsCriticalityDiagnostics.size() >
                          kCriticalityDiagnosticsMaxNoOfErrors) ?
                             kCriticalityDiagnosticsMaxNoOfErrors :
                             m_iEsCriticalityDiagnostics.size();

  for (int i = 0; i < number_items; i++) {
    iEsCriticalityDiagnostics.push_back(m_iEsCriticalityDiagnostics[i]);
  }
}

//------------------------------------------------------------------------------
int CriticalityDiagnostics::encode(Ngap_NGSetupFailure_t& ngSetupFailure) {
  Ngap_NGSetupFailureIEs_t* ie =
      (Ngap_NGSetupFailureIEs_t*) calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_CriticalityDiagnostics;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupFailureIEs__value_PR_CriticalityDiagnostics;

  if (procedureCodeIsSet) {
    Ngap_ProcedureCode_t* procedureCodeIE =
        (Ngap_ProcedureCode_t*) calloc(1, sizeof(Ngap_ProcedureCode_t));
    *procedureCodeIE                                      = procedureCode;
    ie->value.choice.CriticalityDiagnostics.procedureCode = procedureCodeIE;
  }
  if (triggeringMessageIsSet) {
    Ngap_TriggeringMessage_t* triggeringMessageIE =
        (Ngap_TriggeringMessage_t*) calloc(1, sizeof(Ngap_TriggeringMessage_t));
    *triggeringMessageIE = triggeringMessage;
    ie->value.choice.CriticalityDiagnostics.triggeringMessage =
        triggeringMessageIE;
  }
  if (procedureCriticalityIsSet) {
    Ngap_Criticality_t* procedureCriticalityIE =
        (Ngap_Criticality_t*) calloc(1, sizeof(Ngap_Criticality_t));
    *procedureCriticalityIE = procedureCriticality;
    ie->value.choice.CriticalityDiagnostics.procedureCriticality =
        procedureCriticalityIE;
  }

  if (iEsCriticalityDiagnostics.size() > 0) {
    Ngap_CriticalityDiagnostics_IE_List_t* ieList =
        (Ngap_CriticalityDiagnostics_IE_List_t*) calloc(
            1, sizeof(Ngap_CriticalityDiagnostics_IE_List_t));
    for (int i = 0; i < iEsCriticalityDiagnostics.size(); i++) {
      Ngap_CriticalityDiagnostics_IE_Item_t* ieItem =
          (Ngap_CriticalityDiagnostics_IE_Item_t*) calloc(
              1, sizeof(Ngap_CriticalityDiagnostics_IE_Item_t));
      iEsCriticalityDiagnostics[i].encode(*ieItem);
      ASN_SEQUENCE_ADD(&ieList->list, ieItem);
    }
    ie->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics = ieList;
  }
  if (!procedureCodeIsSet && !triggeringMessageIsSet &&
      !procedureCriticalityIsSet && !numberOfIEsCriticalityDiagnostics) {
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
    procedureCodeIsSet = true;
    procedureCode      = *pdu.procedureCode;
  }
  if (pdu.triggeringMessage) {
    triggeringMessageIsSet = true;
    triggeringMessage      = *pdu.triggeringMessage;
  }
  if (pdu.procedureCriticality) {
    procedureCriticalityIsSet = true;
    procedureCriticality      = *pdu.procedureCriticality;
  }
  if (pdu.iEsCriticalityDiagnostics) {
    numberOfIEsCriticalityDiagnostics =
        pdu.iEsCriticalityDiagnostics->list.count;
    for (int i = 0; i < numberOfIEsCriticalityDiagnostics; i++) {
      IEsCriticalityDiagnostics item = {};
      item.decode(*pdu.iEsCriticalityDiagnostics->list.array[i]);
      iEsCriticalityDiagnostics.push_back(item);
    }
  }
  if (!procedureCodeIsSet && !triggeringMessageIsSet &&
      !procedureCriticalityIsSet && !numberOfIEsCriticalityDiagnostics) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getProcedureCodeValue(
    Ngap_ProcedureCode_t& m_procedureCode) const {
  m_procedureCode = procedureCode;
  return procedureCodeIsSet;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getTriggeringMessageValue(
    Ngap_TriggeringMessage_t& m_triggeringMessage) const {
  m_triggeringMessage = triggeringMessage;
  return triggeringMessageIsSet;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getCriticalityValue(
    Ngap_Criticality_t& m_procedureCriticality) const {
  m_procedureCriticality = procedureCriticality;
  return procedureCriticalityIsSet;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::getIEsCriticalityDiagnosticsList(
    std::vector<IEsCriticalityDiagnostics>& m_iEsCriticalityDiagnostics) const {
  m_iEsCriticalityDiagnostics = iEsCriticalityDiagnostics;
}
}  // namespace ngap
