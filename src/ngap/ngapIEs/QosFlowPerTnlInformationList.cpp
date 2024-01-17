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

#include "QosFlowPerTnlInformationList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowPerTnlInformationList::QosFlowPerTnlInformationList() {}

//------------------------------------------------------------------------------
QosFlowPerTnlInformationList::~QosFlowPerTnlInformationList() {}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformationList::set(
    const std::vector<QosFlowPerTnlInformationItem>& list) {
  uint8_t number_items = (list.size() > KMaxNoOfMultiConnectivityMinusOne) ?
                             KMaxNoOfMultiConnectivityMinusOne :
                             list.size();
  list_.insert(list_.begin(), list.begin(), list.begin() + number_items);
}

//------------------------------------------------------------------------------
void QosFlowPerTnlInformationList::get(
    std::vector<QosFlowPerTnlInformationItem>& list) const {
  list = list_;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformationList::encode(
    Ngap_QosFlowPerTNLInformationList_t& qosFlowPerTnlInformationList) {
  for (int i = 0; i < list_.size(); i++) {
    Ngap_QosFlowPerTNLInformationItem_t* item =
        (Ngap_QosFlowPerTNLInformationItem_t*) calloc(
            1, sizeof(Ngap_QosFlowPerTNLInformationItem_t));
    if (!item) return false;
    if (!list_[i].encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&qosFlowPerTnlInformationList.list, item) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowPerTnlInformationList::decode(
    const Ngap_QosFlowPerTNLInformationList_t& qosFlowPerTnlInformationList) {
  for (int i = 0; i < qosFlowPerTnlInformationList.list.count; i++) {
    QosFlowPerTnlInformationItem item = {};
    if (!item.decode(*qosFlowPerTnlInformationList.list.array[i])) return false;
    list_.push_back(item);
  }
  return true;
}

}  // namespace ngap
