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

#include "UERadioCapabilityInfoIndication.hpp"

#include "amf_conversions.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UeRadioCapabilityInfoIndicationMsg::UeRadioCapabilityInfoIndicationMsg()
    : NgapUEMessage() {
  ueRadioCapabilityInfoIndicationIEs = nullptr;
  ueRadioCapabilityForPaging         = std::nullopt;

  setMessageType(NgapMessageType::UE_RADIO_CAPABILITY_INFO_INDICATION);
  initialize();
}

//------------------------------------------------------------------------------
UeRadioCapabilityInfoIndicationMsg::~UeRadioCapabilityInfoIndicationMsg() {}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::initialize() {
  ueRadioCapabilityInfoIndicationIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice
            .UERadioCapabilityInfoIndication);
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setAmfUeNgapId(
    const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_UERadioCapabilityInfoIndicationIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setRanUeNgapId(
    const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_UERadioCapabilityInfoIndicationIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setUERadioCapability(
    const OCTET_STRING_t& capability) {
  ueRadioCapability.set(capability);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapability;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapability;

  if (!ueRadioCapability.encode(ie->value.choice.UERadioCapability)) {
    Logger::ngap().error("Encode NGAP UERadioCapability IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  int ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NGAP UERadioCapability IE error");
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::getUERadioCapability(
    OCTET_STRING_t& capability) {
  ueRadioCapability.get(capability);
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setUERadioCapabilityForPaging(
    const OCTET_STRING_t& ue_radio_capability_for_paging_of_nr,
    const OCTET_STRING_t& ue_radio_capability_for_paging_of_eutra) {
  if (!(amf_conv::check_octet_string(ue_radio_capability_for_paging_of_nr) or
        amf_conv::check_octet_string(
            ue_radio_capability_for_paging_of_eutra))) {
    return;
  }
  UERadioCapabilityForPaging tmp = {};

  if (amf_conv::check_octet_string(ue_radio_capability_for_paging_of_nr)) {
    tmp.setUERadioCapabilityForPagingOfNR(ue_radio_capability_for_paging_of_nr);
  }
  if (amf_conv::check_octet_string(ue_radio_capability_for_paging_of_eutra)) {
    tmp.setUERadioCapabilityForPagingOfEUTRA(
        ue_radio_capability_for_paging_of_eutra);
  }
  ueRadioCapabilityForPaging = std::optional<UERadioCapabilityForPaging>(tmp);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_UERadioCapabilityInfoIndicationIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapabilityForPaging;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapabilityForPaging;

  int ret = ueRadioCapabilityForPaging.value().encode(
      ie->value.choice.UERadioCapabilityForPaging);
  if (!ret) {
    Logger::ngap().error("Encode NGAP UERadioCapabilityForPaging IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UERadioCapabilityForPaging IE error");
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityInfoIndicationMsg::getUERadioCapabilityForPaging(
    OCTET_STRING_t& ue_radio_capability_for_paging_of_nr,
    OCTET_STRING_t& ue_radio_capability_for_paging_of_eutra) {
  if (!ueRadioCapabilityForPaging.has_value()) return false;
  ueRadioCapabilityForPaging.value().getUERadioCapabilityForPagingOfNR(
      ue_radio_capability_for_paging_of_nr);
  ueRadioCapabilityForPaging.value().getUERadioCapabilityForPagingOfEUTRA(
      ue_radio_capability_for_paging_of_eutra);
  return true;
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityInfoIndicationMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UERadioCapabilityInfoIndication &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UERadioCapabilityInfoIndication) {
      ueRadioCapabilityInfoIndicationIEs =
          &ngapPdu->choice.initiatingMessage->value.choice
               .UERadioCapabilityInfoIndication;
    } else {
      Logger::ngap().error(
          "Check UERadioCapabilityInfoIndication message error!");
      return false;
    }
  } else {
    Logger::ngap().error("MessageType error!");
    return false;
  }
  for (int i = 0;
       i < ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.count; i++) {
    switch (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
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
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UERadioCapability: {
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapability) {
          ueRadioCapability.set(
              ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                  ->value.choice.UERadioCapability);
        } else {
          Logger::ngap().error("Decoded NGAP UERadioCapability IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UERadioCapabilityForPaging: {
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapabilityForPaging) {
          UERadioCapabilityForPaging tmp = {};
          if (!tmp.decode(
                  ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                      ->value.choice.UERadioCapabilityForPaging)) {
            Logger::ngap().error(
                "Decoded NGAP UERadioCapabilityForPaging IE error");
            return false;
          }
          ueRadioCapabilityForPaging =
              std::optional<UERadioCapabilityForPaging>(tmp);
        } else {
          Logger::ngap().error(
              "Decoded NGAP UERadioCapabilityForPaging IE error");
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
