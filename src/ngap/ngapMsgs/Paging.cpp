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

#include "Paging.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PagingMsg::PagingMsg() {
  m_PagingIes = nullptr;

  NgapMessage::setMessageType(NgapMessageType::PAGING);
  initialize();
}

//------------------------------------------------------------------------------
PagingMsg::~PagingMsg() {}

//------------------------------------------------------------------------------
void PagingMsg::initialize() {
  m_PagingIes = &(ngapPdu->choice.initiatingMessage->value.choice.Paging);
}

//------------------------------------------------------------------------------
bool PagingMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_Paging &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_Paging) {
      m_PagingIes = &ngapPdu->choice.initiatingMessage->value.choice.Paging;
    } else {
      Logger::ngap().error("Check Paging message error");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error");
    return false;
  }
  for (int i = 0; i < m_PagingIes->protocolIEs.list.count; i++) {
    switch (m_PagingIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_UEPagingIdentity: {
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_UEPagingIdentity) {
          if (!m_UePagingIdentity.decode(m_PagingIes->protocolIEs.list.array[i]
                                             ->value.choice.UEPagingIdentity)) {
            Logger::ngap().error("Decoded NGAP UEPagingIdentity IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP UEPagingIdentity IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TAIListForPaging: {
        if (m_PagingIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_PagingIes->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_TAIListForPaging) {
          if (!m_TaiListForPaging.decode(m_PagingIes->protocolIEs.list.array[i]
                                             ->value.choice.TAIListForPaging)) {
            Logger::ngap().error("Decoded NGAP TAIListForPaging IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP TAIListForPaging IE error");
          return false;
        }
      } break;
      default: {
        Logger::ngap().warn(
            "Not decoded IE %d", m_PagingIes->protocolIEs.list.array[i]->id);

        return true;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
void PagingMsg::setUePagingIdentity(
    const std::string& setId, const std::string& pointer, std::string tmsi) {
  m_UePagingIdentity.set(setId, pointer, tmsi);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UEPagingIdentity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_UEPagingIdentity;

  int ret = m_UePagingIdentity.encode(ie->value.choice.UEPagingIdentity);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UEPagingIdentity IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP UEPagingIdentity IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::getUePagingIdentity(std::string& _5g_s_tmsi) const {
  m_UePagingIdentity.get(_5g_s_tmsi);
}

//------------------------------------------------------------------------------
void PagingMsg::getUePagingIdentity(
    std::string& setId, std::string& pointer, std::string& tmsi) const {
  m_UePagingIdentity.get(setId, pointer, tmsi);
}

//------------------------------------------------------------------------------
void PagingMsg::setTaiListForPaging(const std::vector<Tai_t>& list) {
  if (list.size() == 0) {
    Logger::ngap().warn("Setup failed, vector is empty");
    return;
  }

  std::vector<Tai> tailist;

  PlmnId plmnid[list.size()];
  TAC tac[list.size()];
  for (int i = 0; i < list.size(); i++) {
    Tai tai = {};
    plmnid[i].set(list[i].mcc, list[i].mnc);
    tac[i].set(list[i].tac);
    tai.set(plmnid[i], tac[i]);
    tailist.push_back(tai);
  }
  m_TaiListForPaging.set(tailist);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_TAIListForPaging;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_TAIListForPaging;

  int ret = m_TaiListForPaging.encode(ie->value.choice.TAIListForPaging);
  if (!ret) {
    Logger::ngap().error("Encode NGAP TAIListForPaging IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_PagingIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP TAIListForPaging IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::getTaiListForPaging(std::vector<Tai_t>& list) const {
  std::vector<Tai> taiList;
  m_TaiListForPaging.get(taiList);

  for (auto& tai : taiList) {
    Tai_t t = {};
    tai.get(t);
    list.push_back(t);
  }
}

}  // namespace ngap
