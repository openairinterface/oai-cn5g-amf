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

#include "PlmnSupportItem.hpp"

extern "C" {
#include "Ngap_SliceSupportItem.h"
}

namespace ngap {

//------------------------------------------------------------------------------
PlmnSupportItem::PlmnSupportItem() {}

//------------------------------------------------------------------------------
PlmnSupportItem::~PlmnSupportItem() {}

//------------------------------------------------------------------------------
void PlmnSupportItem::set(
    const PlmnId& plmnId, const std::vector<SNssai>& sNssais) {
  m_PlmnId           = plmnId;
  m_SliceSupportList = sNssais;
}

//------------------------------------------------------------------------------
void PlmnSupportItem::get(PlmnId& plmnId, std::vector<SNssai>& sNssais) const {
  plmnId  = m_PlmnId;
  sNssais = m_SliceSupportList;
}

//------------------------------------------------------------------------------
bool PlmnSupportItem::encode(Ngap_PLMNSupportItem_t& plmnSupportItem) const {
  if (!m_PlmnId.encode(plmnSupportItem.pLMNIdentity)) return false;
  for (std::vector<SNssai>::const_iterator it = m_SliceSupportList.begin();
       it < m_SliceSupportList.end(); ++it) {
    Ngap_SliceSupportItem_t* slice =
        (Ngap_SliceSupportItem_t*) calloc(1, sizeof(Ngap_SliceSupportItem_t));
    if (!it->encode(slice->s_NSSAI)) return false;
    ASN_SEQUENCE_ADD(&plmnSupportItem.sliceSupportList.list, slice);
  }
  return true;
}

//------------------------------------------------------------------------------
bool PlmnSupportItem::decode(const Ngap_PLMNSupportItem_t& plmnSupportItem) {
  if (!m_PlmnId.decode(plmnSupportItem.pLMNIdentity)) return false;

  for (int i = 0; i < plmnSupportItem.sliceSupportList.list.count; i++) {
    SNssai snssai = {};
    if (!snssai.decode(plmnSupportItem.sliceSupportList.list.array[i]->s_NSSAI))
      return false;
    m_SliceSupportList.push_back(snssai);
  }
  return true;
}
}  // namespace ngap
