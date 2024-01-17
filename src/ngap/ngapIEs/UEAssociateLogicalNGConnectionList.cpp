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

#include "UEAssociatedLogicalNGConnectionList.hpp"

#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UEAssociatedLogicalNGConnectionList::UEAssociatedLogicalNGConnectionList() {}

//------------------------------------------------------------------------------
UEAssociatedLogicalNGConnectionList::~UEAssociatedLogicalNGConnectionList() {}

//------------------------------------------------------------------------------
void UEAssociatedLogicalNGConnectionList::set(
    std::vector<UEAssociatedLogicalNGConnectionItem>& list) {
  list_ = list;
  return;
}

//------------------------------------------------------------------------------
void UEAssociatedLogicalNGConnectionList::get(
    std::vector<UEAssociatedLogicalNGConnectionItem>& list) {
  list = list_;
}

//------------------------------------------------------------------------------
void UEAssociatedLogicalNGConnectionList::addItem(
    UEAssociatedLogicalNGConnectionItem& item) {
  list_.push_back(item);
}

//------------------------------------------------------------------------------
bool UEAssociatedLogicalNGConnectionList::encode(
    Ngap_UE_associatedLogicalNG_connectionList_t& list) {
  for (auto l : list_) {
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
bool UEAssociatedLogicalNGConnectionList::decode(
    const Ngap_UE_associatedLogicalNG_connectionList_t& list) {
  list_.clear();
  for (int i = 0; i < list.list.count; i++) {
    UEAssociatedLogicalNGConnectionItem item = {};
    if (!item.decode(*list.list.array[i])) return false;
    list_.push_back(item);
  }
  return true;
}

}  // namespace ngap
