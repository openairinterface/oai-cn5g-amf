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

#include "PduSessionResourceReleaseResponse.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseResponseMsg::PduSessionResourceReleaseResponseMsg()
    : NgapUEMessage() {
  pduSessionResourceReleaseResponseIEs = nullptr;
  userLocationInformation              = std::nullopt;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_RELEASE_RESPONSE);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceReleaseResponseMsg::~PduSessionResourceReleaseResponseMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::initialize() {
  pduSessionResourceReleaseResponseIEs =
      &(ngapPdu->choice.successfulOutcome->value.choice
            .PDUSessionResourceReleaseResponse);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setPduSessionResourceReleasedList(
    const std::vector<PDUSessionResourceReleasedItem_t>& list) {
  std::vector<PduSessionResourceReleasedItemRelRes> item_rel_res_list;
  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceReleasedItemRelRes item_rel_res = {};
    PduSessionId pdu_session_id                       = {};
    pdu_session_id.set(list[i].pduSessionId);

    item_rel_res.set(
        pdu_session_id, list[i].pduSessionResourceReleaseResponseTransfer);
    item_rel_res_list.push_back(item_rel_res);
  }

  pduSessionResourceReleasedList.set(item_rel_res_list);

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceReleasedListRelRes;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_PDUSessionResourceReleasedListRelRes;

  int ret = pduSessionResourceReleasedList.encode(
      ie->value.choice.PDUSessionResourceReleasedListRelRes);
  if (!ret) {
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceReleasedListRelRes IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceReleasedListRelRes IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseResponseMsg::getPduSessionResourceReleasedList(
    std::vector<PDUSessionResourceReleasedItem_t>& list) {
  std::vector<PduSessionResourceReleasedItemRelRes> item_rel_res_list;
  pduSessionResourceReleasedList.get(item_rel_res_list);

  for (auto& item : item_rel_res_list) {
    PDUSessionResourceReleasedItem_t rel = {};
    PduSessionId pdu_session_id          = {};

    item.get(pdu_session_id, rel.pduSessionResourceReleaseResponseTransfer);
    pdu_session_id.get(rel.pduSessionId);

    list.push_back(rel);
  }

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setUserLocationInfoNR(
    const NrCgi_t& cig, const Tai_t& tai) {
  UserLocationInformation tmp = {};

  UserLocationInformationNR information_nr = {};
  NrCgi nR_CGI                             = {};
  TAI tai_nr                               = {};
  nR_CGI.set(cig.mcc, cig.mnc, cig.nrCellID);
  tai_nr.setTAI(tai);
  information_nr.set(nR_CGI, tai_nr);
  tmp.setInformation(information_nr);
  userLocationInformation = std::optional<UserLocationInformation>(tmp);

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation.value().encode(
      ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::nas_mm().warn("Encode UserLocationInformation IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::nas_mm().warn("Encode UserLocationInformation IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseResponseMsg::getUserLocationInfoNR(
    NrCgi_t& cig, Tai_t& tai) {
  if (!userLocationInformation.has_value()) return false;

  UserLocationInformationNR information_nr = {};
  if (!userLocationInformation.value().getInformation(information_nr))
    return false;

  if (userLocationInformation.value().getChoiceOfUserLocationInformation() !=
      Ngap_UserLocationInformation_PR_userLocationInformationNR)
    return false;
  NrCgi nR_CGI = {};
  TAI nR_TAI   = {};
  information_nr.get(nR_CGI, nR_TAI);
  PlmnId cgi_plmnId             = {};
  NRCellIdentity nRCellIdentity = {};
  nR_CGI.get(cig);
  nR_TAI.getTAI(tai);

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseResponseMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceRelease &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceReleaseResponse) {
      pduSessionResourceReleaseResponseIEs =
          &ngapPdu->choice.successfulOutcome->value.choice
               .PDUSessionResourceReleaseResponse;
    } else {
      Logger::nas_mm().warn(
          "Check PDUSessionResourceReleaseResponse message error");
      return false;
    }
  } else {
    Logger::nas_mm().warn("MessageType error");
    return false;
  }

  for (int i = 0;
       i < pduSessionResourceReleaseResponseIEs->protocolIEs.list.count; i++) {
    switch (
        pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  pduSessionResourceReleaseResponseIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  pduSessionResourceReleaseResponseIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceReleasedListRelRes: {
        if (pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_PDUSessionResourceReleasedListRelRes) {
          if (!pduSessionResourceReleasedList.decode(
                  pduSessionResourceReleaseResponseIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionResourceReleasedListRelRes)) {
            Logger::nas_mm().warn(
                "Decoded NGAP PDUSessionResourceReleasedListRelRes IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn(
              "Decoded NGAP PDUSessionResourceReleasedListRelRes IE error");
          return false;
        }
      } break;
      default: {
        Logger::nas_mm().warn("Decoded NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}

}  // namespace ngap
