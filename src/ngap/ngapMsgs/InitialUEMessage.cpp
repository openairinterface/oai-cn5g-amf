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

#include "InitialUEMessage.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
InitialUEMessageMsg::InitialUEMessageMsg() : NgapMessage() {
  initialUEMessageIEs = nullptr;
  uEContextRequest    = std::nullopt;
  fivegSTmsi          = std::nullopt;
  amfSetId            = std::nullopt;
  allowedNssai        = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::INITIAL_UE_MESSAGE);
  initialize();
}

//------------------------------------------------------------------------------
InitialUEMessageMsg::~InitialUEMessageMsg() {}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::initialize() {
  initialUEMessageIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.InitialUEMessage);
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setRanUENgapID(const uint32_t& value) {
  ranUeNgapId.set(value);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setNasPdu(const bstring& pdu) {
  nasPdu.set(pdu);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_NAS_PDU;

  int ret = nasPdu.encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS PDU IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS PDU IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setUserLocationInfoNR(
    const struct NrCgi_s& cig, const struct Tai_s& tai) {
  UserLocationInformationNR information_nr;
  NrCgi nR_CGI = {};
  nR_CGI.set(cig.mcc, cig.mnc, cig.nrCellID);

  TAI tai_nr = {};
  tai_nr.setTAI(tai);
  information_nr.set(nR_CGI, tai_nr);
  userLocationInformation.setInformation(information_nr);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation;

  int ret =
      userLocationInformation.encode(ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode UserLocationInformation IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setRRCEstablishmentCause(
    const e_Ngap_RRCEstablishmentCause& cause) {
  rRCEstablishmentCause.set(cause);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RRCEstablishmentCause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause;

  int ret =
      rRCEstablishmentCause.encode(ie->value.choice.RRCEstablishmentCause);
  if (!ret) {
    Logger::ngap().error("Encode RRCEstablishmentCause IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RRCEstablishmentCause IE error");
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setUeContextRequest(
    const e_Ngap_UEContextRequest& ue_ctx_req) {
  uEContextRequest = std::make_optional<UEContextRequest>(ue_ctx_req);

  Ngap_InitialUEMessage_IEs_t* ie = (Ngap_InitialUEMessage_IEs_t*) calloc(
      1, sizeof(Ngap_InitialUEMessage_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UEContextRequest;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest;

  int ret = uEContextRequest.value().encode(ie->value.choice.UEContextRequest);
  if (!ret) {
    Logger::ngap().error("Encode UEContextRequest IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialUEMessageIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UEContextRequest IE error");
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getAMFSetID(uint16_t& amf_set_id) {
  if (!amfSetId.has_value()) return false;
  amfSetId.value().get(amf_set_id);
  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getAMFSetID(std::string& amf_set_id) {
  if (!amfSetId.has_value()) return false;
  amfSetId.value().get(amf_set_id);
  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::setAMFSetID(const uint16_t& amf_set_id) {
  AmfSetId tmp = {};
  if (!tmp.set(amf_set_id)) return false;
  amfSetId = std::optional<AmfSetId>(tmp);
  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_InitialUEMessage &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_InitialUEMessage) {
      initialUEMessageIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.InitialUEMessage;
    } else {
      Logger::ngap().error("Check InitialUEMessage message error");
      return false;
    }
  } else {
    Logger::ngap().error("Check MessageType error");
    return false;
  }
  for (int i = 0; i < initialUEMessageIEs->protocolIEs.list.count; i++) {
    switch (initialUEMessageIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  initialUEMessageIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
          Logger::ngap().debug("Received RanUeNgapId %d ", ranUeNgapId.get());

        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_NAS_PDU) {
          if (!nasPdu.decode(initialUEMessageIEs->protocolIEs.list.array[i]
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
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation) {
          if (!userLocationInformation.decode(
                  initialUEMessageIEs->protocolIEs.list.array[i]
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
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause) {
          if (!rRCEstablishmentCause.decode(
                  initialUEMessageIEs->protocolIEs.list.array[i]
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
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest) {
          UEContextRequest tmp = {};
          if (!tmp.decode(initialUEMessageIEs->protocolIEs.list.array[i]
                              ->value.choice.UEContextRequest)) {
            Logger::ngap().error("Decoded NGAP UEContextRequest IE error");
            return false;
          }
          uEContextRequest = std::optional<UEContextRequest>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP UEContextRequest IE error");
          return false;
        }

      } break;

      case Ngap_ProtocolIE_ID_id_FiveG_S_TMSI: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_FiveG_S_TMSI) {
          FiveGSTmsi tmp = {};
          if (!tmp.decode(initialUEMessageIEs->protocolIEs.list.array[i]
                              ->value.choice.FiveG_S_TMSI)) {
            Logger::ngap().error("Decoded NGAP FiveG_S_TMSI IE error");
            return false;
          }
          fivegSTmsi = std::optional<FiveGSTmsi>(tmp);
        }
      } break;
      case Ngap_ProtocolIE_ID_id_AMFSetID: {
        if (initialUEMessageIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            initialUEMessageIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_InitialUEMessage_IEs__value_PR_AMFSetID) {
          AmfSetId tmp = {};
          if (!tmp.decode(initialUEMessageIEs->protocolIEs.list.array[i]
                              ->value.choice.AMFSetID)) {
            Logger::ngap().error("Decoded NGAP AMF Set ID IE error");
            return false;
          }
          amfSetId = std::optional<AmfSetId>(tmp);
        }

      } break;
      default: {
        Logger::ngap().warn(
            "Not decoded IE %d",
            initialUEMessageIEs->protocolIEs.list.array[i]->id);
        return true;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getRanUENgapID(uint32_t& value) {
  value = ranUeNgapId.get();
  return true;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getNasPdu(bstring& pdu) {
  return nasPdu.get(pdu);
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getUserLocationInfoNR(
    struct NrCgi_s& cig, struct Tai_s& tai) {
  UserLocationInformationNR information_nr = {};
  userLocationInformation.getInformation(information_nr);
  if (userLocationInformation.getChoiceOfUserLocationInformation() !=
      Ngap_UserLocationInformation_PR_userLocationInformationNR)
    return false;
  NrCgi nR_CGI = {};
  TAI nR_TAI   = {};
  information_nr.get(nR_CGI, nR_TAI);
  nR_CGI.get(cig);
  nR_TAI.getTAI(tai);

  return true;
}

//------------------------------------------------------------------------------
int InitialUEMessageMsg::getRRCEstablishmentCause() {
  return rRCEstablishmentCause.get();
}

//------------------------------------------------------------------------------
int InitialUEMessageMsg::getUeContextRequest() const {
  if (uEContextRequest.has_value()) {
    return uEContextRequest.value().get();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::get5GS_TMSI(std::string& _5GsTmsi) {
  if (fivegSTmsi.has_value()) {
    fivegSTmsi.value().getTmsi(_5GsTmsi);
    return true;
  } else
    return false;
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::get5GS_TMSI(
    std ::string& setid, std ::string& pointer, std ::string& tmsi) {
  if (fivegSTmsi.has_value()) {
    fivegSTmsi.value().get(setid, pointer, tmsi);
    return true;
  } else
    return false;
}

//------------------------------------------------------------------------------
void InitialUEMessageMsg::setAllowedNssai(const AllowedNSSAI& allowed_nssai) {
  allowedNssai = std::make_optional<AllowedNSSAI>(allowed_nssai);
}

//------------------------------------------------------------------------------
bool InitialUEMessageMsg::getAllowedNssai(AllowedNSSAI& allowed_nssai) const {
  if (!allowedNssai.has_value()) return false;
  allowed_nssai = allowedNssai.value();
  return true;
}

}  // namespace ngap
