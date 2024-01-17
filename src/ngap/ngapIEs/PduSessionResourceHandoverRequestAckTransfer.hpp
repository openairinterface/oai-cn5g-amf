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

#ifndef _PDU_SESSION_RESOURCE_HANDOVER_REQUEST_ACK_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_HANDOVER_REQUEST_ACK_TRANSFER_H_

#include "NgapIEsStruct.hpp"
#include "QosFlowPerTnlInformation.hpp"
#include "QosFlowListWithDataForwarding.hpp"
#include "SecurityResult.hpp"
#include "UPTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_HandoverRequestAcknowledgeTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {
class PduSessionResourceHandoverRequestAckTransfer {
 public:
  PduSessionResourceHandoverRequestAckTransfer();
  virtual ~PduSessionResourceHandoverRequestAckTransfer();

  bool getUpTransportLayerInformation2(GtpTunnel_t*& upTnlInfo);
  bool getqosFlowSetupResponseList(
      std::vector<QosFlowLItemWithDataForwarding_t>& list);

  bool decode(uint8_t* buf, int buf_size);  // TODO: remove naked pointer

 private:
  Ngap_HandoverRequestAcknowledgeTransfer_t*
      handoverRequestAcknowledegTransferIEs;
  // TODO: DL NG-U UP TNL Information //Mandatory
  UpTransportLayerInformation dLForwardingUP_TNLInformation;  // Mandatory
  // TODO: Security Result //Optional
  QosFlowListWithDataForwarding QosFlowSetupResponseList;  // Mandatory
  // TODO: QoS Flow Failed to Setup List //Optional
  // TODO: Data Forwarding Response DRB List //Optional
  // TODO: Additional DL UP TNL Information for HO List //Range 0..1
  // TODO: UL Forwarding UP TNL Information //Optional
  // TODO: Additional UL Forwarding UP TNL Information
  // TODO: Data Forwarding Response E-RAB List //Optional
};
}  // namespace ngap

#endif
