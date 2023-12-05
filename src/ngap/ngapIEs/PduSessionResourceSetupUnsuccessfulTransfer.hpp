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

#ifndef _PDU_SESSION_RESOURCE_SETUP_UNSUCCESSFUL_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_SETUP_UNSUCCESSFUL_TRANSFER_H_

#include "Cause.hpp"
#include "NgapIEsStruct.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupUnsuccessfulTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class PduSessionResourceSetupUnSuccessfulTransferIE {
 public:
  PduSessionResourceSetupUnSuccessfulTransferIE();
  virtual ~PduSessionResourceSetupUnSuccessfulTransferIE();

  void setCauseRadioNetwork(e_Ngap_CauseRadioNetwork cause_value);
  void setCauseTransport(e_Ngap_CauseTransport cause_value);
  void setCauseNas(e_Ngap_CauseNas cause_value);
  void setCauseProtocol(e_Ngap_CauseProtocol cause_value);
  void setCauseMisc(e_Ngap_CauseMisc cause_value);
  long getChoiceOfCause() const;
  long getCause() const;

  int encode(uint8_t* buf, int buf_size);   // TODO: remove naked pointer
  bool decode(uint8_t* buf, int buf_size);  // TODO: remove naked pointer

 private:
  Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t*
      pduSessionResourceSetupUnsuccessfulTransferIEs;

  Cause cause_;  // Mandatory
  // TODO: Criticality Diagnostics //Optional
};

}  // namespace ngap
#endif
