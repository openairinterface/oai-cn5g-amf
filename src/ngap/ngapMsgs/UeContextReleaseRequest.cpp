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

#include "UeContextReleaseRequest.hpp"

#include "logger.hpp"
#include "utils.hpp"

using namespace ngap;

//------------------------------------------------------------------------------
UeContextReleaseRequestMsg::UeContextReleaseRequestMsg() : NgapUeMessage() {
  m_UEContextReleaseRequestIes      = nullptr;
  m_PduSessionResourceListCxtRelReq = std::nullopt;

  setMessageType(NgapMessageType::UE_CONTEXT_RELEASE_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
UeContextReleaseRequestMsg::~UeContextReleaseRequestMsg() {}

//------------------------------------------------------------------------------
void UeContextReleaseRequestMsg::initialize() {
  m_UEContextReleaseRequestIes = &(
      ngapPdu->choice.initiatingMessage->value.choice.UEContextReleaseRequest);
}

//------------------------------------------------------------------------------
void UeContextReleaseRequestMsg::setAmfUeNgapId(const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_AMF_UE_NGAP_ID;
  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&m_UEContextReleaseRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UeContextReleaseRequestMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_RAN_UE_NGAP_ID;
  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&m_UEContextReleaseRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UeContextReleaseRequestMsg::setPduSessionResourceList(
    const PduSessionResourceListCxtRelReq& pduSessionResourceListCxtRelReq) {
  m_PduSessionResourceListCxtRelReq =
      std::optional<PduSessionResourceListCxtRelReq>(
          pduSessionResourceListCxtRelReq);

  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseRequest_IEs__value_PR_PDUSessionResourceListCxtRelReq;
  int ret = m_PduSessionResourceListCxtRelReq.value().encode(
      ie->value.choice.PDUSessionResourceListCxtRelReq);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceListCxtRelReq IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&m_UEContextReleaseRequestIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceListCxtRelReq IE error");
}

//------------------------------------------------------------------------------
bool UeContextReleaseRequestMsg::getPduSessionResourceList(
    PduSessionResourceListCxtRelReq& pduSessionResourceListCxtRelReq) const {
  if (!m_PduSessionResourceListCxtRelReq.has_value()) return false;
  pduSessionResourceListCxtRelReq = m_PduSessionResourceListCxtRelReq.value();
  return true;
}

//------------------------------------------------------------------------------
void UeContextReleaseRequestMsg::setCauseRadioNetwork(
    const e_Ngap_CauseRadioNetwork& cause) {
  m_CauseValue.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  m_CauseValue.set(cause);
  addCauseIe();
}

//------------------------------------------------------------------------------
bool UeContextReleaseRequestMsg::getCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork& causeRadioNetwork) const {
  if (m_CauseValue.get() < 0) {
    Logger::ngap().error("Get Cause value from UEContextReleaseRequest Error");
    return false;
  }
  causeRadioNetwork = (e_Ngap_CauseRadioNetwork) m_CauseValue.get();
  return true;
}
//------------------------------------------------------------------------------
void UeContextReleaseRequestMsg::addCauseIe() {
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_Cause;
  m_CauseValue.encode(ie->value.choice.Cause);
  int ret =
      ASN_SEQUENCE_ADD(&m_UEContextReleaseRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode Cause IE error");
}

//------------------------------------------------------------------------------
bool UeContextReleaseRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;
  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UEContextReleaseRequest &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UEContextReleaseRequest) {
      m_UEContextReleaseRequestIes = &ngapPdu->choice.initiatingMessage->value
                                          .choice.UEContextReleaseRequest;
    } else {
      Logger::ngap().error("Check UEContextReleaseRequest message error");
      return false;
    }
  } else {
    Logger::ngap().error(
        "TypeOfMessage of UEContextReleaseRequest is not initiatingMessage");

    return false;
  }
  for (int i = 0; i < m_UEContextReleaseRequestIes->protocolIEs.list.count;
       i++) {
    switch (m_UEContextReleaseRequestIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
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
        if (m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelReq: {
        if (m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_PDUSessionResourceListCxtRelReq) {
          PduSessionResourceListCxtRelReq tmp = {};
          if (!tmp.decode(
                  m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceListCxtRelReq)) {
            Logger::ngap().error(
                "Decode NGAP PDUSessionResourceListCxtRelReq IE error");
            return false;
          }
          m_PduSessionResourceListCxtRelReq =
              std::optional<PduSessionResourceListCxtRelReq>(tmp);
        } else {
          Logger::ngap().error(
              "Decode NGAP PDUSessionResourceListCxtRelReq IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_Cause) {
          if (!m_CauseValue.decode(
                  m_UEContextReleaseRequestIes->protocolIEs.list.array[i]
                      ->value.choice.Cause)) {
            Logger::ngap().error("Decode NGAP Cause IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP Cause IE error");
          return false;
        }
      } break;
    }
  }
  return true;
}
