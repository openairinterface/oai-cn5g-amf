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

#include "PduSessionResourceListCxtRelReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceListCxtRelReq::PduSessionResourceListCxtRelReq() {}

//------------------------------------------------------------------------------
PduSessionResourceListCxtRelReq::~PduSessionResourceListCxtRelReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceListCxtRelReq::set(
    const std::vector<PduSessionResourceItemCxtRelReq>& list) {
  item_list_.clear();
  for (auto i : list) {
    item_list_.push_back(i);
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceListCxtRelReq::get(
    std::vector<PduSessionResourceItemCxtRelReq>& list) const {
  list.clear();
  for (auto i : item_list_) {
    list.push_back(i);
  }
}

//------------------------------------------------------------------------------
bool PduSessionResourceListCxtRelReq::encode(
    Ngap_PDUSessionResourceListCxtRelReq_t&
        pdu_session_resource_list_cxt_rel_req) {
  for (auto& cxt_rel_req : item_list_) {
    Ngap_PDUSessionResourceItemCxtRelReq_t* item =
        (Ngap_PDUSessionResourceItemCxtRelReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceItemCxtRelReq_t));
    if (!item) return false;
    if (!cxt_rel_req.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&pdu_session_resource_list_cxt_rel_req.list, item) !=
        0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceListCxtRelReq::decode(
    const Ngap_PDUSessionResourceListCxtRelReq_t&
        pdu_session_resource_list_cxt_rel_req) {
  for (int i = 0; i < pdu_session_resource_list_cxt_rel_req.list.count; i++) {
    PduSessionResourceItemCxtRelReq item = {};
    if (!item.decode(*pdu_session_resource_list_cxt_rel_req.list.array[i]))
      return false;
    item_list_.push_back(item);
  }
  return true;
}

}  // namespace ngap
