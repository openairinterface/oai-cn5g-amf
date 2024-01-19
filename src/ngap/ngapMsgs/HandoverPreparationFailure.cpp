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

#include "HandoverPreparationFailure.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
HandoverPreparationFailure::HandoverPreparationFailure() : NgapUEMessage() {
  hoPreparationFailureIEs = nullptr;
  criticalityDiagnostics  = nullptr;

  setMessageType(NgapMessageType::HANDOVER_PREPARATION_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
HandoverPreparationFailure::~HandoverPreparationFailure() {}

//------------------------------------------------------------------------------
void HandoverPreparationFailure::initialize() {
  hoPreparationFailureIEs = &(ngapPdu->choice.unsuccessfulOutcome->value.choice
                                  .HandoverPreparationFailure);
}

//------------------------------------------------------------------------------
void HandoverPreparationFailure::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_HandoverPreparationFailureIEs_t* ie =
      (Ngap_HandoverPreparationFailureIEs_t*) calloc(
          1, sizeof(Ngap_HandoverPreparationFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverPreparationFailureIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&hoPreparationFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverPreparationFailure::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_HandoverPreparationFailureIEs_t* ie =
      (Ngap_HandoverPreparationFailureIEs_t*) calloc(
          1, sizeof(Ngap_HandoverPreparationFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverPreparationFailureIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&hoPreparationFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverPreparationFailure::setCause(
    const Ngap_Cause_PR& causePresent,
    const long& value)  //
{
  Ngap_HandoverPreparationFailureIEs_t* ie =
      (Ngap_HandoverPreparationFailureIEs_t*) calloc(
          1, sizeof(Ngap_HandoverPreparationFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverPreparationFailureIEs__value_PR_Cause;

  cause.setChoiceOfCause(causePresent);
  if (causePresent != Ngap_Cause_PR_NOTHING) cause.setValue(value);
  cause.encode(ie->value.choice.Cause);
  int ret = ASN_SEQUENCE_ADD(&hoPreparationFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode Cause IE error");
}

//------------------------------------------------------------------------------
Ngap_Cause_PR HandoverPreparationFailure::getChoiceOfCause() {
  return cause.getChoiceOfCause();
}

//------------------------------------------------------------------------------
bool HandoverPreparationFailure::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    if (ngapPdu->choice.unsuccessfulOutcome &&
        ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_HandoverPreparation &&
        ngapPdu->choice.unsuccessfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.unsuccessfulOutcome->value.present ==
            Ngap_UnsuccessfulOutcome__value_PR_HandoverPreparationFailure) {
      hoPreparationFailureIEs = &ngapPdu->choice.unsuccessfulOutcome->value
                                     .choice.HandoverPreparationFailure;
    } else {
      Logger::ngap().error("Check HandoverPreparationFailure message error");
      return false;
    }
  } else {
    Logger::ngap().error("HandoverPreparationFailure MessageType error");
    return false;
  }

  for (int i = 0; i < hoPreparationFailureIEs->protocolIEs.list.count; i++) {
    switch (hoPreparationFailureIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (hoPreparationFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            hoPreparationFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverPreparationFailureIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  hoPreparationFailureIEs->protocolIEs.list.array[i]
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
        if (hoPreparationFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            hoPreparationFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverPreparationFailureIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  hoPreparationFailureIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (hoPreparationFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            hoPreparationFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverPreparationFailureIEs__value_PR_Cause) {
          if (!cause.decode(hoPreparationFailureIEs->protocolIEs.list.array[i]
                                ->value.choice.Cause)) {
            Logger::ngap().error("Decoded NGAP Cause IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP Cause IE error");
          return false;
        }

      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (hoPreparationFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            hoPreparationFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverPreparationFailureIEs__value_PR_CriticalityDiagnostics) {
          // TODO:
        } else {
          Logger::ngap().error("Decoded NGAP CriticalityDiagnostics IE error");
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
