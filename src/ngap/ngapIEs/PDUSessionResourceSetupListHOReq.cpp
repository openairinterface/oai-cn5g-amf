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

#include "PDUSessionResourceSetupListHOReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupListHOReq::PDUSessionResourceSetupListHOReq() {}

//------------------------------------------------------------------------------
PDUSessionResourceSetupListHOReq::~PDUSessionResourceSetupListHOReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListHOReq::set(
    const std::vector<PDUSessionResourceSetupItemHOReq>& list) {
  list_ = list;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListHOReq::get(
    std::vector<PDUSessionResourceSetupItemHOReq>& list) {
  list = list_;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListHOReq::encode(
    Ngap_PDUSessionResourceSetupListHOReq_t* resource_list) {
  for (auto item : list_) {
    Ngap_PDUSessionResourceSetupItemHOReq_t* request =
        (Ngap_PDUSessionResourceSetupItemHOReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemHOReq_t));
    if (!request) return false;
    if (!item.encode(request)) return false;
    if (ASN_SEQUENCE_ADD(&resource_list->list, request) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListHOReq::decode(
    Ngap_PDUSessionResourceSetupListHOReq_t* resource_list) {
  for (int i = 0; i < resource_list->list.count; i++) {
    PDUSessionResourceSetupItemHOReq item = {};
    if (!item.decode(resource_list->list.array[i])) return false;
    list_.push_back(item);
  }
  return true;
}

}  // namespace ngap
