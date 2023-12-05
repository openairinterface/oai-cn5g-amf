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

#include "QosFlowListWithDataForwarding.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowListWithDataForwarding::QosFlowListWithDataForwarding() {}

//------------------------------------------------------------------------------
QosFlowListWithDataForwarding::~QosFlowListWithDataForwarding() {}

//------------------------------------------------------------------------------
bool QosFlowListWithDataForwarding::decode(
    const Ngap_QosFlowListWithDataForwarding& qosFlowSetupResponseList) {
  for (int i = 0; i < qosFlowSetupResponseList.list.count; i++) {
    QosFlowItemWithDataForWarding item = {};
    if (!item.decode(*qosFlowSetupResponseList.list.array[i])) return false;
    list_.push_back(item);
  }
  return true;
}

//------------------------------------------------------------------------------
void QosFlowListWithDataForwarding::set(
    const std::vector<QosFlowItemWithDataForWarding>& list) {
  uint8_t number_items =
      (list.size() > kMaxNoQoSFlows) ? kMaxNoQoSFlows : list.size();
  list_.insert(list_.begin(), list.begin(), list.begin() + number_items);
}

//------------------------------------------------------------------------------
void QosFlowListWithDataForwarding::get(
    std::vector<QosFlowItemWithDataForWarding>& list) const {
  list = list_;
}

}  // namespace ngap
