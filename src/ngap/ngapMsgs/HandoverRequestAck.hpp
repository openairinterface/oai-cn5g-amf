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

#ifndef _HANDOVER_REQUEST_ACK_H_
#define _HANDOVER_REQUEST_ACK_H_

#include "NgapUEMessage.hpp"
#include "PduSessionResourceAdmittedList.hpp"
#include "PduSessionResourceFailedToSetupListHoAck.hpp"

extern "C" {
#include "Ngap_HandoverRequestAcknowledge.h"
}

namespace ngap {

class HandoverRequestAck : public NgapUEMessage {
 public:
  HandoverRequestAck();
  virtual ~HandoverRequestAck();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setPDUSessionResourceAdmittedList(
      const PduSessionResourceAdmittedList& admittedList);
  bool getPDUSessionResourceAdmittedList(
      std::vector<PDUSessionResourceAdmittedItem_t>& list);

  void setPDUSessionResourceFailedToSetupListHOAck(
      const PduSessionResourceFailedToSetupListHoAck& list);
  void setPDUSessionResourceFailedToSetupListHOAck(
      std::vector<PduSessionResourceItem>& list);
  bool getPDUSessionResourceFailedToSetupListHOAck(
      std::vector<PduSessionResourceItem>& list);

  void setTargetToSource_TransparentContainer(
      const OCTET_STRING_t& targetTosource);
  OCTET_STRING_t getTargetToSource_TransparentContainer();

 private:
  Ngap_HandoverRequestAcknowledge_t* handoverRequestAckIEs;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  PduSessionResourceAdmittedList pduSessionResourceAdmittedList;  // Mandatory
  std::optional<PduSessionResourceFailedToSetupListHoAck>
      PDUSessionResourceFailedToSetupList;                // Optional
  OCTET_STRING_t TargetToSource_TransparentContainer;     // TODO: Mandatory
  Ngap_CriticalityDiagnostics_t* CriticalityDiagnostics;  // TODO: Optional
  // TODO: NPN Access Information (Optional, Rel 16.14.0)
};

}  // namespace ngap

#endif
