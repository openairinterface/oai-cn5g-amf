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

#include "UeContextReleaseComplete.hpp"

#include "logger.hpp"
#include "utils.hpp"

using namespace ngap;

//------------------------------------------------------------------------------
UEContextReleaseCompleteMsg::UEContextReleaseCompleteMsg() : NgapUeMessage() {
  m_UEContextReleaseCompleteIes     = nullptr;
  m_UserLocationInformation         = std::nullopt;
  m_PduSessionResourceListCxtRelCpl = std::nullopt;

  setMessageType(NgapMessageType::UE_CONTEXT_RELEASE_COMPLETE);
  initialize();
}

//------------------------------------------------------------------------------
UEContextReleaseCompleteMsg::~UEContextReleaseCompleteMsg() {}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::initialize() {
  m_UEContextReleaseCompleteIes = &(
      ngapPdu->choice.successfulOutcome->value.choice.UEContextReleaseComplete);
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setAmfUeNgapId(const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID;
  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&m_UEContextReleaseCompleteIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID;
  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&m_UEContextReleaseCompleteIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setUserLocationInfoNr(
    const NrCgi_t& cig, const Tai_t& tai) {
  UserLocationInformation m_userLocationInformation = {};

  UserLocationInformationNr userLocationInformationNR = {};
  NrCgi nrCgi                                         = {};
  nrCgi.set(cig.mcc, cig.mnc, cig.nrCellId);

  Tai taiNr = {};
  taiNr.set(tai);
  userLocationInformationNR.set(nrCgi, taiNr);

  m_userLocationInformation.set(userLocationInformationNR);

  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_UserLocationInformation;

  int ret = m_userLocationInformation.encode(
      ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  m_UserLocationInformation =
      std::optional<UserLocationInformation>{m_userLocationInformation};

  ret = ASN_SEQUENCE_ADD(&m_UEContextReleaseCompleteIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::getUserLocationInfoNr(
    NrCgi_t& cig, Tai_t& tai) const {
  if (m_UserLocationInformation.has_value()) {
    UserLocationInformationNr userLocationInformationNR = {};
    if (!m_UserLocationInformation.value().get(userLocationInformationNR))
      return;

    NrCgi nrCgi = {};
    Tai taiNr   = {};
    userLocationInformationNR.get(nrCgi, taiNr);
    PlmnId plmnIdCgi              = {};
    NrCellIdentity nrCellIdentity = {};

    nrCgi.get(plmnIdCgi, nrCellIdentity);
    cig.nrCellId = nrCellIdentity.get();
    plmnIdCgi.getMcc(cig.mcc);
    plmnIdCgi.getMnc(cig.mnc);

    PlmnId plmnId = {};
    TAC tac       = {};
    taiNr.get(plmnId, tac);

    plmnId.getMcc(tai.mcc);
    plmnId.getMnc(tai.mnc);
    tai.tac = tac.get() & 0x00ffffff;
  }
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setPduSessionResourceCxtRelCplList(
    const std::vector<PDUSessionResourceCxtRelCplItem_t>& list) {
  PduSessionResourceListCxtRelCpl m_pduSessionResourceListCxtRelCpl = {};

  std::vector<PduSessionResourceItemCxtRelCpl> cxtRelCplList;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceItemCxtRelCpl item = {};
    PduSessionId pduSessionId            = {};
    pduSessionId.set(list[i].pduSessionId);

    item.set(pduSessionId);
    cxtRelCplList.push_back(item);
  }

  m_pduSessionResourceListCxtRelCpl.set(cxtRelCplList);

  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelCpl;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_PDUSessionResourceListCxtRelCpl;

  int ret = m_pduSessionResourceListCxtRelCpl.encode(
      ie->value.choice.PDUSessionResourceListCxtRelCpl);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceReleasedListRelRes IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  m_PduSessionResourceListCxtRelCpl =
      std::optional<PduSessionResourceListCxtRelCpl>{
          m_pduSessionResourceListCxtRelCpl};

  ret = ASN_SEQUENCE_ADD(&m_UEContextReleaseCompleteIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceReleasedListRelRes IE error");
}

//------------------------------------------------------------------------------
bool UEContextReleaseCompleteMsg::getPduSessionResourceCxtRelCplList(
    std::vector<PDUSessionResourceCxtRelCplItem_t>& list) const {
  std::vector<PduSessionResourceItemCxtRelCpl> cxtRelCplList;

  if (m_PduSessionResourceListCxtRelCpl.has_value()) {
    m_PduSessionResourceListCxtRelCpl.value().get(cxtRelCplList);
  } else {
    return false;
  }

  for (auto& item : cxtRelCplList) {
    PDUSessionResourceCxtRelCplItem_t rel = {};
    PduSessionId pduSessionId             = {};
    item.get(pduSessionId);
    pduSessionId.get(rel.pduSessionId);
    list.push_back(rel);
  }
  return true;
}

//------------------------------------------------------------------------------
bool UEContextReleaseCompleteMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;
  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_UEContextRelease &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_UEContextReleaseComplete) {
      m_UEContextReleaseCompleteIes = &ngapPdu->choice.successfulOutcome->value
                                           .choice.UEContextReleaseComplete;
    } else {
      Logger::ngap().error("Check UEContextReleaseComplete message error");
      return false;
    }
  } else {
    Logger::ngap().error(
        "TypeOfMessage of UEContextReleaseComplete is not SuccessfulOutcome");
    return false;
  }

  for (int i = 0; i < m_UEContextReleaseCompleteIes->protocolIEs.list.count;
       i++) {
    switch (m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
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
        if (m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        if (m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_UserLocationInformation) {
          UserLocationInformation m_userLocationInformation = {};

          if (!m_userLocationInformation.decode(
                  m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                      ->value.choice.UserLocationInformation)) {
            Logger::ngap().error(
                "Decode NGAP UserLocationInformation IE error");
            return false;
          }
          m_UserLocationInformation =
              std::optional<UserLocationInformation>{m_userLocationInformation};
        } else {
          Logger::ngap().error("Decode NGAP UserLocationInformation IE error");
          return false;
        }
      } break;

        // TODO: Information on Recommended Cells and RAN Nodes for Paging

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelCpl: {
        if (m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_PDUSessionResourceListCxtRelCpl) {
          PduSessionResourceListCxtRelCpl m_pduSessionResourceListCxtRelCpl =
              {};
          if (!m_pduSessionResourceListCxtRelCpl.decode(
                  m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceListCxtRelCpl)) {
            Logger::ngap().error(
                "Decode NGAP PDUSessionResourceListCxtRelCpl IE error");
            return false;
          }
          m_PduSessionResourceListCxtRelCpl =
              std::optional<PduSessionResourceListCxtRelCpl>{
                  m_pduSessionResourceListCxtRelCpl};
        } else {
          Logger::ngap().error(
              "Decode NGAP PDUSessionResourceListCxtRelCpl IE error");
          return false;
        }

      } break;
        // TODO: Criticality Diagnostics

      default: {
        Logger::ngap().error(
            "Unknown IE 0x%x",
            m_UEContextReleaseCompleteIes->protocolIEs.list.array[i]->id);
        return true;
      }
    }
  }

  return true;
}
