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

#include "PduSessionResourceListCxtRelCpl.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceListCxtRelCpl::PduSessionResourceListCxtRelCpl() {}

//------------------------------------------------------------------------------
PduSessionResourceListCxtRelCpl::~PduSessionResourceListCxtRelCpl() {}

//------------------------------------------------------------------------------
void PduSessionResourceListCxtRelCpl::set(
    const std::vector<PduSessionResourceItemCxtRelCpl>& list) {
  m_ItemList.clear();
  for (auto i : list) {
    m_ItemList.push_back(i);
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceListCxtRelCpl::get(
    std::vector<PduSessionResourceItemCxtRelCpl>& list) const {
  list.clear();
  for (auto i : m_ItemList) {
    list.push_back(i);
  }
}

//------------------------------------------------------------------------------
bool PduSessionResourceListCxtRelCpl::encode(
    Ngap_PDUSessionResourceListCxtRelCpl_t& pduSessionResourceListCxtRelCpl)
    const {
  for (auto& cxtRelCpl : m_ItemList) {
    Ngap_PDUSessionResourceItemCxtRelCpl_t* item =
        (Ngap_PDUSessionResourceItemCxtRelCpl_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceItemCxtRelCpl_t));
    if (!item) return false;
    if (!cxtRelCpl.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceListCxtRelCpl.list, item) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceListCxtRelCpl::decode(
    const Ngap_PDUSessionResourceListCxtRelCpl_t&
        pduSessionResourceListCxtRelCpl) {
  for (int i = 0; i < pduSessionResourceListCxtRelCpl.list.count; i++) {
    PduSessionResourceItemCxtRelCpl item = {};
    if (!item.decode(*pduSessionResourceListCxtRelCpl.list.array[i]))
      return false;
    m_ItemList.push_back(item);
  }
  return true;
}

}  // namespace ngap
