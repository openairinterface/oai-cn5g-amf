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
    : NgapUEMessage() {
  pduSessionResourceSetupResponseIEs          = nullptr;
  pduSessionResourceSetupResponseList         = std::nullopt;
  pduSessionResourceFailedToSetupResponseList = std::nullopt;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_SETUP_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseMsg::~PduSessionResourceSetupResponseMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::initialize() {
  pduSessionResourceSetupResponseIEs =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .PDUSessionResourceSetupResponse);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceSetupResponseList(
    const std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  PduSessionResourceSetupListSURes tmp = {};

  std::vector<PduSessionResourceSetupItemSURes> item_su_res_list;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceSetupItemSURes item = {};
    PduSessionId pdu_session_id           = {};
    pdu_session_id.set(list[i].pduSessionId);

    item.set(pdu_session_id, list[i].pduSessionResourceSetupResponseTransfer);
    item_su_res_list.push_back(item);
  }

  tmp.set(item_su_res_list);
  pduSessionResourceSetupResponseList =
      std::optional<PduSessionResourceSetupListSURes>(tmp);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes;

  int ret = pduSessionResourceSetupResponseList.value().encode(
      ie->value.choice.PDUSessionResourceSetupListSURes);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSURes IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceSetupListSURes IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList.has_value()) return false;

  std::vector<PduSessionResourceSetupItemSURes> item_su_res_list;
  pduSessionResourceSetupResponseList.value().get(item_su_res_list);

  for (auto& item : item_su_res_list) {
    PDUSessionResourceSetupResponseItem_t response = {};

    PduSessionId pdu_session_id = {};
    item.get(pdu_session_id, response.pduSessionResourceSetupResponseTransfer);
    pdu_session_id.get(response.pduSessionId);

    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceFailedToSetupList(
    const std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  PduSessionResourceFailedToSetupListSURes tmp = {};

  std::vector<PduSessionResourceFailedToSetupItemSURes> item_su_res_list;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceFailedToSetupItemSURes item = {};
    PduSessionId pdu_session_id                   = {};
    pdu_session_id.set(list[i].pduSessionId);

    item.set(
        pdu_session_id, list[i].pduSessionResourceSetupUnsuccessfulTransfer);
    item_su_res_list.push_back(item);
  }

  tmp.set(item_su_res_list);
  pduSessionResourceFailedToSetupResponseList =
      std::optional<PduSessionResourceFailedToSetupListSURes>(tmp);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes;

  int ret = pduSessionResourceFailedToSetupResponseList->encode(
      ie->value.choice.PDUSessionResourceFailedToSetupListSURes);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListSURes IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListSURes IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList.has_value()) return false;

  std::vector<PduSessionResourceFailedToSetupItemSURes> item_su_res_list;
  pduSessionResourceFailedToSetupResponseList.value().get(item_su_res_list);

  for (auto& item : item_su_res_list) {
    PDUSessionResourceFailedToSetupItem_t failed_to_response_item = {};
    PduSessionId pdu_session_id                                   = {};

    item.get(
        pdu_session_id,
        failed_to_response_item.pduSessionResourceSetupUnsuccessfulTransfer);
    pdu_session_id.get(failed_to_response_item.pduSessionId);

    list.push_back(failed_to_response_item);
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
      pduSessionResourceSetupResponseIEs =
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
       i < pduSessionResourceSetupResponseIEs->protocolIEs.list.count; i++) {
    switch (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
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
        if (/*pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
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
        if (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes) {
          PduSessionResourceSetupListSURes tmp = {};
          if (!tmp.decode(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceSetupListSURes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListSURes IE error!");
            return false;
          }
          pduSessionResourceSetupResponseList =
              std::optional<PduSessionResourceSetupListSURes>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListSURes IE error!");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes: {
        if (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes) {
          PduSessionResourceFailedToSetupListSURes tmp = {};
          if (!tmp.decode(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice
                      .PDUSessionResourceFailedToSetupListSURes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListSURes IE "
                "error!");
            return false;
          }
          pduSessionResourceFailedToSetupResponseList =
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
