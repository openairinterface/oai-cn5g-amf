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

#include "NgResetAck.hpp"

#include <vector>

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NgResetAckMsg::NgResetAckMsg() {
  m_NgResetAckIes                       = nullptr;
  m_UeAssociatedLogicalNgConnectionList = std::nullopt;
  m_CriticalityDiagnostics              = nullptr;
  NgapMessage::setMessageType(NgapMessageType::NG_RESET_ACKNOWLEDGE);
  initialize();
}
//------------------------------------------------------------------------------
NgResetAckMsg::~NgResetAckMsg() {
  if (m_CriticalityDiagnostics)
    utils::free_wrapper((void**) &m_CriticalityDiagnostics);
}

//------------------------------------------------------------------------------
void NgResetAckMsg::initialize() {
  m_NgResetAckIes =
      &(ngapPdu->choice.successfulOutcome->value.choice.NGResetAcknowledge);
}

//------------------------------------------------------------------------------
void NgResetAckMsg::setUeAssociatedLogicalNgConnectionList(
    const std::vector<UeAssociatedLogicalNgConnectionItem>& list) {
  UeAssociatedLogicalNgConnectionList tmp = {};
  tmp.set(list);
  m_UeAssociatedLogicalNgConnectionList =
      std::make_optional<UeAssociatedLogicalNgConnectionList>(tmp);

  addUeAssociatedLogicalNgConnectionList();
}

//------------------------------------------------------------------------------
void NgResetAckMsg::getUeAssociatedLogicalNgConnectionList(
    std::vector<UeAssociatedLogicalNgConnectionItem>& list) const {
  if (m_UeAssociatedLogicalNgConnectionList.has_value()) {
    m_UeAssociatedLogicalNgConnectionList.value().get(list);
  }
}

//------------------------------------------------------------------------------
void NgResetAckMsg::addUeAssociatedLogicalNgConnectionList() {
  Ngap_NGResetAcknowledgeIEs_t* ie = (Ngap_NGResetAcknowledgeIEs_t*) calloc(
      1, sizeof(Ngap_NGResetAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UE_associatedLogicalNG_connectionList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_NGResetAcknowledgeIEs__value_PR_UE_associatedLogicalNG_connectionList;

  if (!m_UeAssociatedLogicalNgConnectionList.value().encode(
          ie->value.choice.UE_associatedLogicalNG_connectionList)) {
    Logger::ngap().error(
        "Encode NGAP UE_associatedLogicalNG_connectionList IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgResetAckIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP UE_associatedLogicalNG_connectionList IE error");
}

//------------------------------------------------------------------------------
bool NgResetAckMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGReset &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_NGResetAcknowledge) {
      m_NgResetAckIes =
          &ngapPdu->choice.successfulOutcome->value.choice.NGResetAcknowledge;
      for (int i = 0; i < m_NgResetAckIes->protocolIEs.list.count; i++) {
        switch (m_NgResetAckIes->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_UE_associatedLogicalNG_connectionList: {
            if (m_NgResetAckIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                m_NgResetAckIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetAcknowledgeIEs__value_PR_UE_associatedLogicalNG_connectionList) {
              UeAssociatedLogicalNgConnectionList tmp = {};
              if (!tmp.decode(m_NgResetAckIes->protocolIEs.list.array[i]
                                  ->value.choice
                                  .UE_associatedLogicalNG_connectionList)) {
                Logger::ngap().error(
                    "Decoded NGAP UE_associatedLogicalNG_connectionList IE "
                    "error");
                return false;
              }
              m_UeAssociatedLogicalNgConnectionList =
                  std::make_optional<UeAssociatedLogicalNgConnectionList>(tmp);
            } else {
              Logger::ngap().error(
                  "Decoded NGAP UE_associatedLogicalNG_connectionList IE "
                  "error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
            // TODO:
          } break;
          default: {
            Logger::ngap().error(
                "Decoded NGAP NGResetAck message PDU IE error");
            return false;
          }
        }
      }
    } else {
      Logger::ngap().error("Check NGResetAck message error!");
      return false;
    }
  } else {
    Logger::ngap().error("Check NGResetAck message error!");
    return false;
  }
  return true;
}

}  // namespace ngap
