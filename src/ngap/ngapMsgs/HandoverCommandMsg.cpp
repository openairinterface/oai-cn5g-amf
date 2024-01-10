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

#include "HandoverCommandMsg.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceHandoverItem.h"
}

namespace ngap {

//------------------------------------------------------------------------------
HandoverCommandMsg::HandoverCommandMsg() : NgapUEMessage() {
  nASSecurityParametersFromNGRAN       = std::nullopt;
  pDUSessionResourceToReleaseListHOCmd = std::nullopt;
  pDUSessionResourceHandoverList       = std::nullopt;
  criticalityDiagnostics               = nullptr;
  handoverCommandIEs                   = nullptr;

  setMessageType(NgapMessageType::HANDOVER_COMMAND);
  initialize();
}

//------------------------------------------------------------------------------
HandoverCommandMsg::~HandoverCommandMsg() {
  // TODO:
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::initialize() {
  handoverCommandIEs =
      &ngapPdu->choice.successfulOutcome->value.choice.HandoverCommand;
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setHandoverType(const long& type) {
  handoverType = type;
  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_HandoverType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_HandoverType;
  ie->value.choice.HandoverType = type;
  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);

  if (ret != 0) Logger::ngap().error("Encode HandoverType IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setNASSecurityParametersFromNGRAN(
    const OCTET_STRING_t& nasSecurityParameters) {
  Ngap_NASSecurityParametersFromNGRAN_t tmp = {};
  amf_conv::octet_string_copy(tmp, nasSecurityParameters);
  nASSecurityParametersFromNGRAN =
      std::optional<Ngap_NASSecurityParametersFromNGRAN_t>(tmp);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NASSecurityParametersFromNGRAN;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_NASSecurityParametersFromNGRAN;
  if (!amf_conv::octet_string_copy(
          ie->value.choice.NASSecurityParametersFromNGRAN,
          nASSecurityParametersFromNGRAN.value())) {
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NASSecurityParametersFromNGRAN IE error");
}

//------------------------------------------------------------------------------
bool HandoverCommandMsg::getNASSecurityParametersFromNGRAN(
    OCTET_STRING_t& nasSecurityParameters) {
  if (!nASSecurityParametersFromNGRAN.has_value()) return false;
  return amf_conv::octet_string_copy(
      nasSecurityParameters, nASSecurityParametersFromNGRAN.value());
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setPduSessionResourceHandoverList(
    const PduSessionResourceHandoverList& list) {
  pDUSessionResourceHandoverList =
      std::optional<PduSessionResourceHandoverList>(list);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList;

  if (!pDUSessionResourceHandoverList.value().encode(
          ie->value.choice.PDUSessionResourceHandoverList)) {
    Logger::ngap().error("Encode PDUSessionResourceHandoverListItem IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceHandoverList IE error");
}

//------------------------------------------------------------------------------
bool HandoverCommandMsg::getPduSessionResourceHandoverList(
    PduSessionResourceHandoverList& list) {
  if (pDUSessionResourceHandoverList.has_value()) {
    list = pDUSessionResourceHandoverList.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setPDUSessionResourceToReleaseListHOCmd(
    const PduSessionResourceToReleaseListHandoverCmd& list) {
  pDUSessionResourceToReleaseListHOCmd =
      std::optional<PduSessionResourceToReleaseListHandoverCmd>(list);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListHOCmd;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceToReleaseListHOCmd;

  if (!pDUSessionResourceToReleaseListHOCmd.value().encode(
          ie->value.choice.PDUSessionResourceToReleaseListHOCmd)) {
    Logger::ngap().error(
        "Encode PDUSessionResourceToReleaseListHOCmd IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode PDUSessionResourceToReleaseListHOCmd IE error");
}

//------------------------------------------------------------------------------
bool HandoverCommandMsg::getPDUSessionResourceToReleaseListHOCmd(
    PduSessionResourceToReleaseListHandoverCmd& list) {
  if (!pDUSessionResourceToReleaseListHOCmd.has_value()) return false;
  list = pDUSessionResourceToReleaseListHOCmd.value();
  return true;
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setTargetToSource_TransparentContainer(
    const OCTET_STRING_t& targetTosource) {
  amf_conv::octet_string_copy(
      targetToSource_TransparentContainer, targetTosource);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer;
  amf_conv::octet_string_copy(
      ie->value.choice.TargetToSource_TransparentContainer, targetTosource);

  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode HandoverType IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::getTargetToSource_TransparentContainer(
    OCTET_STRING_t& targetTosource) {
  targetTosource = targetToSource_TransparentContainer;
}
//------------------------------------------------------------------------------
bool HandoverCommandMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  if (!ngapMsgPdu) return false;
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngapPdu->choice.successfulOutcome &&
        ngapPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_HandoverPreparation &&
        ngapPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_HandoverCommand) {
      handoverCommandIEs =
          &ngapPdu->choice.successfulOutcome->value.choice.HandoverCommand;
    } else {
      Logger::ngap().error("Check Handover Command message error");
      return false;
    }
  } else {
    Logger::ngap().error("Handover Command MessageType error");
    return false;
  }
  for (int i = 0; i < handoverCommandIEs->protocolIEs.list.count; i++) {
    switch (handoverCommandIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(handoverCommandIEs->protocolIEs.list.array[i]
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
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(handoverCommandIEs->protocolIEs.list.array[i]
                                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_HandoverType: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_HandoverType) {
          handoverType = handoverCommandIEs->protocolIEs.list.array[i]
                             ->value.choice.HandoverType;
        } else {
          Logger::ngap().error("Decoded NGAP Handover Type IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList) {
          PduSessionResourceHandoverList tmp = {};
          if (tmp.decode(handoverCommandIEs->protocolIEs.list.array[i]
                             ->value.choice.PDUSessionResourceHandoverList)) {
            pDUSessionResourceHandoverList =
                std::optional<PduSessionResourceHandoverList>(tmp);
          } else {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceHandoverList IE error");
            return false;
          }
        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceHandoverList IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListHOCmd: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceToReleaseListHOCmd) {
          PduSessionResourceToReleaseListHandoverCmd tmp = {};
          if (tmp.decode(
                  handoverCommandIEs->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceToReleaseListHOCmd)) {
            pDUSessionResourceToReleaseListHOCmd =
                std::optional<PduSessionResourceToReleaseListHandoverCmd>(tmp);
          } else {
            Logger::ngap().error(
                "Decoded NGAP PDUSessionResourceToReleaseListHOCmd IE error");
            return false;
          }

        } else {
          Logger::ngap().error(
              "Decoded NGAP PDUSessionResourceToReleaseListHOCmd IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer) {
        } else {
          Logger::ngap().error(
              "Decoded NGAP TargetToSource_TransparentContainer IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_CriticalityDiagnostics) {
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
