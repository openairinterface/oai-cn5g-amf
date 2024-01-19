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

#include "PduSessionResourceModifyRequest.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceModifyRequestMsg::PduSessionResourceModifyRequestMsg()
    : NgapUEMessage() {
  ranPagingPriority = nullptr;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_MODIFY_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceModifyRequestMsg::~PduSessionResourceModifyRequestMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::initialize() {
  pduSessionResourceModifyRequestIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .PDUSessionResourceModifyRequest);
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceModifyRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceModifyRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setRanPagingPriority(
    const uint32_t& priority) {
  if (!ranPagingPriority) ranPagingPriority = new RanPagingPriority();

  ranPagingPriority->set(priority);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RANPagingPriority;

  int ret = ranPagingPriority->encode(ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceModifyRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
int PduSessionResourceModifyRequestMsg::getRanPagingPriority() {
  if (!ranPagingPriority) return -1;
  return ranPagingPriority->get();
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setPduSessionResourceModifyRequestList(
    const std::vector<PDUSessionResourceModifyRequestItem_t>& list) {
  std::vector<PduSessionResourceModifyItemModReq>
      m_pduSessionResourceModifyItemModReq;

  for (int i = 0; i < list.size(); i++) {
    PduSessionId pDUSessionID = {};
    pDUSessionID.set(list[i].pduSessionId);
    NasPdu nAS_PDU = {};
    if (amf_conv::check_bstring(list[i].nas_pdu)) {
      nAS_PDU.set(list[i].nas_pdu);
    }
    std::optional<S_NSSAI> s_NSSAI = std::nullopt;
    if (list[i].s_nssai.has_value()) {
      S_NSSAI tmp = {};
      tmp.setSd(list[i].s_nssai.value().sd);
      tmp.setSst(list[i].s_nssai.value().sst);
      s_NSSAI = std::optional<S_NSSAI>(tmp);
    }

    PduSessionResourceModifyItemModReq item = {};

    item.set(
        pDUSessionID, nAS_PDU, list[i].pduSessionResourceModifyRequestTransfer,
        s_NSSAI);
    m_pduSessionResourceModifyItemModReq.push_back(item);
  }

  pduSessionResourceModifyList.set(m_pduSessionResourceModifyItemModReq);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceModifyListModReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_PDUSessionResourceModifyListModReq;

  int ret = pduSessionResourceModifyList.encode(
      ie->value.choice.PDUSessionResourceModifyListModReq);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceModifyListModReq IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceModifyRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyRequestMsg::getPduSessionResourceModifyRequestList(
    std::vector<PDUSessionResourceModifyRequestItem_t>& list) {
  std::vector<PduSessionResourceModifyItemModReq>
      m_pduSessionResourceModifyItemModReq;
  int num = 0;
  pduSessionResourceModifyList.get(m_pduSessionResourceModifyItemModReq);

  for (int i = 0; i < m_pduSessionResourceModifyItemModReq.size(); i++) {
    PDUSessionResourceModifyRequestItem_t request = {};

    PduSessionId pDUSessionID      = {};
    std::optional<NasPdu> nAS_PDU  = std::nullopt;
    std::optional<S_NSSAI> s_NSSAI = std::nullopt;

    m_pduSessionResourceModifyItemModReq[i].get(
        pDUSessionID, nAS_PDU, request.pduSessionResourceModifyRequestTransfer,
        s_NSSAI);

    pDUSessionID.get(request.pduSessionId);
    if (nAS_PDU.has_value()) nAS_PDU.value().get(request.nas_pdu);
    if (s_NSSAI.has_value()) {
      S_Nssai tmp = {};
      s_NSSAI.value().getSd(tmp.sd);
      s_NSSAI.value().getSst(tmp.sst);
      request.s_nssai = std::optional<S_Nssai>(tmp);
    }

    list.push_back(request);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceModify &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_PDUSessionResourceModifyRequest) {
      pduSessionResourceModifyRequestIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .PDUSessionResourceModifyRequest;
    } else {
      Logger::ngap().error(
          "Check PDUSessionResourceModifyRequest message error!");

      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0;
       i < pduSessionResourceModifyRequestIEs->protocolIEs.list.count; i++) {
    switch (pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RANPagingPriority) {
          ranPagingPriority = new RanPagingPriority();
          if (!ranPagingPriority->decode(
                  pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.RANPagingPriority)) {
            Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceModifyListModReq: {
        if (pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_PDUSessionResourceModifyListModReq) {
          if (!pduSessionResourceModifyList.decode(
                  pduSessionResourceModifyRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceModifyListModReq)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceModifyListModReq IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceModifyListModReq IE error");

          return false;
        }
      } break;
      default: {
        Logger::ngap().error("Decoded NGAP Message PDU error");

        return false;
      }
    }
  }

  return true;
}

}  // namespace ngap
