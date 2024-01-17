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

#ifndef PDU_SESSION_RESOURCE_FAILED_TO_SETUP_LIST_HO_ACK_H_
#define PDU_SESSION_RESOURCE_FAILED_TO_SETUP_LIST_HO_ACK_H_

#include <vector>

#include "PduSessionResourceItem.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceFailedToSetupListHOAck.h"
}

namespace ngap {

class PduSessionResourceFailedToSetupListHoAck {
 public:
  PduSessionResourceFailedToSetupListHoAck();
  virtual ~PduSessionResourceFailedToSetupListHoAck();

  void set(const std::vector<PduSessionResourceItem>& list);
  void get(std::vector<PduSessionResourceItem>& list) const;

  bool encode(Ngap_PDUSessionResourceFailedToSetupListHOAck_t& list);
  bool decode(const Ngap_PDUSessionResourceFailedToSetupListHOAck_t& List);

 private:
  std::vector<PduSessionResourceItem> item_list_;
};

}  // namespace ngap

#endif
