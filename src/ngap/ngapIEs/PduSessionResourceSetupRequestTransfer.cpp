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
  pduSessionResourceSetupRequestTransferIEs =
      (Ngap_PDUSessionResourceSetupRequestTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransfer_t));
  pduSessionAggregateMaximumBitRateIE = std::nullopt;
  dataForwardingNotPossible           = std::nullopt;
  securityIndication                  = std::nullopt;
  networkInstance                     = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestTransferIE::
    ~PduSessionResourceSetupRequestTransferIE() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::
    setPduSessionAggregateMaximumBitRate(
        long bit_rate_downlink, long bit_rate_uplink) {
  pduSessionAggregateMaximumBitRateIE =
      std::make_optional<PduSessionAggregateMaximumBitRate>(
          bit_rate_downlink, bit_rate_uplink);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;

  int ret = pduSessionAggregateMaximumBitRateIE.value().encode(
      ie->value.choice.PDUSessionAggregateMaximumBitRate);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionAggregateMaximumBitRate IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode PDUSessionAggregateMaximumBitRate IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setUlNgUUpTnlInformation(
    GtpTunnel_t upTnlInfo) {
  TransportLayerAddress transport_layer_address = {};
  transport_layer_address.setTransportLayerAddress(upTnlInfo.ip_address);
  GtpTeid gtp_teid = {};
  gtp_teid.setGtpTeid(upTnlInfo.gtp_teid);
  upTransportLayerInformation.setUpTransportLayerInformation(
      transport_layer_address, gtp_teid);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation;

  int ret = upTransportLayerInformation.encode(
      ie->value.choice.UPTransportLayerInformation);
  if (!ret) {
    Logger::ngap().error("Encode UPTransportLayerInformation IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
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
  dataForwardingNotPossible =
      std::make_optional<DataForwardingNotPossible>(tmp);

  if (!ret) {
    Logger::ngap().error("Encode DataForwardingNotPossible IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::ngap().error("Encode DataForwardingNotPossible IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setPduSessionType(
    e_Ngap_PDUSessionType type) {
  pduSessionType.set(type);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionType;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType;

  int ret = pduSessionType.encode(ie->value.choice.PDUSessionType);
  if (!ret) {
    Logger::ngap().error("Encode PDUSessionType IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode PDUSessionType IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setSecurityIndication(
    e_Ngap_IntegrityProtectionIndication e_integrity_protection,
    e_Ngap_ConfidentialityProtectionIndication e_confidentiality_protection,
    e_Ngap_MaximumIntegrityProtectedDataRate
        e_max_integrity_protected_data_rate) {
  IntegrityProtectionIndication integrity_protection_indication = {};
  ConfidentialityProtectionIndication confidentiality_protection_indication =
      {};
  std::optional<MaximumIntegrityProtectedDataRate>
      maximum_integrity_protected_data_rate =
          std::make_optional<MaximumIntegrityProtectedDataRate>(
              e_max_integrity_protected_data_rate);
  integrity_protection_indication.set(e_integrity_protection);
  confidentiality_protection_indication.set(e_confidentiality_protection);

  securityIndication = std::make_optional<SecurityIndication>(
      integrity_protection_indication, confidentiality_protection_indication,
      maximum_integrity_protected_data_rate,
      maximum_integrity_protected_data_rate);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SecurityIndication;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication;

  int ret =
      securityIndication.value().encode(ie->value.choice.SecurityIndication);
  if (!ret) {
    Logger::ngap().error("Encode SecurityIndication IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityIndication IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setSecurityIndication(
    e_Ngap_IntegrityProtectionIndication e_integrity_protection,
    e_Ngap_ConfidentialityProtectionIndication e_confidentiality_protection) {
  IntegrityProtectionIndication integrity_protection_indication = {};
  ConfidentialityProtectionIndication confidentiality_protection_indication =
      {};

  integrity_protection_indication.set(e_integrity_protection);
  confidentiality_protection_indication.set(e_confidentiality_protection);

  securityIndication = std::make_optional<SecurityIndication>(
      integrity_protection_indication, confidentiality_protection_indication,
      std::nullopt, std::nullopt);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SecurityIndication;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication;

  int ret =
      securityIndication.value().encode(ie->value.choice.SecurityIndication);
  if (!ret) {
    Logger::ngap().error("Encode SecurityIndication IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode SecurityIndication IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setNetworkInstance(long value) {
  networkInstance = std::make_optional<NetworkInstance>(value);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NetworkInstance;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_NetworkInstance;

  int ret = networkInstance.value().encode(ie->value.choice.NetworkInstance);
  if (!ret) {
    Logger::ngap().error("Encode NetworkInstance IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode NetworkInstance IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setQosFlowSetupRequestList(
    std::vector<QosFlowSetupReq_t> list) {
  std::vector<QosFlowSetupRequestItem> vector_items;

  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier qos_flow_identifier = {};
    qos_flow_identifier.setQosFlowIdentifier(list[i].qos_flow_id);

    QosCharacteristics qos_characteristics = {};
    if (list[i].qflqp.qosc.nonDy) {
      FiveQI five_qi                                     = {};
      std::optional<PriorityLevelQos> priority_level_qos = std::nullopt;
      std::optional<AveragingWindow> averaging_window    = std::nullopt;
      std::optional<MaximumDataBurstVolume> max_data_burst_volume =
          std::nullopt;

      five_qi.setFiveQI(list[i].qflqp.qosc.nonDy->_5QI);
      if (list[i].qflqp.qosc.nonDy->priorityLevelQos) {
        PriorityLevelQos tmp = {};
        tmp.setPriorityLevelQos(*list[i].qflqp.qosc.nonDy->priorityLevelQos);
        priority_level_qos = std::make_optional<PriorityLevelQos>(tmp);
      }
      if (list[i].qflqp.qosc.nonDy->averagingWindow) {
        AveragingWindow tmp = {};
        tmp.setAveragingWindow(*list[i].qflqp.qosc.nonDy->averagingWindow);
        averaging_window = std::make_optional<AveragingWindow>(tmp);
      }
      if (list[i].qflqp.qosc.nonDy->maximumDataBurstVolume) {
        MaximumDataBurstVolume tmp = {};
        tmp.set(*list[i].qflqp.qosc.nonDy->maximumDataBurstVolume);
        max_data_burst_volume = std::make_optional<MaximumDataBurstVolume>(tmp);
      }

      NonDynamic5QIDescriptor non_dynamic_5qi_descriptor = {};
      non_dynamic_5qi_descriptor.set(
          five_qi, priority_level_qos, averaging_window, max_data_burst_volume);

      qos_characteristics.setQosCharacteristics(non_dynamic_5qi_descriptor);
    } else {
      PriorityLevelQos priority_level_qos             = {};
      PacketDelayBudget packet_delay_budget           = {};
      PacketErrorRate packet_error_rate               = {};
      std::optional<FiveQI> five_qi                   = std::nullopt;
      std::optional<DelayCritical> delay_critical     = std::nullopt;
      std::optional<AveragingWindow> averaging_window = std::nullopt;
      std::optional<MaximumDataBurstVolume> max_data_burst_volume =
          std::nullopt;

      priority_level_qos.setPriorityLevelQos(
          list[i].qflqp.qosc.dy->priorityLevelQos);
      packet_delay_budget.setPacketDelayBudget(
          list[i].qflqp.qosc.dy->packetDelayBudget);
      packet_error_rate.setPacketErrorRate(
          list[i].qflqp.qosc.dy->packetErrorRate.pERScalar,
          list[i].qflqp.qosc.dy->packetErrorRate.pERExponent);
      if (list[i].qflqp.qosc.dy->_5QI) {
        FiveQI tmp = {};
        tmp.setFiveQI(*list[i].qflqp.qosc.dy->_5QI);
        five_qi = std::make_optional<FiveQI>(tmp);
      }
      if (list[i].qflqp.qosc.dy->delayCritical) {
        DelayCritical tmp = {};
        tmp.setDelayCritical(*list[i].qflqp.qosc.dy->delayCritical);
        delay_critical = std::make_optional<DelayCritical>(tmp);
      }
      if (list[i].qflqp.qosc.dy->averagingWindow) {
        AveragingWindow tmp = {};
        tmp.setAveragingWindow(*list[i].qflqp.qosc.dy->averagingWindow);
        averaging_window = std::make_optional<AveragingWindow>(tmp);
      }
      if (list[i].qflqp.qosc.dy->maximumDataBurstVolume) {
        MaximumDataBurstVolume tmp = {};
        tmp.set(*list[i].qflqp.qosc.dy->maximumDataBurstVolume);
        max_data_burst_volume = std::make_optional<MaximumDataBurstVolume>(tmp);
      }

      Dynamic5QIDescriptor dynamic_5qi_descriptor = {};
      dynamic_5qi_descriptor.set(
          priority_level_qos, packet_delay_budget, packet_error_rate, five_qi,
          delay_critical, averaging_window, max_data_burst_volume);

      qos_characteristics.setQosCharacteristics(dynamic_5qi_descriptor);
    }

    PriorityLevelARP priority_level_arp = {};
    priority_level_arp.setPriorityLevelARP(list[i].qflqp.arp.priorityLevelARP);
    Pre_emptionCapability pre_emption_capability = {};
    pre_emption_capability.set(list[i].qflqp.arp.pre_emptionCapability);
    Pre_emptionVulnerability pre_emption_vulnerability = {};
    pre_emption_vulnerability.set(list[i].qflqp.arp.pre_emptionVulnerability);
    AllocationAndRetentionPriority arp = {};
    arp.set(
        priority_level_arp, pre_emption_capability, pre_emption_vulnerability);

    std::optional<GbrQoSFlowInformation> gbr_qos_information = std::nullopt;
    if (list[i].qflqp.gbr_qos_info) {
      std::optional<NotificationControl> m_notificationControl = std::nullopt;
      if (list[i].qflqp.gbr_qos_info->notificationControl) {
        NotificationControl tmp = {};
        tmp.setNotificationControl(
            *list[i].qflqp.gbr_qos_info->notificationControl);
        m_notificationControl = std::make_optional<NotificationControl>(tmp);
      }

      std::optional<PacketLossRate> max_packet_loss_rate_dl = std::nullopt;
      if (list[i].qflqp.gbr_qos_info->maximumPacketLossRateDL) {
        PacketLossRate tmp = {};
        tmp.setPacketLossRate(
            *list[i].qflqp.gbr_qos_info->maximumPacketLossRateDL);
        max_packet_loss_rate_dl = std::make_optional<PacketLossRate>(tmp);
      }

      std::optional<PacketLossRate> max_packet_loss_rate_ul = std::nullopt;
      if (list[i].qflqp.gbr_qos_info->maximumPacketLossRateUL) {
        PacketLossRate tmp = {};
        tmp.setPacketLossRate(
            *list[i].qflqp.gbr_qos_info->maximumPacketLossRateUL);
        max_packet_loss_rate_ul = std::make_optional<PacketLossRate>(tmp);
      }

      gbr_qos_information = std::make_optional<GbrQoSFlowInformation>(
          list[i].qflqp.gbr_qos_info->maximumFlowBitRateDL,
          list[i].qflqp.gbr_qos_info->maximumFlowBitRateUL,
          list[i].qflqp.gbr_qos_info->guaranteedFlowBitRateDL,
          list[i].qflqp.gbr_qos_info->guaranteedFlowBitRateUL,
          m_notificationControl, max_packet_loss_rate_dl,
          max_packet_loss_rate_ul);
    }

    std::optional<ReflectiveQosAttribute> reflective_qos_attribute =
        std::nullopt;
    if (list[i].qflqp.reflectiveQosAttribute) {
      reflective_qos_attribute = std::make_optional<ReflectiveQosAttribute>(
          *list[i].qflqp.reflectiveQosAttribute);
    }

    std::optional<AdditionalQosFlowInformation>
        additional_qos_flow_information = std::nullopt;
    if (list[i].qflqp.additionalQosFlowInformation) {
      additional_qos_flow_information =
          std::make_optional<AdditionalQosFlowInformation>(
              *list[i].qflqp.additionalQosFlowInformation);
    }

    QosFlowLevelQosParameters qos_flow_level_qos_parameters = {};
    qos_flow_level_qos_parameters.set(
        qos_characteristics, arp, gbr_qos_information, reflective_qos_attribute,
        additional_qos_flow_information);

    QosFlowSetupRequestItem qos_flow_setup_request_item = {};
    qos_flow_setup_request_item.set(
        qos_flow_identifier, qos_flow_level_qos_parameters);
    vector_items.push_back(qos_flow_setup_request_item);
  }

  qosFlowSetupRequestList.set(vector_items);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

  int ret =
      qosFlowSetupRequestList.encode(ie->value.choice.QosFlowSetupRequestList);
  if (!ret) {
    Logger::ngap().error("Encode QosFlowSetupRequestList IE error");
    utils::free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::ngap().error("Encode QosFlowSetupRequestList IE error");
  // utils::free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupRequestTransferIE::encode(
    uint8_t* buf, int buf_size) {
  output_wrapper::print_asn_msg(
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer,
      pduSessionResourceSetupRequestTransferIEs);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer, NULL,
      pduSessionResourceSetupRequestTransferIEs, buf, buf_size);
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
      (void**) &pduSessionResourceSetupRequestTransferIEs, buf, buf_size);
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
  // pduSessionResourceSetupRequestTransferIEs);

  for (int i = 0;
       i < pduSessionResourceSetupRequestTransferIEs->protocolIEs.list.count;
       i++) {
    switch (
        pduSessionResourceSetupRequestTransferIEs->protocolIEs.list.array[i]
            ->id) {
      case Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate) {
          PduSessionAggregateMaximumBitRate aggregate_maximum_bit_rate = {};

          if (!aggregate_maximum_bit_rate.decode(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionAggregateMaximumBitRate)) {
            Logger::ngap().error(
                "Decode NGAP PDUSessionAggregateMaximumBitRate IE error");
            return false;
          }
          pduSessionAggregateMaximumBitRateIE =
              std::make_optional<PduSessionAggregateMaximumBitRate>(
                  aggregate_maximum_bit_rate);
        } else {
          Logger::ngap().error(
              "Decode NGAP PDUSessionAggregateMaximumBitRate IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation) {
          if (!upTransportLayerInformation.decode(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
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
        if ((pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                 .array[i]
                 ->criticality) == Ngap_Criticality_reject &&
            (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                 .array[i]
                 ->value.present ==
             Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_DataForwardingNotPossible)) {
          DataForwardingNotPossible data_forwarding_not_possible = {};
          if (!data_forwarding_not_possible.decode(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.DataForwardingNotPossible)) {
            Logger::ngap().error(
                "Decode NGAP DataForwardingNotPossible IE error");
            return false;
          }
          dataForwardingNotPossible =
              std::make_optional<DataForwardingNotPossible>(
                  data_forwarding_not_possible);
        } else {
          Logger::ngap().error(
              "Decode NGAP DataForwardingNotPossible IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionType: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType) {
          if (!pduSessionType.decode(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
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
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication) {
          SecurityIndication security_indication = {};
          if (!security_indication.decode(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.SecurityIndication)) {
            Logger::ngap().error("Decode NGAP SecurityIndication IE error");

            return false;
          }
          securityIndication =
              std::make_optional<SecurityIndication>(security_indication);
        } else {
          Logger::ngap().error("Decode NGAP SecurityIndication IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NetworkInstance: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_NetworkInstance) {
          NetworkInstance network_instance = {};
          if (!network_instance.decode(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.NetworkInstance)) {
            Logger::ngap().error("Decode NGAP NetworkInstance IE error");
            return false;
          }
          networkInstance =
              std::make_optional<NetworkInstance>(network_instance);
        } else {
          Logger::ngap().error("Decode NGAP NetworkInstance IE error");
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList) {
          if (!qosFlowSetupRequestList.decode(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
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
        long& bit_rate_downlink, long& bit_rate_uplink) const {
  if (!pduSessionAggregateMaximumBitRateIE.has_value()) return false;

  if (!pduSessionAggregateMaximumBitRateIE.value().get(
          bit_rate_downlink, bit_rate_uplink))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getUlNgUUpTnlInformation(
    GtpTunnel_t& upTnlInfo) const {
  TransportLayerAddress transport_layer_address = {};
  GtpTeid gtp_teid                              = {};
  if (!upTransportLayerInformation.getUpTransportLayerInformation(
          transport_layer_address, gtp_teid))
    return false;
  if (!transport_layer_address.getTransportLayerAddress(upTnlInfo.ip_address))
    return false;
  if (!gtp_teid.getGtpTeid(upTnlInfo.gtp_teid)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getDataForwardingNotPossible()
    const {
  if (!dataForwardingNotPossible.has_value()) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getPduSessionType(
    long& type) const {
  if (!pduSessionType.get(type)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getSecurityIndication(
    long& integrity_protection, long& confidentiality_protection,
    long& maxIntProtDataRate) const {
  if (!securityIndication.has_value()) return false;

  IntegrityProtectionIndication m_integrityProtectionIndication = {};
  ConfidentialityProtectionIndication m_confidentialityProtectionIndication =
      {};
  std::optional<MaximumIntegrityProtectedDataRate>
      m_maximumIntegrityProtectedDataRateUL = std::nullopt;
  std::optional<MaximumIntegrityProtectedDataRate>
      m_maximumIntegrityProtectedDataRateDL = std::nullopt;

  securityIndication.value().getSecurityIndication(
      m_integrityProtectionIndication, m_confidentialityProtectionIndication,
      m_maximumIntegrityProtectedDataRateUL,
      m_maximumIntegrityProtectedDataRateDL);

  if (!m_integrityProtectionIndication.get(integrity_protection)) return false;
  if (!m_confidentialityProtectionIndication.get(confidentiality_protection))
    return false;
  if (m_maximumIntegrityProtectedDataRateUL.has_value())
    m_maximumIntegrityProtectedDataRateUL.value().get(maxIntProtDataRate);
  else
    maxIntProtDataRate = -1;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getNetworkInstance(
    long& value) const {
  if (!networkInstance.has_value()) return false;

  if (!networkInstance.value().get(value)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getQosFlowSetupRequestList(
    std::vector<QosFlowSetupReq_t>& list) const {
  std::vector<QosFlowSetupRequestItem> vector_items;
  qosFlowSetupRequestList.get(vector_items);

  for (int i = 0; i < vector_items.size(); i++) {
    QosFlowIdentifier qos_flow_identifier                   = {};
    QosFlowLevelQosParameters qos_flow_level_qos_parameters = {};

    vector_items[i].get(qos_flow_identifier, qos_flow_level_qos_parameters);

    QosFlowSetupReq_t qos_flow_setup_request;
    if (!qos_flow_identifier.getQosFlowIdentifier(
            qos_flow_setup_request.qos_flow_id))
      return false;
    QosCharacteristics qos_characteristics                   = {};
    AllocationAndRetentionPriority arp                       = {};
    std::optional<GbrQoSFlowInformation> gbr_qos_information = std::nullopt;
    std::optional<ReflectiveQosAttribute> reflective_qos_attribute =
        std::nullopt;
    std::optional<AdditionalQosFlowInformation>
        additional_qos_flow_information = std::nullopt;
    qos_flow_level_qos_parameters.get(
        qos_characteristics, arp, gbr_qos_information, reflective_qos_attribute,
        additional_qos_flow_information);

    if (qos_characteristics.QosCharacteristicsPresent() ==
        Ngap_QosCharacteristics_PR_nonDynamic5QI) {
      qos_flow_setup_request.qflqp.qosc.nonDy =
          (NonDynamic5QI_t*) calloc(1, sizeof(NonDynamic5QI_t));
      std::optional<NonDynamic5QIDescriptor> non_dynamic_5qi_descriptor =
          std::nullopt;
      qos_characteristics.getQosCharacteristics(non_dynamic_5qi_descriptor);
      FiveQI five_qi                                     = {};
      std::optional<PriorityLevelQos> priority_level_qos = std::nullopt;
      std::optional<AveragingWindow> averaging_window    = std::nullopt;
      std::optional<MaximumDataBurstVolume> max_data_burst_volume =
          std::nullopt;

      if (non_dynamic_5qi_descriptor.has_value())
        non_dynamic_5qi_descriptor.value().get(
            five_qi, priority_level_qos, averaging_window,
            max_data_burst_volume);

      five_qi.getFiveQI(qos_flow_setup_request.qflqp.qosc.nonDy->_5QI);

      if (priority_level_qos.has_value()) {
        qos_flow_setup_request.qflqp.qosc.nonDy->priorityLevelQos =
            (long*) calloc(1, sizeof(long));
        priority_level_qos.value().getPriorityLevelQos(
            *qos_flow_setup_request.qflqp.qosc.nonDy->priorityLevelQos);
      } else {
        qos_flow_setup_request.qflqp.qosc.nonDy->priorityLevelQos = NULL;
      }

      if (averaging_window.has_value()) {
        qos_flow_setup_request.qflqp.qosc.nonDy->averagingWindow =
            (long*) calloc(1, sizeof(long));
        averaging_window.value().getAveragingWindow(
            *qos_flow_setup_request.qflqp.qosc.nonDy->averagingWindow);
      } else {
        qos_flow_setup_request.qflqp.qosc.nonDy->averagingWindow = NULL;
      }

      if (max_data_burst_volume.has_value()) {
        qos_flow_setup_request.qflqp.qosc.nonDy->maximumDataBurstVolume =
            (long*) calloc(1, sizeof(long));
        max_data_burst_volume.value().get(
            *qos_flow_setup_request.qflqp.qosc.nonDy->maximumDataBurstVolume);
      } else {
        qos_flow_setup_request.qflqp.qosc.nonDy->maximumDataBurstVolume = NULL;
      }
    } else if (
        qos_characteristics.QosCharacteristicsPresent() ==
        Ngap_QosCharacteristics_PR_dynamic5QI) {
      qos_flow_setup_request.qflqp.qosc.dy =
          (Dynamic5QI_t*) calloc(1, sizeof(Dynamic5QI_t));
      std::optional<Dynamic5QIDescriptor> dynamic_5qi_descriptor = std::nullopt;
      qos_characteristics.getQosCharacteristics(dynamic_5qi_descriptor);
      PriorityLevelQos priority_level_qos   = {};
      PacketDelayBudget packet_delay_budget = {};
      PacketErrorRate packet_error_rate     = {};

      std::optional<FiveQI> five_qi                   = std::nullopt;
      std::optional<DelayCritical> delay_critical     = std::nullopt;
      std::optional<AveragingWindow> averaging_window = std::nullopt;
      std::optional<MaximumDataBurstVolume> max_data_burst_volume =
          std::nullopt;
      if (dynamic_5qi_descriptor.has_value())
        dynamic_5qi_descriptor.value().get(
            priority_level_qos, packet_delay_budget, packet_error_rate, five_qi,
            delay_critical, averaging_window, max_data_burst_volume);

      priority_level_qos.getPriorityLevelQos(
          qos_flow_setup_request.qflqp.qosc.dy->priorityLevelQos);
      packet_delay_budget.getPacketDelayBudget(
          qos_flow_setup_request.qflqp.qosc.dy->packetDelayBudget);
      packet_error_rate.getPacketErrorRate(
          qos_flow_setup_request.qflqp.qosc.dy->packetErrorRate.pERScalar,
          qos_flow_setup_request.qflqp.qosc.dy->packetErrorRate.pERExponent);

      if (five_qi.has_value()) {
        qos_flow_setup_request.qflqp.qosc.dy->_5QI =
            (long*) calloc(1, sizeof(long));
        five_qi.value().getFiveQI(*qos_flow_setup_request.qflqp.qosc.dy->_5QI);
      } else {
        qos_flow_setup_request.qflqp.qosc.dy->_5QI = NULL;
      }

      if (delay_critical.has_value()) {
        qos_flow_setup_request.qflqp.qosc.dy->delayCritical =
            (e_Ngap_DelayCritical*) calloc(1, sizeof(e_Ngap_DelayCritical));
        delay_critical.value().getDelayCritical(
            *qos_flow_setup_request.qflqp.qosc.dy->delayCritical);
      } else {
        qos_flow_setup_request.qflqp.qosc.dy->delayCritical = NULL;
      }

      if (averaging_window.has_value()) {
        qos_flow_setup_request.qflqp.qosc.dy->averagingWindow =
            (long*) calloc(1, sizeof(long));
        averaging_window.value().getAveragingWindow(
            *qos_flow_setup_request.qflqp.qosc.dy->averagingWindow);
      } else {
        qos_flow_setup_request.qflqp.qosc.dy->averagingWindow = NULL;
      }

      if (max_data_burst_volume.has_value()) {
        qos_flow_setup_request.qflqp.qosc.dy->maximumDataBurstVolume =
            (long*) calloc(1, sizeof(long));
        max_data_burst_volume.value().get(
            *qos_flow_setup_request.qflqp.qosc.dy->maximumDataBurstVolume);
      } else {
        qos_flow_setup_request.qflqp.qosc.dy->maximumDataBurstVolume = NULL;
      }

    } else
      return false;

    PriorityLevelARP priority_level_arp                = {};
    Pre_emptionCapability pre_emption_capability       = {};
    Pre_emptionVulnerability pre_emption_vulnerability = {};

    if (!arp.get(
            priority_level_arp, pre_emption_capability,
            pre_emption_vulnerability))
      return false;

    priority_level_arp.getPriorityLevelARP(
        qos_flow_setup_request.qflqp.arp.priorityLevelARP);
    pre_emption_capability.get(
        qos_flow_setup_request.qflqp.arp.pre_emptionCapability);
    pre_emption_vulnerability.get(
        qos_flow_setup_request.qflqp.arp.pre_emptionVulnerability);

    if (gbr_qos_information.has_value()) {
      qos_flow_setup_request.qflqp.gbr_qos_info =
          (GBR_QosInformation_t*) calloc(1, sizeof(GBR_QosInformation_t));
      std::optional<NotificationControl> m_notificationControl = std::nullopt;
      std::optional<PacketLossRate> max_packet_loss_rate_dl    = std::nullopt;
      std::optional<PacketLossRate> max_packet_loss_rate_ul    = std::nullopt;

      gbr_qos_information.value().get(
          qos_flow_setup_request.qflqp.gbr_qos_info->maximumFlowBitRateDL,
          qos_flow_setup_request.qflqp.gbr_qos_info->maximumFlowBitRateUL,
          qos_flow_setup_request.qflqp.gbr_qos_info->guaranteedFlowBitRateDL,
          qos_flow_setup_request.qflqp.gbr_qos_info->guaranteedFlowBitRateUL,
          m_notificationControl, max_packet_loss_rate_dl,
          max_packet_loss_rate_ul);

      if (m_notificationControl) {
        qos_flow_setup_request.qflqp.gbr_qos_info->notificationControl =
            (e_Ngap_NotificationControl*) calloc(
                1, sizeof(e_Ngap_NotificationControl));
        m_notificationControl->getNotificationControl(
            *qos_flow_setup_request.qflqp.gbr_qos_info->notificationControl);
      } else {
        qos_flow_setup_request.qflqp.gbr_qos_info->notificationControl = NULL;
      }

      if (max_packet_loss_rate_dl) {
        qos_flow_setup_request.qflqp.gbr_qos_info->maximumPacketLossRateDL =
            (long*) calloc(1, sizeof(long));
        max_packet_loss_rate_dl->getPacketLossRate(
            *qos_flow_setup_request.qflqp.gbr_qos_info
                 ->maximumPacketLossRateDL);
      } else {
        qos_flow_setup_request.qflqp.gbr_qos_info->maximumPacketLossRateDL =
            NULL;
      }

      if (max_packet_loss_rate_ul) {
        qos_flow_setup_request.qflqp.gbr_qos_info->maximumPacketLossRateUL =
            (long*) calloc(1, sizeof(long));
        max_packet_loss_rate_ul->getPacketLossRate(
            *qos_flow_setup_request.qflqp.gbr_qos_info
                 ->maximumPacketLossRateUL);
      } else {
        qos_flow_setup_request.qflqp.gbr_qos_info->maximumPacketLossRateUL =
            NULL;
      }
    } else {
      qos_flow_setup_request.qflqp.gbr_qos_info = NULL;
    }

    if (reflective_qos_attribute.has_value()) {
      qos_flow_setup_request.qflqp.reflectiveQosAttribute =
          (e_Ngap_ReflectiveQosAttribute*) calloc(
              1, sizeof(e_Ngap_ReflectiveQosAttribute));
      reflective_qos_attribute.value().get(
          *qos_flow_setup_request.qflqp.reflectiveQosAttribute);
    } else {
      qos_flow_setup_request.qflqp.reflectiveQosAttribute = NULL;
    }

    if (additional_qos_flow_information) {
      qos_flow_setup_request.qflqp.additionalQosFlowInformation =
          (e_Ngap_AdditionalQosFlowInformation*) calloc(
              1, sizeof(e_Ngap_AdditionalQosFlowInformation));
      additional_qos_flow_information->get(
          *qos_flow_setup_request.qflqp.additionalQosFlowInformation);
    } else {
      qos_flow_setup_request.qflqp.additionalQosFlowInformation = NULL;
    }

    list.push_back(qos_flow_setup_request);
  }

  return true;
}

}  // namespace ngap
