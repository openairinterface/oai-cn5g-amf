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

#ifndef _HANDOVER_PREPARATION_FAILURE_H_
#define _HANDOVER_PREPARATION_FAILURE_H_

#include "Cause.hpp"
#include "NgapUEMessage.hpp"

extern "C" {
#include "Ngap_CriticalityDiagnostics.h"
#include "Ngap_HandoverPreparationFailure.h"
}

namespace ngap {

class HandoverPreparationFailure : public NgapUEMessage {
 public:
  HandoverPreparationFailure();
  virtual ~HandoverPreparationFailure();

  void initialize();
  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void getCause(Cause& cause) const;
  void setCause(const Ngap_Cause_PR& causePresent, const long& value = 0);
  Ngap_Cause_PR getChoiceOfCause();

 private:
  Ngap_HandoverPreparationFailure_t* hoPreparationFailureIEs;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  Cause cause;                                            // Mandatory
  Ngap_CriticalityDiagnostics_t* criticalityDiagnostics;  // Optional
  // TODO: Target to Source Failure Transparent Container (Optional,
  // Rel 16.14.0)
};

}  // namespace ngap

#endif
