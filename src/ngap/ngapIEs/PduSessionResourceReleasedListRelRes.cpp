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

#include "PduSessionResourceReleasedListRelRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleasedListRelRes::PduSessionResourceReleasedListRelRes() {}

//------------------------------------------------------------------------------
PduSessionResourceReleasedListRelRes::~PduSessionResourceReleasedListRelRes() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleasedListRelRes::set(
    const std::vector<PduSessionResourceReleasedItemRelRes>& list) {
  item_list_ = list;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleasedListRelRes::get(
    std::vector<PduSessionResourceReleasedItemRelRes>& list) {
  list = item_list_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleasedListRelRes::encode(
    Ngap_PDUSessionResourceReleasedListRelRes_t&
        pdu_session_resource_released_list_rel_res) {
  for (auto& item : item_list_) {
    Ngap_PDUSessionResourceReleasedItemRelRes_t* rel =
        (Ngap_PDUSessionResourceReleasedItemRelRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceReleasedItemRelRes_t));
    if (!rel) return false;
    if (!item.encode(*rel)) return false;
    if (ASN_SEQUENCE_ADD(
            &pdu_session_resource_released_list_rel_res.list, rel) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleasedListRelRes::decode(
    const Ngap_PDUSessionResourceReleasedListRelRes_t&
        pdu_session_resource_released_list_rel_res) {
  for (int i = 0; i < pdu_session_resource_released_list_rel_res.list.count;
       i++) {
    PduSessionResourceReleasedItemRelRes item = {};
    if (!item.decode(*pdu_session_resource_released_list_rel_res.list.array[i]))
      return false;
    item_list_.push_back(item);
  }
  return true;
}

}  // namespace ngap
