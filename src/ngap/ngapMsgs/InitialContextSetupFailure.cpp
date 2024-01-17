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

#include "InitialContextSetupFailure.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupFailureMsg::InitialContextSetupFailureMsg() {
  initialContextSetupFailureIEs              = nullptr;
  pduSessionResourceFailedToSetupFailureList = std::nullopt;

  setMessageType(NgapMessageType::INITIAL_CONTEXT_SETUP_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
InitialContextSetupFailureMsg::~InitialContextSetupFailureMsg() {
  if (initialContextSetupFailureIEs) free(initialContextSetupFailureIEs);
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::initialize() {
  initialContextSetupFailureIEs = &(ngapPdu->choice.unsuccessfulOutcome->value
                                        .choice.InitialContextSetupFailure);
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setPduSessionResourceFailedToSetupList(
    const std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  pduSessionResourceFailedToSetupFailureList =
      std::make_optional<PduSessionResourceFailedToSetupListCxtFail>();

  std::vector<PduSessionResourceFailedToSetupItemCxtFail> cxt_fail_list;
  cxt_fail_list.reserve(list.size());

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceFailedToSetupItemCxtFail item_cxt_fail = {};
    PduSessionId pdu_session_id                              = {};
    pdu_session_id.set(list[i].pduSessionId);

    item_cxt_fail.set(
        pdu_session_id, list[i].pduSessionResourceSetupUnsuccessfulTransfer);
    cxt_fail_list.push_back(item_cxt_fail);
  }

  pduSessionResourceFailedToSetupFailureList.value().set(cxt_fail_list);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtFail;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_PDUSessionResourceFailedToSetupListCxtFail;

  int ret = pduSessionResourceFailedToSetupFailureList.value().encode(
      ie->value.choice.PDUSessionResourceFailedToSetupListCxtFail);
  if (!ret) {
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtFail IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode PDUSessionResourceFailedToSetupListCxtFail IE error");
}

//------------------------------------------------------------------------------
bool InitialContextSetupFailureMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    if (ngapPdu->choice.unsuccessfulOutcome &&
        ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_InitialContextSetup &&
        ngapPdu->choice.unsuccessfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.unsuccessfulOutcome->value.present ==
            Ngap_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure) {
      initialContextSetupFailureIEs =
          &ngapPdu->choice.unsuccessfulOutcome->value.choice
               .InitialContextSetupFailure;
    } else {
      Logger::ngap().error("Check InitialContextSetupFailure message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < initialContextSetupFailureIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupFailureIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  initialContextSetupFailureIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  initialContextSetupFailureIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtFail: {
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_PDUSessionResourceFailedToSetupListCxtFail) {
          PduSessionResourceFailedToSetupListCxtFail tmp = {};
          if (!tmp.decode(
                  initialContextSetupFailureIEs->protocolIEs.list.array[i]
                      ->value.choice
                      .PDUSessionResourceFailedToSetupListCxtFail)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListCxtFail IE "
                "error");
            return false;
          }
          pduSessionResourceFailedToSetupFailureList =
              std::optional<PduSessionResourceFailedToSetupListCxtFail>(tmp);

        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceFailedToSetupListCxtFail IE "
              "error");
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

//------------------------------------------------------------------------------
bool InitialContextSetupFailureMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupFailureList.has_value()) return false;

  std::vector<PduSessionResourceFailedToSetupItemCxtFail> cxt_fail_list;

  pduSessionResourceFailedToSetupFailureList.value().get(cxt_fail_list);

  for (std::vector<PduSessionResourceFailedToSetupItemCxtFail>::iterator it =
           std::begin(cxt_fail_list);
       it < std::end(cxt_fail_list); ++it) {
    PDUSessionResourceFailedToSetupItem_t failedToFailure = {};

    PduSessionId pdu_session_id = {};
    it->get(
        pdu_session_id,
        failedToFailure.pduSessionResourceSetupUnsuccessfulTransfer);
    pdu_session_id.get(failedToFailure.pduSessionId);

    list.push_back(failedToFailure);
  }

  return true;
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setCause(
    const long& cause, const Ngap_Cause_PR& cause_present) {
  cause_.set(cause, cause_present);
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setCause(const Cause& cause) {
  cause_.set(cause.getValue(), cause.getChoiceOfCause());
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::getCause(Cause& cause) const {
  cause.set(cause_.getValue(), cause_.getChoiceOfCause());
}

}  // namespace ngap
