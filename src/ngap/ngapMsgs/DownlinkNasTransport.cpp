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

#include "DownlinkNasTransport.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
DownLinkNasTransportMsg::DownLinkNasTransportMsg() : NgapUEMessage() {
  downLinkNasTransportIEs = nullptr;
  oldAMF                  = std::nullopt;
  ranPagingPriority       = std::nullopt;
  indexToRFSP             = std::nullopt;

  setMessageType(NgapMessageType::DOWNLINK_NAS_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
DownLinkNasTransportMsg::~DownLinkNasTransportMsg() {}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::initialize() {
  downLinkNasTransportIEs =
      &(ngapPdu->choice.initiatingMessage->value.choice.DownlinkNASTransport);
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setAmfUeNgapId(const unsigned long& id) {
  amfUeNgapId.set(id);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setRanUeNgapId(const uint32_t& ran_ue_ngap_id) {
  ranUeNgapId.set(ran_ue_ngap_id);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setOldAmf(const std::string& name) {
  AmfName tmp = {};
  tmp.setValue(name);
  oldAMF = std::optional<AmfName>(tmp);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_OldAMF;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMFName;

  int ret = oldAMF.value().encode(ie->value.choice.AMFName);
  if (!ret) {
    Logger::ngap().error("Encode oldAmfName IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode oldAmfName IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getOldAmf(std::string& name) {
  if (!oldAMF.has_value()) return false;
  oldAMF.value().getValue(name);
  return true;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::setRanPagingPriority(
    const uint32_t& pagingPriority) {
  RanPagingPriority tmp = {};
  if (!tmp.set(pagingPriority)) return false;
  ranPagingPriority = std::optional<RanPagingPriority>(tmp);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RANPagingPriority;

  int ret =
      ranPagingPriority.value().encode(ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode RANPagingPriority IE error");
    free_wrapper((void**) &ie);
    return false;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) {
    Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getRanPagingPriority(
    uint32_t& ran_paging_priority) {
  if (!ranPagingPriority.has_value()) return false;
  ran_paging_priority = ranPagingPriority.value().get();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setNasPdu(const bstring& pdu) {
  nasPdu.set(pdu);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_NAS_PDU;

  int ret = nasPdu.encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS_PDU IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getNasPdu(bstring& pdu) {
  return nasPdu.get(pdu);
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setMobilityRestrictionList(
    const MobilityRestrictionList& mobility_restriction_list) {
  mobilityRestrictionList =
      std::make_optional<MobilityRestrictionList>(mobility_restriction_list);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MobilityRestrictionList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_DownlinkNASTransport_IEs__value_PR_MobilityRestrictionList;

  int ret = mobilityRestrictionList.value().encode(
      ie->value.choice.MobilityRestrictionList);
  if (!ret) {
    Logger::ngap().error("Encode MobilityRestrictionList IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP MobilityRestrictionList IE error");

  return;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getMobilityRestrictionList(
    MobilityRestrictionList& mobility_restriction_list) const {
  if (!mobilityRestrictionList.has_value()) return false;
  mobility_restriction_list = mobilityRestrictionList.value();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setUEAggregateMaxBitRate(
    const UEAggregateMaxBitRate& bit_rate) {
  uEAggregateMaxBitRate = std::make_optional<UEAggregateMaxBitRate>(bit_rate);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_DownlinkNASTransport_IEs__value_PR_UEAggregateMaximumBitRate;

  int ret = uEAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode UEAggregateMaximumBitRate IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UEAggregateMaximumBitRate IE error");

  return;
};

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getUEAggregateMaxBitRate(
    UEAggregateMaxBitRate& bit_rate) {
  if (!uEAggregateMaxBitRate.has_value()) return false;
  bit_rate = uEAggregateMaxBitRate.value();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setIndex2Rat_FrequencySelectionPriority(
    const uint32_t& value) {
  indexToRFSP = std::make_optional<IndexToRfsp>(value);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_IndexToRFSP;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_IndexToRFSP;

  int ret = indexToRFSP.value().encode(ie->value.choice.IndexToRFSP);
  if (!ret) {
    Logger::ngap().error("Encode IndexToRFSP IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode IndexToRFSP IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getIndex2Rat_FrequencySelectionPriority(
    uint32_t& index) const {
  if (!indexToRFSP.has_value()) return false;

  index = indexToRFSP.value().get();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setAllowedNssai(
    const AllowedNSSAI& allowed_nssai) {
  allowedNssai = std::make_optional<AllowedNSSAI>(allowed_nssai);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AllowedNSSAI;

  int ret = allowedNssai.value().encode(ie->value.choice.AllowedNSSAI);
  if (!ret) {
    Logger::ngap().error("Encode AllowedNSSAI IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&downLinkNasTransportIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getAllowedNssai(
    AllowedNSSAI& allowed_nssai) const {
  if (!allowedNssai.has_value()) return false;
  allowed_nssai = allowedNssai.value();
  return true;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::decode(Ngap_NGAP_PDU_t* ngapMsgPdu) {
  ngapPdu = ngapMsgPdu;

  if (ngapPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngapPdu->choice.initiatingMessage &&
        ngapPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_DownlinkNASTransport &&
        ngapPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        ngapPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_DownlinkNASTransport) {
      downLinkNasTransportIEs =
          &ngapPdu->choice.initiatingMessage->value.choice.DownlinkNASTransport;
    } else {
      Logger::ngap().error("Decode NGAP DownlinkNASTransport error");
      return false;
    }
  } else {
    Logger::ngap().error("Decode NGAP MessageType IE error");
    return false;
  }
  for (int i = 0; i < downLinkNasTransportIEs->protocolIEs.list.count; i++) {
    switch (downLinkNasTransportIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!amfUeNgapId.decode(
                  downLinkNasTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!ranUeNgapId.decode(
                  downLinkNasTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_OldAMF: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AMFName) {
          AmfName tmp = {};
          if (!tmp.decode(downLinkNasTransportIEs->protocolIEs.list.array[i]
                              ->value.choice.AMFName)) {
            Logger::ngap().error("Decode NGAP OldAMFName IE error");
            return false;
          }
          oldAMF = std::optional<AmfName>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP OldAMFName IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_RANPagingPriority) {
          RanPagingPriority tmp = {};
          if (!tmp.decode(downLinkNasTransportIEs->protocolIEs.list.array[i]
                              ->value.choice.RANPagingPriority)) {
            Logger::ngap().error("Decode NGAP RANPagingPriority IE error");
            return false;
          }
          ranPagingPriority = std::optional<RanPagingPriority>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP RANPagingPriority IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_NAS_PDU) {
          if (!nasPdu.decode(downLinkNasTransportIEs->protocolIEs.list.array[i]
                                 ->value.choice.NAS_PDU)) {
            Logger::ngap().error("Decode NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP NAS_PDU IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_MobilityRestrictionList: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_MobilityRestrictionList) {
          MobilityRestrictionList tmp = {};
          if (!tmp.decode(downLinkNasTransportIEs->protocolIEs.list.array[i]
                              ->value.choice.MobilityRestrictionList)) {
            Logger::ngap().error(
                "Decode NGAP MobilityRestrictionList IE error");
            return false;
          }
          mobilityRestrictionList = std::optional<MobilityRestrictionList>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP MobilityRestrictionList IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_IndexToRFSP: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_IndexToRFSP) {
          IndexToRfsp tmp = {};
          if (!tmp.decode(downLinkNasTransportIEs->protocolIEs.list.array[i]
                              ->value.choice.IndexToRFSP)) {
            Logger::ngap().error("Decode NGAP IndexToRFSP IE error");
            return false;
          }
          indexToRFSP = std::optional<IndexToRfsp>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP IndexToRFSP IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_UEAggregateMaximumBitRate) {
          UEAggregateMaxBitRate tmp = {};
          if (!tmp.decode(downLinkNasTransportIEs->protocolIEs.list.array[i]
                              ->value.choice.UEAggregateMaximumBitRate)) {
            Logger::ngap().error(
                "Decode NGAP UEAggregateMaximumBitRate IE error");
            return false;
          }
          uEAggregateMaxBitRate = std::optional<UEAggregateMaxBitRate>(tmp);
        } else {
          Logger::ngap().error(
              "Decode NGAP UEAggregateMaximumBitRate IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        if (downLinkNasTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            downLinkNasTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AllowedNSSAI) {
          AllowedNSSAI tmp = {};
          if (!tmp.decode(downLinkNasTransportIEs->protocolIEs.list.array[i]
                              ->value.choice.AllowedNSSAI)) {
            Logger::ngap().error("Decode NGAP AllowedNSSAI IE error");
            return false;
          }
          allowedNssai = std::optional<AllowedNSSAI>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP AllowedNSSAI IE error");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error("Decode NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}

}  // namespace ngap
