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

#include "PduSessionResourceFailedToSetupListSURes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceFailedToSetupListSURes::
    PduSessionResourceFailedToSetupListSURes() {}

//------------------------------------------------------------------------------
PduSessionResourceFailedToSetupListSURes::
    ~PduSessionResourceFailedToSetupListSURes() {}

//------------------------------------------------------------------------------
void PduSessionResourceFailedToSetupListSURes::set(
    const std::vector<PduSessionResourceFailedToSetupItemSURes>& list) {
  m_ItemList = list;
}

//------------------------------------------------------------------------------
void PduSessionResourceFailedToSetupListSURes::get(
    std::vector<PduSessionResourceFailedToSetupItemSURes>& list) const {
  list = m_ItemList;
}
//------------------------------------------------------------------------------
bool PduSessionResourceFailedToSetupListSURes::encode(
    Ngap_PDUSessionResourceFailedToSetupListSURes_t& pduSessionResourceRes)
    const {
  for (auto& item : m_ItemList) {
    Ngap_PDUSessionResourceFailedToSetupItemSURes_t* item_su_res =
        (Ngap_PDUSessionResourceFailedToSetupItemSURes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceFailedToSetupItemSURes_t));
    if (!item_su_res) return false;
    if (!item.encode(*item_su_res)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceRes.list, item_su_res) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceFailedToSetupListSURes::decode(
    const Ngap_PDUSessionResourceFailedToSetupListSURes_t&
        pduSessionResourceRes) {
  for (int i = 0; i < pduSessionResourceRes.list.count; i++) {
    PduSessionResourceFailedToSetupItemSURes item = {};
    if (!item.decode(*pduSessionResourceRes.list.array[i])) return false;
    m_ItemList.push_back(item);
  }

  return true;
}

}  // namespace ngap
