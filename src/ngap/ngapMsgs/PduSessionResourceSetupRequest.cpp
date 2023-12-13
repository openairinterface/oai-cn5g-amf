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
#include "conversions.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestMsg::PduSessionResourceSetupRequestMsg()
    : NgapUEMessage() {
  pduSessionResourceSetupRequestIEs = nullptr;
  ranPagingPriority                 = std::nullopt;
  nasPdu                            = std::nullopt;
  uEAggregateMaxBitRate             = std::nullopt;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_SETUP_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestMsg::~PduSessionResourceSetupRequestMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::initialize() {
  pduSessionResourceSetupRequestIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .PDUSessionResourceSetupRequest);
}

//-----------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setUEAggregateMaxBitRate(
    const uint64_t& bit_rate_downlink, const uint64_t& bit_rate_uplink) {
  UEAggregateMaxBitRate tmp = {};
  tmp.set(bit_rate_downlink, bit_rate_uplink);
  uEAggregateMaxBitRate = std::optional<UEAggregateMaxBitRate>(tmp);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

  int ret = uEAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UEAggregateMaxBitRate IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UEAggregateMaxBitRate IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getUEAggregateMaxBitRate(
    uint64_t& bit_rate_downlink, uint64_t& bit_rate_uplink) const {
  if (!uEAggregateMaxBitRate.has_value()) return false;
  return uEAggregateMaxBitRate.value().get(bit_rate_downlink, bit_rate_uplink);
}
//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setRanPagingPriority(
    const uint32_t& priority) {
  RanPagingPriority tmp = {};
  tmp.set(priority);
  ranPagingPriority = std::optional<RanPagingPriority>(tmp);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RANPagingPriority;

  int ret =
      ranPagingPriority.value().encode(ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getRanPagingPriority(
    uint32_t& ran_paging_priority) {
  if (!ranPagingPriority.has_value()) return false;
  ran_paging_priority = ranPagingPriority.value().get();
  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setNasPdu(const bstring& pdu) {
  NasPdu tmp = {};
  tmp.set(pdu);
  nasPdu = std::optional<NasPdu>(tmp);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU;

  int ret = nasPdu.value().encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NGAP NAS_PDU IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getNasPdu(bstring& pdu) {
  if (!nasPdu.has_value()) return false;
  return nasPdu.value().get(pdu);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestMsg::setPduSessionResourceSetupRequestList(
    const std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  std::vector<PduSessionResourceSetupItemSUReq> itemSUReqList;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceSetupItemSUReq itemSUReq = {};
    PduSessionId pDUSessionID                  = {};
    pDUSessionID.set(list[i].pduSessionId);
    std::optional<NasPdu> m_nAS_PDU = std::nullopt;
    if (conv::check_bstring(list[i].nas_pdu)) {
      NasPdu tmp = {};
      tmp.set(list[i].nas_pdu);
      m_nAS_PDU = std::optional<NasPdu>(tmp);
    }
    S_NSSAI s_NSSAI = {};
    s_NSSAI.setSst(list[i].s_nssai.sst);

    uint32_t sd_int_value = 0;
    if (conv::sd_string_to_int(list[i].s_nssai.sd, sd_int_value)) {
      if (sd_int_value != SD_NO_VALUE) s_NSSAI.setSd(list[i].s_nssai.sd);
    }

    itemSUReq.set(
        pDUSessionID, m_nAS_PDU, s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
    itemSUReqList.push_back(itemSUReq);
  }

  pduSessionResourceSetupRequestList.set(itemSUReqList);

  Ngap_PDUSessionResourceSetupRequestIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq;

  int ret = pduSessionResourceSetupRequestList.encode(
      ie->value.choice.PDUSessionResourceSetupListSUReq);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSUReq IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestMsg::getPduSessionResourceSetupRequestList(
    std::vector<PDUSessionResourceSetupRequestItem_t>& list) {
  std::vector<PduSessionResourceSetupItemSUReq> itemSUReqList;
  pduSessionResourceSetupRequestList.get(itemSUReqList);

  for (auto& item : itemSUReqList) {
    PDUSessionResourceSetupRequestItem_t request = {};

    PduSessionId pDUSessionID     = {};
    std::optional<NasPdu> nAS_PDU = std::nullopt;
    S_NSSAI s_NSSAI               = {};
    item.get(
        pDUSessionID, nAS_PDU, s_NSSAI,
        request.pduSessionResourceSetupRequestTransfer);
    pDUSessionID.get(request.pduSessionId);
    s_NSSAI.getSst(request.s_nssai.sst);
    s_NSSAI.getSd(request.s_nssai.sd);
    if (nAS_PDU.has_value()) {
      nAS_PDU.value().get(request.nas_pdu);
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
      pduSessionResourceSetupRequestIEs =
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
  for (int i = 0; i < pduSessionResourceSetupRequestIEs->protocolIEs.list.count;
       i++) {
    switch (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
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
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
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
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_RANPagingPriority) {
          RanPagingPriority tmp = {};
          if (!tmp.decode(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.RANPagingPriority)) {
            Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
            return false;
          }
          ranPagingPriority = std::optional<RanPagingPriority>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP RANPagingPriority IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU) {
          NasPdu tmp = {};
          if (!tmp.decode(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
            return false;
          }
          nasPdu = std::optional<NasPdu>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq) {
          if (!pduSessionResourceSetupRequestList.decode(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
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
      // TODO:uEAggregateMaxBitRate
      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate) {
          UEAggregateMaxBitRate tmp = {};
          if (!tmp.decode(
                  pduSessionResourceSetupRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.UEAggregateMaximumBitRate)) {
            Logger::ngap().error("Decoded NGAP UEAggregateMaxBitRate IE error");
            return false;
          }
          uEAggregateMaxBitRate = std::optional<UEAggregateMaxBitRate>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP UEAggregateMaxBitRate IE error");
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
