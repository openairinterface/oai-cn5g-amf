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

#include "ServedGuamiList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
ServedGuamiList::ServedGuamiList() {}

//------------------------------------------------------------------------------
ServedGuamiList::~ServedGuamiList() {}

//------------------------------------------------------------------------------
void ServedGuamiList::set(const std::vector<ServedGuamiItem>& list) {
  itemList = list;
}

//------------------------------------------------------------------------------
void ServedGuamiList::get(std::vector<ServedGuamiItem>& list) const {
  list = itemList;
}

//------------------------------------------------------------------------------
void ServedGuamiList::addItem(const ServedGuamiItem& item) {
  itemList.push_back(item);
}

//------------------------------------------------------------------------------
bool ServedGuamiList::encode(Ngap_ServedGUAMIList_t& servedGUAMIList) {
  for (std::vector<ServedGuamiItem>::iterator it = std::begin(itemList);
       it != std::end(itemList); ++it) {
    Ngap_ServedGUAMIItem* guamiItem =
        (Ngap_ServedGUAMIItem*) calloc(1, sizeof(Ngap_ServedGUAMIItem));
    if (!guamiItem) return false;
    if (!it->encode(*guamiItem)) return false;
    if (ASN_SEQUENCE_ADD(&servedGUAMIList.list, guamiItem) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool ServedGuamiList::decode(const Ngap_ServedGUAMIList_t& pdu) {
  itemList.clear();
  for (int i = 0; i < pdu.list.count; i++) {
    ServedGuamiItem item = {};
    if (item.decode(*pdu.list.array[i])) return false;
    itemList.push_back(item);
  }
  return true;
}

}  // namespace ngap
