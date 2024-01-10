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

#include "RerouteNASRequest.hpp"

#include "amf.hpp"
#include "common_defs.h"
#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RerouteNASRequest::RerouteNASRequest() : NgapMessage() {
  rerouteNASRequestIEs = nullptr;
  amfUeNgapId          = std::nullopt;
  allowedNssai         = std::nullopt;

  NgapMessage::setMessageType(NgapMessageType::REROUTE_NAS_REQUEST);
  initialize();
}

//------------------------------------------------------------------------------
RerouteNASRequest::~RerouteNASRequest() {}

//------------------------------------------------------------------------------
void RerouteNASRequest::initialize() {
  rerouteNASRequestIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.RerouteNASRequest);
}

//------------------------------------------------------------------------------
void RerouteNASRequest::setAmfUeNgapId(const unsigned long& id) {
  AmfUeNgapId tmp = {};
  tmp.set(id);
  amfUeNgapId = std::optional<AmfUeNgapId>(tmp);

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.value().encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&rerouteNASRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
bool RerouteNASRequest::getAmfUeNgapId(unsigned long& id) {
  if (!amfUeNgapId.has_value()) return false;
  id = amfUeNgapId->get();
  return true;
}

//------------------------------------------------------------------------------
void RerouteNASRequest::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&rerouteNASRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
uint32_t RerouteNASRequest::getRanUeNgapId() {
  return ranUeNgapId.get();
}

//------------------------------------------------------------------------------
void RerouteNASRequest::setAllowedNssai(const std::vector<S_Nssai>& list) {
  AllowedNSSAI tmp = {};

  std::vector<S_NSSAI> snssaiList;
  for (int i = 0; i < list.size(); i++) {
    S_NSSAI snssai = {};
    snssai.setSst(list[i].sst);

    uint32_t sd = SD_NO_VALUE;
    if (!list[i].sd.empty()) {
      amf_conv::sd_string_to_int(list[i].sd, sd);
    }
    snssai.setSd(sd);
    snssaiList.push_back(snssai);
  }
  tmp.set(snssaiList);
  allowedNssai = std::optional<AllowedNSSAI>(tmp);

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_AllowedNSSAI;

  int ret = allowedNssai.value().encode(ie->value.choice.AllowedNSSAI);
  if (!ret) {
    Logger::ngap().error("Encode AllowedNSSAI IE error!");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&rerouteNASRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error!");
}

//------------------------------------------------------------------------------
bool RerouteNASRequest::getAllowedNssai(std::vector<S_Nssai>& list) {
  if (!allowedNssai.has_value()) return false;

  std::vector<S_NSSAI> snssaiList;
  allowedNssai.value().get(snssaiList);
  for (std::vector<S_NSSAI>::iterator it = std::begin(snssaiList);
       it < std::end(snssaiList); ++it) {
    S_Nssai s_nssai = {};
    it->getSst(s_nssai.sst);
    it->getSd(s_nssai.sd);
    list.push_back(s_nssai);
  }
  return true;
}

//------------------------------------------------------------------------------
void RerouteNASRequest::setNgapMessage(const OCTET_STRING_t& message) {
  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NGAP_Message;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_OCTET_STRING;
  ie->value.choice.OCTET_STRING = message;

  int ret = ASN_SEQUENCE_ADD(&rerouteNASRequestIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP Message IE error!");
}

//------------------------------------------------------------------------------
bool RerouteNASRequest::getNgapMessage(OCTET_STRING_t& message) const {
  message = ngapMessage;
  return true;
}

//------------------------------------------------------------------------------
bool RerouteNASRequest::setAMFSetID(const uint16_t& amf_set_id) {
  if (!amfSetID.set(amf_set_id)) return false;

  Ngap_RerouteNASRequest_IEs_t* ie = (Ngap_RerouteNASRequest_IEs_t*) calloc(
      1, sizeof(Ngap_RerouteNASRequest_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMFSetID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_RerouteNASRequest_IEs__value_PR_AMFSetID;

  int ret = amfSetID.encode(ie->value.choice.AMFSetID);
  if (!ret) {
    Logger::ngap().error("Encode AMFSetID IE error!");
    utils::free_wrapper((void**) &ie);
    return false;
  }

  ret = ASN_SEQUENCE_ADD(&rerouteNASRequestIEs->protocolIEs.list, ie);
  if (ret != 0) {
    Logger::ngap().error("Encode AMFSetID IE error!");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void RerouteNASRequest::getAMFSetID(std::string& amf_set_id) {
  amfSetID.get(amf_set_id);
}

//------------------------------------------------------------------------------
bool RerouteNASRequest::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_RerouteNASRequest &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_RerouteNASRequest) {
      rerouteNASRequestIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.RerouteNASRequest;
    } else {
      Logger::ngap().error("Check RerouteNASRequest message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < rerouteNASRequestIEs->protocolIEs.list.count; i++) {
    switch (rerouteNASRequestIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (rerouteNASRequestIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            rerouteNASRequestIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_AMF_UE_NGAP_ID) {
          AmfUeNgapId tmp = {};
          if (!tmp.decode(rerouteNASRequestIEs->protocolIEs.list.array[i]
                              ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
          amfUeNgapId = std::optional<AmfUeNgapId>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (rerouteNASRequestIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            rerouteNASRequestIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  rerouteNASRequestIEs->protocolIEs.list.array[i]
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
        if (rerouteNASRequestIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            rerouteNASRequestIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_OCTET_STRING) {
          ngapMessage = rerouteNASRequestIEs->protocolIEs.list.array[i]
                            ->value.choice.OCTET_STRING;
          Logger::ngap().error("Decoded NGAP Message IE error");
        }
      } break;

      case Ngap_ProtocolIE_ID_id_AMFSetID: {
        if (rerouteNASRequestIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            rerouteNASRequestIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_AMFSetID) {
          if (!amfSetID.decode(rerouteNASRequestIEs->protocolIEs.list.array[i]
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
        if (rerouteNASRequestIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            rerouteNASRequestIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_RerouteNASRequest_IEs__value_PR_AllowedNSSAI) {
          AllowedNSSAI tmp = {};
          if (!allowedNssai->decode(
                  rerouteNASRequestIEs->protocolIEs.list.array[i]
                      ->value.choice.AllowedNSSAI)) {
            Logger::ngap().error("Decoded NGAP AllowedNSSAI IE error");
            return false;
          }
          allowedNssai = std::optional<AllowedNSSAI>(tmp);
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
