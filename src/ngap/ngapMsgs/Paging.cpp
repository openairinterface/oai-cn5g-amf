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
  pagingIEs = nullptr;

  NgapMessage::setMessageType(NgapMessageType::PAGING);
  initialize();
}

//------------------------------------------------------------------------------
PagingMsg::~PagingMsg() {}

//------------------------------------------------------------------------------
void PagingMsg::initialize() {
  pagingIEs = &(ngapPdu->choice.initiatingMessage->value.choice.Paging);
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
      pagingIEs = &ngapPdu->choice.initiatingMessage->value.choice.Paging;
    } else {
      Logger::ngap().error("Check Paging message error");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error");
    return false;
  }
  for (int i = 0; i < pagingIEs->protocolIEs.list.count; i++) {
    switch (pagingIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_UEPagingIdentity: {
        if (pagingIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            pagingIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_UEPagingIdentity) {
          if (!uePagingIdentity.decode(pagingIEs->protocolIEs.list.array[i]
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
        if (pagingIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            pagingIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_PagingIEs__value_PR_TAIListForPaging) {
          if (!taIListForPaging.decode(pagingIEs->protocolIEs.list.array[i]
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
            "Not decoded IE %d", pagingIEs->protocolIEs.list.array[i]->id);

        return true;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
void PagingMsg::setUEPagingIdentity(
    const std::string& setId, const std::string& pointer, std::string tmsi) {
  uePagingIdentity.setUEPagingIdentity(setId, pointer, tmsi);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UEPagingIdentity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_UEPagingIdentity;

  int ret = uePagingIdentity.encode(ie->value.choice.UEPagingIdentity);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UEPagingIdentity IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&pagingIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP UEPagingIdentity IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::getUEPagingIdentity(std::string& _5g_s_tmsi) {
  uePagingIdentity.getUEPagingIdentity(_5g_s_tmsi);
}

//------------------------------------------------------------------------------
void PagingMsg::getUEPagingIdentity(
    std::string& setId, std::string& pointer, std::string& tmsi) {
  uePagingIdentity.getUEPagingIdentity(setId, pointer, tmsi);
}

//------------------------------------------------------------------------------
void PagingMsg::setTAIListForPaging(const std::vector<Tai_t>& list) {
  if (list.size() == 0) {
    Logger::ngap().warn("Setup failed, vector is empty");
    return;
  }

  std::vector<TAI> tailist;

  PlmnId plmnid[list.size()];
  TAC tac[list.size()];
  for (int i = 0; i < list.size(); i++) {
    TAI tai = {};
    plmnid[i].set(list[i].mcc, list[i].mnc);
    tac[i].set(list[i].tac);
    tai.setTAI(plmnid[i], tac[i]);
    tailist.push_back(tai);
  }
  taIListForPaging.setTAIListForPaging(tailist);

  Ngap_PagingIEs_t* ie =
      (Ngap_PagingIEs_t*) calloc(1, sizeof(Ngap_PagingIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_TAIListForPaging;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_PagingIEs__value_PR_TAIListForPaging;

  int ret = taIListForPaging.encode(ie->value.choice.TAIListForPaging);
  if (!ret) {
    Logger::ngap().error("Encode NGAP TAIListForPaging IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&pagingIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP TAIListForPaging IE error");
}

//------------------------------------------------------------------------------
void PagingMsg::getTAIListForPaging(std::vector<Tai_t>& list) {
  std::vector<TAI> taiList;
  taIListForPaging.getTAIListForPaging(taiList);

  for (auto& tai : taiList) {
    Tai_t t = {};
    tai.getTAI(t);
    list.push_back(t);
  }
}

}  // namespace ngap
