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

#include "NgapUeMessage.hpp"
#include "PduSessionResourceAdmittedList.hpp"
#include "PduSessionResourceFailedToSetupListHoAck.hpp"

extern "C" {
#include "Ngap_HandoverRequestAcknowledge.h"
}

namespace ngap {

class HandoverRequestAck : public NgapUeMessage {
 public:
  HandoverRequestAck();
  virtual ~HandoverRequestAck();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setPduSessionResourceAdmittedList(
      const PduSessionResourceAdmittedList& admittedList);
  bool getPduSessionResourceAdmittedList(
      std::vector<PDUSessionResourceAdmittedItem_t>& list) const;

  void setPduSessionResourceFailedToSetupListHOAck(
      const PduSessionResourceFailedToSetupListHoAck& list);
  void setPduSessionResourceFailedToSetupListHOAck(
      const std::vector<PduSessionResourceItem>& list);
  bool getPduSessionResourceFailedToSetupListHOAck(
      std::vector<PduSessionResourceItem>& list) const;

  void setTargetToSourceTransparentContainer(
      const OCTET_STRING_t& targetTosource);
  OCTET_STRING_t getTargetToSourceTransparentContainer() const;

 private:
  Ngap_HandoverRequestAcknowledge_t* m_HandoverRequestAckIes;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  PduSessionResourceAdmittedList m_PduSessionResourceAdmittedList;  // Mandatory
  std::optional<PduSessionResourceFailedToSetupListHoAck>
      m_PduSessionResourceFailedToSetupList;                // Optional
  OCTET_STRING_t m_TargetToSourceTransparentContainer;      // TODO: Mandatory
  Ngap_CriticalityDiagnostics_t* m_CriticalityDiagnostics;  // TODO: Optional
  // TODO: NPN Access Information (Optional, Rel 16.14.0)
};

}  // namespace ngap

#endif
