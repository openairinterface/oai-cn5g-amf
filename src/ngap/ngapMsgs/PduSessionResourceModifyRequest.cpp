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
    : NgapUeMessage() {
  m_RanPagingPriority = nullptr;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_MODIFY_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceModifyRequestMsg::~PduSessionResourceModifyRequestMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::initialize() {
  m_PduSessionResourceModifyRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .PDUSessionResourceModifyRequest);
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setAmfUeNgapId(
    const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceModifyRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceModifyRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setRanPagingPriority(
    const uint32_t& priority) {
  if (!m_RanPagingPriority) m_RanPagingPriority = new RanPagingPriority();

  m_RanPagingPriority->set(priority);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RANPagingPriority;

  int ret = m_RanPagingPriority->encode(ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceModifyRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
int PduSessionResourceModifyRequestMsg::getRanPagingPriority() const {
  if (!m_RanPagingPriority) return -1;
  return m_RanPagingPriority->get();
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyRequestMsg::setPduSessionResourceModifyRequestList(
    const std::vector<PDUSessionResourceModifyRequestItem_t>& list) {
  std::vector<PduSessionResourceModifyItemModReq>
      pduSessionResourceModifyItemModReq;

  for (int i = 0; i < list.size(); i++) {
    PduSessionId pduSessionId = {};
    pduSessionId.set(list[i].pduSessionId);
    NasPdu nasPdu = {};
    if (amf_conv::check_bstring(list[i].nasPdu)) {
      nasPdu.set(list[i].nasPdu);
    }
    std::optional<SNssai> snssai = std::nullopt;
    if (list[i].sNssai.has_value()) {
      SNssai tmp = {};
      tmp.setSd(list[i].sNssai.value().sd);
      tmp.setSst(list[i].sNssai.value().sst);
      snssai = std::optional<ngap::SNssai>(tmp);
    }

    PduSessionResourceModifyItemModReq item = {};

    item.set(
        pduSessionId, nasPdu, list[i].pduSessionResourceModifyRequestTransfer,
        snssai);
    pduSessionResourceModifyItemModReq.push_back(item);
  }

  m_PduSessionResourceModifyList.set(pduSessionResourceModifyItemModReq);

  Ngap_PDUSessionResourceModifyRequestIEs_t* ie =
      (Ngap_PDUSessionResourceModifyRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceModifyListModReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceModifyRequestIEs__value_PR_PDUSessionResourceModifyListModReq;

  int ret = m_PduSessionResourceModifyList.encode(
      ie->value.choice.PDUSessionResourceModifyListModReq);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceModifyListModReq IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceModifyRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyRequestMsg::getPduSessionResourceModifyRequestList(
    std::vector<PDUSessionResourceModifyRequestItem_t>& list) const {
  std::vector<PduSessionResourceModifyItemModReq>
      pduSessionResourceModifyItemModReq;
  m_PduSessionResourceModifyList.get(pduSessionResourceModifyItemModReq);

  for (int i = 0; i < pduSessionResourceModifyItemModReq.size(); i++) {
    PDUSessionResourceModifyRequestItem_t request = {};

    PduSessionId pduSessionId    = {};
    std::optional<NasPdu> nasPdu = std::nullopt;
    std::optional<SNssai> SNssai = std::nullopt;

    pduSessionResourceModifyItemModReq[i].get(
        pduSessionId, nasPdu, request.pduSessionResourceModifyRequestTransfer,
        SNssai);

    pduSessionId.get(request.pduSessionId);
    if (nasPdu.has_value()) nasPdu.value().get(request.nasPdu);
    if (SNssai.has_value()) {
      S_Nssai tmp = {};
      SNssai.value().getSd(tmp.sd);
      SNssai.value().getSst(tmp.sst);
      request.sNssai = std::optional<S_Nssai>(tmp);
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
      m_PduSessionResourceModifyRequestIes =
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
       i < m_PduSessionResourceModifyRequestIes->protocolIEs.list.count; i++) {
    switch (
        m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_PduSessionResourceModifyRequestIes->protocolIEs.list
                      .array[i]
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
        if (m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_PduSessionResourceModifyRequestIes->protocolIEs.list
                      .array[i]
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
        if (m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_RANPagingPriority) {
          m_RanPagingPriority = new RanPagingPriority();
          if (!m_RanPagingPriority->decode(
                  m_PduSessionResourceModifyRequestIes->protocolIEs.list
                      .array[i]
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
        if (m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceModifyRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceModifyRequestIEs__value_PR_PDUSessionResourceModifyListModReq) {
          if (!m_PduSessionResourceModifyList.decode(
                  m_PduSessionResourceModifyRequestIes->protocolIEs.list
                      .array[i]
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
