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

#include "HandoverRequestAck.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
HandoverRequestAck::HandoverRequestAck() : NgapUEMessage() {
  PDUSessionResourceFailedToSetupList = std::nullopt;
  CriticalityDiagnostics              = nullptr;
  handoverRequestAckIEs               = nullptr;
  setMessageType(NgapMessageType::HANDOVER_REQUEST_ACKNOWLEDGE);
  initialize();
}

//------------------------------------------------------------------------------
HandoverRequestAck::~HandoverRequestAck() {}

//------------------------------------------------------------------------------
void HandoverRequestAck::initialize() {
  handoverRequestAckIEs = &(ngapPdu->choice.successfulOutcome->value.choice
                                .HandoverRequestAcknowledge);
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
OCTET_STRING_t HandoverRequestAck::getTargetToSource_TransparentContainer() {
  return TargetToSource_TransparentContainer;
}

void HandoverRequestAck::setTargetToSource_TransparentContainer(
    const OCTET_STRING_t& targetTosource) {
  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_TargetToSource_TransparentContainer;
  amf_conv::octet_string_copy(
      ie->value.choice.TargetToSource_TransparentContainer, targetTosource);
  int ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP TargetToSource_TransparentContainer IE error");
}

//------------------------------------------------------------------------------
bool HandoverRequestAck::getPDUSessionResourceAdmittedList(
    std::vector<PDUSessionResourceAdmittedItem_t>& list) {
  std::vector<PduSessionResourceItem> admittedItemList;
  pduSessionResourceAdmittedList.get(admittedItemList);

  for (auto& item : admittedItemList) {
    PDUSessionResourceAdmittedItem_t response = {};
    PduSessionId pDUSessionID                 = {};
    item.get(pDUSessionID, response.handoverRequestAcknowledgeTransfer);
    pDUSessionID.get(response.pduSessionId);
    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setPDUSessionResourceAdmittedList(
    const PduSessionResourceAdmittedList& admittedList) {
  pduSessionResourceAdmittedList = admittedList;
  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceAdmittedList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceAdmittedList;

  pduSessionResourceAdmittedList.encode(
      ie->value.choice.PDUSessionResourceAdmittedList);

  int ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP PDUSessionResourceAdmittedList IE error");
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setPDUSessionResourceFailedToSetupListHOAck(
    const PduSessionResourceFailedToSetupListHoAck& list) {
  PDUSessionResourceFailedToSetupList =
      std::optional<PduSessionResourceFailedToSetupListHoAck>(list);

  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));

  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListHOAck;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceFailedToSetupListHOAck;

  PDUSessionResourceFailedToSetupList.value().encode(
      ie->value.choice.PDUSessionResourceFailedToSetupListHOAck);

  int ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListHOAck IE error");
}

//------------------------------------------------------------------------------
void HandoverRequestAck::setPDUSessionResourceFailedToSetupListHOAck(
    std::vector<PduSessionResourceItem>& list) {
  PduSessionResourceFailedToSetupListHoAck tmp = {};
  tmp.set(list);
  PDUSessionResourceFailedToSetupList =
      std::optional<PduSessionResourceFailedToSetupListHoAck>(tmp);

  Ngap_HandoverRequestAcknowledgeIEs_t* ie =
      (Ngap_HandoverRequestAcknowledgeIEs_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));

  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListHOAck;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceFailedToSetupListHOAck;

  PDUSessionResourceFailedToSetupList.value().encode(
      ie->value.choice.PDUSessionResourceFailedToSetupListHOAck);

  int ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode NGAP PDUSessionResourceFailedToSetupListHOAck IE error");
}

//------------------------------------------------------------------------------
bool HandoverRequestAck::getPDUSessionResourceFailedToSetupListHOAck(
    std::vector<PduSessionResourceItem>& list) {
  if (!PDUSessionResourceFailedToSetupList.has_value()) return false;
  PDUSessionResourceFailedToSetupList.value().get(list);
  return true;
}

//------------------------------------------------------------------------------
bool HandoverRequestAck::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_HandoverResourceAllocation &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge) {
      handoverRequestAckIEs = &ngapPdu->choice.successfulOutcome->value.choice
                                   .HandoverRequestAcknowledge;
    } else {
      Logger::ngap().error("Check handoverRequestAck message error");
      return false;
    }
  } else {
    Logger::ngap().error("handoverRequestAck MessageType error");
    return false;
  }
  for (int i = 0; i < handoverRequestAckIEs->protocolIEs.list.count; i++) {
    switch (handoverRequestAckIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  handoverRequestAckIEs->protocolIEs.list.array[i]
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
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  handoverRequestAckIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceAdmittedList: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceAdmittedList) {
          if (!pduSessionResourceAdmittedList.decode(
                  handoverRequestAckIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceAdmittedList)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceAdmittedList IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceAdmittedList IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListHOAck: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceFailedToSetupListHOAck) {
          PduSessionResourceFailedToSetupListHoAck tmp = {};
          if (!tmp.decode(handoverRequestAckIEs->protocolIEs.list.array[i]
                              ->value.choice
                              .PDUSessionResourceFailedToSetupListHOAck)) {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceFailedToSetupListHOAck IE "
                "error");
            return false;
          }
          PDUSessionResourceFailedToSetupList =
              std::optional<PduSessionResourceFailedToSetupListHoAck>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceFailedToSetupListHOAck IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_TargetToSource_TransparentContainer) {
          amf_conv::octet_string_copy(
              TargetToSource_TransparentContainer,
              handoverRequestAckIEs->protocolIEs.list.array[i]
                  ->value.choice.TargetToSource_TransparentContainer);
        } else {
          Logger::ngap().error(
              "Decoded NGAP TargetToSource_TransparentContainer IE error");

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

}  // namespace ngap
