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

#include "BroadcastPlmnItem.hpp"

#include "PlmnId.hpp"
#include "SNssai.hpp"

extern "C" {
#include "Ngap_BroadcastPLMNList.h"
#include "Ngap_SliceSupportItem.h"
}

namespace ngap {

//------------------------------------------------------------------------------
BroadcastPlmnItem::BroadcastPlmnItem() {}

//------------------------------------------------------------------------------
BroadcastPlmnItem::~BroadcastPlmnItem() {}

//------------------------------------------------------------------------------
void BroadcastPlmnItem::set(
    const PlmnId& plmn, const std::vector<SNssai>& sliceList) {
  m_Plmn               = plmn;
  m_SupportedSliceList = sliceList;
}

//------------------------------------------------------------------------------
void BroadcastPlmnItem::get(
    PlmnId& plmn, std::vector<SNssai>& sliceList) const {
  plmn      = m_Plmn;
  sliceList = m_SupportedSliceList;
}

//------------------------------------------------------------------------------
bool BroadcastPlmnItem::encode(Ngap_BroadcastPLMNItem_t& plmnItem) const {
  if (!m_Plmn.encode(plmnItem.pLMNIdentity)) return false;

  for (std::vector<SNssai>::const_iterator it =
           std::begin(m_SupportedSliceList);
       it < std::end(m_SupportedSliceList); ++it) {
    Ngap_SliceSupportItem_t* slice =
        (Ngap_SliceSupportItem_t*) calloc(1, sizeof(Ngap_SliceSupportItem_t));
    if (!slice) return false;
    if (!it->encode(slice->s_NSSAI)) return false;
    if (ASN_SEQUENCE_ADD(&plmnItem.tAISliceSupportList.list, slice) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastPlmnItem::decode(const Ngap_BroadcastPLMNItem_t& pdu) {
  if (!m_Plmn.decode(pdu.pLMNIdentity)) return false;
  for (int i = 0; i < pdu.tAISliceSupportList.list.count; i++) {
    SNssai snssai = {};
    if (!snssai.decode(pdu.tAISliceSupportList.list.array[i]->s_NSSAI))
      return false;
    m_SupportedSliceList.push_back(snssai);
  }
  return true;
}

}  // namespace ngap
