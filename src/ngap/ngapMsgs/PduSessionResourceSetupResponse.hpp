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

#ifndef _PDU_SESSION_RESOURCE_SETUP_RESPONSE_H_
#define _PDU_SESSION_RESOURCE_SETUP_RESPONSE_H_

#include "NgapUEMessage.hpp"
#include "PduSessionResourceFailedToSetupListSURes.hpp"
#include "PduSessionResourceSetupListSURes.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupResponse.h"
}

namespace ngap {

class PduSessionResourceSetupResponseMsg : public NgapUEMessage {
 public:
  PduSessionResourceSetupResponseMsg();
  virtual ~PduSessionResourceSetupResponseMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setPduSessionResourceSetupResponseList(
      const std::vector<PDUSessionResourceSetupResponseItem_t>& list);
  bool getPduSessionResourceSetupResponseList(
      std::vector<PDUSessionResourceSetupResponseItem_t>& list);

  void setPduSessionResourceFailedToSetupList(
      const std::vector<PDUSessionResourceFailedToSetupItem_t>& list);

  bool getPduSessionResourceFailedToSetupList(
      std::vector<PDUSessionResourceFailedToSetupItem_t>& list);

 private:
  Ngap_PDUSessionResourceSetupResponse_t* pduSessionResourceSetupResponseIEs;
  std::optional<PduSessionResourceSetupListSURes>
      pduSessionResourceSetupResponseList;  // Optional
  std::optional<PduSessionResourceFailedToSetupListSURes>
      pduSessionResourceFailedToSetupResponseList;  // Optional
  // TODO: Criticality Diagnostics (Optional)
};

}  // namespace ngap
#endif
