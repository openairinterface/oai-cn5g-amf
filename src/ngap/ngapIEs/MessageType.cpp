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

#include "MessageType.hpp"

#include "logger.hpp"

extern "C" {
#include "Ngap_Criticality.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProcedureCode.h"
#include "Ngap_SuccessfulOutcome.h"
#include "Ngap_UnsuccessfulOutcome.h"
}

namespace ngap {

//------------------------------------------------------------------------------
MessageType::MessageType() {
  m_Criticality = Ngap_Criticality_reject;
}

MessageType::MessageType(
    const Ngap_ProcedureCode_t& procedureCode, Ngap_NGAP_PDU_PR typeOfMessage) {
  m_Criticality   = Ngap_Criticality_reject;
  m_ProcedureCode = procedureCode;
  m_TypeOfMessage = typeOfMessage;
}

//------------------------------------------------------------------------------
MessageType::~MessageType() {}

//------------------------------------------------------------------------------
void MessageType::setProcedureCode(const Ngap_ProcedureCode_t& procedureCode) {
  m_ProcedureCode = procedureCode;
}

//------------------------------------------------------------------------------
void MessageType::setTypeOfMessage(Ngap_NGAP_PDU_PR typeOfMessage) {
  m_TypeOfMessage = typeOfMessage;
}

//------------------------------------------------------------------------------
void MessageType::setCriticality(Ngap_Criticality criticality) {
  m_Criticality = criticality;
}

//------------------------------------------------------------------------------
void MessageType::setValuePresent(
    Ngap_InitiatingMessage__value_PR valuePresent) {
  m_InitiatingMsgValuePresent = valuePresent;
}

//------------------------------------------------------------------------------
void MessageType::setValuePresent(
    Ngap_SuccessfulOutcome__value_PR valuePresent) {
  m_SuccessfulOutcomeValuePresent = valuePresent;
}

//------------------------------------------------------------------------------
void MessageType::setValuePresent(
    Ngap_UnsuccessfulOutcome__value_PR valuePresent) {
  m_UnsuccessfulOutcomeValuePresent = valuePresent;
}

//------------------------------------------------------------------------------
Ngap_ProcedureCode_t MessageType::getProcedureCode() const {
  return m_ProcedureCode;
}

//------------------------------------------------------------------------------
Ngap_NGAP_PDU_PR MessageType::getTypeOfMessage() const {
  return m_TypeOfMessage;
}

//------------------------------------------------------------------------------
Ngap_Criticality MessageType::getCriticality() const {
  return m_Criticality;
}

//------------------------------------------------------------------------------
int MessageType::encode(Ngap_NGAP_PDU_t& pdu) const {
  pdu.present = m_TypeOfMessage;
  switch (m_TypeOfMessage) {
    case Ngap_NGAP_PDU_PR_initiatingMessage: {
      pdu.choice.initiatingMessage = (Ngap_InitiatingMessage_t*) calloc(
          1, sizeof(Ngap_InitiatingMessage_t));
      pdu.choice.initiatingMessage->procedureCode = m_ProcedureCode;
      pdu.choice.initiatingMessage->criticality   = m_Criticality;
      pdu.choice.initiatingMessage->value.present = m_InitiatingMsgValuePresent;
      break;
    }
    case Ngap_NGAP_PDU_PR_successfulOutcome: {
      pdu.choice.successfulOutcome = (Ngap_SuccessfulOutcome_t*) calloc(
          1, sizeof(Ngap_SuccessfulOutcome_t));
      pdu.choice.successfulOutcome->procedureCode = m_ProcedureCode;
      pdu.choice.successfulOutcome->criticality   = m_Criticality;
      pdu.choice.successfulOutcome->value.present =
          m_SuccessfulOutcomeValuePresent;
      break;
    }
    case Ngap_NGAP_PDU_PR_unsuccessfulOutcome: {
      pdu.choice.unsuccessfulOutcome = (Ngap_UnsuccessfulOutcome_t*) calloc(
          1, sizeof(Ngap_UnsuccessfulOutcome_t));
      pdu.choice.unsuccessfulOutcome->procedureCode = m_ProcedureCode;
      pdu.choice.unsuccessfulOutcome->criticality   = m_Criticality;
      pdu.choice.unsuccessfulOutcome->value.present =
          m_UnsuccessfulOutcomeValuePresent;
      break;
    }
    case Ngap_NGAP_PDU_PR_NOTHING: {
      Logger::ngap().debug(
          "Ngap_NGAP_PDU_PR_NOTHING (messageType encode error)");
      return 0;
    }
  }
  return 1;
}

}  // namespace ngap
