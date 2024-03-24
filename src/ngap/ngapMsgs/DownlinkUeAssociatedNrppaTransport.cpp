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

#include "DownlinkUeAssociatedNrppaTransport.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DownlinkUeAssociatedNrppaTransportMsg::DownlinkUeAssociatedNrppaTransportMsg()
    : NgapUeMessage() {
  setMessageType(NgapMessageType::DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
DownlinkUeAssociatedNrppaTransportMsg::
    ~DownlinkUeAssociatedNrppaTransportMsg() {}

//------------------------------------------------------------------------------
void DownlinkUeAssociatedNrppaTransportMsg::initialize() {
  m_DownlinkUeAssociatedNrppaTransportIes =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .DownlinkUEAssociatedNRPPaTransport);
}

//------------------------------------------------------------------------------
void DownlinkUeAssociatedNrppaTransportMsg::setAmfUeNgapId(
    const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownlinkUeAssociatedNrppaTransportMsg::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
bool DownlinkUeAssociatedNrppaTransportMsg::decode(
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
      m_DownlinkUeAssociatedNrppaTransportIes =
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
       i < m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.count;
       i++) {
    switch (m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                ->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list
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
        if (m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list
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
        if (m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RoutingID) {
          amf_conv::octet_string_2_bstring(
              m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list
                  .array[i]
                  ->value.choice.RoutingID,
              m_RoutingId);
        } else {
          Logger::ngap().error("Decode NGAP RoutingID IE error");
          return false;
        }

      } break;
      case Ngap_ProtocolIE_ID_id_NRPPa_PDU: {
        if (m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU) {
          amf_conv::octet_string_2_bstring(
              m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list
                  .array[i]
                  ->value.choice.NRPPa_PDU,
              m_NrppaPdu);
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
void DownlinkUeAssociatedNrppaTransportMsg::setRoutingId(const bstring& pdu) {
  m_RoutingId = bstrcpy(pdu);
  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RoutingID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_RoutingID;

  amf_conv::bstring_2_octet_string(m_RoutingId, ie->value.choice.RoutingID);

  int ret = ASN_SEQUENCE_ADD(
      &m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RoutingID IE error");
}

//------------------------------------------------------------------------------
void DownlinkUeAssociatedNrppaTransportMsg::getRoutingId(bstring& pdu) const {
  pdu = bstrcpy(m_RoutingId);
}

//------------------------------------------------------------------------------
void DownlinkUeAssociatedNrppaTransportMsg::setNrppaPdu(const bstring& pdu) {
  m_NrppaPdu = bstrcpy(pdu);

  Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t* ie =
      (Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkUEAssociatedNRPPaTransportIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NRPPa_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkUEAssociatedNRPPaTransportIEs__value_PR_NRPPa_PDU;

  amf_conv::bstring_2_octet_string(m_NrppaPdu, ie->value.choice.NRPPa_PDU);

  int ret = ASN_SEQUENCE_ADD(
      &m_DownlinkUeAssociatedNrppaTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NRPPa_PDU IE error");
}

//------------------------------------------------------------------------------
void DownlinkUeAssociatedNrppaTransportMsg::getNrppaPdu(bstring& pdu) const {
  pdu = m_NrppaPdu;
}

}  // namespace ngap
