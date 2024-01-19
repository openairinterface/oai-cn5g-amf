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

#include "DownlinkNonUEAssociatedNRPPaTransport.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DownlinkNonUEAssociatedNRPPaTransportMsg::
    DownlinkNonUEAssociatedNRPPaTransportMsg()
    : NgapMessage() {
  setMessageType(NgapMessageType::DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
DownlinkNonUEAssociatedNRPPaTransportMsg::
    ~DownlinkNonUEAssociatedNRPPaTransportMsg() {}

//------------------------------------------------------------------------------
void DownlinkNonUEAssociatedNRPPaTransportMsg::initialize() {
  downlinkNonUEAssociatedNRPPaTransportIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .DownlinkNonUEAssociatedNRPPaTransport);
}

//------------------------------------------------------------------------------
bool DownlinkNonUEAssociatedNRPPaTransportMsg::decode(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_DownlinkNonUEAssociatedNRPPaTransport &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_DownlinkNonUEAssociatedNRPPaTransport) {
      downlinkNonUEAssociatedNRPPaTransportIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .DownlinkNonUEAssociatedNRPPaTransport;
    } else {
      Logger::ngap().error(
          "Decode NGAP DownlinkNonUEAssociatedNRPPaTransport error");
      return false;
    }
  } else {
    Logger::ngap().error("Decode NGAP MessageType IE error");
    return false;
  }

  for (int i = 0;
       i < downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.count;
       i++) {
    switch (downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                ->id) {
      case Ngap_ProtocolIE_ID_id_RoutingID: {
        if (downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs__value_PR_RoutingID) {
          amf_conv::octet_string_2_bstring(
              downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list
                  .array[i]
                  ->value.choice.RoutingID,
              routingID);
        } else {
          Logger::ngap().error("Decode NGAP RoutingID IE error");
          return false;
        }

      } break;
      case Ngap_ProtocolIE_ID_id_NRPPa_PDU: {
        if (downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU) {
          amf_conv::octet_string_2_bstring(
              downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list
                  .array[i]
                  ->value.choice.NRPPa_PDU,
              nRPPaPDU);
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
void DownlinkNonUEAssociatedNRPPaTransportMsg::setRoutingId(
    const bstring& pdu) {
  routingID = bstrcpy(pdu);
  Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RoutingID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs__value_PR_RoutingID;

  amf_conv::bstring_2_octet_string(routingID, ie->value.choice.RoutingID);

  int ret = ASN_SEQUENCE_ADD(
      &downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RoutingID IE error");
}

//------------------------------------------------------------------------------
void DownlinkNonUEAssociatedNRPPaTransportMsg::getRoutingId(bstring& pdu) {
  pdu = bstrcpy(routingID);
}

//------------------------------------------------------------------------------
void DownlinkNonUEAssociatedNRPPaTransportMsg::setNRPPaPdu(const bstring& pdu) {
  nRPPaPDU = bstrcpy(pdu);
  Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NRPPa_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkNonUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU;

  amf_conv::bstring_2_octet_string(nRPPaPDU, ie->value.choice.NRPPa_PDU);

  int ret = ASN_SEQUENCE_ADD(
      &downlinkNonUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NRPPa_PDU IE error");
}

//------------------------------------------------------------------------------
void DownlinkNonUEAssociatedNRPPaTransportMsg::getNRPPaPdu(bstring& pdu) {
  pdu = nRPPaPDU;
}

}  // namespace ngap
