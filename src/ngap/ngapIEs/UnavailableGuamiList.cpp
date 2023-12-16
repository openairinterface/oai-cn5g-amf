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

#include "UnavailableGuamiList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UnavailableGuamiList::UnavailableGuamiList() {}

//------------------------------------------------------------------------------
UnavailableGuamiList::~UnavailableGuamiList() {}

//------------------------------------------------------------------------------
void UnavailableGuamiList::set(const std::vector<UnavailableGuamiItem>& list) {
  // store maximum 256 items
  uint32_t size =
      (list.size() > kMaxNoOfServedGuami) ? kMaxNoOfServedGuami : list.size();
  list_.assign(list.begin(), list.begin() + size - 1);
}

//------------------------------------------------------------------------------
void UnavailableGuamiList::get(std::vector<UnavailableGuamiItem>& list) const {
  list = list_;
}

//------------------------------------------------------------------------------
void UnavailableGuamiList::addItem(const UnavailableGuamiItem& item) {
  if (list_.size() < kMaxNoOfServedGuami) list_.push_back(item);
}

//------------------------------------------------------------------------------
bool UnavailableGuamiList::encode(Ngap_UnavailableGUAMIList_t& list) {
  for (std::vector<UnavailableGuamiItem>::iterator it = std::begin(list_);
       it != std::end(list_); ++it) {
    Ngap_UnavailableGUAMIItem* item = (Ngap_UnavailableGUAMIItem*) calloc(
        1, sizeof(Ngap_UnavailableGUAMIItem));
    if (!item) return false;
    if (!it->encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&list.list, item) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UnavailableGuamiList::decode(const Ngap_UnavailableGUAMIList_t& list) {
  list_.clear();
  // store maximum 256 items
  uint32_t size = (list.list.count > kMaxNoOfServedGuami) ?
                      kMaxNoOfServedGuami :
                      list.list.count;

  for (int i = 0; i < size; i++) {
    UnavailableGuamiItem item = {};
    if (item.decode(*list.list.array[i])) return false;
    list_.push_back(item);
  }
  return true;
}

}  // namespace ngap
