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

#ifndef _PDU_SESSION_RESOURCE_SETUP_LIST_CXT_REQ_H_
#define _PDU_SESSION_RESOURCE_SETUP_LIST_CXT_REQ_H_

#include "PDUSessionResourceSetupItemCxtReq.hpp"
#include <vector>

extern "C" {
#include "Ngap_PDUSessionResourceSetupListCxtReq.h"
}

namespace ngap {

class PDUSessionResourceSetupListCxtReq {
 public:
  PDUSessionResourceSetupListCxtReq();
  virtual ~PDUSessionResourceSetupListCxtReq();

  void set(const std::vector<PDUSessionResourceSetupItemCxtReq>& itemList);
  void get(std::vector<PDUSessionResourceSetupItemCxtReq>& itemList) const;

  bool encode(Ngap_PDUSessionResourceSetupListCxtReq_t*
                  pduSessionResourceSetupListCxtReq);
  bool decode(Ngap_PDUSessionResourceSetupListCxtReq_t*
                  pduSessionResourceSetupListCxtReq);

 private:
  std::vector<PDUSessionResourceSetupItemCxtReq>
      pduSessionResourceSetupItemCxtReqList;
};

}  // namespace ngap

#endif
