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
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DownLinkNasTransportMsg::DownLinkNasTransportMsg() : NgapUeMessage() {
  m_DownLinkNasTransportIes = nullptr;
  m_OldAmf                  = std::nullopt;
  m_RanPagingPriority       = std::nullopt;
  m_IndexToRfsp             = std::nullopt;

  setMessageType(NgapMessageType::DOWNLINK_NAS_TRANSPORT);
  initialize();
}

//------------------------------------------------------------------------------
DownLinkNasTransportMsg::~DownLinkNasTransportMsg() {}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::initialize() {
  m_DownLinkNasTransportIes =
      &(ngapPdu->choice.initiatingMessage->value.choice.DownlinkNASTransport);
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setAmfUeNgapId(const unsigned long& id) {
  NgapUeMessage::m_AmfUeNgapId.set(id);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_AmfUeNgapId.encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setRanUeNgapId(const uint32_t& ranUeNgapId) {
  NgapUeMessage::m_RanUeNgapId.set(ranUeNgapId);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret =
      NgapUeMessage::m_RanUeNgapId.encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setOldAmf(const std::string& name) {
  AmfName tmp = {};
  tmp.set(name);
  m_OldAmf = std::optional<AmfName>(tmp);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_OldAMF;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMFName;

  int ret = m_OldAmf.value().encode(ie->value.choice.AMFName);
  if (!ret) {
    Logger::ngap().error("Encode oldAmfName IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode oldAmfName IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getOldAmf(std::string& name) const {
  if (!m_OldAmf.has_value()) return false;
  m_OldAmf.value().get(name);
  return true;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::setRanPagingPriority(
    const uint32_t& pagingPriority) {
  RanPagingPriority tmp = {};
  if (!tmp.set(pagingPriority)) return false;
  m_RanPagingPriority = std::optional<RanPagingPriority>(tmp);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RANPagingPriority;

  int ret =
      m_RanPagingPriority.value().encode(ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::ngap().error("Encode RANPagingPriority IE error");
    utils::free_wrapper((void**) &ie);
    return false;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) {
    Logger::ngap().error("Encode NGAP RANPagingPriority IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getRanPagingPriority(
    uint32_t& ranPagingPriority) const {
  if (!m_RanPagingPriority.has_value()) return false;
  ranPagingPriority = m_RanPagingPriority.value().get();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setNasPdu(const bstring& pdu) {
  m_NasPdu.set(pdu);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_NAS_PDU;

  int ret = m_NasPdu.encode(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::ngap().error("Encode NAS_PDU IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NAS_PDU IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getNasPdu(bstring& pdu) const {
  return m_NasPdu.get(pdu);
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setMobilityRestrictionList(
    const MobilityRestrictionList& mobilityRestrictionList) {
  m_MobilityRestrictionList =
      std::make_optional<MobilityRestrictionList>(mobilityRestrictionList);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_MobilityRestrictionList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_DownlinkNASTransport_IEs__value_PR_MobilityRestrictionList;

  int ret = m_MobilityRestrictionList.value().encode(
      ie->value.choice.MobilityRestrictionList);
  if (!ret) {
    Logger::ngap().error("Encode MobilityRestrictionList IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP MobilityRestrictionList IE error");

  return;
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getMobilityRestrictionList(
    MobilityRestrictionList& mobilityRestrictionList) const {
  if (!m_MobilityRestrictionList.has_value()) return false;
  mobilityRestrictionList = m_MobilityRestrictionList.value();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setUeAggregateMaxBitRate(
    const UeAggregateMaxBitRate& bitRate) {
  m_UeAggregateMaxBitRate = std::make_optional<UeAggregateMaxBitRate>(bitRate);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_DownlinkNASTransport_IEs__value_PR_UEAggregateMaximumBitRate;

  int ret = m_UeAggregateMaxBitRate.value().encode(
      ie->value.choice.UEAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode UEAggregateMaximumBitRate IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode NGAP UEAggregateMaximumBitRate IE error");

  return;
};

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getUeAggregateMaxBitRate(
    UeAggregateMaxBitRate& bitRate) const {
  if (!m_UeAggregateMaxBitRate.has_value()) return false;
  bitRate = m_UeAggregateMaxBitRate.value();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setIndex2RatFrequencySelectionPriority(
    const uint32_t& value) {
  m_IndexToRfsp = std::make_optional<IndexToRfsp>(value);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_IndexToRFSP;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_IndexToRFSP;

  int ret = m_IndexToRfsp.value().encode(ie->value.choice.IndexToRFSP);
  if (!ret) {
    Logger::ngap().error("Encode IndexToRFSP IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode IndexToRFSP IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getIndex2RatFrequencySelectionPriority(
    uint32_t& index) const {
  if (!m_IndexToRfsp.has_value()) return false;

  index = m_IndexToRfsp.value().get();
  return true;
}

//------------------------------------------------------------------------------
void DownLinkNasTransportMsg::setAllowedNssai(
    const AllowedNSSAI& allowedNssai) {
  m_AllowedNssai = std::make_optional<AllowedNSSAI>(allowedNssai);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AllowedNSSAI;

  int ret = m_AllowedNssai.value().encode(ie->value.choice.AllowedNSSAI);
  if (!ret) {
    Logger::ngap().error("Encode AllowedNSSAI IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&m_DownLinkNasTransportIes->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode AllowedNSSAI IE error");
}

//------------------------------------------------------------------------------
bool DownLinkNasTransportMsg::getAllowedNssai(
    AllowedNSSAI& allowedNssai) const {
  if (!m_AllowedNssai.has_value()) return false;
  allowedNssai = m_AllowedNssai.value();
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
      m_DownLinkNasTransportIes =
          &ngapPdu->choice.initiatingMessage->value.choice.DownlinkNASTransport;
    } else {
      Logger::ngap().error("Decode NGAP DownlinkNASTransport error");
      return false;
    }
  } else {
    Logger::ngap().error("Decode NGAP MessageType IE error");
    return false;
  }
  for (int i = 0; i < m_DownLinkNasTransportIes->protocolIEs.list.count; i++) {
    switch (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID) {
          if (!NgapUeMessage::m_AmfUeNgapId.decode(
                  m_DownLinkNasTransportIes->protocolIEs.list.array[i]
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
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID) {
          if (!NgapUeMessage::m_RanUeNgapId.decode(
                  m_DownLinkNasTransportIes->protocolIEs.list.array[i]
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
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AMFName) {
          AmfName tmp = {};
          if (!tmp.decode(m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                              ->value.choice.AMFName)) {
            Logger::ngap().error("Decode NGAP OldAMFName IE error");
            return false;
          }
          m_OldAmf = std::optional<AmfName>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP OldAMFName IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_RANPagingPriority) {
          RanPagingPriority tmp = {};
          if (!tmp.decode(m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                              ->value.choice.RANPagingPriority)) {
            Logger::ngap().error("Decode NGAP RANPagingPriority IE error");
            return false;
          }
          m_RanPagingPriority = std::optional<RanPagingPriority>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP RANPagingPriority IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_NAS_PDU) {
          if (!m_NasPdu.decode(
                  m_DownLinkNasTransportIes->protocolIEs.list.array[i]
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
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_MobilityRestrictionList) {
          MobilityRestrictionList tmp = {};
          if (!tmp.decode(m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                              ->value.choice.MobilityRestrictionList)) {
            Logger::ngap().error(
                "Decode NGAP MobilityRestrictionList IE error");
            return false;
          }
          m_MobilityRestrictionList =
              std::optional<MobilityRestrictionList>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP MobilityRestrictionList IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_IndexToRFSP: {
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_IndexToRFSP) {
          IndexToRfsp tmp = {};
          if (!tmp.decode(m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                              ->value.choice.IndexToRFSP)) {
            Logger::ngap().error("Decode NGAP IndexToRFSP IE error");
            return false;
          }
          m_IndexToRfsp = std::optional<IndexToRfsp>(tmp);
        } else {
          Logger::ngap().error("Decode NGAP IndexToRFSP IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate: {
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_UEAggregateMaximumBitRate) {
          UeAggregateMaxBitRate tmp = {};
          if (!tmp.decode(m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                              ->value.choice.UEAggregateMaximumBitRate)) {
            Logger::ngap().error(
                "Decode NGAP UEAggregateMaximumBitRate IE error");
            return false;
          }
          m_UeAggregateMaxBitRate = std::optional<UeAggregateMaxBitRate>(tmp);
        } else {
          Logger::ngap().error(
              "Decode NGAP UEAggregateMaximumBitRate IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_AllowedNSSAI: {
        if (m_DownLinkNasTransportIes->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_DownlinkNASTransport_IEs__value_PR_AllowedNSSAI) {
          AllowedNSSAI tmp = {};
          if (!tmp.decode(m_DownLinkNasTransportIes->protocolIEs.list.array[i]
                              ->value.choice.AllowedNSSAI)) {
            Logger::ngap().error("Decode NGAP AllowedNSSAI IE error");
            return false;
          }
          m_AllowedNssai = std::optional<AllowedNSSAI>(tmp);
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
