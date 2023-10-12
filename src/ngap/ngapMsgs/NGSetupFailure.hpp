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

#ifndef _NG_SETUP_FAILURE_H_
#define _NG_SETUP_FAILURE_H_

#include <optional>

#include "Cause.hpp"
#include "MessageType.hpp"
#include "NgapMessage.hpp"
#include "TimeToWait.hpp"

namespace ngap {

class NGSetupFailureMsg : public NgapMessage {
 public:
  NGSetupFailureMsg();
  virtual ~NGSetupFailureMsg();

  void initialize();

  void set(
      const e_Ngap_CauseRadioNetwork& cause_value,
      const e_Ngap_TimeToWait& time_to_wait);
  void set(
      const e_Ngap_CauseTransport& cause_value,
      const e_Ngap_TimeToWait& time_to_wait);
  void set(
      const e_Ngap_CauseNas& cause_value,
      const e_Ngap_TimeToWait& time_to_wait);
  void set(
      const e_Ngap_CauseProtocol& cause_value,
      const e_Ngap_TimeToWait& time_to_wait);
  void set(
      const e_Ngap_CauseMisc& cause_value,
      const e_Ngap_TimeToWait& time_to_wait);

  void setCauseRadioNetwork(const e_Ngap_CauseRadioNetwork& cause_value);
  bool getCauseRadioNetwork(e_Ngap_CauseRadioNetwork&);

  void setCauseTransport(const e_Ngap_CauseTransport& cause_value);
  bool getCauseTransport(e_Ngap_CauseTransport&);

  void setCauseNas(const e_Ngap_CauseNas& cause_value);
  bool getCauseNas(e_Ngap_CauseNas&);

  void setCauseProtocol(const e_Ngap_CauseProtocol& cause_value);
  bool getCauseProtocol(e_Ngap_CauseProtocol&);

  void setCauseMisc(const e_Ngap_CauseMisc& cause_value);
  bool getCauseMisc(e_Ngap_CauseMisc&);

  bool getCauseType(Ngap_Cause_PR&);

  bool getTimeToWait(e_Ngap_TimeToWait&);
  void setTimeToWait(const e_Ngap_TimeToWait&);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGSetupFailure_t* ngSetupFailureIEs;
  Cause cause;                           // Mandatory
  std::optional<TimeToWait> timeToWait;  // Optional
  // TODO: CriticalityDiagnostics *criticalityDiagnostics; //Optional

  void addCauseIE();
  void addTimeToWaitIE();
};
}  // namespace ngap
#endif
