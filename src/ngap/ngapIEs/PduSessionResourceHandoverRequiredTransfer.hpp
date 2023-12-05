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

#ifndef _PDU_SESSION_RESOURCE_HANDOVER_REQUIRED_TRANSFER_H_
#define _PDU_SESSION_RESOURCE_HANDOVER_REQUIRED_TRANSFER_H_

#include <optional>

#include "NgapIEsStruct.hpp"
#include "QosFlowPerTnlInformation.hpp"
#include "SecurityResult.hpp"

extern "C" {
#include "Ngap_HandoverRequiredTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class PduSessionResourceHandoverRequiredTransfer {
 public:
  PduSessionResourceHandoverRequiredTransfer();
  virtual ~PduSessionResourceHandoverRequiredTransfer();

  void setDirectForwardingPathAvailability(
      Ngap_DirectForwardingPathAvailability_t directForwardingPathAvailability);
  bool getDirectForwardingPathAvailability(
      long& directForwardingPathAvailability) const;

  int encode(uint8_t* buf, int buf_size);   // TODO: remove naked pointer
  bool decode(uint8_t* buf, int buf_size);  // TODO: remove naked pointer

 private:
  Ngap_HandoverRequiredTransfer_t* handoverrquiredTransferIEs;
  std::optional<Ngap_DirectForwardingPathAvailability_t>
      directForwardingPathAvailability_;  // Optional
};

}  // namespace ngap
#endif
