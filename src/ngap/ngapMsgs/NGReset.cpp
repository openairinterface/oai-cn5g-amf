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

#include "NGReset.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
NGResetMsg::NGResetMsg() {
  ngResetIEs = nullptr;

  NgapMessage::setMessageType(NgapMessageType::NG_RESET);
  initialize();
}

//------------------------------------------------------------------------------
NGResetMsg::~NGResetMsg() {}

//------------------------------------------------------------------------------
void NGResetMsg::initialize() {
  ngResetIEs = &(ngapPdu->choice.initiatingMessage->value.choice.NGReset);
}

//------------------------------------------------------------------------------
void NGResetMsg::setCause(const Cause& c) {
  cause = c;

  Ngap_NGResetIEs_t* ie =
      (Ngap_NGResetIEs_t*) calloc(1, sizeof(Ngap_NGResetIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGResetIEs__value_PR_Cause;

  if (!cause.encode(ie->value.choice.Cause)) {
    Logger::ngap().error("Encode NGAP Cause IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngResetIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP Cause IE error");
}

//------------------------------------------------------------------------------
void NGResetMsg::setResetType(const ResetType& r) {
  resetType = r;

  Ngap_NGResetIEs_t* ie =
      (Ngap_NGResetIEs_t*) calloc(1, sizeof(Ngap_NGResetIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_ResetType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGResetIEs__value_PR_ResetType;

  if (!resetType.encode(ie->value.choice.ResetType)) {
    Logger::ngap().error("Encode NGAP ResetType IE error");
    free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngResetIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP ResetType IE error");
}

//------------------------------------------------------------------------------
void NGResetMsg::getCause(Cause& c) {
  c = cause;
}

//------------------------------------------------------------------------------
bool NGResetMsg::getResetType(ResetType& r) {
  if (resetType.getResetType() == Ngap_ResetType_PR_nG_Interface) {
    long ng_interface = 0;
    resetType.getResetType(ng_interface);
    r.setResetType(ng_interface);
  } else if (resetType.getResetType() == Ngap_ResetType_PR_partOfNG_Interface) {
    // TODO
  }
  return true;
}

//------------------------------------------------------------------------------
bool NGResetMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_NGReset &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_NGReset) {
      ngResetIEs = &ngapPdu->choice.initiatingMessage->value.choice.NGReset;
      for (int i = 0; i < ngResetIEs->protocolIEs.list.count; i++) {
        switch (ngResetIEs->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_Cause: {
            if (ngResetIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                ngResetIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetIEs__value_PR_Cause) {
              if (!cause.decode(ngResetIEs->protocolIEs.list.array[i]
                                    ->value.choice.Cause)) {
                Logger::ngap().error("Decoded NGAP Cause IE error");
                return false;
              }
            } else {
              Logger::ngap().error("Decoded NGAP Cause IE error");
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_ResetType: {
            if (ngResetIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                ngResetIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetIEs__value_PR_ResetType) {
              if (!resetType.decode(ngResetIEs->protocolIEs.list.array[i]
                                        ->value.choice.ResetType)) {
                Logger::ngap().error("Decoded NGAP ResetType IE error");
                return false;
              }

            } else {
              Logger::ngap().error("Decoded NGAP ResetType IE error");
              return false;
            }
          } break;

          default: {
            Logger::ngap().error("Decoded NGAP message PDU IE error");
            return false;
          }
        }
      }
    } else {
      Logger::ngap().error("Check NGReset message error!");
      return false;
    }
  } else {
    Logger::ngap().error("Check NGReset message error!");
    return false;
  }
  return true;
}

}  // namespace ngap
