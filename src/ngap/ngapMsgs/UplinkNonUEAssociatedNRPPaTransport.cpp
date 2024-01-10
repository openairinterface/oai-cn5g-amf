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

#include "UplinkNonUEAssociatedNRPPaTransport.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UplinkNonUEAssociatedNRPPaTransportMsg::UplinkNonUEAssociatedNRPPaTransportMsg()
    : NgapMessage() {
  uplinkNonUEAssociatedNRPPaTransportIEs = nullptr;

  setMessageType(NgapMessageType::UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
UplinkNonUEAssociatedNRPPaTransportMsg::
    ~UplinkNonUEAssociatedNRPPaTransportMsg() {}

//------------------------------------------------------------------------------
void UplinkNonUEAssociatedNRPPaTransportMsg::initialize() {
  uplinkNonUEAssociatedNRPPaTransportIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .UplinkNonUEAssociatedNRPPaTransport);
}

//------------------------------------------------------------------------------
void UplinkNonUEAssociatedNRPPaTransportMsg::setRoutingId(
    const OCTET_STRING_t& id) {
  routingID = id;
  Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RoutingID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_RoutingID;

  ie->value.choice.RoutingID = routingID;

  int ret = ASN_SEQUENCE_ADD(
      &uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RoutingID IE error");
}

//------------------------------------------------------------------------------
void UplinkNonUEAssociatedNRPPaTransportMsg::getRoutingId(OCTET_STRING_t& id) {
  id = routingID;
}

//------------------------------------------------------------------------------
void UplinkNonUEAssociatedNRPPaTransportMsg::setNRPPaPdu(
    const OCTET_STRING_t& pdu) {
  nRPPaPDU = pdu;

  Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NRPPa_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU;

  ie->value.choice.NRPPa_PDU = nRPPaPDU;

  int ret = ASN_SEQUENCE_ADD(
      &uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NRPPa_PDU IE error");
}

//------------------------------------------------------------------------------
void UplinkNonUEAssociatedNRPPaTransportMsg::getNRPPaPdu(OCTET_STRING_t& pdu) {
  pdu = nRPPaPDU;
}

//------------------------------------------------------------------------------
bool UplinkNonUEAssociatedNRPPaTransportMsg::decode(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UplinkNonUEAssociatedNRPPaTransport &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UplinkNonUEAssociatedNRPPaTransport) {
      uplinkNonUEAssociatedNRPPaTransportIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .UplinkNonUEAssociatedNRPPaTransport;
    } else {
      Logger::ngap().error(
          "Decode NGAP UplinkNonUEAssociatedNRPPaTransport error");
      return false;
    }
  } else {
    Logger::ngap().error("Decode NGAP MessageType IE error");
    return false;
  }

  for (int i = 0;
       i < uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.count;
       i++) {
    switch (
        uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_RoutingID: {
        if (uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_RoutingID) {
          routingID =
              uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                  ->value.choice.RoutingID;
        } else {
          Logger::ngap().error("Decode NGAP RoutingID IE error");
          return false;
        }

      } break;
      case Ngap_ProtocolIE_ID_id_NRPPa_PDU: {
        if (uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU) {
          nRPPaPDU =
              uplinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
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

}  // namespace ngap
