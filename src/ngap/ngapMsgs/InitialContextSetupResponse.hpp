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

#ifndef _INITIAL_CONTEXT_SETUP_RESPONSE_H_
#define _INITIAL_CONTEXT_SETUP_RESPONSE_H_

#include <optional>

#include "NgapUeMessage.hpp"
#include "PduSessionResourceFailedToSetupListCxtRes.hpp"
#include "PduSessionResourceSetupListCxtRes.hpp"

extern "C" {
#include "Ngap_InitialContextSetupResponse.h"
}

namespace ngap {

class InitialContextSetupResponseMsg : public NgapUeMessage {
 public:
  InitialContextSetupResponseMsg();
  virtual ~InitialContextSetupResponseMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setPduSessionResourceSetupResponseList(
      const std::vector<PDUSessionResourceSetupResponseItem_t>& list);
  bool getPduSessionResourceSetupResponseList(
      std::vector<PDUSessionResourceSetupResponseItem_t>& list) const;

  void setPduSessionResourceFailedToSetupList(
      const std::vector<PDUSessionResourceFailedToSetupItem_t>& list);
  bool getPduSessionResourceFailedToSetupList(
      std::vector<PDUSessionResourceFailedToSetupItem_t>& list) const;

 private:
  Ngap_InitialContextSetupResponse_t* m_InitialContextSetupResponseIes;
  // AMF_UE_NGAP_ID //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  std::optional<PduSessionResourceSetupListCxtRes>
      m_PduSessionResourceSetupResponseList;  // Optional
  std::optional<PduSessionResourceFailedToSetupListCxtRes>
      m_PduSessionResourceFailedToSetupResponseList;  // Optional
  // TODO: Criticality Diagnostics (Optional)
};

}  // namespace ngap
#endif
