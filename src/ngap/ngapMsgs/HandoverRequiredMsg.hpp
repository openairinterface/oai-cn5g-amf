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

#ifndef _HANDOVER_REQUIRED_H_
#define _HANDOVER_REQUIRED_H_

#include <optional>

#include "Cause.hpp"
#include "GlobalgNBId.hpp"
#include "NgapUEMessage.hpp"
#include "PduSessionResourceListHandoverRqd.hpp"
#include "TAI.hpp"

extern "C" {
#include "Ngap_HandoverRequired.h"
#include "Ngap_TargetID.h"
}

namespace ngap {

class HandoverRequiredMsg : public NgapUEMessage {
 public:
  HandoverRequiredMsg();
  virtual ~HandoverRequiredMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  Ngap_HandoverType_t getHandoverType();
  // TODO Setter

  Ngap_Cause_PR getChoiceOfCause();
  long getCauseValue();
  // TODO Setter

  bool getTargetID(GlobalgNBId& gnbId, TAI& tai);
  // TODO Setter

  bool getDirectForwardingPathAvailability(long& value) const;
  // TODO: Setter

  bool getPDUSessionResourceList(PduSessionResourceListHandoverRqd& list);
  // TODO Setter

  OCTET_STRING_t getSourceToTarget_TransparentContainer();
  // TODO Setter

 private:
  Ngap_HandoverRequired_t* handoverRequiredIEs;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  Ngap_HandoverType_t handoverType;  // Mandatory
  Cause cause;                       // Mandatory
  Ngap_TargetID_t targetID;          // Mandatory
  std::optional<Ngap_DirectForwardingPathAvailability_t>
      directForwardingPathAvailability;                      // Optional
  PduSessionResourceListHandoverRqd pDUSessionResourceList;  // Mandatory
  Ngap_SourceToTarget_TransparentContainer_t
      sourceToTarget_TransparentContainer;  // Mandatory
};

}  // namespace ngap

#endif
