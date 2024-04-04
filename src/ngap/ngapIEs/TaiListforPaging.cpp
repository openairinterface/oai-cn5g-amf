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

#include "TaiListforPaging.hpp"

extern "C" {
#include "Ngap_TAIListForPagingItem.h"
}

namespace ngap {

//------------------------------------------------------------------------------
TaiListForPaging::TaiListForPaging() {}

//------------------------------------------------------------------------------
TaiListForPaging::~TaiListForPaging() {}

//------------------------------------------------------------------------------
void TaiListForPaging::set(const std::vector<Tai>& list) {
  m_TaiList = list;
}

//------------------------------------------------------------------------------
void TaiListForPaging::get(std::vector<Tai>& list) const {
  list = m_TaiList;
}
//------------------------------------------------------------------------------
bool TaiListForPaging::encode(Ngap_TAIListForPaging_t& pdu) const {
  for (auto& tai : m_TaiList) {
    Ngap_TAIListForPagingItem_t* ta = (Ngap_TAIListForPagingItem_t*) calloc(
        1, sizeof(Ngap_TAIListForPagingItem_t));
    if (!tai.encode(ta->tAI)) return false;
    if (ASN_SEQUENCE_ADD(&pdu.list, ta) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool TaiListForPaging::decode(const Ngap_TAIListForPaging_t& pdu) {
  if (pdu.list.count < 0) return false;
  for (int i = 0; i < pdu.list.count; i++) {
    Tai tai = {};
    if (!tai.decode(pdu.list.array[i]->tAI)) return false;
    m_TaiList.push_back(tai);
  }

  return true;
}
}  // namespace ngap
