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

#include "DownlinkUEAssociatedNRPPaTransport.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
DownlinkUEAssociatedNRPPaTransportMsg::DownlinkUEAssociatedNRPPaTransportMsg()
    : NgapUEMessage() {
  setMessageType(NgapMessageType::DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
DownlinkUEAssociatedNRPPaTransportMsg::
    ~DownlinkUEAssociatedNRPPaTransportMsg() {}

//------------------------------------------------------------------------------
void DownlinkUEAssociatedNRPPaTransportMsg::initialize() {
  downlinkUEAssociatedNRPPaTransportIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .DownlinkUEAssociatedNRPPaTransport);
}

//------------------------------------------------------------------------------
void DownlinkUEAssociatedNRPPaTransportMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownlinkUEAssociatedNRPPaTransportMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool DownlinkUEAssociatedNRPPaTransportMsg::decodeFromPdu(
    Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_DownlinkUEAssociatedNRPPaTransport &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_DownlinkUEAssociatedNRPPaTransport) {
      downlinkUEAssociatedNRPPaTransportIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .DownlinkUEAssociatedNRPPaTransport;
    } else {
      Logger::ngap().error(
          "Decode NGAP DownlinkUEAssociatedNRPPaTransport error");
      return false;
    }
  } else {
    Logger::ngap().error("Decode NGAP MessageType IE error");
    return false;
  }

  for (int i = 0;
       i < downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.count; i++) {
    switch (
        downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decodefromAMF_UE_NGAP_ID(
                  downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list
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
        if (downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list
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
        if (downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RoutingID) {
          routingID =
              downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                  ->value.choice.RoutingID;
        } else {
          Logger::ngap().error("Decode NGAP RoutingID IE error");
          return false;
        }

      } break;
      case Ngap_ProtocolIE_ID_id_NRPPa_PDU: {
        if (downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU) {
          nRPPaPDU =
              downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list.array[i]
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
void DownlinkUEAssociatedNRPPaTransportMsg::setRoutingID(
    const OCTET_STRING_t& id) {
  routingID = id;
  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RoutingID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RoutingID;

  ie->value.choice.RoutingID = routingID;

  int ret = ASN_SEQUENCE_ADD(
      &downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RoutingID IE error");
}

//------------------------------------------------------------------------------
void DownlinkUEAssociatedNRPPaTransportMsg::getRoutingID(OCTET_STRING_t& id) {
  id = routingID;
}

//------------------------------------------------------------------------------
void DownlinkUEAssociatedNRPPaTransportMsg::setNRPPaPdu(
    const OCTET_STRING_t& pdu) {
  nRPPaPDU = pdu;

  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NRPPa_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU;

  ie->value.choice.NRPPa_PDU = nRPPaPDU;

  int ret = ASN_SEQUENCE_ADD(
      &downlinkUEAssociatedNRPPaTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NRPPa_PDU IE error");
}

//------------------------------------------------------------------------------
void DownlinkUEAssociatedNRPPaTransportMsg::getNRPPaPdu(OCTET_STRING_t& pdu) {
  pdu = nRPPaPDU;
}

}  // namespace ngap
