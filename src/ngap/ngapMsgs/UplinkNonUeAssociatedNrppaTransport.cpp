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

#include "UplinkNonUeAssociatedNrppaTransport.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UplinkNonUeAssociatedNrppaTransportMsg::UplinkNonUeAssociatedNrppaTransportMsg()
    : NgapMessage() {
  m_UplinkNonUeAssociatedNrppaTransportIes = nullptr;

  setMessageType(NgapMessageType::UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
UplinkNonUeAssociatedNrppaTransportMsg::
    ~UplinkNonUeAssociatedNrppaTransportMsg() {}

//------------------------------------------------------------------------------
void UplinkNonUeAssociatedNrppaTransportMsg::initialize() {
  m_UplinkNonUeAssociatedNrppaTransportIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .UplinkNonUEAssociatedNRPPaTransport);
}

//------------------------------------------------------------------------------
void UplinkNonUeAssociatedNrppaTransportMsg::setRoutingId(
    const OCTET_STRING_t& id) {
  m_RoutingId = id;
  Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RoutingID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_RoutingID;

  ie->value.choice.RoutingID = m_RoutingId;

  int ret = ASN_SEQUENCE_ADD(
      &m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RoutingID IE error");
}

//------------------------------------------------------------------------------
void UplinkNonUeAssociatedNrppaTransportMsg::getRoutingId(
    OCTET_STRING_t& id) const {
  id = m_RoutingId;
}

//------------------------------------------------------------------------------
OCTET_STRING_t UplinkNonUeAssociatedNrppaTransportMsg::getRoutingId() const {
  return m_RoutingId;
}

//------------------------------------------------------------------------------
void UplinkNonUeAssociatedNrppaTransportMsg::setNrppaPdu(
    const OCTET_STRING_t& pdu) {
  m_NrppaPdu = pdu;

  Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_UplinkNonUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NRPPa_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU;

  ie->value.choice.NRPPa_PDU = m_NrppaPdu;

  int ret = ASN_SEQUENCE_ADD(
      &m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NRPPa_PDU IE error");
}

//------------------------------------------------------------------------------
void UplinkNonUeAssociatedNrppaTransportMsg::getNrppaPdu(
    OCTET_STRING_t& pdu) const {
  pdu = m_NrppaPdu;
}

//------------------------------------------------------------------------------
OCTET_STRING_t UplinkNonUeAssociatedNrppaTransportMsg::getNrppaPdu() const {
  return m_NrppaPdu;
}

//------------------------------------------------------------------------------
bool UplinkNonUeAssociatedNrppaTransportMsg::decode(
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
      m_UplinkNonUeAssociatedNrppaTransportIes =
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
       i < m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list.count;
       i++) {
    switch (m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                ->id) {
      case Ngap_ProtocolIE_ID_id_RoutingID: {
        if (m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_RoutingID) {
          m_RoutingId =
              m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list
                  .array[i]
                  ->value.choice.RoutingID;
        } else {
          Logger::ngap().error("Decode NGAP RoutingID IE error");
          return false;
        }

      } break;
      case Ngap_ProtocolIE_ID_id_NRPPa_PDU: {
        if (m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UplinkNonUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU) {
          m_NrppaPdu =
              m_UplinkNonUeAssociatedNrppaTransportIes->protocolIEs.list
                  .array[i]
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
