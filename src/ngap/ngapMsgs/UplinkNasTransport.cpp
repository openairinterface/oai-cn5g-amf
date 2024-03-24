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

#include "UplinkNasTransport.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UplinkNasTransportMsg::UplinkNasTransportMsg() : NgapUeMessage() {
  m_UplinkNasTransportIes = nullptr;

  setMessageType(NgapMessageType::UPLINK_NAS_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
UplinkNasTransportMsg::~UplinkNasTransportMsg() {}

//------------------------------------------------------------------------------
void UplinkNasTransportMsg::initialize() {
  m_UplinkNasTransportIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.UplinkNASTransport);
}

//------------------------------------------------------------------------------
void UplinkNasTransportMsg::setAmfUeNgapId(const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_UplinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UplinkNasTransportMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_UplinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UplinkNasTransportMsg::setNasPdu(const bstring& pdu) {
  m_NasPdu.set(pdu);

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_NAS_PDU;

  int ret = m_NasPdu.encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NGAP NAS_PDU IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_UplinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool UplinkNasTransportMsg::getNasPdu(bstring& pdu) const {
  return m_NasPdu.get(pdu);
}

//------------------------------------------------------------------------------
void UplinkNasTransportMsg::setUserLocationInfoNr(
    const NrCgi_t& cig, const Tai_t& tai) {
  UserLocationInformationNr userLocationInformationNR;
  NrCgi nrCgi = {};
  Tai taiNr   = {};

  nrCgi.set(cig);
  taiNr.set(tai);
  userLocationInformationNR.set(nrCgi, taiNr);
  m_UserLocationInformation.set(userLocationInformationNR);

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UplinkNASTransport_IEs__value_PR_UserLocationInformation;

  int ret = m_UserLocationInformation.encode(
      ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_UplinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
bool UplinkNasTransportMsg::getUserLocationInfoNr(
    NrCgi_t& cig, Tai_t& tai) const {
  UserLocationInformationNr userLocationInformationNR = {};
  if (!m_UserLocationInformation.get(userLocationInformationNR)) return false;

  if (m_UserLocationInformation.getChoiceOfUserLocationInformation() !=
      Ngap_UserLocationInformation_PR_userLocationInformationNR)
    return false;

  NrCgi nrCgi = {};
  Tai taiNr   = {};
  userLocationInformationNR.get(nrCgi, taiNr);
  nrCgi.get(cig);
  taiNr.get(tai);

  return true;
}

//------------------------------------------------------------------------------
bool UplinkNasTransportMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UplinkNASTransport &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UplinkNASTransport) {
      m_UplinkNasTransportIes =
          &ngapPdu->choice.initiatingMessage->value.choice.UplinkNASTransport;
    } else {
      Logger::ngap().error("Check UplinkNASTransport message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < m_UplinkNasTransportIes->protocolIEs.list.count; i++) {
    switch (m_UplinkNasTransportIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_UplinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_UplinkNasTransportIes->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_UplinkNasTransportIes->protocolIEs.list.array[i]
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
        if (m_UplinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_UplinkNasTransportIes->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_UplinkNasTransportIes->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (m_UplinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_UplinkNasTransportIes->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_NAS_PDU) {
          if (!m_NasPdu.decode(
                  m_UplinkNasTransportIes->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        if (m_UplinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_UplinkNasTransportIes->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_UserLocationInformation) {
          if (!m_UserLocationInformation.decode(
                  m_UplinkNasTransportIes->protocolIEs.list.array[i]
                      ->value.choice.UserLocationInformation)) {
            Logger::ngap().error(
                "Decoded NGAP UserLocationInformation IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP UserLocationInformation IE error");
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
