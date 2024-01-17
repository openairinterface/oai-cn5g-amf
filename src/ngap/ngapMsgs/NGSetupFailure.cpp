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

#include "NGSetupFailure.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NGSetupFailureMsg::NGSetupFailureMsg() : NgapMessage() {
  ngSetupFailureIEs = nullptr;
  // criticalityDiagnostics = NULL;
  timeToWait = std::nullopt;
  NgapMessage::setMessageType(NgapMessageType::NG_SETUP_FAILURE);
  initialize();
}

//------------------------------------------------------------------------------
NGSetupFailureMsg::~NGSetupFailureMsg() {}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::initialize() {
  ngSetupFailureIEs =
      &(ngapPdu->choice.unsuccessfulOutcome->value.choice.NGSetupFailure);
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::addCauseIE() {
  Ngap_NGSetupFailureIEs_t* ie =
      (Ngap_NGSetupFailureIEs_t*) calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupFailureIEs__value_PR_Cause;

  if (!cause.encode(ie->value.choice.Cause)) {
    Logger::ngap().error("Encode NGAP Cause IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP Cause IE error");
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::addTimeToWaitIE() {
  if (!timeToWait.has_value()) return;

  Ngap_NGSetupFailureIEs_t* ie =
      (Ngap_NGSetupFailureIEs_t*) calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_TimeToWait;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupFailureIEs__value_PR_TimeToWait;

  if (!timeToWait.value().encode(ie->value.choice.TimeToWait)) {
    Logger::ngap().error("Encode NGAP Cause IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&ngSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP TimeToWait IE error");
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::set(
    const e_Ngap_CauseRadioNetwork& cause_value,
    const e_Ngap_TimeToWait& time_to_wait) {
  cause.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  cause.setValue(cause_value);
  addCauseIE();

  timeToWait = std::make_optional<TimeToWait>(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseRadioNetwork(
    const e_Ngap_CauseRadioNetwork& cause_value) {
  cause.setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  cause.setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::set(
    const e_Ngap_CauseTransport& cause_value,
    const e_Ngap_TimeToWait& time_to_wait) {
  cause.setChoiceOfCause(Ngap_Cause_PR_transport);
  cause.setValue(cause_value);
  addCauseIE();

  timeToWait = std::make_optional<TimeToWait>(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseTransport(
    const e_Ngap_CauseTransport& cause_value) {
  cause.setChoiceOfCause(Ngap_Cause_PR_transport);
  cause.setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::set(
    const e_Ngap_CauseNas& cause_value, const e_Ngap_TimeToWait& time_to_wait) {
  cause.setChoiceOfCause(Ngap_Cause_PR_nas);
  cause.setValue(cause_value);
  addCauseIE();

  timeToWait = std::make_optional<TimeToWait>(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseNas(const e_Ngap_CauseNas& cause_value) {
  cause.setChoiceOfCause(Ngap_Cause_PR_nas);
  cause.setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::set(
    const e_Ngap_CauseProtocol& cause_value,
    const e_Ngap_TimeToWait& time_to_wait) {
  cause.setChoiceOfCause(Ngap_Cause_PR_protocol);
  cause.setValue(cause_value);
  addCauseIE();

  timeToWait = std::make_optional<TimeToWait>(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseProtocol(
    const e_Ngap_CauseProtocol& cause_value) {
  cause.setChoiceOfCause(Ngap_Cause_PR_protocol);
  cause.setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::set(
    const e_Ngap_CauseMisc& cause_value,
    const e_Ngap_TimeToWait& time_to_wait) {
  cause.setChoiceOfCause(Ngap_Cause_PR_misc);
  cause.setValue(cause_value);
  addCauseIE();

  timeToWait = std::make_optional<TimeToWait>(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseMisc(const e_Ngap_CauseMisc& cause_value) {
  cause.setChoiceOfCause(Ngap_Cause_PR_misc);
  cause.setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;
  if (ngapPdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    if (ngapPdu->choice.unsuccessfulOutcome &&
        ngapPdu->choice.unsuccessfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngapPdu->choice.unsuccessfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.unsuccessfulOutcome->value.present ==
            Ngap_UnsuccessfulOutcome__value_PR_NGSetupFailure) {
      ngSetupFailureIEs =
          &ngapPdu->choice.unsuccessfulOutcome->value.choice.NGSetupFailure;
    } else {
      Logger::ngap().error("Check NGSetupFailure message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0; i < ngSetupFailureIEs->protocolIEs.list.count; i++) {
    switch (ngSetupFailureIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ngSetupFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ngSetupFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupFailureIEs__value_PR_Cause) {
          if (!cause.decode(ngSetupFailureIEs->protocolIEs.list.array[i]
                                ->value.choice.Cause)) {
            Logger::ngap().error("Decoded NGAP Cause IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP Cause IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TimeToWait: {
        if (ngSetupFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ngSetupFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupFailureIEs__value_PR_TimeToWait) {
          TimeToWait tmp = {};
          if (!tmp.decode(ngSetupFailureIEs->protocolIEs.list.array[i]
                              ->value.choice.TimeToWait)) {
            Logger::ngap().error("Decoded NGAP TimeToWait IE error");
            return false;
          }
          timeToWait = std::optional<TimeToWait>(tmp);
        } else {
          Logger::ngap().error("Decoded NGAP TimeToWait IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        Logger::ngap().debug("Decoded NGAP CriticalityDiagnostics IE ");
      } break;
      default: {
        Logger::ngap().error("Decoded NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseType(Ngap_Cause_PR& causePresent) {
  if (cause.getChoiceOfCause() < 0) {
    return false;
  }
  causePresent = cause.getChoiceOfCause();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork& causeRadioNetwork) {
  if (cause.getValue() < 0) {
    return false;
  }
  causeRadioNetwork = (e_Ngap_CauseRadioNetwork) cause.getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseTransport(
    e_Ngap_CauseTransport& causeTransport) {
  if (cause.getValue() < 0) {
    return false;
  }
  causeTransport = (e_Ngap_CauseTransport) cause.getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseNas(e_Ngap_CauseNas& causeNas) {
  if (cause.getValue() < 0) {
    return false;
  }
  causeNas = (e_Ngap_CauseNas) cause.getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseProtocol(e_Ngap_CauseProtocol& causeProtocol) {
  if (cause.getValue() < 0) {
    return false;
  }
  causeProtocol = (e_Ngap_CauseProtocol) cause.getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseMisc(e_Ngap_CauseMisc& causeMisc) {
  if (cause.getValue() < 0) {
    return false;
  }
  causeMisc = (e_Ngap_CauseMisc) cause.getValue();
  return true;
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setTimeToWait(const e_Ngap_TimeToWait& time) {
  timeToWait = std::make_optional<TimeToWait>(time);
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getTimeToWait(e_Ngap_TimeToWait& time) {
  if (timeToWait.has_value()) {
    return false;
  }
  time = (e_Ngap_TimeToWait) timeToWait.value().getValue();
  return true;
}

}  // namespace ngap
