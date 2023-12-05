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

#include "GlobalNgENBId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GlobalNgENBId::GlobalNgENBId() {}

//------------------------------------------------------------------------------
GlobalNgENBId::~GlobalNgENBId() {}

//------------------------------------------------------------------------------
void GlobalNgENBId::set(const PlmnId& plmn_id, const NgENB_ID& ng_enb_id) {
  plmn_id_   = plmn_id;
  ng_enb_id_ = ng_enb_id;
}

//------------------------------------------------------------------------------
void GlobalNgENBId::get(PlmnId& plmn_id, NgENB_ID& ng_enb_id) const {
  plmn_id   = plmn_id_;
  ng_enb_id = ng_enb_id_;
}

//------------------------------------------------------------------------------
bool GlobalNgENBId::encode(Ngap_GlobalNgENB_ID_t& global_ng_enb_id) {
  if (!plmn_id_.encode(global_ng_enb_id.pLMNIdentity)) return false;
  if (!ng_enb_id_.encode(global_ng_enb_id.ngENB_ID)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool GlobalNgENBId::decode(const Ngap_GlobalNgENB_ID_t& global_ng_enb_id) {
  if (!plmn_id_.decode(global_ng_enb_id.pLMNIdentity)) return false;
  if (!ng_enb_id_.decode(global_ng_enb_id.ngENB_ID)) return false;
  return true;
}
}  // namespace ngap
