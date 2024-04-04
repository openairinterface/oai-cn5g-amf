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
HandoverCommandMsg::HandoverCommandMsg() : NgapUeMessage() {
  m_NasSecurityParametersFromNgRan       = std::nullopt;
  m_PduSessionResourceToReleaseListHOCmd = std::nullopt;
  m_PduSessionResourceHandoverList       = std::nullopt;
  m_CriticalityDiagnostics               = nullptr;
  m_HandoverCommandIes                   = nullptr;

  setMessageType(NgapMessageType::HANDOVER_COMMAND);
  initialize();
}

//------------------------------------------------------------------------------
HandoverCommandMsg::~HandoverCommandMsg() {
  // TODO:
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::initialize() {
  m_HandoverCommandIes =
      &ngapPdu->choice.successfulOutcome->value.choice.HandoverCommand;
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setAmfUeNgapId(const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_HandoverCommandIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_HandoverCommandIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setHandoverType(const long& type) {
  m_HandoverType = type;
  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_HandoverType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_HandoverType;
  ie->value.choice.HandoverType = type;
  int ret = ASN_SEQUENCE_ADD(&m_HandoverCommandIes->protocolIEs.list, ie);

  if (ret != 0) Logger::ngap().error("Encode HandoverType IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setNasSecurityParametersFromNgRan(
    const OCTET_STRING_t& nasSecurityParameters) {
  Ngap_NASSecurityParametersFromNGRAN_t tmp = {};
  amf_conv::octet_string_copy(tmp, nasSecurityParameters);
  m_NasSecurityParametersFromNgRan =
      std::optional<Ngap_NASSecurityParametersFromNGRAN_t>(tmp);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NASSecurityParametersFromNGRAN;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_NASSecurityParametersFromNGRAN;
  if (!amf_conv::octet_string_copy(
          ie->value.choice.NASSecurityParametersFromNGRAN,
          m_NasSecurityParametersFromNgRan.value())) {
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_HandoverCommandIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NASSecurityParametersFromNGRAN IE error");
}

//------------------------------------------------------------------------------
bool HandoverCommandMsg::getNasSecurityParametersFromNgRan(
    OCTET_STRING_t& nasSecurityParameters) const {
  if (!m_NasSecurityParametersFromNgRan.has_value()) return false;
  return amf_conv::octet_string_copy(
      nasSecurityParameters, m_NasSecurityParametersFromNgRan.value());
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setPduSessionResourceHandoverList(
    const PduSessionResourceHandoverList& list) {
  m_PduSessionResourceHandoverList =
      std::optional<PduSessionResourceHandoverList>(list);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList;

  if (!m_PduSessionResourceHandoverList.value().encode(
          ie->value.choice.PDUSessionResourceHandoverList)) {
    Logger::ngap().error("Encode PDUSessionResourceHandoverListItem IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_HandoverCommandIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionResourceHandoverList IE error");
}

//------------------------------------------------------------------------------
bool HandoverCommandMsg::getPduSessionResourceHandoverList(
    PduSessionResourceHandoverList& list) const {
  if (m_PduSessionResourceHandoverList.has_value()) {
    list = m_PduSessionResourceHandoverList.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setPduSessionResourceToReleaseListHOCmd(
    const PduSessionResourceToReleaseListHandoverCmd& list) {
  m_PduSessionResourceToReleaseListHOCmd =
      std::optional<PduSessionResourceToReleaseListHandoverCmd>(list);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListHOCmd;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceToReleaseListHOCmd;

  if (!m_PduSessionResourceToReleaseListHOCmd.value().encode(
          ie->value.choice.PDUSessionResourceToReleaseListHOCmd)) {
    Logger::ngap().error(
        "Encode PDUSessionResourceToReleaseListHOCmd IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(&m_HandoverCommandIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error(
        "Encode PDUSessionResourceToReleaseListHOCmd IE error");
}

//------------------------------------------------------------------------------
bool HandoverCommandMsg::getPduSessionResourceToReleaseListHOCmd(
    PduSessionResourceToReleaseListHandoverCmd& list) const {
  if (!m_PduSessionResourceToReleaseListHOCmd.has_value()) return false;
  list = m_PduSessionResourceToReleaseListHOCmd.value();
  return true;
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::setTargetToSourceTransparentContainer(
    const OCTET_STRING_t& targetTosource) {
  amf_conv::octet_string_copy(
      m_TargetToSourceTransparentContainer, targetTosource);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer;
  amf_conv::octet_string_copy(
      ie->value.choice.TargetToSource_TransparentContainer, targetTosource);

  int ret = ASN_SEQUENCE_ADD(&m_HandoverCommandIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode HandoverType IE error");
}

//------------------------------------------------------------------------------
void HandoverCommandMsg::getTargetToSourceTransparentContainer(
    OCTET_STRING_t& targetTosource) const {
  targetTosource = m_TargetToSourceTransparentContainer;
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
      m_HandoverCommandIes =
          &ngapPdu->choice.successfulOutcome->value.choice.HandoverCommand;
    } else {
      Logger::ngap().error("Check Handover Command message error");
      return false;
    }
  } else {
    Logger::ngap().error("Handover Command MessageType error");
    return false;
  }
  for (int i = 0; i < m_HandoverCommandIes->protocolIEs.list.count; i++) {
    switch (m_HandoverCommandIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_HandoverCommandIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_HandoverCommandIes->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_HandoverCommandIes->protocolIEs.list.array[i]
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
        if (m_HandoverCommandIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_HandoverCommandIes->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_HandoverCommandIes->protocolIEs.list.array[i]
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
        if (m_HandoverCommandIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_HandoverCommandIes->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_HandoverType) {
          m_HandoverType = m_HandoverCommandIes->protocolIEs.list.array[i]
                               ->value.choice.HandoverType;
        } else {
          Logger::ngap().error("Decoded NGAP Handover Type IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList: {
        if (m_HandoverCommandIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_HandoverCommandIes->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList) {
          PduSessionResourceHandoverList tmp = {};
          if (tmp.decode(m_HandoverCommandIes->protocolIEs.list.array[i]
                             ->value.choice.PDUSessionResourceHandoverList)) {
            m_PduSessionResourceHandoverList =
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
        if (m_HandoverCommandIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_HandoverCommandIes->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceToReleaseListHOCmd) {
          PduSessionResourceToReleaseListHandoverCmd tmp = {};
          if (tmp.decode(
                  m_HandoverCommandIes->protocolIEs.list.array[i]
                      ->value.choice.PDUSessionResourceToReleaseListHOCmd)) {
            m_PduSessionResourceToReleaseListHOCmd =
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
        if (m_HandoverCommandIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_HandoverCommandIes->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer) {
        } else {
          Logger::ngap().error(
              "Decoded NGAP TargetToSource_TransparentContainer IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (m_HandoverCommandIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_HandoverCommandIes->protocolIEs.list.array[i]->value.present ==
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
