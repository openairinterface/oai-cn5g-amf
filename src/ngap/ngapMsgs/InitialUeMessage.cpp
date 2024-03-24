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

#include "InitialUeMessage.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
InitialUeMessageMsg::InitialUeMessageMsg() : NgapMessage() {
  m_InitialUEMessageIes = nullptr;
  m_UeContextRequest    = std::nullopt;
  m_FiveGSTmsi          = std::nullopt;
  m_AmfSetId            = std::nullopt;
  m_AllowedNssai        = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::INITIAL_UE_MESSAGE);
  initialize();
}

//------------------------------------------------------------------------------
InitialUeMessageMsg::~InitialUeMessageMsg() {}

//------------------------------------------------------------------------------
void InitialUeMessageMsg::initialize() {
  m_InitialUEMessageIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.InitialUEMessage);
}

//------------------------------------------------------------------------------
void InitialUeMessageMsg::setRanUeNgapId(const uint32_t& value) {
  m_RanUeNgapId.set(value);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_InitialUEMessageIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialUeMessageMsg::setNasPdu(const bstring& pdu) {
  m_NasPdu.set(pdu);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_NAS_PDU;

  int ret = m_NasPdu.encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS PDU IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_InitialUEMessageIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS PDU IE error");
}

//------------------------------------------------------------------------------
void InitialUeMessageMsg::setUserLocationInfoNr(
    const struct NrCgi_s& cig, const struct Tai_s& tai) {
  UserLocationInformationNr information_nr;
  NrCgi nR_CGI = {};
  nR_CGI.set(cig.mcc, cig.mnc, cig.nrCellId);

  Tai tai_nr = {};
  tai_nr.set(tai);
  information_nr.set(nR_CGI, tai_nr);
  m_UserLocationInformation.set(information_nr);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation;

  int ret = m_UserLocationInformation.encode(
      ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode UserLocationInformation IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_InitialUEMessageIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
void InitialUeMessageMsg::setRrcEstablishmentCause(
    const e_Ngap_RRCEstablishmentCause& cause) {
  m_RrcEstablishmentCause.set(cause);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RRCEstablishmentCause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause;

  int ret =
      m_RrcEstablishmentCause.encode(ie->value.choice.RRCEstablishmentCause);
  if (!ret) {
    Logger::ngap().error("Encode RRCEstablishmentCause IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_InitialUEMessageIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RRCEstablishmentCause IE error");
}

//------------------------------------------------------------------------------
void InitialUeMessageMsg::setUeContextRequest(
    const e_Ngap_UEContextRequest& UeCtxReq) {
  m_UeContextRequest = std::make_optional<UeContextRequest>(UeCtxReq);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UEContextRequest;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest;

  int ret =
      m_UeContextRequest.value().encode(ie->value.choice.UEContextRequest);
  if (!ret) {
    Logger::ngap().error("Encode UEContextRequest IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_InitialUEMessageIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UEContextRequest IE error");
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::getAmfSetId(uint16_t& amfSetId) const {
  if (!m_AmfSetId.has_value()) return false;
  m_AmfSetId.value().get(amfSetId);
  return true;
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::getAmfSetId(std::string& amfSetId) const {
  if (!m_AmfSetId.has_value()) return false;
  m_AmfSetId.value().get(amfSetId);
  return true;
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::setAmfSetId(uint16_t amfSetId) {
  AmfSetId tmp = {};
  if (!tmp.set(amfSetId)) return false;
  m_AmfSetId = std::optional<AmfSetId>(tmp);
  return true;
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_InitialUEMessage &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_InitialUEMessage) {
      m_InitialUEMessageIes =
          &ngapPdu->choice.initiatingMessage->value.choice.InitialUEMessage;
    } else {
      Logger::ngap().error("Check InitialUEMessage message error");
      return false;
    }
  } else {
    Logger::ngap().error("Check MessageType error");
    return false;
  }
  for (int i = 0; i < m_InitialUEMessageIes->protocolIEs.list.count; i++) {
    switch (m_InitialUEMessageIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (m_InitialUEMessageIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_InitialUEMessageIes->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!m_RanUeNgapId.decode(
                  m_InitialUEMessageIes->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
          Logger::ngap().debug("Received RanUeNgapId %d ", m_RanUeNgapId.get());

        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (m_InitialUEMessageIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_InitialUEMessageIes->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_NAS_PDU) {
          if (!m_NasPdu.decode(m_InitialUEMessageIes->protocolIEs.list.array[i]
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
        // TODO: to be verified
        if (m_InitialUEMessageIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_InitialUEMessageIes->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation) {
          if (!m_UserLocationInformation.decode(
                  m_InitialUEMessageIes->protocolIEs.list.array[i]
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
      case Ngap_ProtocolIE_ID_id_RRCEstablishmentCause: {
        if (m_InitialUEMessageIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_InitialUEMessageIes->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause) {
          if (!m_RrcEstablishmentCause.decode(
                  m_InitialUEMessageIes->protocolIEs.list.array[i]
                      ->value.choice.RRCEstablishmentCause)) {
            Logger::ngap().error("Decoded NGAP RRCEstablishmentCause IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RRCEstablishmentCause IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UEContextRequest: {
        if (m_InitialUEMessageIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_InitialUEMessageIes->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest) {
          UeContextRequest tmp = {};
          if (!tmp.decode(m_InitialUEMessageIes->protocolIEs.list.array[i]
                              ->value.choice.UEContextRequest)) {
            Logger::ngap().error("Decoded NGAP UEContextRequest IE error");
            return false;
          }
          m_UeContextRequest = std::optional<UeContextRequest>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP UEContextRequest IE error");
          return false;
        }

      } break;

      case Ngap_ProtocolIE_ID_id_FiveG_S_TMSI: {
        if (m_InitialUEMessageIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_InitialUEMessageIes->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_FiveG_S_TMSI) {
          FiveGSTmsi tmp = {};
          if (!tmp.decode(m_InitialUEMessageIes->protocolIEs.list.array[i]
                              ->value.choice.FiveG_S_TMSI)) {
            Logger::ngap().error("Decoded NGAP FiveG_S_TMSI IE error");
            return false;
          }
          m_FiveGSTmsi = std::optional<FiveGSTmsi>(tmp);
        }
      } break;
      case Ngap_ProtocolIE_ID_id_AMFSetID: {
        if (m_InitialUEMessageIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_InitialUEMessageIes->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_AMFSetID) {
          AmfSetId tmp = {};
          if (!tmp.decode(m_InitialUEMessageIes->protocolIEs.list.array[i]
                              ->value.choice.AMFSetID)) {
            Logger::ngap().error("Decoded NGAP AMF Set ID IE error");
            return false;
          }
          m_AmfSetId = std::optional<AmfSetId>(tmp);
        }

      } break;
      default: {
        Logger::ngap().warn(
            "Not decoded IE %d",
            m_InitialUEMessageIes->protocolIEs.list.array[i]->id);
        return true;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::getRanUENgapID(uint32_t& value) const {
  value = m_RanUeNgapId.get();
  return true;
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::getNasPdu(bstring& pdu) const {
  return m_NasPdu.get(pdu);
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::getUserLocationInfoNr(
    struct NrCgi_s& cig, struct Tai_s& tai) const {
  UserLocationInformationNr information_nr = {};
  m_UserLocationInformation.get(information_nr);
  if (m_UserLocationInformation.getChoiceOfUserLocationInformation() !=
      Ngap_UserLocationInformation_PR_userLocationInformationNR)
    return false;
  NrCgi nR_CGI = {};
  Tai nR_TAI   = {};
  information_nr.get(nR_CGI, nR_TAI);
  nR_CGI.get(cig);
  nR_TAI.get(tai);

  return true;
}

//------------------------------------------------------------------------------
int InitialUeMessageMsg::getRrcEstablishmentCause() const {
  return m_RrcEstablishmentCause.get();
}

//------------------------------------------------------------------------------
int InitialUeMessageMsg::getUeContextRequest() const {
  if (m_UeContextRequest.has_value()) {
    return m_UeContextRequest.value().get();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::get5GSTmsi(std::string& fiveGsTmsi) const {
  if (m_FiveGSTmsi.has_value()) {
    m_FiveGSTmsi.value().getTmsi(fiveGsTmsi);
    return true;
  } else
    return false;
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::get5GSTmsi(
    std ::string& setid, std ::string& pointer, std ::string& tmsi) const {
  if (m_FiveGSTmsi.has_value()) {
    m_FiveGSTmsi.value().get(setid, pointer, tmsi);
    return true;
  } else
    return false;
}

//------------------------------------------------------------------------------
void InitialUeMessageMsg::setAllowedNssai(const AllowedNSSAI& allowedNssai) {
  m_AllowedNssai = std::make_optional<AllowedNSSAI>(allowedNssai);
}

//------------------------------------------------------------------------------
bool InitialUeMessageMsg::getAllowedNssai(AllowedNSSAI& allowedNssai) const {
  if (!m_AllowedNssai.has_value()) return false;
  allowedNssai = m_AllowedNssai.value();
  return true;
}

}  // namespace ngap
