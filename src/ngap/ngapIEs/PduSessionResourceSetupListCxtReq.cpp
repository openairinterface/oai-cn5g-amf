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

#include "PduSessionResourceSetupListCxtReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupListCxtReq::PduSessionResourceSetupListCxtReq() {}

//------------------------------------------------------------------------------
PduSessionResourceSetupListCxtReq::~PduSessionResourceSetupListCxtReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListCxtReq::set(
    const std::vector<PduSessionResourceSetupItemCxtReq>& itemList) {
  pduSessionResourceSetupItemCxtReqList = itemList;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListCxtReq::get(
    std::vector<PduSessionResourceSetupItemCxtReq>& itemList) const {}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListCxtReq::encode(
    Ngap_PDUSessionResourceSetupListCxtReq_t&
        pduSessionResourceSetupListCxtReq) {
  for (std::vector<PduSessionResourceSetupItemCxtReq>::iterator it =
           std::begin(pduSessionResourceSetupItemCxtReqList);
       it < std::end(pduSessionResourceSetupItemCxtReqList); ++it) {
    Ngap_PDUSessionResourceSetupItemCxtReq_t* request =
        (Ngap_PDUSessionResourceSetupItemCxtReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemCxtReq_t));
    if (!request) return false;
    if (!it->encode(*request)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListCxtReq.list, request) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListCxtReq::decode(
    const Ngap_PDUSessionResourceSetupListCxtReq_t&
        pduSessionResourceSetupListCxtReq) {
  pduSessionResourceSetupItemCxtReqList.reserve(
      pduSessionResourceSetupListCxtReq.list.count);

  for (int i = 0; i < pduSessionResourceSetupListCxtReq.list.count; i++) {
    PduSessionResourceSetupItemCxtReq item = {};
    if (!item.decode(*pduSessionResourceSetupListCxtReq.list.array[i]))
      return false;
    pduSessionResourceSetupItemCxtReqList.push_back(item);
  }

  return true;
}

}  // namespace ngap
