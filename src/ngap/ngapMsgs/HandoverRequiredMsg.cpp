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

#include "HandoverRequiredMsg.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
HandoverRequiredMsg::HandoverRequiredMsg() : NgapUEMessage() {
  directForwardingPathAvailability = std::nullopt;
  handoverRequiredIEs              = nullptr;

  setMessageType(NgapMessageType::HANDOVER_REQUIRED);
  initialize();
}

//------------------------------------------------------------------------------
HandoverRequiredMsg::~HandoverRequiredMsg() {}

//------------------------------------------------------------------------------
void HandoverRequiredMsg::initialize() {
  handoverRequiredIEs =
      &ngapPdu->choice.initiatingMessage->value.choice.HandoverRequired;
}

//------------------------------------------------------------------------------
void HandoverRequiredMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_HandoverRequiredIEs_t* ie = (Ngap_HandoverRequiredIEs_t*) calloc(
      1, sizeof(Ngap_HandoverRequiredIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequiredIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequiredIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void HandoverRequiredMsg::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_HandoverRequiredIEs_t* ie = (Ngap_HandoverRequiredIEs_t*) calloc(
      1, sizeof(Ngap_HandoverRequiredIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequiredIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequiredIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
Ngap_HandoverType_t HandoverRequiredMsg::getHandoverType() {
  return handoverType;
}

//------------------------------------------------------------------------------
Ngap_Cause_PR HandoverRequiredMsg::getChoiceOfCause() {
  return cause.getChoiceOfCause();
}

//------------------------------------------------------------------------------
long HandoverRequiredMsg::getCauseValue() {
  return cause.getValue();
}

bool HandoverRequiredMsg::getTargetID(GlobalgNBId& gnbId, TAI& tai) {
  if (targetID.present == Ngap_TargetID_PR_targetRANNodeID) {
    if (targetID.choice.targetRANNodeID->globalRANNodeID.present ==
        Ngap_GlobalRANNodeID_PR_globalGNB_ID) {
      gnbId.decode(*targetID.choice.targetRANNodeID->globalRANNodeID.choice
                        .globalGNB_ID);
      tai.decode(targetID.choice.targetRANNodeID->selectedTAI);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
OCTET_STRING_t HandoverRequiredMsg::getSourceToTarget_TransparentContainer() {
  return sourceToTarget_TransparentContainer;
}

//------------------------------------------------------------------------------
bool HandoverRequiredMsg::getPDUSessionResourceList(
    PduSessionResourceListHandoverRqd& list) {
  list = pDUSessionResourceList;
  return true;
}

//------------------------------------------------------------------------------
bool HandoverRequiredMsg::getDirectForwardingPathAvailability(
    long& value) const {
  if (directForwardingPathAvailability.has_value()) return false;
  value = directForwardingPathAvailability.value();
  return true;
}

//------------------------------------------------------------------------------
bool HandoverRequiredMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_HandoverPreparation &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_HandoverRequired) {
      handoverRequiredIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.HandoverRequired;
    } else {
      Logger::ngap().error("Check HandoverRequired message error!");
      return false;
    }
  } else {
    Logger::ngap().error("HandoverRequired MessageType error!");
    return false;
  }
  for (int i = 0; i < handoverRequiredIEs->protocolIEs.list.count; i++) {
    switch (handoverRequiredIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  handoverRequiredIEs->protocolIEs.list.array[i]
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
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  handoverRequiredIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_HandoverType: {
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_HandoverType) {
          handoverType = handoverRequiredIEs->protocolIEs.list.array[i]
                             ->value.choice.HandoverType;
        } else {
          Logger::ngap().error("Decoded NGAP Handover Type error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_Cause) {
          if (!cause.decode(handoverRequiredIEs->protocolIEs.list.array[i]
                                ->value.choice.Cause)) {
            Logger::ngap().error("Decoded NGAP Cause IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP Cause IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TargetID: {
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_TargetID) {
          targetID = handoverRequiredIEs->protocolIEs.list.array[i]
                         ->value.choice.TargetID;
        } else {
          Logger::ngap().error("Decoded NGAP TargetID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_DirectForwardingPathAvailability: {
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_DirectForwardingPathAvailability) {
          directForwardingPathAvailability =
              std::optional<Ngap_DirectForwardingPathAvailability_t>(
                  handoverRequiredIEs->protocolIEs.list.array[i]
                      ->value.choice.DirectForwardingPathAvailability);
        } else {
          Logger::ngap().error(
              "Decoded NGAP DirectForwardingPathAvailability IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceListHORqd: {
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_PDUSessionResourceListHORqd) {
          if (!pDUSessionResourceList.decode(
                  handoverRequiredIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceListHORqd)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_SourceToTarget_TransparentContainer: {
        if (handoverRequiredIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequiredIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequiredIEs__value_PR_SourceToTarget_TransparentContainer) {
          amf_conv::octet_string_copy(
              sourceToTarget_TransparentContainer,
              handoverRequiredIEs->protocolIEs.list.array[i]
                  ->value.choice.SourceToTarget_TransparentContainer);
        } else {
          Logger::ngap().error(
              "Decoded NGAP SourceToTarget_TransparentContainer IE error");
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

}  // namespace ngap
