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
  m_PduSessionResourceSetupResponseTransferIe =
      (Ngap_PDUSessionResourceSetupResponseTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseTransfer_t));
  m_AdditionalDlQosFlowPerTnlInformation = std::nullopt;
  m_SecurityResult                       = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseTransferIE::
    ~PduSessionResourceSetupResponseTransferIE() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::set(
    const GtpTunnel_t& upTransportLayerInfo,
    const std::vector<AssociatedQosFlow_t>& list) {
  UpTransportLayerInformation upTransportLayerInformation = {};
  TransportLayerAddress transportLayerAddress             = {};
  GtpTeid gtpTeid                                         = {};
  transportLayerAddress.set(upTransportLayerInfo.ipAddress);
  gtpTeid.set(upTransportLayerInfo.gtpTeid);
  upTransportLayerInformation.set(transportLayerAddress, gtpTeid);

  AssociatedQosFlowList associatedQosFlowList = {};
  std::vector<AssociatedQosFlowItem> flowItemVector;
  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier qosFlowIdentifier = {};
    qosFlowIdentifier.set(list[i].qosFlowIdentifier);
    AssociatedQosFlowItem item = {};
    if (list[i].qosFlowMappingIndication) {
      item.set(*list[i].qosFlowMappingIndication, qosFlowIdentifier);
    } else {
      item.set(qosFlowIdentifier);
    }
    flowItemVector.push_back(item);
  }
  associatedQosFlowList.set(flowItemVector);

  m_DlQosFlowPerTnlInformation.set(
      upTransportLayerInformation, associatedQosFlowList);

  int ret = m_DlQosFlowPerTnlInformation.encode(
      m_PduSessionResourceSetupResponseTransferIe->dLQosFlowPerTNLInformation);
  if (!ret) {
    Logger::ngap().error("Encode DLQoSFlowPerTNLInformation IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::
    setAdditionalDLQoSFlowPerTNLInformation(
        const QosFlowPerTnlInformationList&
            additionDlQoSFlowPerTnlInformation) {
  /*  UpTransportLayerInformation upTransportLayerInformation = {};

    TransportLayerAddress transportLayerAddress = {};
    GtpTeid gtpTeid                              = {};
    transportLayerAddress.set(
        upTransportLayerInfo.ipAddress);
    gtpTeid.set(upTransportLayerInfo.gtpTeid);
    upTransportLayerInformation.set(
        transportLayerAddress, gtpTeid);

    AssociatedQosFlowList associatedQosFlowList = {};
    std::vector<AssociatedQosFlowItem> vector_associated_qos_flow_item;
    for (int i = 0; i < list.size(); i++) {
      AssociatedQosFlowItem item            = {};
      QosFlowIdentifier qosFlowIdentifier = {};
      qosFlowIdentifier.set(list[i].qosFlowIdentifier);
      if (list[i].qosFlowMappingIndication) {
        item.set(
            *list[i].qosFlowMappingIndication, qosFlowIdentifier);
      } else {
        item.set(qosFlowIdentifier);
      }
      vector_associated_qos_flow_item.push_back(item);
    }
    associatedQosFlowList.set(
        vector_associated_qos_flow_item);

    QosFlowPerTnlInformation additional_qos_flow = {};
    additional_qos_flow.set(
        upTransportLayerInformation, associatedQosFlowList);
    m_AdditionalDlQosFlowPerTnlInformation =
        std::make_optional<QosFlowPerTnlInformationList>(additional_qos_flow);

    Ngap_QosFlowPerTNLInformationList_t* ie =
                (Ngap_QosFlowPerTNLInformationList_t*) calloc(1,
    sizeof(Ngap_QosFlowPerTNLInformationList_t));

    int ret = m_AdditionalDlQosFlowPerTnlInformation.value().encode(ie);
    if (!ret) {
      Logger::ngap().error(
          "Encode AdditionalDLQoSFlowPerTNLInformation IE error");
      return;
    }
    m_PduSessionResourceSetupResponseTransferIe
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
  security_result.set(
      integrity_protection_result_tmp, confidentiality_protection_result_tmp);
  m_SecurityResult = std::make_optional<SecurityResult>(security_result);

  Ngap_SecurityResult_t* ie =
      (Ngap_SecurityResult_t*) calloc(1, sizeof(Ngap_SecurityResult_t));
  int ret = m_SecurityResult.value().encode(*ie);
  if (!ret) {
    Logger::ngap().error("Encode SecurityResult IE error");
    return;
  }
  m_PduSessionResourceSetupResponseTransferIe->securityResult = ie;
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupResponseTransferIE::encode(
    uint8_t* buf, int buf_size) {
  output_wrapper::print_asn_msg(
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
      m_PduSessionResourceSetupResponseTransferIe);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer, NULL,
      m_PduSessionResourceSetupResponseTransferIe, buf, buf_size);
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
      (void**) &m_PduSessionResourceSetupResponseTransferIe, buf, buf_size);
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
  // m_PduSessionResourceSetupResponseTransferIe);

  if (!m_DlQosFlowPerTnlInformation.decode(
          m_PduSessionResourceSetupResponseTransferIe
              ->dLQosFlowPerTNLInformation)) {
    Logger::ngap().error("Decode NGAP DLQoSFlowPerTNLInformation IE error");
    return false;
  }

  if (m_PduSessionResourceSetupResponseTransferIe
          ->additionalDLQosFlowPerTNLInformation) {
    QosFlowPerTnlInformationList additional_qos_flow = {};
    if (!additional_qos_flow.decode(
            *m_PduSessionResourceSetupResponseTransferIe
                 ->additionalDLQosFlowPerTNLInformation)) {
      Logger::ngap().error(
          "Decode NGAP AdditionalDLQoSFlowPerTNLInformation IE error");
      return false;
    }
    m_AdditionalDlQosFlowPerTnlInformation =
        std::make_optional<QosFlowPerTnlInformationList>(additional_qos_flow);
  }
  if (m_PduSessionResourceSetupResponseTransferIe->securityResult) {
    SecurityResult security_result = {};
    if (!security_result.decode(
            *m_PduSessionResourceSetupResponseTransferIe->securityResult)) {
      Logger::ngap().error("Decode NGAP SecurityResult IE error");
      return false;
    }
    m_SecurityResult = std::make_optional<SecurityResult>(security_result);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::get(
    GtpTunnel_t& upTransportLayerInfo,
    std::vector<AssociatedQosFlow_t>& list) const {
  UpTransportLayerInformation upTransportLayerInformation = {};
  AssociatedQosFlowList associatedQosFlowList             = {};
  m_DlQosFlowPerTnlInformation.get(
      upTransportLayerInformation, associatedQosFlowList);
  TransportLayerAddress transportLayerAddress = {};
  GtpTeid gtpTeid                             = {};
  upTransportLayerInformation.get(transportLayerAddress, gtpTeid);
  transportLayerAddress.get(upTransportLayerInfo.ipAddress);
  gtpTeid.get(upTransportLayerInfo.gtpTeid);

  std::vector<AssociatedQosFlowItem> vector_associated_qos_flow_item;
  associatedQosFlowList.get(vector_associated_qos_flow_item);
  for (int i = 0; i < vector_associated_qos_flow_item.size(); i++) {
    AssociatedQosFlow_t AssociatedQosFlow_str;
    long m_qosFlowMappingIndication;
    QosFlowIdentifier qosFlowIdentifier = {};
    vector_associated_qos_flow_item[i].get(
        m_qosFlowMappingIndication, qosFlowIdentifier);
    qosFlowIdentifier.get(AssociatedQosFlow_str.qosFlowIdentifier);
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
        QosFlowPerTnlInformationList& additionDlQoSFlowPerTnlInformation)
        const {
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::getSecurityResult(
    long& integrity_protection_result,
    long& confidentialityProtectionResult) const {
  if (!m_SecurityResult.has_value()) return false;

  IntegrityProtectionResult integrity_protection_result_tmp             = {};
  ConfidentialityProtectionResult confidentiality_protection_result_tmp = {};

  m_SecurityResult.value().get(
      integrity_protection_result_tmp, confidentiality_protection_result_tmp);

  integrity_protection_result_tmp.get(integrity_protection_result);
  confidentiality_protection_result_tmp.get(confidentialityProtectionResult);

  return true;
}
}  // namespace ngap
