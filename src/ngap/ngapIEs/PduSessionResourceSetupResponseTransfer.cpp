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

#include "PduSessionResourceSetupResponseTransfer.hpp"

#include "logger.hpp"
#include "output_wrapper.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseTransferIE::
    PduSessionResourceSetupResponseTransferIE() {
  pduSessionResourceSetupResponseTransferIEs =
      (Ngap_PDUSessionResourceSetupResponseTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseTransfer_t));
  additionalDLQoSFlowPerTNLInformation = std::nullopt;
  securityResult                       = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseTransferIE::
    ~PduSessionResourceSetupResponseTransferIE() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::set(
    const GtpTunnel_t& up_transport_layer_info,
    const std::vector<AssociatedQosFlow_t>& list) {
  UpTransportLayerInformation up_transport_layer_information = {};
  TransportLayerAddress transport_layer_address              = {};
  GtpTeid gtp_teid                                           = {};
  transport_layer_address.setTransportLayerAddress(
      up_transport_layer_info.ip_address);
  gtp_teid.setGtpTeid(up_transport_layer_info.gtp_teid);
  up_transport_layer_information.setUpTransportLayerInformation(
      transport_layer_address, gtp_teid);

  AssociatedQosFlowList associated_qos_flow_list = {};
  std::vector<AssociatedQosFlowItem> vector_flow_items;
  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier qos_flow_identifier = {};
    qos_flow_identifier.setQosFlowIdentifier(list[i].qosFlowIdentifier);
    AssociatedQosFlowItem item = {};
    if (list[i].qosFlowMappingIndication) {
      item.setAssociatedQosFlowItem(
          *list[i].qosFlowMappingIndication, qos_flow_identifier);
    } else {
      item.setAssociatedQosFlowItem(qos_flow_identifier);
    }
    vector_flow_items.push_back(item);
  }
  associated_qos_flow_list.setAssociatedQosFlowList(vector_flow_items);

  dlQoSFlowPerTNLInformation.setQoSFlowPerTNLInformation(
      up_transport_layer_information, associated_qos_flow_list);

  int ret = dlQoSFlowPerTNLInformation.encode(
      pduSessionResourceSetupResponseTransferIEs->dLQosFlowPerTNLInformation);
  if (!ret) {
    Logger::ngap().error("Encode DLQoSFlowPerTNLInformation IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::
    setAdditionalDLQoSFlowPerTNLInformation(
        QosFlowPerTnlInformationList& additionDLQoSFlowPerTNLInformation) {
  /*  UpTransportLayerInformation up_transport_layer_information = {};

    TransportLayerAddress transport_layer_address = {};
    GtpTeid gtp_teid                              = {};
    transport_layer_address.setTransportLayerAddress(
        up_transport_layer_info.ip_address);
    gtp_teid.setGtpTeid(up_transport_layer_info.gtp_teid);
    up_transport_layer_information.setUpTransportLayerInformation(
        transport_layer_address, gtp_teid);

    AssociatedQosFlowList associated_qos_flow_list = {};
    std::vector<AssociatedQosFlowItem> vector_associated_qos_flow_item;
    for (int i = 0; i < list.size(); i++) {
      AssociatedQosFlowItem item            = {};
      QosFlowIdentifier qos_flow_identifier = {};
      qos_flow_identifier.setQosFlowIdentifier(list[i].qosFlowIdentifier);
      if (list[i].qosFlowMappingIndication) {
        item.setAssociatedQosFlowItem(
            *list[i].qosFlowMappingIndication, qos_flow_identifier);
      } else {
        item.setAssociatedQosFlowItem(qos_flow_identifier);
      }
      vector_associated_qos_flow_item.push_back(item);
    }
    associated_qos_flow_list.setAssociatedQosFlowList(
        vector_associated_qos_flow_item);

    QosFlowPerTnlInformation additional_qos_flow = {};
    additional_qos_flow.setQoSFlowPerTNLInformation(
        up_transport_layer_information, associated_qos_flow_list);
    additionalDLQoSFlowPerTNLInformation =
        std::make_optional<QosFlowPerTnlInformationList>(additional_qos_flow);

    Ngap_QosFlowPerTNLInformationList_t* ie =
                (Ngap_QosFlowPerTNLInformationList_t*) calloc(1,
    sizeof(Ngap_QosFlowPerTNLInformationList_t));

    int ret = additionalDLQoSFlowPerTNLInformation.value().encode(ie);
    if (!ret) {
      Logger::ngap().error(
          "Encode AdditionalDLQoSFlowPerTNLInformation IE error");
      return;
    }
    pduSessionResourceSetupResponseTransferIEs
        ->additionalDLQosFlowPerTNLInformation = ie;
        */
  // TODO
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::setSecurityResult(
    e_Ngap_IntegrityProtectionResult e_integrity_protection_result,
    e_Ngap_ConfidentialityProtectionResult
        e_confidentiality_protection_result) {
  IntegrityProtectionResult integrity_protection_result_tmp = {};
  integrity_protection_result_tmp.set(e_integrity_protection_result);
  ConfidentialityProtectionResult confidentiality_protection_result_tmp = {};
  confidentiality_protection_result_tmp.set(
      e_confidentiality_protection_result);
  SecurityResult security_result = {};
  security_result.setSecurityResult(
      integrity_protection_result_tmp, confidentiality_protection_result_tmp);
  securityResult = std::make_optional<SecurityResult>(security_result);

  Ngap_SecurityResult_t* ie =
      (Ngap_SecurityResult_t*) calloc(1, sizeof(Ngap_SecurityResult_t));
  int ret = securityResult.value().encode(*ie);
  if (!ret) {
    Logger::ngap().error("Encode SecurityResult IE error");
    return;
  }
  pduSessionResourceSetupResponseTransferIEs->securityResult = ie;
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupResponseTransferIE::encode(
    uint8_t* buf, int buf_size) {
  output_wrapper::print_asn_msg(
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
      pduSessionResourceSetupResponseTransferIEs);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer, NULL,
      pduSessionResourceSetupResponseTransferIEs, buf, buf_size);
  Logger::ngap().debug("er.encoded %d", er.encoded);
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceSetupResponseTransferIE::decode(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
      (void**) &pduSessionResourceSetupResponseTransferIEs, buf, buf_size);
  if (rc.code == RC_OK) {
    Logger::ngap().debug("Decoded successfully");
  } else if (rc.code == RC_WMORE) {
    Logger::ngap().debug("More data expected, call again");
    return false;
  } else {
    Logger::ngap().error("Failure to decode data");
    return false;
  }
  Logger::ngap().debug("rc.consumed to decode %d", rc.consumed);
  // asn_fprint(stderr, &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
  // pduSessionResourceSetupResponseTransferIEs);

  if (!dlQoSFlowPerTNLInformation.decode(
          pduSessionResourceSetupResponseTransferIEs
              ->dLQosFlowPerTNLInformation)) {
    Logger::ngap().error("Decode NGAP DLQoSFlowPerTNLInformation IE error");
    return false;
  }

  if (pduSessionResourceSetupResponseTransferIEs
          ->additionalDLQosFlowPerTNLInformation) {
    QosFlowPerTnlInformationList additional_qos_flow = {};
    if (!additional_qos_flow.decode(
            *pduSessionResourceSetupResponseTransferIEs
                 ->additionalDLQosFlowPerTNLInformation)) {
      Logger::ngap().error(
          "Decode NGAP AdditionalDLQoSFlowPerTNLInformation IE error");
      return false;
    }
    additionalDLQoSFlowPerTNLInformation =
        std::make_optional<QosFlowPerTnlInformationList>(additional_qos_flow);
  }
  if (pduSessionResourceSetupResponseTransferIEs->securityResult) {
    SecurityResult security_result = {};
    if (!security_result.decode(
            *pduSessionResourceSetupResponseTransferIEs->securityResult)) {
      Logger::ngap().error("Decode NGAP SecurityResult IE error");
      return false;
    }
    securityResult = std::make_optional<SecurityResult>(security_result);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::get(
    GtpTunnel_t& up_transport_layer_info,
    std::vector<AssociatedQosFlow_t>& list) const {
  UpTransportLayerInformation up_transport_layer_information = {};
  AssociatedQosFlowList associated_qos_flow_list             = {};
  dlQoSFlowPerTNLInformation.getQoSFlowPerTNLInformation(
      up_transport_layer_information, associated_qos_flow_list);
  TransportLayerAddress transport_layer_address = {};
  GtpTeid gtp_teid                              = {};
  up_transport_layer_information.getUpTransportLayerInformation(
      transport_layer_address, gtp_teid);
  transport_layer_address.getTransportLayerAddress(
      up_transport_layer_info.ip_address);
  gtp_teid.getGtpTeid(up_transport_layer_info.gtp_teid);

  std::vector<AssociatedQosFlowItem> vector_associated_qos_flow_item;
  associated_qos_flow_list.getAssociatedQosFlowList(
      vector_associated_qos_flow_item);
  for (int i = 0; i < vector_associated_qos_flow_item.size(); i++) {
    AssociatedQosFlow_t AssociatedQosFlow_str;
    long m_qosFlowMappingIndication;
    QosFlowIdentifier qos_flow_identifier = {};
    vector_associated_qos_flow_item[i].getAssociatedQosFlowItem(
        m_qosFlowMappingIndication, qos_flow_identifier);
    qos_flow_identifier.getQosFlowIdentifier(
        AssociatedQosFlow_str.qosFlowIdentifier);
    if (m_qosFlowMappingIndication < 0) {
      AssociatedQosFlow_str.qosFlowMappingIndication = NULL;
    } else {
      AssociatedQosFlow_str.qosFlowMappingIndication =
          (e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication*) calloc(
              1,
              sizeof(e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication));
      *AssociatedQosFlow_str.qosFlowMappingIndication =
          (e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication)
              m_qosFlowMappingIndication;
    }

    list.push_back(AssociatedQosFlow_str);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::
    getAdditionalDLQoSFlowPerTNLInformation(
        QosFlowPerTnlInformationList& additionDLQoSFlowPerTNLInformation)
        const {
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::getSecurityResult(
    long& integrity_protection_result,
    long& confidentialityProtectionResult) const {
  if (!securityResult.has_value()) return false;

  IntegrityProtectionResult integrity_protection_result_tmp             = {};
  ConfidentialityProtectionResult confidentiality_protection_result_tmp = {};

  securityResult.value().getSecurityResult(
      integrity_protection_result_tmp, confidentiality_protection_result_tmp);

  integrity_protection_result_tmp.get(integrity_protection_result);
  confidentiality_protection_result_tmp.get(confidentialityProtectionResult);

  return true;
}
}  // namespace ngap
