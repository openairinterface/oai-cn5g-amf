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

#include "UeAssociatedLogicalNgConnectionList.hpp"
#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UeAssociatedLogicalNgConnectionList::UeAssociatedLogicalNgConnectionList() {}

//------------------------------------------------------------------------------
UeAssociatedLogicalNgConnectionList::~UeAssociatedLogicalNgConnectionList() {}

//------------------------------------------------------------------------------
void UeAssociatedLogicalNgConnectionList::set(
    const std::vector<UeAssociatedLogicalNgConnectionItem>& list) {
  m_ItemList = list;
  return;
}

//------------------------------------------------------------------------------
void UeAssociatedLogicalNgConnectionList::get(
    std::vector<UeAssociatedLogicalNgConnectionItem>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
void UeAssociatedLogicalNgConnectionList::addItem(
    UeAssociatedLogicalNgConnectionItem& item) {
  m_ItemList.push_back(item);
}

//------------------------------------------------------------------------------
bool UeAssociatedLogicalNgConnectionList::encode(
    Ngap_UE_associatedLogicalNG_connectionList_t& list) const {
  for (auto l : m_ItemList) {
    Ngap_UE_associatedLogicalNG_connectionItem_t* item =
        (Ngap_UE_associatedLogicalNG_connectionItem_t*) calloc(
            1, sizeof(Ngap_UE_associatedLogicalNG_connectionItem_t));
    if (!item) return false;
    if (!l.encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&list.list, item) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UeAssociatedLogicalNgConnectionList::decode(
    const Ngap_UE_associatedLogicalNG_connectionList_t& list) {
  m_ItemList.clear();
  for (int i = 0; i < list.list.count; i++) {
    UeAssociatedLogicalNgConnectionItem item = {};
    if (!item.decode(*list.list.array[i])) return false;
    m_ItemList.push_back(item);
  }
  return true;
}

}  // namespace ngap
