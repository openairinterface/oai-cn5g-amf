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

#include "PduSessionResourceModifyListModRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceModifyListModRes::PduSessionResourceModifyListModRes() {}

//------------------------------------------------------------------------------
PduSessionResourceModifyListModRes::~PduSessionResourceModifyListModRes() {}

//------------------------------------------------------------------------------
void PduSessionResourceModifyListModRes::set(
    const std::vector<PduSessionResourceModifyItemModRes>& list) {
  item_list_ = list;
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyListModRes::encode(
    Ngap_PDUSessionResourceModifyListModRes_t& list) const {
  for (auto pdu : item_list_) {
    Ngap_PDUSessionResourceModifyItemModRes_t* item =
        (Ngap_PDUSessionResourceModifyItemModRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceModifyItemModRes_t));

    if (!item) return false;
    if (!pdu.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&list.list, item) != 0) return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyListModRes::decode(
    const Ngap_PDUSessionResourceModifyListModRes_t&
        pdu_session_resource_modify_list_mod_res) {
  uint32_t num_pdu_sessions =
      pdu_session_resource_modify_list_mod_res.list.count;

  for (int i = 0; i < num_pdu_sessions; i++) {
    PduSessionResourceModifyItemModRes item = {};

    if (!item.decode(*pdu_session_resource_modify_list_mod_res.list.array[i]))
      return false;
    item_list_.push_back(item);
  }

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyListModRes::get(
    std::vector<PduSessionResourceModifyItemModRes>& list) const {
  list = item_list_;
}

}  // namespace ngap
