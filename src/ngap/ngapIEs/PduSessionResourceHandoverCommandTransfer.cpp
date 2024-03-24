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

#include "PduSessionResourceHandoverCommandTransfer.hpp"

#include <vector>

#include "logger.hpp"
#include "output_wrapper.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceHandoverCommandTransfer::
    PduSessionResourceHandoverCommandTransfer() {
  m_HandoverCommandTransferIe = (Ngap_HandoverCommandTransfer_t*) calloc(
      1, sizeof(Ngap_HandoverCommandTransfer_t));
  m_DlForwardingUpTnlInformation = std::nullopt;
  m_QosFlowToBeForwardedList     = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceHandoverCommandTransfer::
    ~PduSessionResourceHandoverCommandTransfer() {}
void PduSessionResourceHandoverCommandTransfer::setQosFlowToBeForwardedList(
    const std::vector<QosFlowToBeForwardedItem_t>& list) {
  QosFlowToBeForwardedList qosList = {};

  std::vector<QosFlowToBeForwardedItem> item_list;

  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier qfi             = {};
    QosFlowToBeForwardedItem qos_item = {};
    qfi.set(list[i].qfi);

    qos_item.set(qfi);
    item_list.push_back(qos_item);
  }

  qosList.set(item_list);
  m_QosFlowToBeForwardedList =
      std::make_optional<QosFlowToBeForwardedList>(qosList);
  int ret = m_QosFlowToBeForwardedList.value().encode(
      m_HandoverCommandTransferIe->qosFlowToBeForwardedList);
  Logger::ngap().debug(
      "Number of QoS flows in the list %d",
      m_HandoverCommandTransferIe->qosFlowToBeForwardedList->list.count);
  if (m_HandoverCommandTransferIe->qosFlowToBeForwardedList->list.array) {
    if (m_HandoverCommandTransferIe->qosFlowToBeForwardedList->list.array[0]) {
      Logger::ngap().debug(
          "QFI in the list %d",
          m_HandoverCommandTransferIe->qosFlowToBeForwardedList->list.array[0]
              ->qosFlowIdentifier);
    }
  }

  if (!ret) {
    Logger::ngap().debug("Encode QosFlowToBeForwardedList IE error");
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceHandoverCommandTransfer::setUPTransportLayerInformation(
    const GtpTunnel_t& upTransportLayerInfo) {
  UpTransportLayerInformation tmp               = {};
  TransportLayerAddress m_transportLayerAddress = {};
  GtpTeid m_gtpTeid                             = {};
  m_transportLayerAddress.set(upTransportLayerInfo.ipAddress);
  m_gtpTeid.set(upTransportLayerInfo.gtpTeid);
  tmp.set(m_transportLayerAddress, m_gtpTeid);
  m_DlForwardingUpTnlInformation =
      std::make_optional<UpTransportLayerInformation>(tmp);

  m_HandoverCommandTransferIe->dLForwardingUP_TNLInformation =
      (Ngap_UPTransportLayerInformation*) calloc(
          1, sizeof(Ngap_UPTransportLayerInformation));
  int ret = m_DlForwardingUpTnlInformation.value().encode(
      *m_HandoverCommandTransferIe->dLForwardingUP_TNLInformation);
  if (!ret) {
    Logger::ngap().debug("Encode dLForwardingUP_TNLInformation IE error");
    return;
  }
}

//------------------------------------------------------------------------------
int PduSessionResourceHandoverCommandTransfer::encode(
    uint8_t* buf, int buf_size) const {
  output_wrapper::print_asn_msg(
      &asn_DEF_Ngap_HandoverCommandTransfer, m_HandoverCommandTransferIe);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_HandoverCommandTransfer, NULL, m_HandoverCommandTransferIe,
      buf, buf_size);
  Logger::ngap().debug("er.encoded( %d)", er.encoded);
  return er.encoded;
}

}  // namespace ngap
