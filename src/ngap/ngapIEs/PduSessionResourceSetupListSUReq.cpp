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

#include "PduSessionResourceSetupListSUReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupListSUReq::PduSessionResourceSetupListSUReq() {}

//------------------------------------------------------------------------------
PduSessionResourceSetupListSUReq::~PduSessionResourceSetupListSUReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListSUReq::set(
    const std::vector<PduSessionResourceSetupItemSUReq>& list) {
  m_ItemList = list;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListSUReq::get(
    std::vector<PduSessionResourceSetupItemSUReq>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListSUReq::encode(
    Ngap_PDUSessionResourceSetupListSUReq_t& pduSessionResourceList) const {
  for (auto& item : m_ItemList) {
    Ngap_PDUSessionResourceSetupItemSUReq_t* request =
        (Ngap_PDUSessionResourceSetupItemSUReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemSUReq_t));
    if (!request) return false;
    if (!item.encode(*request)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceList.list, request) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListSUReq::decode(
    const Ngap_PDUSessionResourceSetupListSUReq_t& pduSessionResourceList) {
  for (int i = 0; i < pduSessionResourceList.list.count; i++) {
    PduSessionResourceSetupItemSUReq item = {};
    if (!item.decode(*pduSessionResourceList.list.array[i])) return false;
    m_ItemList.push_back(item);
  }

  return true;
}

}  // namespace ngap
