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

#ifndef _PDU_SESSION_RESOURCE_HANDOVER_COMMAND_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_HANDOVER_COMMAND_TRANSFER_H_

#include "NgapIEsStruct.hpp"
#include "QosFlowToBeForwardedList.hpp"
#include "UPTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_HandoverCommandTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {
class PduSessionResourceHandoverCommandTransfer {
 public:
  PduSessionResourceHandoverCommandTransfer();
  virtual ~PduSessionResourceHandoverCommandTransfer();

  void setUPTransportLayerInformation(GtpTunnel_t uptlinfo);
  void setQosFlowToBeForwardedList(
      std::vector<QosFlowToBeForwardedItem_t> list);

  int encode(uint8_t* buf, int buf_size);  // TODO: remove naked pointer

 private:
  Ngap_HandoverCommandTransfer_t* handovercommandtransferIE;
  std::optional<UpTransportLayerInformation>
      dLForwardingUP_TNLInformation;  // Optional
  std::optional<QosFlowToBeForwardedList>
      qosFlowToBeForwardedList;  // Optional (list 0..1)
  // TODO: Data Forwarding Response DRB List //Optional
  // TODO: Additional DL Forwarding UP TNL Information //Optional
  // TODO: UL Forwarding UP TNL Information //Optional
  // TODO: Additional UL Forwarding UP TNL Information //Optional
  // TODO: Data Forwarding Response E-RAB List //Optional
};

}  // namespace ngap
#endif
