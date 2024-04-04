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

#include "PduSessionResourceSetupRequest.hpp"

#include "3gpp_23.003.h"
#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestMsg::PduSessionResourceSetupRequestMsg()
    : NgapUeMessage() {
  m_PduSessionResourceSetupRequestIes = nullptr;
  m_RanPagingPriority                 = std::nullopt;
  m_NasPdu                            = std::nullopt;
  m_UeAggregateMaxBitRate             = std::nullopt;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestMsg::~PduSessionResourceSetupRequestMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::initialize() {
  m_PduSessionResourceSetupRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .PDUSessionResourceSetupRequest);
}

//-----------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setUeAggregateMaxBitRate(
    const uint64_t& bitRateDl, const uint64_t& bitRateUl) {
  UeAggregateMaxBitRate tmp = {};
  tmp.set(bitRateDl, bitRateUl);
  m_UeAggregateMaxBitRate = std::optional<UeAggregateMaxBitRate>(tmp);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = m_UeAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UeAggregateMaxBitRate IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UeAggregateMaxBitRate IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getUeAggregateMaxBitRate(
    uint64_t& bitRateDl, uint64_t& bitRateUl) const {
  if (!m_UeAggregateMaxBitRate.has_value()) return false;
  return m_UeAggregateMaxBitRate.value().get(bitRateDl, bitRateUl);
}
//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setAmfUeNgapId(
    const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setRanPagingPriority(
    const uint32_t& priority) {
  RanPagingPriority tmp = {};
  tmp.set(priority);
  m_RanPagingPriority = std::optional<RanPagingPriority>(tmp);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RANPagingPriority;

  int ret =
      m_RanPagingPriority.value().encode(ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getRanPagingPriority(
    uint32_t& ran_paging_priority) const {
  if (!m_RanPagingPriority.has_value()) return false;
  ran_paging_priority = m_RanPagingPriority.value().get();
  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setNasPdu(const bstring& pdu) {
  NasPdu tmp = {};
  tmp.set(pdu);
  m_NasPdu = std::optional<NasPdu>(tmp);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU;

  int ret = m_NasPdu.value().encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NGAP NAS_PDU IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getNasPdu(bstring& pdu) const {
  if (!m_NasPdu.has_value()) return false;
  return m_NasPdu.value().get(pdu);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setPduSessionResourceSetupRequestList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  std::vector<PduSessionResourceSetupItemSUReq> itemSUReqList;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceSetupItemSUReq itemSUReq = {};
    PduSessionId pduSessionId                  = {};
    pduSessionId.set(list[i].pduSessionId);
    std::optional<NasPdu> nasPdu = std::nullopt;
    if (amf_conv::check_bstring(list[i].nasPdu)) {
      NasPdu tmp = {};
      tmp.set(list[i].nasPdu);
      nasPdu = std::optional<NasPdu>(tmp);
    }
    SNssai SNssai = {};
    SNssai.setSst(list[i].sNssai.sst);

    uint32_t sd_int_value = 0;
    if (amf_conv::sd_string_to_int(list[i].sNssai.sd, sd_int_value)) {
      if (sd_int_value != SD_NO_VALUE) SNssai.setSd(list[i].sNssai.sd);
    }

    itemSUReq.set(
        pduSessionId, nasPdu, SNssai,
        list[i].pduSessionResourceSetupRequestTransfer);
    itemSUReqList.push_back(itemSUReq);
  }

  m_PduSessionResourceSetupRequestList.set(itemSUReqList);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq;

  int ret = m_PduSessionResourceSetupRequestList.encode(
      ie->value.choice.PDUSessionResourceSetupListSUReq);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getPduSessionResourceSetupRequestList(
    std::vector<PDUSessionResourceSetupRequestItem_t>& list) const {
  std::vector<PduSessionResourceSetupItemSUReq> itemSUReqList;
  m_PduSessionResourceSetupRequestList.get(itemSUReqList);

  for (auto& item : itemSUReqList) {
    PDUSessionResourceSetupRequestItem_t request = {};

    PduSessionId pduSessionId    = {};
    std::optional<NasPdu> nasPdu = std::nullopt;
    SNssai SNssai                = {};
    item.get(
        pduSessionId, nasPdu, SNssai,
        request.pduSessionResourceSetupRequestTransfer);
    pduSessionId.get(request.pduSessionId);
    SNssai.getSst(request.sNssai.sst);
    SNssai.getSd(request.sNssai.sd);
    if (nasPdu.has_value()) {
      nasPdu.value().get(request.nasPdu);
    }
    list.push_back(request);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceSetup &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_PDUSessionResourceSetupRequest) {
      m_PduSessionResourceSetupRequestIes =
          &ngapPdu->choice.initiatingMessage->value.choice
               .PDUSessionResourceSetupRequest;
    } else {
      Logger::ngap().error(
          "Check PDUSessionResourceSetupRequest message error!");

      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0;
       i < m_PduSessionResourceSetupRequestIes->protocolIEs.list.count; i++) {
    switch (
        m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_PduSessionResourceSetupRequestIes->protocolIEs.list
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
        if (m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_PduSessionResourceSetupRequestIes->protocolIEs.list
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
        if (m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RANPagingPriority) {
          RanPagingPriority tmp = {};
          if (!tmp.decode(m_PduSessionResourceSetupRequestIes->protocolIEs.list
                              .array[i]
                              ->value.choice.RANPagingPriority)) {
            Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
            return false;
          }
          m_RanPagingPriority = std::optional<RanPagingPriority>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU) {
          NasPdu tmp = {};
          if (!tmp.decode(m_PduSessionResourceSetupRequestIes->protocolIEs.list
                              .array[i]
                              ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
            return false;
          }
          m_NasPdu = std::optional<NasPdu>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq: {
        if (m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq) {
          if (!m_PduSessionResourceSetupRequestList.decode(
                  m_PduSessionResourceSetupRequestIes->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionResourceSetupListSUReq)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListSUReq IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListSUReq IE error");

          return false;
        }
      } break;
      // TODO:m_UeAggregateMaxBitRate
      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_PduSessionResourceSetupRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate) {
          UeAggregateMaxBitRate tmp = {};
          if (!tmp.decode(m_PduSessionResourceSetupRequestIes->protocolIEs.list
                              .array[i]
                              ->value.choice.UEAggregateMaximumBitRate)) {
            Logger::ngap().error("Decoded NGAP UeAggregateMaxBitRate IE error");
            return false;
          }
          m_UeAggregateMaxBitRate = std::optional<UeAggregateMaxBitRate>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP UeAggregateMaxBitRate IE error");
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
