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

#ifndef _UE_CONTEXT_RELEASE_REQUEST_H_
#define _UE_CONTEXT_RELEASE_REQUEST_H_

#include "Cause.hpp"
#include "NgapUEMessage.hpp"
#include "PduSessionResourceListCxtRelReq.hpp"

extern "C" {
#include "Ngap_UEContextReleaseRequest.h"
}

namespace ngap {

class UEContextReleaseRequestMsg : public NgapUEMessage {
 public:
  UEContextReleaseRequestMsg();
  ~UEContextReleaseRequestMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setPDUSessionResourceList(const PduSessionResourceListCxtRelReq&
                                     pdu_session_resource_list_cxt_rel_req);
  bool getPDUSessionResourceList(
      PduSessionResourceListCxtRelReq& pdu_session_resource_list_cxt_rel_req);

  void addCauseIE();
  void setCauseRadioNetwork(const e_Ngap_CauseRadioNetwork& cause);
  bool getCauseRadioNetwork(e_Ngap_CauseRadioNetwork& cause);

 private:
  Ngap_UEContextReleaseRequest_t* ies;

  // AMF_UE_NGAP_ID //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  std::optional<PduSessionResourceListCxtRelReq>
      pdu_session_resource_list_cxt_rel_req_;  // Optional
  Cause causeValue;                            // Mandatory
};

}  // namespace ngap

#endif
