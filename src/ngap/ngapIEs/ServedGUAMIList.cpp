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

#include "ServedGUAMIList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
ServedGUAMIList::ServedGUAMIList() {}

//------------------------------------------------------------------------------
ServedGUAMIList::~ServedGUAMIList() {}

//------------------------------------------------------------------------------
void ServedGUAMIList::set(const std::vector<ServedGUAMIItem>& list) {
  itemList = list;
}

//------------------------------------------------------------------------------
void ServedGUAMIList::get(std::vector<ServedGUAMIItem>& list) const {
  list = itemList;
}

//------------------------------------------------------------------------------
void ServedGUAMIList::addItem(const ServedGUAMIItem& item) {
  itemList.push_back(item);
}

//------------------------------------------------------------------------------
bool ServedGUAMIList::encode(Ngap_ServedGUAMIList_t& servedGUAMIList) {
  for (std::vector<ServedGUAMIItem>::iterator it = std::begin(itemList);
       it != std::end(itemList); ++it) {
    Ngap_ServedGUAMIItem* guamiItem =
        (Ngap_ServedGUAMIItem*) calloc(1, sizeof(Ngap_ServedGUAMIItem));
    if (!guamiItem) return false;
    if (!it->encode2ServedGUAMIItem(guamiItem)) return false;
    if (ASN_SEQUENCE_ADD(&servedGUAMIList.list, guamiItem) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool ServedGUAMIList::decode(const Ngap_ServedGUAMIList_t& pdu) {
  itemList.clear();
  for (int i = 0; i < pdu.list.count; i++) {
    ServedGUAMIItem item = {};
    if (item.decodefromServedGUAMIItem(pdu.list.array[i])) return false;
    itemList.push_back(item);
  }
  return true;
}

}  // namespace ngap
