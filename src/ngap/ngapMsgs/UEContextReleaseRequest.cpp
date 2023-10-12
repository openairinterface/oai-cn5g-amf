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

#include "UEContextReleaseRequest.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}
using namespace ngap;

//------------------------------------------------------------------------------
UEContextReleaseRequestMsg::UEContextReleaseRequestMsg() : NgapUEMessage() {
  ies                                    = nullptr;
  pdu_session_resource_list_cxt_rel_req_ = std::nullopt;

  setMessageType(NgapMessageType::UE_CONTEXT_RELEASE_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
UEContextReleaseRequestMsg::~UEContextReleaseRequestMsg() {}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::initialize() {
  ies = &(
      ngapPdu->choice.initiatingMessage->value.choice.UEContextReleaseRequest);
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_AMF_UE_NGAP_ID;
  int ret           = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_RAN_UE_NGAP_ID;
  int ret           = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setPDUSessionResourceList(
    const PduSessionResourceListCxtRelReq&
        pdu_session_resource_list_cxt_rel_req) {
  pdu_session_resource_list_cxt_rel_req_ =
      std::optional<PduSessionResourceListCxtRelReq>(
          pdu_session_resource_list_cxt_rel_req);

  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseRequest_IEs__value_PR_PDUSessionResourceListCxtRelReq;
  int ret = pdu_session_resource_list_cxt_rel_req_.value().encode(
      ie->value.choice.PDUSessionResourceListCxtRelReq);
  if (!ret) {
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceListCxtRelReq IE error");
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceListCxtRelReq IE error");
}

//------------------------------------------------------------------------------
bool UEContextReleaseRequestMsg::getPDUSessionResourceList(
    PduSessionResourceListCxtRelReq& pdu_session_resource_list_cxt_rel_req) {
  if (!pdu_session_resource_list_cxt_rel_req_.has_value()) return false;
  pdu_session_resource_list_cxt_rel_req =
      pdu_session_resource_list_cxt_rel_req_.value();
  return true;
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setCauseRadioNetwork(
    const e_Ngap_CauseRadioNetwork& cause) {
  causeValue.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  causeValue.setValue(cause);
  addCauseIE();
}

//------------------------------------------------------------------------------
bool UEContextReleaseRequestMsg::getCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork& causeRadioNetwork) {
  if (causeValue.getValue() < 0) {
    Logger::ngap().error("Get Cause value from UEContextReleaseRequest Error");
    return false;
  }
  causeRadioNetwork = (e_Ngap_CauseRadioNetwork) causeValue.getValue();
  return true;
}
//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::addCauseIE() {
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_Cause;
  causeValue.encode(ie->value.choice.Cause);
  int ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode Cause IE error");
}

//------------------------------------------------------------------------------
bool UEContextReleaseRequestMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;
  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UEContextReleaseRequest &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UEContextReleaseRequest) {
      ies = &ngapPdu->choice.initiatingMessage->value.choice
                 .UEContextReleaseRequest;
    } else {
      Logger::ngap().error("Check UEContextReleaseRequest message error");
      return false;
    }
  } else {
    Logger::ngap().error(
        "TypeOfMessage of UEContextReleaseRequest is not initiatingMessage");

    return false;
  }
  for (int i = 0; i < ies->protocolIEs.list.count; i++) {
    switch (ies->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(ies->protocolIEs.list.array[i]
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
                Ngap_Criticality_reject &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_RAN_UE_NGAP_ID) {
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
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceListCxtRelReq: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_PDUSessionResourceListCxtRelReq) {
          PduSessionResourceListCxtRelReq tmp = {};
          if (!tmp.decode(ies->protocolIEs.list.array[i]
                              ->value.choice.PDUSessionResourceListCxtRelReq)) {
            Logger::ngap().error(
                "Decode NGAP PDUSessionResourceListCxtRelReq IE error");
            return false;
          }
          pdu_session_resource_list_cxt_rel_req_ =
              std::optional<PduSessionResourceListCxtRelReq>(tmp);
        } else {
          Logger::ngap().error(
              "Decode NGAP PDUSessionResourceListCxtRelReq IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_Cause) {
          if (!causeValue.decode(
                  ies->protocolIEs.list.array[i]->value.choice.Cause)) {
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
