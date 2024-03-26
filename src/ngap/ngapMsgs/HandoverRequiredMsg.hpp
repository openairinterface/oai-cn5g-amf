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
#include "GlobalGnbId.hpp"
#include "NgapUeMessage.hpp"
#include "PduSessionResourceListHandoverRqd.hpp"
#include "Tai.hpp"

extern "C" {
#include "Ngap_HandoverRequired.h"
#include "Ngap_TargetID.h"
}

namespace ngap {

class HandoverRequiredMsg : public NgapUeMessage {
 public:
  HandoverRequiredMsg();
  virtual ~HandoverRequiredMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  Ngap_HandoverType_t getHandoverType() const;
  // TODO Setter

  Ngap_Cause_PR getChoiceOfCause() const;
  long getCauseValue() const;
  // TODO Setter

  bool getTargetId(GlobalGnbId& gnbId, Tai& tai) const;
  // TODO Setter

  bool getDirectForwardingPathAvailability(long& value) const;
  // TODO: Setter

  bool getPduSessionResourceList(PduSessionResourceListHandoverRqd& list) const;
  // TODO Setter

  OCTET_STRING_t getSourceToTargetTransparentContainer() const;
  // TODO Setter

 private:
  Ngap_HandoverRequired_t* m_HandoverRequiredIes;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  Ngap_HandoverType_t m_HandoverType;  // Mandatory
  Cause m_Cause;                       // Mandatory
  Ngap_TargetID_t m_TargetId;          // Mandatory
  std::optional<Ngap_DirectForwardingPathAvailability_t>
      m_DirectForwardingPathAvailability;                      // Optional
  PduSessionResourceListHandoverRqd m_PduSessionResourceList;  // Mandatory
  Ngap_SourceToTarget_TransparentContainer_t
      m_SourceToTargetTransparentContainer;  // Mandatory
};

}  // namespace ngap

#endif
