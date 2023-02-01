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

#include "PDUSessionResourceFailedToSetupListCxtRes.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListCxtRes::
    PDUSessionResourceFailedToSetupListCxtRes() {}

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListCxtRes::
    ~PDUSessionResourceFailedToSetupListCxtRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListCxtRes::set(
    const std::vector<PDUSessionResourceFailedToSetupItemCxtRes>& list) {
  item_list_ = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListCxtRes::get(
    std::vector<PDUSessionResourceFailedToSetupItemCxtRes>& list) {
  list = item_list_;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListCxtRes::encode(
    Ngap_PDUSessionResourceFailedToSetupListCxtRes_t*
        pduSessionResourceFailedToSetupListCxtRes) {
  for (std::vector<PDUSessionResourceFailedToSetupItemCxtRes>::iterator it =
           std::begin(item_list_);
       it < std::end(item_list_); ++it) {
    Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t* failedToResponse =
        (Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t));
    if (!failedToResponse) return false;

    if (!it->encode(failedToResponse)) return false;
    if (ASN_SEQUENCE_ADD(
            &pduSessionResourceFailedToSetupListCxtRes->list,
            failedToResponse) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListCxtRes::decode(
    Ngap_PDUSessionResourceFailedToSetupListCxtRes_t*
        pduSessionResourceFailedToSetupListCxtRes) {
  item_list_.reserve(pduSessionResourceFailedToSetupListCxtRes->list.count);
  for (int i = 0; i < pduSessionResourceFailedToSetupListCxtRes->list.count;
       i++) {
    PDUSessionResourceFailedToSetupItemCxtRes item = {};
    if (!item.decode(pduSessionResourceFailedToSetupListCxtRes->list.array[i]))
      return false;
    item_list_.push_back(item);
  }

  return true;
}

}  // namespace ngap
