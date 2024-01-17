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

#include "InitialContextSetupResponse.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupResponseMsg::InitialContextSetupResponseMsg()
    : NgapUEMessage() {
  initialContextSetupResponseIEs              = nullptr;
  pduSessionResourceSetupResponseList         = std::nullopt;
  pduSessionResourceFailedToSetupResponseList = std::nullopt;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupResponseMsg::~InitialContextSetupResponseMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::initialize() {
  initialContextSetupResponseIEs = &(ngapPdu->choice.successfulOutcome->value
                                         .choice.InitialContextSetupResponse);
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setPduSessionResourceSetupResponseList(
    const std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  PduSessionResourceSetupListCxtRes tmp = {};

  std::vector<PduSessionResourceSetupItemCxtRes> item_cxt_res_list;
  for (int i = 0; i < list.size(); i++) {
    PduSessionId pdu_session_id = {};
    pdu_session_id.set(list[i].pduSessionId);
    PduSessionResourceSetupItemCxtRes item_cxt_res = {};
    item_cxt_res.set(
        pdu_session_id, list[i].pduSessionResourceSetupResponseTransfer);
    item_cxt_res_list.push_back(item_cxt_res);
  }

  tmp.set(item_cxt_res_list);
  pduSessionResourceSetupResponseList =
      std::optional<PduSessionResourceSetupListCxtRes>(tmp);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes;

  int ret = pduSessionResourceSetupResponseList.value().encode(
      ie->value.choice.PDUSessionResourceSetupListCxtRes);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtRes IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceSetupListCxtRes IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setPduSessionResourceFailedToSetupList(
    const std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  PduSessionResourceFailedToSetupListCxtRes tmp = {};

  std::vector<PduSessionResourceFailedToSetupItemCxtRes> item_cxt_res_list;
  item_cxt_res_list.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PduSessionId pdu_session_id = {};
    pdu_session_id.set(list[i].pduSessionId);
    PduSessionResourceFailedToSetupItemCxtRes item_cxt_res = {};
    item_cxt_res.set(
        pdu_session_id, list[i].pduSessionResourceSetupUnsuccessfulTransfer);
    item_cxt_res_list.push_back(item_cxt_res);
  }

  tmp.set(item_cxt_res_list);
  pduSessionResourceFailedToSetupResponseList =
      std::optional<PduSessionResourceFailedToSetupListCxtRes>(tmp);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtRes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListCxtRes;

  int ret = pduSessionResourceFailedToSetupResponseList.value().encode(
      ie->value.choice.PDUSessionResourceFailedToSetupListCxtRes);
  if (!ret) {
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtRes IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtRes IE error");
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_InitialContextSetup &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_InitialContextSetupResponse) {
      initialContextSetupResponseIEs = &ngapPdu->choice.successfulOutcome->value
                                            .choice.InitialContextSetupResponse;
    } else {
      Logger::ngap().error("Check InitialContextSetupResponse message error");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error");
    return false;
  }
  for (int i = 0; i < initialContextSetupResponseIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_InitialContextSetupResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
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
        // TODO: to be verified
        if (/*initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_InitialContextSetupResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes: {
        if (initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes) {
          PduSessionResourceSetupListCxtRes tmp = {};
          if (!tmp.decode(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceSetupListCxtRes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error");

            return false;
          }
          pduSessionResourceSetupResponseList =
              std::optional<PduSessionResourceSetupListCxtRes>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error");

          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtRes: {
        if (initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListCxtRes) {
          PduSessionResourceFailedToSetupListCxtRes tmp = {};
          if (!tmp.decode(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice
                      .PDUSessionResourceFailedToSetupListCxtRes)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListCxtRes IE "
                "error");

            return false;
          }
          pduSessionResourceFailedToSetupResponseList =
              std::optional<PduSessionResourceFailedToSetupListCxtRes>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceFailedToSetupListCxtRes IE "
              "error");

          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decoded NGAP message PDU error");

        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::getPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList.has_value()) return false;

  std::vector<PduSessionResourceSetupItemCxtRes> item_cxt_res_list;
  pduSessionResourceSetupResponseList.value().get(item_cxt_res_list);

  for (std::vector<PduSessionResourceSetupItemCxtRes>::iterator it =
           std::begin(item_cxt_res_list);
       it < std::end(item_cxt_res_list); ++it) {
    PDUSessionResourceSetupResponseItem_t response = {};

    PduSessionId pdu_session_id = {};
    it->get(pdu_session_id, response.pduSessionResourceSetupResponseTransfer);
    pdu_session_id.get(response.pduSessionId);
    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList.has_value()) return false;

  std::vector<PduSessionResourceFailedToSetupItemCxtRes> item_cxt_res_list;
  pduSessionResourceFailedToSetupResponseList.value().get(item_cxt_res_list);

  for (std::vector<PduSessionResourceFailedToSetupItemCxtRes>::iterator it =
           std::begin(item_cxt_res_list);
       it < std::end(item_cxt_res_list); ++it) {
    PDUSessionResourceFailedToSetupItem_t failedToResponse = {};

    PduSessionId pdu_session_id = {};
    it->get(
        pdu_session_id,
        failedToResponse.pduSessionResourceSetupUnsuccessfulTransfer);
    pdu_session_id.get(failedToResponse.pduSessionId);

    list.push_back(failedToResponse);
  }

  return true;
}

}  // namespace ngap
