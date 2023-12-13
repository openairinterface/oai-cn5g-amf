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

#include "PduSessionResourceSetupListSURes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupListSURes::PduSessionResourceSetupListSURes() {}

//------------------------------------------------------------------------------
PduSessionResourceSetupListSURes::~PduSessionResourceSetupListSURes() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListSURes::set(
    const std::vector<PduSessionResourceSetupItemSURes>& list) {
  item_list_ = list;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListSURes::get(
    std::vector<PduSessionResourceSetupItemSURes>& list) const {
  list = item_list_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListSURes::encode(
    Ngap_PDUSessionResourceSetupListSURes_t&
        pdu_session_resource_setup_list_su_res) {
  for (auto& item : item_list_) {
    Ngap_PDUSessionResourceSetupItemSURes_t* response =
        (Ngap_PDUSessionResourceSetupItemSURes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemSURes_t));
    if (!response) return false;
    if (!item.encode(*response)) return false;
    if (ASN_SEQUENCE_ADD(
            &pdu_session_resource_setup_list_su_res.list, response) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListSURes::decode(
    const Ngap_PDUSessionResourceSetupListSURes_t&
        pdu_session_resource_setup_list_su_res) {
  for (int i = 0; i < pdu_session_resource_setup_list_su_res.list.count; i++) {
    PduSessionResourceSetupItemSURes item = {};
    if (!item.decode(*pdu_session_resource_setup_list_su_res.list.array[i]))
      return false;
    item_list_.push_back(item);
  }

  return true;
}

}  // namespace ngap
