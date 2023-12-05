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

#include "QosFlowSetupRequestList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowSetupRequestList::QosFlowSetupRequestList() {}

//------------------------------------------------------------------------------
QosFlowSetupRequestList::~QosFlowSetupRequestList() {}

//------------------------------------------------------------------------------
void QosFlowSetupRequestList::set(
    const std::vector<QosFlowSetupRequestItem>& list) {
  uint8_t number_items =
      (list.size() > kMaxNoOfQoSFlows) ? kMaxNoOfQoSFlows : list.size();
  list_.insert(list_.begin(), list.begin(), list.begin() + number_items);
}

//------------------------------------------------------------------------------
void QosFlowSetupRequestList::get(
    std::vector<QosFlowSetupRequestItem>& list) const {
  list = list_;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestList::encode(
    Ngap_QosFlowSetupRequestList_t& qosFlowSetupRequestList) {
  for (int i = 0; i < list_.size(); i++) {
    Ngap_QosFlowSetupRequestItem_t* item =
        (Ngap_QosFlowSetupRequestItem_t*) calloc(
            1, sizeof(Ngap_QosFlowSetupRequestItem_t));
    if (!item) return false;
    if (!list_[i].encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&qosFlowSetupRequestList.list, item) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestList::decode(
    const Ngap_QosFlowSetupRequestList_t& qosFlowSetupRequestList) {
  for (int i = 0; i < qosFlowSetupRequestList.list.count; i++) {
    QosFlowSetupRequestItem item = {};
    if (!item.decode(*qosFlowSetupRequestList.list.array[i])) return false;
    list_.push_back(item);
  }
  return true;
}

}  // namespace ngap
