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

#include "PduSessionResourceSetupListCxtRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupListCxtRes::PduSessionResourceSetupListCxtRes() {}

//------------------------------------------------------------------------------
PduSessionResourceSetupListCxtRes::~PduSessionResourceSetupListCxtRes() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListCxtRes::set(
    const std::vector<PduSessionResourceSetupItemCxtRes>& list) {
  item_list_ = list;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupListCxtRes::get(
    std::vector<PduSessionResourceSetupItemCxtRes>& list) const {
  list = item_list_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListCxtRes::encode(
    Ngap_PDUSessionResourceSetupListCxtRes_t&
        pduSessionResourceSetupListCxtRes) {
  for (std::vector<PduSessionResourceSetupItemCxtRes>::iterator it =
           std::begin(item_list_);
       it < std::end(item_list_); ++it) {
    Ngap_PDUSessionResourceSetupItemCxtRes_t* response =
        (Ngap_PDUSessionResourceSetupItemCxtRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemCxtRes_t));
    if (!response) return false;
    if (!it->encode(*response)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListCxtRes.list, response) !=
        0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupListCxtRes::decode(
    const Ngap_PDUSessionResourceSetupListCxtRes_t&
        pduSessionResourceSetupListCxtRes) {
  item_list_.reserve(pduSessionResourceSetupListCxtRes.list.count);
  for (int i = 0; i < pduSessionResourceSetupListCxtRes.list.count; i++) {
    PduSessionResourceSetupItemCxtRes item = {};
    if (!item.decode(*pduSessionResourceSetupListCxtRes.list.array[i]))
      return false;
    item_list_.push_back(item);
  }

  return true;
}

}  // namespace ngap
