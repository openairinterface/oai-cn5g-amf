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

#include "UplinkUEAssociatedNRPPaTransport.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UplinkUEAssociatedNRPPaTransportMsg::UplinkUEAssociatedNRPPaTransportMsg()
    : NgapUEMessage() {
  uplinkUEAssociatedNRPPaTransportIEs = nullptr;

  setMessageType(NgapMessageType::UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
UplinkUEAssociatedNRPPaTransportMsg::~UplinkUEAssociatedNRPPaTransportMsg() {}

//------------------------------------------------------------------------------
void UplinkUEAssociatedNRPPaTransportMsg::initialize() {
  uplinkUEAssociatedNRPPaTransportIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .UplinkUEAssociatedNRPPaTransport);
}

//------------------------------------------------------------------------------
void UplinkUEAssociatedNRPPaTransportMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_UplinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UplinkUEAssociatedNRPPaTransportMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_UplinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool UplinkUEAssociatedNRPPaTransportMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UplinkUEAssociatedNRPPaTransport &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UplinkUEAssociatedNRPPaTransport) {
      uplinkUEAssociatedNRPPaTransportIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .UplinkUEAssociatedNRPPaTransport;
    } else {
      Logger::ngap().error(
          "Decode NGAP UplinkUEAssociatedNRPPaTransport error");
      return false;
    }
  } else {
    Logger::ngap().error("Decode NGAP MessageType IE error");
    return false;
  }

  for (int i = 0;
       i < uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.count; i++) {
    switch (
        uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list
                      .array[i]
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
        if (uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RoutingID: {
        if (uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_RoutingID) {
          routingID =
              uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                  ->value.choice.RoutingID;
        } else {
          Logger::ngap().error("Decode NGAP RoutingID IE error");
          return false;
        }

      } break;
      case Ngap_ProtocolIE_ID_id_NRPPa_PDU: {
        if (uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU) {
          nRPPaPDU =
              uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                  ->value.choice.NRPPa_PDU;
        } else {
          Logger::ngap().error("Decode NGAP NRPPa PDU IE error");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decode NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
void UplinkUEAssociatedNRPPaTransportMsg::setRoutingId(
    const OCTET_STRING_t& id) {
  routingID = id;
  Ngap_UplinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RoutingID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_RoutingID;

  ie->value.choice.RoutingID = routingID;

  int ret = ASN_SEQUENCE_ADD(
      &uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RoutingID IE error");
}

//------------------------------------------------------------------------------
void UplinkUEAssociatedNRPPaTransportMsg::getRoutingId(
    OCTET_STRING_t& id) const {
  id = routingID;
}

//------------------------------------------------------------------------------
OCTET_STRING_t UplinkUEAssociatedNRPPaTransportMsg::getRoutingId() const {
  return routingID;
}

//------------------------------------------------------------------------------
void UplinkUEAssociatedNRPPaTransportMsg::setNRPPaPdu(
    const OCTET_STRING_t& pdu) {
  nRPPaPDU = pdu;

  Ngap_UplinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NRPPa_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU;

  ie->value.choice.NRPPa_PDU = nRPPaPDU;

  int ret = ASN_SEQUENCE_ADD(
      &uplinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NRPPa_PDU IE error");
}

//------------------------------------------------------------------------------
void UplinkUEAssociatedNRPPaTransportMsg::getNRPPaPdu(
    OCTET_STRING_t& pdu) const {
  pdu = nRPPaPDU;
}

//------------------------------------------------------------------------------
OCTET_STRING_t UplinkUEAssociatedNRPPaTransportMsg::getNRPPaPdu() const {
  return nRPPaPDU;
}

}  // namespace ngap
