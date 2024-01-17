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

#ifndef _PDU_SESSION_RESOURCE_SETUP_LIST_SU_RES_H_
#define _PDU_SESSION_RESOURCE_SETUP_LIST_SU_RES_H_

#include <vector>

#include "PduSessionResourceSetupItemSURes.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupListSURes.h"
}

namespace ngap {

class PduSessionResourceSetupListSURes {
 public:
  PduSessionResourceSetupListSURes();
  virtual ~PduSessionResourceSetupListSURes();

  void set(const std::vector<PduSessionResourceSetupItemSURes>& list);
  void get(std::vector<PduSessionResourceSetupItemSURes>& list) const;

  bool encode(Ngap_PDUSessionResourceSetupListSURes_t&
                  pdu_session_resource_setup_list_su_res);
  bool decode(const Ngap_PDUSessionResourceSetupListSURes_t&
                  pdu_session_resource_setup_list_su_res);

 private:
  std::vector<PduSessionResourceSetupItemSURes> item_list_;
};

}  // namespace ngap

#endif
