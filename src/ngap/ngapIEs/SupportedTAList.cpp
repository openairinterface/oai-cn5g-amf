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

#include "SupportedTAList.hpp"

namespace ngap {

//------------------------------------------------------------------------------
SupportedTAList::SupportedTAList() {}

//------------------------------------------------------------------------------
SupportedTAList::~SupportedTAList() {}

//------------------------------------------------------------------------------
bool SupportedTAList::encode(Ngap_SupportedTAList_t& supportedTAList) {
  for (std::vector<SupportedTaItem>::iterator it = std::begin(supportedTAItems);
       it < std::end(supportedTAItems); ++it) {
    Ngap_SupportedTAItem_t* ta =
        (Ngap_SupportedTAItem_t*) calloc(1, sizeof(Ngap_SupportedTAItem_t));
    if (!it->encode(*ta)) return false;
    if (ASN_SEQUENCE_ADD(&supportedTAList.list, ta) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool SupportedTAList::decode(const Ngap_SupportedTAList_t& pdu) {
  for (int i = 0; i < pdu.list.count; i++) {
    SupportedTaItem item = {};
    if (!item.decode(*pdu.list.array[i])) return false;
    supportedTAItems.push_back(item);
  }

  return true;
}

//------------------------------------------------------------------------------
void SupportedTAList::setSupportedTaItems(
    const std::vector<SupportedTaItem>& items) {
  supportedTAItems = items;
}

//------------------------------------------------------------------------------
void SupportedTAList::getSupportedTaItems(std::vector<SupportedTaItem>& items) {
  items = supportedTAItems;
}

}  // namespace ngap
