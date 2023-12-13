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
#include "HandoverNotifyMsg.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

using namespace std;
namespace ngap {

//------------------------------------------------------------------------------
HandoverNotifyMsg::HandoverNotifyMsg() : NgapUEMessage() {
  handoverNotifyIEs = nullptr;

  setMessageType(NgapMessageType::HANDOVER_NOTIFY);
  initialize();
}

//------------------------------------------------------------------------------
HandoverNotifyMsg::~HandoverNotifyMsg(){};

//------------------------------------------------------------------------------
void HandoverNotifyMsg::initialize() {
  handoverNotifyIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.HandoverNotify);
}

//------------------------------------------------------------------------------
void HandoverNotifyMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_HandoverNotifyIEs_t* ie =
      (Ngap_HandoverNotifyIEs_t*) calloc(1, sizeof(Ngap_HandoverNotifyIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverNotifyIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverNotifyIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void HandoverNotifyMsg::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_HandoverNotifyIEs_t* ie =
      (Ngap_HandoverNotifyIEs_t*) calloc(1, sizeof(Ngap_HandoverNotifyIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverNotifyIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverNotifyIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error!");
}

//------------------------------------------------------------------------------
void HandoverNotifyMsg::setUserLocationInfoNR(
    const NrCgi_t& cig, const Tai_t& tai) {
  UserLocationInformationNR information_nr = {};
  NrCgi nR_CGI                             = {};
  nR_CGI.set(cig.mcc, cig.mnc, cig.nrCellID);
  TAI tai_nr = {};
  tai_nr.setTAI(tai);
  information_nr.set(nR_CGI, tai_nr);
  userLocationInformation.setInformation(information_nr);

  Ngap_HandoverNotifyIEs_t* ie =
      (Ngap_HandoverNotifyIEs_t*) calloc(1, sizeof(Ngap_HandoverNotifyIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation;

  int ret =
      userLocationInformation.encode(ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::ngap().error("Encode UserLocationInformation IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverNotifyIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode UserLocationInformation IE error");
}

//------------------------------------------------------------------------------
bool HandoverNotifyMsg::getUserLocationInfoNR(NrCgi_t& cig, Tai_t& tai) {
  UserLocationInformationNR information_nr = {};
  if (!userLocationInformation.getInformation(information_nr)) return false;

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
bool HandoverNotifyMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_HandoverNotification &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_HandoverNotify) {
      handoverNotifyIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.HandoverNotify;
    } else {
      Logger::ngap().error("Check HandoverNotify message error!");
      return false;
    }
  } else {
    Logger::ngap().error("HandoverNotify MessageType error!");
    return false;
  }
  for (int i = 0; i < handoverNotifyIEs->protocolIEs.list.count; i++) {
    switch (handoverNotifyIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverNotifyIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverNotifyIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverNotifyIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(handoverNotifyIEs->protocolIEs.list.array[i]
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
        if (handoverNotifyIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverNotifyIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverNotifyIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(handoverNotifyIEs->protocolIEs.list.array[i]
                                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        // TODO: Temporarily disable Criticality check to be tested with dsTest
        /*if (handoverNotifyIEs->protocolIEs.list.array[i]->criticality ==
              Ngap_Criticality_ignore &&
          handoverNotifyIEs->protocolIEs.list.array[i]->value.present ==
              Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation) {
              */
        if (handoverNotifyIEs->protocolIEs.list.array[i]->value.present ==
            Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation) {
          if (!userLocationInformation.decode(
                  handoverNotifyIEs->protocolIEs.list.array[i]
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
