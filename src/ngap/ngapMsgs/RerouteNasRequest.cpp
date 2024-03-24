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

#include "RerouteNasRequest.hpp"

#include "amf.hpp"
#include "amf_conversions.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RerouteNasRequest::RerouteNasRequest() : NgapMessage() {
  m_RerouteNASRequestIes = nullptr;
  m_AmfUeNgapId          = std::nullopt;
  m_AllowedNssai         = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::REROUTE_NAS_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
RerouteNasRequest::~RerouteNasRequest() {}

//------------------------------------------------------------------------------
void RerouteNasRequest::initialize() {
  m_RerouteNASRequestIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.RerouteNASRequest);
}

//------------------------------------------------------------------------------
void RerouteNasRequest::setAmfUeNgapId(const unsigned long& id) {
  AmfUeNgapId tmp = {};
  tmp.set(id);
  m_AmfUeNgapId = std::optional<AmfUeNgapId>(tmp);

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret = m_AmfUeNgapId.value().encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_RerouteNASRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
bool RerouteNasRequest::getAmfUeNgapId(unsigned long& id) const {
  if (!m_AmfUeNgapId.has_value()) return false;
  id = m_AmfUeNgapId->get();
  return true;
}

//------------------------------------------------------------------------------
void RerouteNasRequest::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  m_RanUeNgapId.set(ranUeNgapId);

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_RerouteNASRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
uint32_t RerouteNasRequest::getRanUeNgapId() const {
  return m_RanUeNgapId.get();
}

//------------------------------------------------------------------------------
void RerouteNasRequest::setAllowedNssai(const std::vector<S_Nssai>& list) {
  AllowedNSSAI tmp = {};

  std::vector<SNssai> sNssaiList;
  for (int i = 0; i < list.size(); i++) {
    SNssai sNssai = {};
    sNssai.setSst(list[i].sst);

    uint32_t sd = SD_NO_VALUE;
    if (!list[i].sd.empty()) {
      amf_conv::sd_string_to_int(list[i].sd, sd);
    }
    sNssai.setSd(sd);
    sNssaiList.push_back(sNssai);
  }
  tmp.set(sNssaiList);
  m_AllowedNssai = std::optional<AllowedNSSAI>(tmp);

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_AllowedNSSAI;

  int ret = m_AllowedNssai.value().encode(ie->value.choice.AllowedNSSAI);
  if (!ret) {
    Logger::ngap().error("Encode AllowedNSSAI IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_RerouteNASRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error!");
}

//------------------------------------------------------------------------------
bool RerouteNasRequest::getAllowedNssai(std::vector<S_Nssai>& list) const {
  if (!m_AllowedNssai.has_value()) return false;

  std::vector<SNssai> sNssaiList;
  m_AllowedNssai.value().get(sNssaiList);
  for (std::vector<SNssai>::iterator it = std::begin(sNssaiList);
       it < std::end(sNssaiList); ++it) {
    S_Nssai sNssai = {};
    it->getSst(sNssai.sst);
    it->getSd(sNssai.sd);
    list.push_back(sNssai);
  }
  return true;
}

//------------------------------------------------------------------------------
void RerouteNasRequest::setNgapMessage(const OCTET_STRING_t& message) {
  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NGAP_Message;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_OCTET_STRING;
  ie->value.choice.OCTET_STRING = message;

  int ret = ASN_SEQUENCE_ADD(&m_RerouteNASRequestIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP Message IE error!");
}

//------------------------------------------------------------------------------
bool RerouteNasRequest::getNgapMessage(OCTET_STRING_t& message) const {
  message = m_NgapMessage;
  return true;
}

//------------------------------------------------------------------------------
bool RerouteNasRequest::setAmfSetId(const uint16_t& amfSetId) {
  if (!m_AmfSetId.set(amfSetId)) return false;

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMFSetID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_AMFSetID;

  int ret = m_AmfSetId.encode(ie->value.choice.AMFSetID);
  if (!ret) {
    Logger::ngap().error("Encode AMFSetID IE error!");
    utils::free_wrapper((void**) &ie);
    return false;
  }

  ret = ASN_SEQUENCE_ADD(&m_RerouteNASRequestIes->protocolIEs.list, ie);
  if (ret != 0) {
    Logger::ngap().error("Encode AMFSetID IE error!");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void RerouteNasRequest::getAmfSetId(std::string& amfSetId) const {
  m_AmfSetId.get(amfSetId);
}

//------------------------------------------------------------------------------
bool RerouteNasRequest::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_RerouteNASRequest &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_RerouteNASRequest) {
      m_RerouteNASRequestIes =
          &ngapPdu->choice.initiatingMessage->value.choice.RerouteNASRequest;
    } else {
      Logger::ngap().error("Check RerouteNASRequest message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < m_RerouteNASRequestIes->protocolIEs.list.count; i++) {
    switch (m_RerouteNASRequestIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_RerouteNASRequestIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_RerouteNASRequestIes->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_AMF_UE_NGAP_ID) {
          AmfUeNgapId tmp = {};
          if (!tmp.decode(m_RerouteNASRequestIes->protocolIEs.list.array[i]
                              ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
          m_AmfUeNgapId = std::optional<AmfUeNgapId>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (m_RerouteNASRequestIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_RerouteNASRequestIes->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!m_RanUeNgapId.decode(
                  m_RerouteNASRequestIes->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_NGAP_Message: {
        if (m_RerouteNASRequestIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_RerouteNASRequestIes->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_OCTET_STRING) {
          m_NgapMessage = m_RerouteNASRequestIes->protocolIEs.list.array[i]
                              ->value.choice.OCTET_STRING;
          Logger::ngap().error("Decoded NGAP Message IE error");
        }
      } break;

      case Ngap_ProtocolIE_ID_id_AMFSetID: {
        if (m_RerouteNASRequestIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_RerouteNASRequestIes->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_AMFSetID) {
          if (!m_AmfSetId.decode(
                  m_RerouteNASRequestIes->protocolIEs.list.array[i]
                      ->value.choice.AMFSetID)) {
            Logger::ngap().error("Decoded NGAP AMFSetID error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP AMFSetID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        if (m_RerouteNASRequestIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_RerouteNASRequestIes->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_AllowedNSSAI) {
          AllowedNSSAI tmp = {};
          if (!m_AllowedNssai->decode(
                  m_RerouteNASRequestIes->protocolIEs.list.array[i]
                      ->value.choice.AllowedNSSAI)) {
            Logger::ngap().error("Decoded NGAP AllowedNSSAI IE error");
            return false;
          }
          m_AllowedNssai = std::optional<AllowedNSSAI>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP AllowedNSSAI IE error");
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
