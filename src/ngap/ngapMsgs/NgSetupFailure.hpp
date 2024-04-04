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

class NgSetupFailureMsg : public NgapMessage {
 public:
  NgSetupFailureMsg();
  virtual ~NgSetupFailureMsg();

  void initialize();

  void set(
      const e_Ngap_CauseRadioNetwork& causeValue,
      const e_Ngap_TimeToWait& timeToWait);
  void set(
      const e_Ngap_CauseTransport& causeValue,
      const e_Ngap_TimeToWait& timeToWait);
  void set(
      const e_Ngap_CauseNas& causeValue, const e_Ngap_TimeToWait& timeToWait);
  void set(
      const e_Ngap_CauseProtocol& causeValue,
      const e_Ngap_TimeToWait& timeToWait);
  void set(
      const e_Ngap_CauseMisc& causeValue, const e_Ngap_TimeToWait& timeToWait);

  void setCauseRadioNetwork(const e_Ngap_CauseRadioNetwork& causeValue);
  bool getCauseRadioNetwork(e_Ngap_CauseRadioNetwork&) const;

  void setCauseTransport(const e_Ngap_CauseTransport& causeValue);
  bool getCauseTransport(e_Ngap_CauseTransport&) const;

  void setCauseNas(const e_Ngap_CauseNas& causeValue);
  bool getCauseNas(e_Ngap_CauseNas&) const;

  void setCauseProtocol(const e_Ngap_CauseProtocol& causeValue);
  bool getCauseProtocol(e_Ngap_CauseProtocol&) const;

  void setCauseMisc(const e_Ngap_CauseMisc& causeValue);
  bool getCauseMisc(e_Ngap_CauseMisc&) const;

  bool getCauseType(Ngap_Cause_PR&) const;

  bool getTimeToWait(e_Ngap_TimeToWait&) const;
  void setTimeToWait(const e_Ngap_TimeToWait&);

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGSetupFailure_t* m_NgSetupFailureIes;
  Cause m_Cause;                           // Mandatory
  std::optional<TimeToWait> m_TimeToWait;  // Optional
  // TODO: CriticalityDiagnostics *criticalityDiagnostics; //Optional

  void addCauseIe();
  void addTimeToWaitIE();
};
}  // namespace ngap
#endif
