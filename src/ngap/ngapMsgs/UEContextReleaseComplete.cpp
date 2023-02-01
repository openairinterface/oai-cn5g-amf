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

#include "UEContextReleaseComplete.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}
using namespace ngap;

//------------------------------------------------------------------------------
UEContextReleaseCompleteMsg::UEContextReleaseCompleteMsg() : NgapUEMessage() {
  ies                             = nullptr;
  userLocationInformation         = nullptr;
  pduSessionResourceListCxtRelCpl = std::nullopt;

  setMessageType(NgapMessageType::UE_CONTEXT_RELEASE_COMPLETE);
  initialize();
}

//------------------------------------------------------------------------------
UEContextReleaseCompleteMsg::~UEContextReleaseCompleteMsg() {}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::initialize() {
  ies = &(
      ngapPdu->choice.successfulOutcome->value.choice.UEContextReleaseComplete);
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID;
  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID;
  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setUserLocationInfoNR(
    const NrCgi_t& cig, const Tai_t& tai) {
  if (!userLocationInformation)
    userLocationInformation = new UserLocationInformation();

  UserLocationInformationNR information_nr = {};
  NR_CGI nr_cgi                            = {};
  nr_cgi.setNR_CGI(cig.mcc, cig.mnc, cig.nrCellID);

  TAI tai_nr = {};
  tai_nr.setTAI(tai);
  information_nr.set(nr_cgi, tai_nr);
  userLocationInformation->setInformation(information_nr);

  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation->encode(
      &ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::getUserLocationInfoNR(
    NrCgi_t& cig, Tai_t& tai) {
  if (userLocationInformation) {
    UserLocationInformationNR information_nr = {};
    if (!userLocationInformation->getInformation(information_nr)) return;

    NR_CGI nr_cgi = {};
    TAI tai_nr    = {};
    information_nr.get(nr_cgi, tai_nr);
    PlmnId plmnId_cgi             = {};
    NRCellIdentity nRCellIdentity = {};

    nr_cgi.getNR_CGI(plmnId_cgi, nRCellIdentity);
    cig.nrCellID = nRCellIdentity.getNRCellIdentity();
    plmnId_cgi.getMcc(cig.mcc);
    plmnId_cgi.getMnc(cig.mnc);

    PlmnId plmnId = {};
    TAC tac       = {};
    tai_nr.getTAI(plmnId, tac);

    plmnId.getMcc(tai.mcc);
    plmnId.getMnc(tai.mnc);
    tai.tac = tac.get() & 0x00ffffff;
  }
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setPduSessionResourceCxtRelCplList(
    const std::vector<PDUSessionResourceCxtRelCplItem_t>& list) {
  PDUSessionResourceListCxtRelCpl m_pduSessionResourceListCxtRelCpl = {};

  std::vector<PDUSessionResourceItemCxtRelCpl> cxt_rel_cpl_list;

  for (int i = 0; i < list.size(); i++) {
    PDUSessionResourceItemCxtRelCpl item = {};
    PDUSessionID pdu_session_id          = {};
    pdu_session_id.set(list[i].pduSessionId);

    item.set(pdu_session_id);
    cxt_rel_cpl_list.push_back(item);
  }

  m_pduSessionResourceListCxtRelCpl.setPDUSessionResourceListCxtRelCpl(
      cxt_rel_cpl_list);

  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelCpl;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_PDUSessionResourceListCxtRelCpl;

  int ret =
      m_pduSessionResourceListCxtRelCpl.encode2PDUSessionResourceListCxtRelCpl(
          ie->value.choice.PDUSessionResourceListCxtRelCpl);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceReleasedListRelRes IE error");
    free_wrapper((void**) &ie);
    return;
  }

  pduSessionResourceListCxtRelCpl =
      std::optional<PDUSessionResourceListCxtRelCpl>{
          m_pduSessionResourceListCxtRelCpl};

  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceReleasedListRelRes IE error");
}

//------------------------------------------------------------------------------
bool UEContextReleaseCompleteMsg::getPduSessionResourceCxtRelCplList(
    std::vector<PDUSessionResourceCxtRelCplItem_t>& list) {
  std::vector<PDUSessionResourceItemCxtRelCpl> cxt_rel_cpl_list;

  if (pduSessionResourceListCxtRelCpl.has_value()) {
    pduSessionResourceListCxtRelCpl.value().getPDUSessionResourceListCxtRelCpl(
        cxt_rel_cpl_list);
  } else {
    return false;
  }

  for (auto& item : cxt_rel_cpl_list) {
    PDUSessionResourceCxtRelCplItem_t rel = {};
    PDUSessionID pdu_session_id           = {};
    item.get(pdu_session_id);
    pdu_session_id.get(rel.pduSessionId);
    list.push_back(rel);
  }
  return true;
}

//------------------------------------------------------------------------------
bool UEContextReleaseCompleteMsg::decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;
  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_UEContextRelease &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_UEContextReleaseComplete) {
      ies = &ngapPdu->choice.successfulOutcome->value.choice
                 .UEContextReleaseComplete;
    } else {
      Logger::ngap().error("Check UEContextReleaseComplete message error");
      return false;
    }
  } else {
    Logger::ngap().error(
        "TypeOfMessage of UEContextReleaseComplete is not SuccessfulOutcome");

    return false;
  }
  for (int i = 0; i < ies->protocolIEs.list.count; i++) {
    switch (ies->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  ies->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(ies->protocolIEs.list.array[i]
                                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
        // TODO: User Location Information
        // TODO: Information on Recommended Cells and RAN Nodes for Paging
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelCpl: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_PDUSessionResourceListCxtRelCpl) {
          PDUSessionResourceListCxtRelCpl m_pduSessionResourceListCxtRelCpl =
              {};
          if (!m_pduSessionResourceListCxtRelCpl
                   .decodefromPDUSessionResourceListCxtRelCpl(
                       ies->protocolIEs.list.array[i]
                           ->value.choice.PDUSessionResourceListCxtRelCpl)) {
            Logger::ngap().error(
                "Decode NGAP PDUSessionResourceListCxtRelCpl IE error");
            return false;
          }
          pduSessionResourceListCxtRelCpl =
              std::optional<PDUSessionResourceListCxtRelCpl>{
                  m_pduSessionResourceListCxtRelCpl};
        } else {
          Logger::ngap().error(
              "Decode NGAP PDUSessionResourceListCxtRelCpl IE error");
          return false;
        }

      } break;
        // TODO: Criticality Diagnostics

      default: {
        Logger::ngap().error("Decoded NGAP message PDU error!");
        return false;
      }
    }
  }

  return true;
}
