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

#include "NgReset.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NgResetMsg::NgResetMsg() {
  m_NgResetIes = nullptr;

  NgapMessage::setMessageType(NgapMessageType::NG_RESET);
  initialize();
}

//------------------------------------------------------------------------------
NgResetMsg::~NgResetMsg() {}

//------------------------------------------------------------------------------
void NgResetMsg::initialize() {
  m_NgResetIes = &(ngapPdu->choice.initiatingMessage->value.choice.NGReset);
}

//------------------------------------------------------------------------------
void NgResetMsg::setCause(const Cause& c) {
  m_Cause = c;

  Ngap_NGResetIEs_t* ie =
      (Ngap_NGResetIEs_t*) calloc(1, sizeof(Ngap_NGResetIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGResetIEs__value_PR_Cause;

  if (!m_Cause.encode(ie->value.choice.Cause)) {
    Logger::ngap().error("Encode NGAP Cause IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgResetIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP Cause IE error");
}

//------------------------------------------------------------------------------
void NgResetMsg::setResetType(const ResetType& r) {
  m_ResetType = r;

  Ngap_NGResetIEs_t* ie =
      (Ngap_NGResetIEs_t*) calloc(1, sizeof(Ngap_NGResetIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_ResetType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGResetIEs__value_PR_ResetType;

  if (!m_ResetType.encode(ie->value.choice.ResetType)) {
    Logger::ngap().error("Encode NGAP ResetType IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_NgResetIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP ResetType IE error");
}

//------------------------------------------------------------------------------
void NgResetMsg::getCause(Cause& c) const {
  c = m_Cause;
}

//------------------------------------------------------------------------------
bool NgResetMsg::getResetType(ResetType& r) const {
  if (m_ResetType.getResetType() == Ngap_ResetType_PR_nG_Interface) {
    long ng_interface = 0;
    m_ResetType.getResetType(ng_interface);
    r.setResetType(ng_interface);
  } else if (
      m_ResetType.getResetType() == Ngap_ResetType_PR_partOfNG_Interface) {
    // TODO
  }
  return true;
}

//------------------------------------------------------------------------------
bool NgResetMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_NGReset &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_NGReset) {
      m_NgResetIes = &ngapPdu->choice.initiatingMessage->value.choice.NGReset;
      for (int i = 0; i < m_NgResetIes->protocolIEs.list.count; i++) {
        switch (m_NgResetIes->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_Cause: {
            if (m_NgResetIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                m_NgResetIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetIEs__value_PR_Cause) {
              if (!m_Cause.decode(m_NgResetIes->protocolIEs.list.array[i]
                                      ->value.choice.Cause)) {
                Logger::ngap().error("Decoded NGAP Cause IE error");
                return false;
              }
            } else {
              Logger::ngap().error("Decoded NGAP Cause IE error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_ResetType: {
            if (m_NgResetIes->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                m_NgResetIes->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetIEs__value_PR_ResetType) {
              if (!m_ResetType.decode(m_NgResetIes->protocolIEs.list.array[i]
                                          ->value.choice.ResetType)) {
                Logger::ngap().error("Decoded NGAP ResetType IE error");
                return false;
              }

            } else {
              Logger::ngap().error("Decoded NGAP ResetType IE error");
              return false;
            }
          } break;

          default: {
            Logger::ngap().error("Decoded NGAP message PDU IE error");
            return false;
          }
        }
      }
    } else {
      Logger::ngap().error("Check NGReset message error!");
      return false;
    }
  } else {
    Logger::ngap().error("Check NGReset message error!");
    return false;
  }
  return true;
}

}  // namespace ngap
