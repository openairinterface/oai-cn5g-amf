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

#include "PduSessionResourceReleaseCommand.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseCommandMsg::PduSessionResourceReleaseCommandMsg()
    : NgapUEMessage() {
  pduSessionResourceReleaseCommandIEs = nullptr;
  ranPagingPriority                   = std::nullopt;
  nasPdu                              = std::nullopt;

  setMessageType(NgapMessageType::PDU_SESSION_RESOURCE_RELEASE_COMMAND);
  initialize();
}

//------------------------------------------------------------------------------
PduSessionResourceReleaseCommandMsg::~PduSessionResourceReleaseCommandMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::initialize() {
  pduSessionResourceReleaseCommandIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .PDUSessionResourceReleaseCommand);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setRanPagingPriority(
    const uint32_t& priority) {
  RanPagingPriority tmp = {};
  tmp.set(priority);
  ranPagingPriority = std::optional<RanPagingPriority>(tmp);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RANPagingPriority;

  int ret =
      ranPagingPriority.value().encode(ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::nas_mm().warn("Encode RANPagingPriority IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RANPagingPriority IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::getRanPagingPriority(
    uint32_t& priority) {
  if (!ranPagingPriority.has_value()) return false;
  priority = ranPagingPriority.value().get();
  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setNasPdu(const bstring& pdu) {
  NasPdu tmp = {};
  tmp.set(pdu);
  nasPdu = std::optional<NasPdu>(tmp);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU;

  int ret = nasPdu.value().encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::nas_mm().warn("encode NAS_PDU IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::getNasPdu(bstring& pdu) {
  if (!nasPdu.has_value()) return false;
  return nasPdu.value().get(pdu);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setPduSessionResourceToReleaseList(
    const std::vector<PDUSessionResourceToReleaseItem_t>& list) {
  std::vector<PduSessionResourceToReleaseItemRelCmd> item_rel_cmd_list;

  for (int i = 0; i < list.size(); i++) {
    PduSessionResourceToReleaseItemRelCmd item_rel_cmd = {};
    PduSessionId pdu_session_id                        = {};
    pdu_session_id.set(list[i].pduSessionId);
    item_rel_cmd.set(
        pdu_session_id, list[i].pduSessionResourceReleaseCommandTransfer);
    item_rel_cmd_list.push_back(item_rel_cmd);
  }

  pduSessionResourceToReleaseList.set(item_rel_cmd_list);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd;

  int ret = pduSessionResourceToReleaseList.encode(
      ie->value.choice.PDUSessionResourceToReleaseListRelCmd);
  if (!ret) {
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceToReleaseListRelCmd IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceToReleaseListRelCmd IE error");
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::getPduSessionResourceToReleaseList(
    std::vector<PDUSessionResourceToReleaseItem_t>& list) {
  std::vector<PduSessionResourceToReleaseItemRelCmd> item_rel_cmd_list;
  pduSessionResourceToReleaseList.get(item_rel_cmd_list);

  for (auto& item : item_rel_cmd_list) {
    PDUSessionResourceToReleaseItem_t rel = {};
    PduSessionId pdu_session_id           = {};

    item.get(pdu_session_id, rel.pduSessionResourceReleaseCommandTransfer);
    pdu_session_id.get(rel.pduSessionId);

    list.push_back(rel);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceRelease &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand) {
      pduSessionResourceReleaseCommandIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .PDUSessionResourceReleaseCommand;
    } else {
      Logger::nas_mm().warn(
          "Check PDUSessionResourceReleaseCommand message error!");
      return false;
    }
  } else {
    Logger::nas_mm().warn("MessageType error");
    return false;
  }

  for (int i = 0;
       i < pduSessionResourceReleaseCommandIEs->protocolIEs.list.count; i++) {
    switch (
        pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RANPagingPriority) {
          RanPagingPriority tmp = {};
          if (!tmp.decode(pduSessionResourceReleaseCommandIEs->protocolIEs.list
                              .array[i]
                              ->value.choice.RANPagingPriority)) {
            Logger::nas_mm().warn("Decoded NGAP RANPagingPriority IE error");
            return false;
          }
          ranPagingPriority = std::optional<RanPagingPriority>(tmp);
        } else {
          Logger::nas_mm().warn("Decoded NGAP RANPagingPriority IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU) {
          NasPdu tmp = {};
          if (!tmp.decode(pduSessionResourceReleaseCommandIEs->protocolIEs.list
                              .array[i]
                              ->value.choice.NAS_PDU)) {
            Logger::nas_mm().warn("Decoded NGAP NAS_PDU IE error");
            return false;
          }
          nasPdu = std::optional<NasPdu>(tmp);
        } else {
          Logger::nas_mm().warn("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd) {
          if (!pduSessionResourceToReleaseList.decode(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionResourceToReleaseListRelCmd)) {
            Logger::nas_mm().warn(
                "Decoded NGAP PDUSessionResourceToReleaseListRelCmd IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn(
              "Decoded NGAP PDUSessionResourceToReleaseListRelCmd IE error");
          return false;
        }
      } break;
      default: {
        Logger::nas_mm().warn("Decoded NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}
}  // namespace ngap
