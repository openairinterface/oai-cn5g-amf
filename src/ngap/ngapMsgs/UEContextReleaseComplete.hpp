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

#ifndef _UE_CONTEXT_RELEASE_COMPLETE_H_
#define _UE_CONTEXT_RELEASE_COMPLETE_H_

#include <optional>

#include "NgapUEMessage.hpp"
#include "PduSessionResourceListCxtRelCpl.hpp"
#include "UserLocationInformation.hpp"

extern "C" {
#include "Ngap_UEContextReleaseComplete.h"
}

namespace ngap {

class UEContextReleaseCompleteMsg : public NgapUEMessage {
 public:
  UEContextReleaseCompleteMsg();
  ~UEContextReleaseCompleteMsg();
  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setUserLocationInfoNR(const NrCgi_t& cig, const Tai_t& tai);
  void getUserLocationInfoNR(NrCgi_t& cig, Tai_t& tai);

  void setPduSessionResourceCxtRelCplList(
      const std::vector<PDUSessionResourceCxtRelCplItem_t>& list);
  bool getPduSessionResourceCxtRelCplList(
      std::vector<PDUSessionResourceCxtRelCplItem_t>& list);

 private:
  Ngap_UEContextReleaseComplete_t* ies;
  // AMF_UE_NGAP_ID //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  std::optional<UserLocationInformation> userLocationInformation;  // Optional
  // TODO: Information on Recommended Cells and RAN Nodes for Paging (Optional)
  std::optional<PduSessionResourceListCxtRelCpl>
      pduSessionResourceListCxtRelCpl;  // Optional
  // TODO: Criticality Diagnostics (Optional)
  // TODO: Paging Assistance Data for CE Capable UE (Optional, Rel 16.14.0)
};

}  // namespace ngap

#endif
