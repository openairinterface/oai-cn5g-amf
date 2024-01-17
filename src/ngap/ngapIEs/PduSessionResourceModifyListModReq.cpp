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

#include "PduSessionResourceModifyListModReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceModifyListModReq::PduSessionResourceModifyListModReq() {}

//------------------------------------------------------------------------------
PduSessionResourceModifyListModReq::~PduSessionResourceModifyListModReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceModifyListModReq::set(
    const std::vector<PduSessionResourceModifyItemModReq>& list) {
  item_list_ = list;
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyListModReq::encode(
    Ngap_PDUSessionResourceModifyListModReq_t&
        pdu_session_resource_modify_list_mod_req) {
  for (auto pdu : item_list_) {
    Ngap_PDUSessionResourceModifyItemModReq_t* item =
        (Ngap_PDUSessionResourceModifyItemModReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceModifyItemModReq_t));

    if (!item) return false;
    if (!pdu.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(
            &pdu_session_resource_modify_list_mod_req.list, item) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyListModReq::decode(
    const Ngap_PDUSessionResourceModifyListModReq_t&
        pdu_session_resource_modify_list_mod_req) {
  uint32_t numberofPDUSessions =
      pdu_session_resource_modify_list_mod_req.list.count;

  for (int i = 0; i < numberofPDUSessions; i++) {
    PduSessionResourceModifyItemModReq item = {};

    if (!item.decode(*pdu_session_resource_modify_list_mod_req.list.array[i]))
      return false;
    item_list_.push_back(item);
  }

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyListModReq::get(
    std::vector<PduSessionResourceModifyItemModReq>& list) const {
  list = item_list_;
}

}  // namespace ngap
