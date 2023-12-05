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

#include "GlobalgNBId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GlobalgNBId::GlobalgNBId() {
  plmnId = {};
  gNB_ID = {};
}

//------------------------------------------------------------------------------
GlobalgNBId::~GlobalgNBId() {}

//------------------------------------------------------------------------------
void GlobalgNBId::set(const PlmnId& plmn, const GNB_ID& gnbid) {
  plmnId = plmn;
  gNB_ID = gnbid;
}

//------------------------------------------------------------------------------
void GlobalgNBId::get(PlmnId& plmn, GNB_ID& gnbid) const {
  plmn  = plmnId;
  gnbid = gNB_ID;
}

//------------------------------------------------------------------------------
bool GlobalgNBId::encode(Ngap_GlobalGNB_ID_t& globalgnbid) {
  if (!plmnId.encode(globalgnbid.pLMNIdentity)) return false;
  if (!gNB_ID.encode(globalgnbid.gNB_ID)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool GlobalgNBId::decode(const Ngap_GlobalGNB_ID_t& globalgnbid) {
  if (!plmnId.decode(globalgnbid.pLMNIdentity)) return false;
  if (!gNB_ID.decode(globalgnbid.gNB_ID)) return false;

  return true;
}
}  // namespace ngap
