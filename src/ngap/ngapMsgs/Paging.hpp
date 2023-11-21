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

#ifndef _PAGING_H_
#define _PAGING_H_

#include "NgapMessage.hpp"
#include "TAIListforPaging.hpp"
#include "UEPagingIdentity.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_Paging.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class PagingMsg : public NgapMessage {
 public:
  PagingMsg();
  virtual ~PagingMsg();

  void initialize();
  bool decode(Ngap_NGAP_PDU_t* ngap_msg_pdu) override;

  void setUEPagingIdentity(
      const std::string& setId, const std::string& pointer,
      const std::string tmsi);
  void getUEPagingIdentity(std::string& _5g_s_tmsi);
  void getUEPagingIdentity(
      std::string& setId, std::string& pointer, std::string& tmsi);

  void setTAIListForPaging(const std::vector<Tai_t>& list);
  void getTAIListForPaging(std::vector<Tai_t>& list);

 private:
  Ngap_Paging_t* pagingIEs;

  UEPagingIdentity uePagingIdentity;  // Mandatory
  // TODO: Paging DRX (Optional)
  TAIListForPaging taIListForPaging;  // Mandatory
  // TODO: Paging Priority (Optional)
  // TODO: UE Radio Capability for Paging (Optional)
  // TODO: Paging Origin (Optional)
  // TODO: Assistance Data for Paging (Optional)
  // TODO: NB-IoT Paging eDRX Information (Optional, Rel 16.14.0)
  // TODO: NB-IoT Paging DRX (Optional, Rel 16.14.0)
  // TODO: Enhanced Coverage Restriction (Optional, Rel 16.14.0)
  // TODO: WUS Assistance Information (Optional, Rel 16.14.0)
  // TODO: Paging eDRX Information (Optional, Rel 16.14.0)
  // TODO: CE-mode-B Restricted (Optional, Rel 16.14.0)
};

}  // namespace ngap

#endif
