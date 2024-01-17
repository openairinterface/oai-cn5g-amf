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
    const PlmnId& plmn_id, const std::vector<S_NSSAI>& snssais) {
  plmn_id_            = plmn_id;
  slice_support_list_ = snssais;
}

//------------------------------------------------------------------------------
void PlmnSupportItem::get(PlmnId& plmn_id, std::vector<S_NSSAI>& snssais) {
  plmn_id = plmn_id_;
  snssais = slice_support_list_;
}

//------------------------------------------------------------------------------
bool PlmnSupportItem::encode(Ngap_PLMNSupportItem_t& plmn_support_item) {
  if (!plmn_id_.encode(plmn_support_item.pLMNIdentity)) return false;
  for (std::vector<S_NSSAI>::iterator it = std::begin(slice_support_list_);
       it < std::end(slice_support_list_); ++it) {
    Ngap_SliceSupportItem_t* slice =
        (Ngap_SliceSupportItem_t*) calloc(1, sizeof(Ngap_SliceSupportItem_t));
    if (!it->encode(slice->s_NSSAI)) return false;
    ASN_SEQUENCE_ADD(&plmn_support_item.sliceSupportList.list, slice);
  }
  return true;
}

//------------------------------------------------------------------------------
bool PlmnSupportItem::decode(const Ngap_PLMNSupportItem_t& plmn_support_item) {
  if (!plmn_id_.decode(plmn_support_item.pLMNIdentity)) return false;

  for (int i = 0; i < plmn_support_item.sliceSupportList.list.count; i++) {
    S_NSSAI snssai = {};
    if (!snssai.decode(
            plmn_support_item.sliceSupportList.list.array[i]->s_NSSAI))
      return false;
    slice_support_list_.push_back(snssai);
  }
  return true;
}
}  // namespace ngap
