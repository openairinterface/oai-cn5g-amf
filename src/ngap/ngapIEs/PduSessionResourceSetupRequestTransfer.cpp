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

#include "PduSessionResourceSetupRequestTransfer.hpp"

#include "logger.hpp"
#include "output_wrapper.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestTransferIE::
    PduSessionResourceSetupRequestTransferIE() {
  m_PduSessionResourceSetupRequestTransferIe =
      (Ngap_PDUSessionResourceSetupRequestTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransfer_t));
  m_PduSessionAggregateMaximumBitRateIe = std::nullopt;
  m_DataForwardingNotPossible           = std::nullopt;
  m_SecurityIndication                  = std::nullopt;
  m_NetworkInstance                     = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestTransferIE::
    ~PduSessionResourceSetupRequestTransferIE() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::
    setPduSessionAggregateMaximumBitRate(
        const long& bitRateDl, const long& bitRateUl) {
  m_PduSessionAggregateMaximumBitRateIe =
      std::make_optional<PduSessionAggregateMaximumBitRate>(
          bitRateDl, bitRateUl);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;

  int ret = m_PduSessionAggregateMaximumBitRateIe.value().encode(
      ie->value.choice.PDUSessionAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionAggregateMaximumBitRate IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionAggregateMaximumBitRate IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setUlNgUUpTnlInformation(
    GtpTunnel_t upTnlInfo) {
  TransportLayerAddress transportLayerAddress = {};
  transportLayerAddress.set(upTnlInfo.ipAddress);
  GtpTeid gtpTeid = {};
  gtpTeid.set(upTnlInfo.gtpTeid);
  m_UpTransportLayerInformation.set(transportLayerAddress, gtpTeid);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation;

  int ret = m_UpTransportLayerInformation.encode(
      ie->value.choice.UPTransportLayerInformation);
  if (!ret) {
    Logger::ngap().error("Encode UPTransportLayerInformation IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode UPTransportLayerInformation IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setDataForwardingNotPossible() {
  DataForwardingNotPossible tmp = {};

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_DataForwardingNotPossible;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_DataForwardingNotPossible;

  int ret = tmp.encode(ie->value.choice.DataForwardingNotPossible);
  m_DataForwardingNotPossible =
      std::make_optional<DataForwardingNotPossible>(tmp);

  if (!ret) {
    Logger::ngap().error("Encode DataForwardingNotPossible IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode DataForwardingNotPossible IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setPduSessionType(
    e_Ngap_PDUSessionType type) {
  m_PduSessionType.set(type);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionType;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType;

  int ret = m_PduSessionType.encode(ie->value.choice.PDUSessionType);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionType IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode PDUSessionType IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setSecurityIndication(
    e_Ngap_IntegrityProtectionIndication eIntegrityProtectionIndication,
    e_Ngap_ConfidentialityProtectionIndication
        eConfidentialityProtectionIndication,
    e_Ngap_MaximumIntegrityProtectedDataRate
        eMaximumIntegrityProtectedDataRate) {
  IntegrityProtectionIndication integrityProtectionIndication             = {};
  ConfidentialityProtectionIndication confidentialityProtectionIndication = {};
  std::optional<MaximumIntegrityProtectedDataRate>
      maximumIntegrityProtectedDataRate =
          std::make_optional<MaximumIntegrityProtectedDataRate>(
              eMaximumIntegrityProtectedDataRate);
  integrityProtectionIndication.set(eIntegrityProtectionIndication);
  confidentialityProtectionIndication.set(eConfidentialityProtectionIndication);

  m_SecurityIndication = std::make_optional<SecurityIndication>(
      integrityProtectionIndication, confidentialityProtectionIndication,
      maximumIntegrityProtectedDataRate, maximumIntegrityProtectedDataRate);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SecurityIndication;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication;

  int ret =
      m_SecurityIndication.value().encode(ie->value.choice.SecurityIndication);
  if (!ret) {
    Logger::ngap().error("Encode SecurityIndication IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityIndication IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setSecurityIndication(
    e_Ngap_IntegrityProtectionIndication eIntegrityProtectionIndication,
    e_Ngap_ConfidentialityProtectionIndication
        eConfidentialityProtectionIndication) {
  IntegrityProtectionIndication integrityProtectionIndication             = {};
  ConfidentialityProtectionIndication confidentialityProtectionIndication = {};

  integrityProtectionIndication.set(eIntegrityProtectionIndication);
  confidentialityProtectionIndication.set(eConfidentialityProtectionIndication);

  m_SecurityIndication = std::make_optional<SecurityIndication>(
      integrityProtectionIndication, confidentialityProtectionIndication,
      std::nullopt, std::nullopt);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SecurityIndication;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication;

  int ret =
      m_SecurityIndication.value().encode(ie->value.choice.SecurityIndication);
  if (!ret) {
    Logger::ngap().error("Encode SecurityIndication IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityIndication IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setNetworkInstance(
    const long& value) {
  m_NetworkInstance = std::make_optional<NetworkInstance>(value);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NetworkInstance;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_NetworkInstance;

  int ret = m_NetworkInstance.value().encode(ie->value.choice.NetworkInstance);
  if (!ret) {
    Logger::ngap().error("Encode NetworkInstance IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NetworkInstance IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setQosFlowSetupRequestList(
    std::vector<QosFlowSetupReq_t> list) {
  std::vector<QosFlowSetupRequestItem> itemListVector;

  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier qosFlowIdentifier = {};
    qosFlowIdentifier.set(list[i].qosFlowId);

    QosCharacteristics qosCharacteristics = {};
    if (list[i].qflqp.qosc.nonDynamic5qi) {
      FiveQI fiveqi                                    = {};
      std::optional<PriorityLevelQos> priorityLevelQos = std::nullopt;
      std::optional<AveragingWindow> averaging_window  = std::nullopt;
      std::optional<MaximumDataBurstVolume> maximumDataBurstVolume =
          std::nullopt;

      fiveqi.set(list[i].qflqp.qosc.nonDynamic5qi->_5qi);
      if (list[i].qflqp.qosc.nonDynamic5qi->priorityLevelQos) {
        PriorityLevelQos tmp = {};
        tmp.set(*list[i].qflqp.qosc.nonDynamic5qi->priorityLevelQos);
        priorityLevelQos = std::make_optional<PriorityLevelQos>(tmp);
      }
      if (list[i].qflqp.qosc.nonDynamic5qi->averagingWindow) {
        AveragingWindow tmp = {};
        tmp.set(*list[i].qflqp.qosc.nonDynamic5qi->averagingWindow);
        averaging_window = std::make_optional<AveragingWindow>(tmp);
      }
      if (list[i].qflqp.qosc.nonDynamic5qi->maximumDataBurstVolume) {
        MaximumDataBurstVolume tmp = {};
        tmp.set(*list[i].qflqp.qosc.nonDynamic5qi->maximumDataBurstVolume);
        maximumDataBurstVolume =
            std::make_optional<MaximumDataBurstVolume>(tmp);
      }

      NonDynamic5qiDescriptor nonDynamic5qiDescriptor = {};
      nonDynamic5qiDescriptor.set(
          fiveqi, priorityLevelQos, averaging_window, maximumDataBurstVolume);

      qosCharacteristics.set(nonDynamic5qiDescriptor);
    } else {
      PriorityLevelQos priorityLevelQos               = {};
      PacketDelayBudget packetDelayBudget             = {};
      PacketErrorRate packetErrorRate                 = {};
      std::optional<FiveQI> fiveqi                    = std::nullopt;
      std::optional<DelayCritical> delayCritical      = std::nullopt;
      std::optional<AveragingWindow> averaging_window = std::nullopt;
      std::optional<MaximumDataBurstVolume> maximumDataBurstVolume =
          std::nullopt;

      priorityLevelQos.set(list[i].qflqp.qosc.dynamic5qi->priorityLevelQos);
      packetDelayBudget.set(list[i].qflqp.qosc.dynamic5qi->packetDelayBudget);
      packetErrorRate.set(
          list[i].qflqp.qosc.dynamic5qi->packetErrorRate.scalar,
          list[i].qflqp.qosc.dynamic5qi->packetErrorRate.exponent);
      if (list[i].qflqp.qosc.dynamic5qi->_5qi) {
        FiveQI tmp = {};
        tmp.set(*list[i].qflqp.qosc.dynamic5qi->_5qi);
        fiveqi = std::make_optional<FiveQI>(tmp);
      }
      if (list[i].qflqp.qosc.dynamic5qi->delayCritical) {
        DelayCritical tmp = {};
        tmp.set(*list[i].qflqp.qosc.dynamic5qi->delayCritical);
        delayCritical = std::make_optional<DelayCritical>(tmp);
      }
      if (list[i].qflqp.qosc.dynamic5qi->averagingWindow) {
        AveragingWindow tmp = {};
        tmp.set(*list[i].qflqp.qosc.dynamic5qi->averagingWindow);
        averaging_window = std::make_optional<AveragingWindow>(tmp);
      }
      if (list[i].qflqp.qosc.dynamic5qi->maximumDataBurstVolume) {
        MaximumDataBurstVolume tmp = {};
        tmp.set(*list[i].qflqp.qosc.dynamic5qi->maximumDataBurstVolume);
        maximumDataBurstVolume =
            std::make_optional<MaximumDataBurstVolume>(tmp);
      }

      Dynamic5qiDescriptor dynamic5qiDescriptor = {};
      dynamic5qiDescriptor.set(
          priorityLevelQos, packetDelayBudget, packetErrorRate, fiveqi,
          delayCritical, averaging_window, maximumDataBurstVolume);

      qosCharacteristics.set(dynamic5qiDescriptor);
    }

    PriorityLevelARP priorityLevelArp = {};
    priorityLevelArp.set(list[i].qflqp.arp.priorityLevelArp);
    Pre_emptionCapability pre_emptionCapability = {};
    pre_emptionCapability.set(list[i].qflqp.arp.pre_emptionCapability);
    Pre_emptionVulnerability pre_emptionVulnerability = {};
    pre_emptionVulnerability.set(list[i].qflqp.arp.pre_emptionVulnerability);
    AllocationAndRetentionPriority arp = {};
    arp.set(priorityLevelArp, pre_emptionCapability, pre_emptionVulnerability);

    std::optional<GbrQosFlowInformation> gbrQosFlowInformation = std::nullopt;
    if (list[i].qflqp.gbrQosInformation) {
      std::optional<NotificationControl> m_notificationControl = std::nullopt;
      if (list[i].qflqp.gbrQosInformation->notificationControl) {
        NotificationControl tmp = {};
        tmp.set(*list[i].qflqp.gbrQosInformation->notificationControl);
        m_notificationControl = std::make_optional<NotificationControl>(tmp);
      }

      std::optional<PacketLossRate> maxPacketLossRateDl = std::nullopt;
      if (list[i].qflqp.gbrQosInformation->maximumPacketLossRateDl) {
        PacketLossRate tmp = {};
        tmp.set(*list[i].qflqp.gbrQosInformation->maximumPacketLossRateDl);
        maxPacketLossRateDl = std::make_optional<PacketLossRate>(tmp);
      }

      std::optional<PacketLossRate> maxPacketLossRateUl = std::nullopt;
      if (list[i].qflqp.gbrQosInformation->maximumPacketLossRateUl) {
        PacketLossRate tmp = {};
        tmp.set(*list[i].qflqp.gbrQosInformation->maximumPacketLossRateUl);
        maxPacketLossRateUl = std::make_optional<PacketLossRate>(tmp);
      }

      gbrQosFlowInformation = std::make_optional<GbrQosFlowInformation>(
          list[i].qflqp.gbrQosInformation->maximumFlowBitRateDl,
          list[i].qflqp.gbrQosInformation->maximumFlowBitRateUl,
          list[i].qflqp.gbrQosInformation->guaranteedFlowBitRateDl,
          list[i].qflqp.gbrQosInformation->guaranteedFlowBitRateUl,
          m_notificationControl, maxPacketLossRateDl, maxPacketLossRateUl);
    }

    std::optional<ReflectiveQosAttribute> reflectiveQosAttribute = std::nullopt;
    if (list[i].qflqp.reflectiveQosAttribute) {
      reflectiveQosAttribute = std::make_optional<ReflectiveQosAttribute>(
          *list[i].qflqp.reflectiveQosAttribute);
    }

    std::optional<AdditionalQosFlowInformation> additionalQosFlowInformation =
        std::nullopt;
    if (list[i].qflqp.additionalQosFlowInformation) {
      additionalQosFlowInformation =
          std::make_optional<AdditionalQosFlowInformation>(
              *list[i].qflqp.additionalQosFlowInformation);
    }

    QosFlowLevelQosParameters qosFlowLevelQosParameters = {};
    qosFlowLevelQosParameters.set(
        qosCharacteristics, arp, gbrQosFlowInformation, reflectiveQosAttribute,
        additionalQosFlowInformation);

    QosFlowSetupRequestItem qosFlowSetupRequestItem = {};
    qosFlowSetupRequestItem.set(qosFlowIdentifier, qosFlowLevelQosParameters);
    itemListVector.push_back(qosFlowSetupRequestItem);
  }

  m_QosFlowSetupRequestList.set(itemListVector);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

  int ret = m_QosFlowSetupRequestList.encode(
      ie->value.choice.QosFlowSetupRequestList);
  if (!ret) {
    Logger::ngap().error("Encode QosFlowSetupRequestList IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode QosFlowSetupRequestList IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupRequestTransferIE::encode(
    uint8_t* buf, int buf_size) {
  output_wrapper::print_asn_msg(
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer,
      m_PduSessionResourceSetupRequestTransferIe);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer, NULL,
      m_PduSessionResourceSetupRequestTransferIe, buf, buf_size);
  Logger::ngap().debug("er.encoded( %d)", er.encoded);
  // asn_fprint(stderr, er.failed_type, er.structure_ptr);
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceSetupRequestTransferIE::decode(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer,
      (void**) &m_PduSessionResourceSetupRequestTransferIe, buf, buf_size);
  if (rc.code == RC_OK) {
    Logger::ngap().debug("Decoded successfully");
  } else if (rc.code == RC_WMORE) {
    Logger::ngap().debug("More data expected, call again");
    return false;
  } else {
    Logger::ngap().debug("Failure to decode data");
    return false;
  }

  // asn_fprint(stderr, &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer,
  // m_PduSessionResourceSetupRequestTransferIe);

  for (int i = 0;
       i < m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list.count;
       i++) {
    switch (
        m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list.array[i]
            ->id) {
      case Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate: {
        if (m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate) {
          PduSessionAggregateMaximumBitRate aggregate_maximum_bit_rate = {};

          if (!aggregate_maximum_bit_rate.decode(
                  m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionAggregateMaximumBitRate)) {
            Logger::ngap().error(
                "Decode NGAP PDUSessionAggregateMaximumBitRate IE error");
            return false;
          }
          m_PduSessionAggregateMaximumBitRateIe =
              std::make_optional<PduSessionAggregateMaximumBitRate>(
                  aggregate_maximum_bit_rate);
        } else {
          Logger::ngap().error(
              "Decode NGAP PDUSessionAggregateMaximumBitRate IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation: {
        if (m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation) {
          if (!m_UpTransportLayerInformation.decode(
                  m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                      .array[i]
                      ->value.choice.UPTransportLayerInformation)) {
            Logger::ngap().error(
                "Decode NGAP UPTransportLayerInformation IE error");

            return false;
          }
        } else {
          Logger::ngap().error(
              "Decode NGAP UPTransportLayerInformation IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_DataForwardingNotPossible: {
        if ((m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                 .array[i]
                 ->criticality) == Ngap_Criticality_reject &&
            (m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                 .array[i]
                 ->value.present ==
             Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_DataForwardingNotPossible)) {
          DataForwardingNotPossible data_forwarding_not_possible = {};
          if (!data_forwarding_not_possible.decode(
                  m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                      .array[i]
                      ->value.choice.DataForwardingNotPossible)) {
            Logger::ngap().error(
                "Decode NGAP DataForwardingNotPossible IE error");
            return false;
          }
          m_DataForwardingNotPossible =
              std::make_optional<DataForwardingNotPossible>(
                  data_forwarding_not_possible);
        } else {
          Logger::ngap().error(
              "Decode NGAP DataForwardingNotPossible IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionType: {
        if (m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType) {
          if (!m_PduSessionType.decode(
                  m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionType)) {
            Logger::ngap().error("Decode NGAP PDUSessionType IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP PDUSessionType IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_SecurityIndication: {
        if (m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication) {
          SecurityIndication security_indication = {};
          if (!security_indication.decode(
                  m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                      .array[i]
                      ->value.choice.SecurityIndication)) {
            Logger::ngap().error("Decode NGAP SecurityIndication IE error");

            return false;
          }
          m_SecurityIndication =
              std::make_optional<SecurityIndication>(security_indication);
        } else {
          Logger::ngap().error("Decode NGAP SecurityIndication IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NetworkInstance: {
        if (m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_NetworkInstance) {
          NetworkInstance network_instance = {};
          if (!network_instance.decode(
                  m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                      .array[i]
                      ->value.choice.NetworkInstance)) {
            Logger::ngap().error("Decode NGAP NetworkInstance IE error");
            return false;
          }
          m_NetworkInstance =
              std::make_optional<NetworkInstance>(network_instance);
        } else {
          Logger::ngap().error("Decode NGAP NetworkInstance IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList: {
        if (m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList) {
          if (!m_QosFlowSetupRequestList.decode(
                  m_PduSessionResourceSetupRequestTransferIe->protocolIEs.list
                      .array[i]
                      ->value.choice.QosFlowSetupRequestList)) {
            Logger::ngap().error(
                "Decode NGAP QosFlowSetupRequestList IE error");
            return false;
          }
        } else {
          Logger::ngap().error("Decode NGAP QosFlowSetupRequestList IE error");
          return false;
        }
      } break;

      default: {
        Logger::ngap().error(
            "Decode NGAP message PduSessionResourceSetupRequestTransferIE "
            "error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::
    getPduSessionAggregateMaximumBitRate(
        long& bitRateDl, long& bitRateUl) const {
  if (!m_PduSessionAggregateMaximumBitRateIe.has_value()) return false;

  if (!m_PduSessionAggregateMaximumBitRateIe.value().get(bitRateDl, bitRateUl))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getUlNgUUpTnlInformation(
    GtpTunnel_t& upTnlInfo) const {
  TransportLayerAddress transportLayerAddress = {};
  GtpTeid gtpTeid                             = {};
  if (!m_UpTransportLayerInformation.get(transportLayerAddress, gtpTeid))
    return false;
  if (!transportLayerAddress.get(upTnlInfo.ipAddress)) return false;
  if (!gtpTeid.get(upTnlInfo.gtpTeid)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getDataForwardingNotPossible()
    const {
  if (!m_DataForwardingNotPossible.has_value()) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getPduSessionType(
    long& type) const {
  if (!m_PduSessionType.get(type)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getSecurityIndication(
    long& integrity_protection, long& confidentiality_protection,
    long& maxIntProtDataRate) const {
  if (!m_SecurityIndication.has_value()) return false;

  IntegrityProtectionIndication m_integrityProtectionIndication = {};
  ConfidentialityProtectionIndication m_confidentialityProtectionIndication =
      {};
  std::optional<MaximumIntegrityProtectedDataRate>
      m_maximumIntegrityProtectedDataRateUl = std::nullopt;
  std::optional<MaximumIntegrityProtectedDataRate>
      m_maximumIntegrityProtectedDataRateDl = std::nullopt;

  m_SecurityIndication.value().get(
      m_integrityProtectionIndication, m_confidentialityProtectionIndication,
      m_maximumIntegrityProtectedDataRateUl,
      m_maximumIntegrityProtectedDataRateDl);

  if (!m_integrityProtectionIndication.get(integrity_protection)) return false;
  if (!m_confidentialityProtectionIndication.get(confidentiality_protection))
    return false;
  if (m_maximumIntegrityProtectedDataRateUl.has_value())
    m_maximumIntegrityProtectedDataRateUl.value().get(maxIntProtDataRate);
  else
    maxIntProtDataRate = -1;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getNetworkInstance(
    long& value) const {
  if (!m_NetworkInstance.has_value()) return false;

  if (!m_NetworkInstance.value().get(value)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getQosFlowSetupRequestList(
    std::vector<QosFlowSetupReq_t>& list) const {
  std::vector<QosFlowSetupRequestItem> itemListVector;
  m_QosFlowSetupRequestList.get(itemListVector);

  for (int i = 0; i < itemListVector.size(); i++) {
    QosFlowIdentifier qosFlowIdentifier                 = {};
    QosFlowLevelQosParameters qosFlowLevelQosParameters = {};

    itemListVector[i].get(qosFlowIdentifier, qosFlowLevelQosParameters);

    QosFlowSetupReq_t qosFlowSetupReq;
    if (!qosFlowIdentifier.get(qosFlowSetupReq.qosFlowId)) return false;
    QosCharacteristics qosCharacteristics                        = {};
    AllocationAndRetentionPriority arp                           = {};
    std::optional<GbrQosFlowInformation> gbrQosFlowInformation   = std::nullopt;
    std::optional<ReflectiveQosAttribute> reflectiveQosAttribute = std::nullopt;
    std::optional<AdditionalQosFlowInformation> additionalQosFlowInformation =
        std::nullopt;
    qosFlowLevelQosParameters.get(
        qosCharacteristics, arp, gbrQosFlowInformation, reflectiveQosAttribute,
        additionalQosFlowInformation);

    if (qosCharacteristics.QosCharacteristicsPresent() ==
        Ngap_QosCharacteristics_PR_nonDynamic5QI) {
      qosFlowSetupReq.qflqp.qosc.nonDynamic5qi =
          (NonDynamic5qi_t*) calloc(1, sizeof(NonDynamic5qi_t));
      std::optional<NonDynamic5qiDescriptor> nonDynamic5qiDescriptor =
          std::nullopt;
      qosCharacteristics.get(nonDynamic5qiDescriptor);
      FiveQI fiveqi                                    = {};
      std::optional<PriorityLevelQos> priorityLevelQos = std::nullopt;
      std::optional<AveragingWindow> averaging_window  = std::nullopt;
      std::optional<MaximumDataBurstVolume> maximumDataBurstVolume =
          std::nullopt;

      if (nonDynamic5qiDescriptor.has_value())
        nonDynamic5qiDescriptor.value().get(
            fiveqi, priorityLevelQos, averaging_window, maximumDataBurstVolume);

      fiveqi.get(qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->_5qi);

      if (priorityLevelQos.has_value()) {
        qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->priorityLevelQos =
            (long*) calloc(1, sizeof(long));
        priorityLevelQos.value().get(
            *qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->priorityLevelQos);
      } else {
        qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->priorityLevelQos = NULL;
      }

      if (averaging_window.has_value()) {
        qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->averagingWindow =
            (long*) calloc(1, sizeof(long));
        averaging_window.value().get(
            *qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->averagingWindow);
      } else {
        qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->averagingWindow = NULL;
      }

      if (maximumDataBurstVolume.has_value()) {
        qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->maximumDataBurstVolume =
            (long*) calloc(1, sizeof(long));
        maximumDataBurstVolume.value().get(
            *qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->maximumDataBurstVolume);
      } else {
        qosFlowSetupReq.qflqp.qosc.nonDynamic5qi->maximumDataBurstVolume = NULL;
      }
    } else if (
        qosCharacteristics.QosCharacteristicsPresent() ==
        Ngap_QosCharacteristics_PR_dynamic5QI) {
      qosFlowSetupReq.qflqp.qosc.dynamic5qi =
          (Dynamic5qi_t*) calloc(1, sizeof(Dynamic5qi_t));
      std::optional<Dynamic5qiDescriptor> dynamic5qiDescriptor = std::nullopt;
      qosCharacteristics.get(dynamic5qiDescriptor);
      PriorityLevelQos priorityLevelQos   = {};
      PacketDelayBudget packetDelayBudget = {};
      PacketErrorRate packetErrorRate     = {};

      std::optional<FiveQI> fiveqi                    = std::nullopt;
      std::optional<DelayCritical> delayCritical      = std::nullopt;
      std::optional<AveragingWindow> averaging_window = std::nullopt;
      std::optional<MaximumDataBurstVolume> maximumDataBurstVolume =
          std::nullopt;
      if (dynamic5qiDescriptor.has_value())
        dynamic5qiDescriptor.value().get(
            priorityLevelQos, packetDelayBudget, packetErrorRate, fiveqi,
            delayCritical, averaging_window, maximumDataBurstVolume);

      priorityLevelQos.get(
          qosFlowSetupReq.qflqp.qosc.dynamic5qi->priorityLevelQos);
      packetDelayBudget.get(
          qosFlowSetupReq.qflqp.qosc.dynamic5qi->packetDelayBudget);
      packetErrorRate.get(
          qosFlowSetupReq.qflqp.qosc.dynamic5qi->packetErrorRate.scalar,
          qosFlowSetupReq.qflqp.qosc.dynamic5qi->packetErrorRate.exponent);

      if (fiveqi.has_value()) {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->_5qi =
            (long*) calloc(1, sizeof(long));
        fiveqi.value().get(*qosFlowSetupReq.qflqp.qosc.dynamic5qi->_5qi);
      } else {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->_5qi = NULL;
      }

      if (delayCritical.has_value()) {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->delayCritical =
            (e_Ngap_DelayCritical*) calloc(1, sizeof(e_Ngap_DelayCritical));
        delayCritical.value().get(
            *qosFlowSetupReq.qflqp.qosc.dynamic5qi->delayCritical);
      } else {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->delayCritical = NULL;
      }

      if (averaging_window.has_value()) {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->averagingWindow =
            (long*) calloc(1, sizeof(long));
        averaging_window.value().get(
            *qosFlowSetupReq.qflqp.qosc.dynamic5qi->averagingWindow);
      } else {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->averagingWindow = NULL;
      }

      if (maximumDataBurstVolume.has_value()) {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->maximumDataBurstVolume =
            (long*) calloc(1, sizeof(long));
        maximumDataBurstVolume.value().get(
            *qosFlowSetupReq.qflqp.qosc.dynamic5qi->maximumDataBurstVolume);
      } else {
        qosFlowSetupReq.qflqp.qosc.dynamic5qi->maximumDataBurstVolume = NULL;
      }

    } else
      return false;

    PriorityLevelARP priorityLevelArp                 = {};
    Pre_emptionCapability pre_emptionCapability       = {};
    Pre_emptionVulnerability pre_emptionVulnerability = {};

    if (!arp.get(
            priorityLevelArp, pre_emptionCapability, pre_emptionVulnerability))
      return false;

    priorityLevelArp.get(qosFlowSetupReq.qflqp.arp.priorityLevelArp);
    pre_emptionCapability.get(qosFlowSetupReq.qflqp.arp.pre_emptionCapability);
    pre_emptionVulnerability.get(
        qosFlowSetupReq.qflqp.arp.pre_emptionVulnerability);

    if (gbrQosFlowInformation.has_value()) {
      qosFlowSetupReq.qflqp.gbrQosInformation =
          (GBR_QosInformation_t*) calloc(1, sizeof(GBR_QosInformation_t));
      std::optional<NotificationControl> m_notificationControl = std::nullopt;
      std::optional<PacketLossRate> maxPacketLossRateDl        = std::nullopt;
      std::optional<PacketLossRate> maxPacketLossRateUl        = std::nullopt;

      gbrQosFlowInformation.value().get(
          qosFlowSetupReq.qflqp.gbrQosInformation->maximumFlowBitRateDl,
          qosFlowSetupReq.qflqp.gbrQosInformation->maximumFlowBitRateUl,
          qosFlowSetupReq.qflqp.gbrQosInformation->guaranteedFlowBitRateDl,
          qosFlowSetupReq.qflqp.gbrQosInformation->guaranteedFlowBitRateUl,
          m_notificationControl, maxPacketLossRateDl, maxPacketLossRateUl);

      if (m_notificationControl) {
        qosFlowSetupReq.qflqp.gbrQosInformation->notificationControl =
            (e_Ngap_NotificationControl*) calloc(
                1, sizeof(e_Ngap_NotificationControl));
        m_notificationControl->get(
            *qosFlowSetupReq.qflqp.gbrQosInformation->notificationControl);
      } else {
        qosFlowSetupReq.qflqp.gbrQosInformation->notificationControl = NULL;
      }

      if (maxPacketLossRateDl) {
        qosFlowSetupReq.qflqp.gbrQosInformation->maximumPacketLossRateDl =
            (long*) calloc(1, sizeof(long));
        maxPacketLossRateDl->get(
            *qosFlowSetupReq.qflqp.gbrQosInformation->maximumPacketLossRateDl);
      } else {
        qosFlowSetupReq.qflqp.gbrQosInformation->maximumPacketLossRateDl = NULL;
      }

      if (maxPacketLossRateUl) {
        qosFlowSetupReq.qflqp.gbrQosInformation->maximumPacketLossRateUl =
            (long*) calloc(1, sizeof(long));
        maxPacketLossRateUl->get(
            *qosFlowSetupReq.qflqp.gbrQosInformation->maximumPacketLossRateUl);
      } else {
        qosFlowSetupReq.qflqp.gbrQosInformation->maximumPacketLossRateUl = NULL;
      }
    } else {
      qosFlowSetupReq.qflqp.gbrQosInformation = NULL;
    }

    if (reflectiveQosAttribute.has_value()) {
      qosFlowSetupReq.qflqp.reflectiveQosAttribute =
          (e_Ngap_ReflectiveQosAttribute*) calloc(
              1, sizeof(e_Ngap_ReflectiveQosAttribute));
      reflectiveQosAttribute.value().get(
          *qosFlowSetupReq.qflqp.reflectiveQosAttribute);
    } else {
      qosFlowSetupReq.qflqp.reflectiveQosAttribute = NULL;
    }

    if (additionalQosFlowInformation) {
      qosFlowSetupReq.qflqp.additionalQosFlowInformation =
          (e_Ngap_AdditionalQosFlowInformation*) calloc(
              1, sizeof(e_Ngap_AdditionalQosFlowInformation));
      additionalQosFlowInformation->get(
          *qosFlowSetupReq.qflqp.additionalQosFlowInformation);
    } else {
      qosFlowSetupReq.qflqp.additionalQosFlowInformation = NULL;
    }

    list.push_back(qosFlowSetupReq);
  }

  return true;
}

}  // namespace ngap
