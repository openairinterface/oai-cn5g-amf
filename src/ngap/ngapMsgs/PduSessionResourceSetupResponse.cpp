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

#include "PduSessionResourceSetupResponse.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseMsg::PduSessionResourceSetupResponseMsg()
    : NgapUeMessage() {
  m_PduSessionResourceSetupResponseIes          = nullptr;
  m_PduSessionResourceSetupResponseList         = std::nullopt;
  m_PduSessionResourceFailedToSetupResponseList = std::nullopt;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_SETUP_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseMsg::~PduSessionResourceSetupResponseMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::initialize() {
  m_PduSessionResourceSetupResponseIes =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .PDUSessionResourceSetupResponse);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setAmfUeNgapId(
    const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupResponseIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupResponseIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceSetupResponseList(
    const std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  PduSessionResourceSetupListSURes tmp = {};

  std::vector<PduSessionResourceSetupItemSURes> itemSUResList;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceSetupItemSURes item = {};
    PduSessionId pduSessionId             = {};
    pduSessionId.set(list[i].pduSessionId);

    item.set(pduSessionId, list[i].pduSessionResourceSetupResponseTransfer);
    itemSUResList.push_back(item);
  }

  tmp.set(itemSUResList);
  m_PduSessionResourceSetupResponseList =
      std::optional<PduSessionResourceSetupListSURes>(tmp);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes;

  int ret = m_PduSessionResourceSetupResponseList.value().encode(
      ie->value.choice.PDUSessionResourceSetupListSURes);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSURes IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupResponseIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSURes IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t>& list) const {
  if (!m_PduSessionResourceSetupResponseList.has_value()) return false;

  std::vector<PduSessionResourceSetupItemSURes> itemSUResList;
  m_PduSessionResourceSetupResponseList.value().get(itemSUResList);

  for (auto& item : itemSUResList) {
    PDUSessionResourceSetupResponseItem_t response = {};

    PduSessionId pduSessionId = {};
    item.get(pduSessionId, response.pduSessionResourceSetupResponseTransfer);
    pduSessionId.get(response.pduSessionId);

    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceFailedToSetupList(
    const std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  PduSessionResourceFailedToSetupListSURes tmp = {};

  std::vector<PduSessionResourceFailedToSetupItemSURes> itemSUResList;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceFailedToSetupItemSURes item = {};
    PduSessionId pduSessionId                     = {};
    pduSessionId.set(list[i].pduSessionId);

    item.set(pduSessionId, list[i].pduSessionResourceSetupUnsuccessfulTransfer);
    itemSUResList.push_back(item);
  }

  tmp.set(itemSUResList);
  m_PduSessionResourceFailedToSetupResponseList =
      std::optional<PduSessionResourceFailedToSetupListSURes>(tmp);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes;

  int ret = m_PduSessionResourceFailedToSetupResponseList->encode(
      ie->value.choice.PDUSessionResourceFailedToSetupListSURes);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListSURes IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupResponseIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListSURes IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) const {
  if (!m_PduSessionResourceFailedToSetupResponseList.has_value()) return false;

  std::vector<PduSessionResourceFailedToSetupItemSURes> itemSUResList;
  m_PduSessionResourceFailedToSetupResponseList.value().get(itemSUResList);

  for (auto& item : itemSUResList) {
    PDUSessionResourceFailedToSetupItem_t failedToSetupItem = {};
    PduSessionId pduSessionId                               = {};

    item.get(
        pduSessionId,
        failedToSetupItem.pduSessionResourceSetupUnsuccessfulTransfer);
    pduSessionId.get(failedToSetupItem.pduSessionId);

    list.push_back(failedToSetupItem);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceSetup &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceSetupResponse) {
      m_PduSessionResourceSetupResponseIes =
          &ngapPdu->choice.successfulOutcome->value.choice
               .PDUSessionResourceSetupResponse;
    } else {
      Logger::ngap().error(
          "Check PDUSessionResourceSetupResponse message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0;
       i < m_PduSessionResourceSetupResponseIes->protocolIEs.list.count; i++) {
    switch (
        m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_PduSessionResourceSetupResponseIes->protocolIEs.list
                      .array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error!");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error!");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_PduSessionResourceSetupResponseIes->protocolIEs.list
                      .array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error!");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error!");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes: {
        if (m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes) {
          PduSessionResourceSetupListSURes tmp = {};
          if (!tmp.decode(
                  m_PduSessionResourceSetupResponseIes->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionResourceSetupListSURes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListSURes IE error!");
            return false;
          }
          m_PduSessionResourceSetupResponseList =
              std::optional<PduSessionResourceSetupListSURes>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListSURes IE error!");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes: {
        if (m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_PduSessionResourceSetupResponseIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes) {
          PduSessionResourceFailedToSetupListSURes tmp = {};
          if (!tmp.decode(m_PduSessionResourceSetupResponseIes->protocolIEs.list
                              .array[i]
                              ->value.choice
                              .PDUSessionResourceFailedToSetupListSURes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListSURes IE "
                "error!");
            return false;
          }
          m_PduSessionResourceFailedToSetupResponseList =
              std::optional<PduSessionResourceFailedToSetupListSURes>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceFailedToSetupListSURes IE "
              "error!");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decoded NGAP message PDU error!");
        return false;
      }
    }
  }

  return true;
}

}  // namespace ngap
