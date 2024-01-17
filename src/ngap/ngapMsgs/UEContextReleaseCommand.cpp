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

#include "UEContextReleaseCommand.hpp"

#include "logger.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_UE-NGAP-ID-pair.h"
}

using namespace ngap;

//------------------------------------------------------------------------------
UEContextReleaseCommandMsg::UEContextReleaseCommandMsg() : NgapMessage() {
  ies         = nullptr;
  ranUeNgapId = std::nullopt;

  setMessageType(NgapMessageType::UE_CONTEXT_RELEASE_COMMAND);
  initialize();
}

UEContextReleaseCommandMsg::~UEContextReleaseCommandMsg() {}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::initialize() {
  ies = &(
      ngapPdu->choice.initiatingMessage->value.choice.UEContextReleaseCommand);
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);
  Ngap_UEContextReleaseCommand_IEs_t* ie =
      (Ngap_UEContextReleaseCommand_IEs_t*) calloc(
          1, sizeof(Ngap_UEContextReleaseCommand_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UE_NGAP_IDs;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseCommand_IEs__value_PR_UE_NGAP_IDs;
  ie->value.choice.UE_NGAP_IDs.present = Ngap_UE_NGAP_IDs_PR_aMF_UE_NGAP_ID;
  int ret =
      amfUeNgapId.encode(ie->value.choice.UE_NGAP_IDs.choice.aMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");

    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool UEContextReleaseCommandMsg::getAmfUeNgapId(unsigned long& id) {
  if (!ranUeNgapId.has_value()) {
    id = amfUeNgapId.get();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::setUeNgapIdPair(
    const unsigned long& amfId, const uint32_t& ranId) {
  amfUeNgapId.set(amfId);
  RanUeNgapId tmp = {};
  tmp.set(ranId);
  ranUeNgapId = std::optional<RanUeNgapId>(tmp);
  Ngap_UEContextReleaseCommand_IEs_t* ie =
      (Ngap_UEContextReleaseCommand_IEs_t*) calloc(
          1, sizeof(Ngap_UEContextReleaseCommand_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UE_NGAP_IDs;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseCommand_IEs__value_PR_UE_NGAP_IDs;
  ie->value.choice.UE_NGAP_IDs.present = Ngap_UE_NGAP_IDs_PR_uE_NGAP_ID_pair;
  ie->value.choice.UE_NGAP_IDs.choice.uE_NGAP_ID_pair =
      (Ngap_UE_NGAP_ID_pair_t*) calloc(1, sizeof(Ngap_UE_NGAP_ID_pair_t));
  int ret = amfUeNgapId.encode(
      ie->value.choice.UE_NGAP_IDs.choice.uE_NGAP_ID_pair->aMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ranUeNgapId.value().encode(
      ie->value.choice.UE_NGAP_IDs.choice.uE_NGAP_ID_pair->rAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool UEContextReleaseCommandMsg::getUeNgapIdPair(
    unsigned long& amfId, uint32_t& ranId) {
  if (ranUeNgapId.has_value()) {
    amfId = amfUeNgapId.get();
    ranId = ranUeNgapId.value().get();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::setCauseRadioNetwork(
    const e_Ngap_CauseRadioNetwork& cause) {
  causeValue.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  causeValue.setValue(cause);
  addCauseIE();
}

void UEContextReleaseCommandMsg::setCauseNas(const e_Ngap_CauseNas& cause) {
  causeValue.setChoiceOfCause(Ngap_Cause_PR_nas);
  causeValue.setValue(cause);
  addCauseIE();
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::addCauseIE() {
  Ngap_UEContextReleaseCommand_IEs_t* ie =
      (Ngap_UEContextReleaseCommand_IEs_t*) calloc(
          1, sizeof(Ngap_UEContextReleaseCommand_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_UEContextReleaseCommand_IEs__value_PR_Cause;
  causeValue.encode(ie->value.choice.Cause);
  int ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP Cause IE error");
}

//------------------------------------------------------------------------------
bool UEContextReleaseCommandMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UEContextRelease &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UEContextReleaseCommand) {
      ies = &ngapPdu->choice.initiatingMessage->value.choice
                 .UEContextReleaseCommand;
    } else {
      Logger::ngap().error("Check UEContextReleaseCommand message error!");

      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < ies->protocolIEs.list.count; i++) {
    switch (ies->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseCommand_IEs__value_PR_UE_NGAP_IDs) {
          if (!amfUeNgapId.decode(
                  ies->protocolIEs.list.array[i]
                      ->value.choice.UE_NGAP_IDs.choice.aMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_UE_NGAP_IDs: {
        if (!amfUeNgapId.decode(ies->protocolIEs.list.array[i]
                                    ->value.choice.UE_NGAP_IDs.choice
                                    .uE_NGAP_ID_pair->aMF_UE_NGAP_ID)) {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
        RanUeNgapId tmp = {};
        if (!tmp.decode(ies->protocolIEs.list.array[i]
                            ->value.choice.UE_NGAP_IDs.choice.uE_NGAP_ID_pair
                            ->rAN_UE_NGAP_ID)) {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
        ranUeNgapId = std::optional<RanUeNgapId>(tmp);

      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseCommand_IEs__value_PR_Cause) {
          if (!causeValue.decode(
                  ies->protocolIEs.list.array[i]->value.choice.Cause)) {
            Logger::ngap().error("Decoded NGAP Cause IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP Cause IE error");

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
