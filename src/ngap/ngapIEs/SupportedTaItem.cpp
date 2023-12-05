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

#include "SupportedTaItem.hpp"

#include "BroadcastPlmnItem.hpp"

extern "C" {
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_SupportedTAItem.h"
}

namespace ngap {

//------------------------------------------------------------------------------
SupportedTaItem::SupportedTaItem() {}

//------------------------------------------------------------------------------
SupportedTaItem::~SupportedTaItem() {}

//------------------------------------------------------------------------------
void SupportedTaItem::setTac(const TAC& m_tac) {
  tac = m_tac;
}

//------------------------------------------------------------------------------
void SupportedTaItem::getTac(TAC& m_tac) {
  m_tac = tac;
}

//------------------------------------------------------------------------------
void SupportedTaItem::setBroadcastPlmnList(
    const std::vector<BroadcastPlmnItem>& list) {
  broadcastPLMNList = list;
}

//------------------------------------------------------------------------------
void SupportedTaItem::getBroadcastPlmnList(
    std::vector<BroadcastPlmnItem>& list) {
  list = broadcastPLMNList;
}

//------------------------------------------------------------------------------
bool SupportedTaItem::encode(Ngap_SupportedTAItem_t& ta) {
  if (!tac.encode(ta.tAC)) return false;

  for (std::vector<BroadcastPlmnItem>::iterator it =
           std::begin(broadcastPLMNList);
       it < std::end(broadcastPLMNList); ++it) {
    Ngap_BroadcastPLMNItem_t* plmnItem =
        (Ngap_BroadcastPLMNItem*) calloc(1, sizeof(Ngap_BroadcastPLMNItem));
    if (!it->encode(*plmnItem)) return false;
    if (ASN_SEQUENCE_ADD(&ta.broadcastPLMNList.list, plmnItem) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool SupportedTaItem::decode(const Ngap_SupportedTAItem_t& pdu) {
  if (!tac.decode(pdu.tAC)) return false;
  for (int i = 0; i < pdu.broadcastPLMNList.list.count; i++) {
    BroadcastPlmnItem item = {};
    if (!item.decode(*pdu.broadcastPLMNList.list.array[i])) return false;
    broadcastPLMNList.push_back(item);
  }
  return true;
}

}  // namespace ngap
