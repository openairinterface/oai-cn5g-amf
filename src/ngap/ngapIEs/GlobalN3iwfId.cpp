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

#include "GlobalN3iwfId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GlobalN3iwfId::GlobalN3iwfId() {
  plmnId  = {};
  n3iwfId = {};
}

//------------------------------------------------------------------------------
GlobalN3iwfId::~GlobalN3iwfId() {}

//------------------------------------------------------------------------------
void GlobalN3iwfId::set(const PlmnId& plmn, const N3IWF_ID& n3iwfid) {
  plmnId  = plmn;
  n3iwfId = n3iwfid;
}

//------------------------------------------------------------------------------
void GlobalN3iwfId::get(PlmnId& plmn, N3IWF_ID& n3iwfid) {
  plmn    = plmnId;
  n3iwfid = n3iwfId;
}

//------------------------------------------------------------------------------
bool GlobalN3iwfId::encode(Ngap_GlobalN3IWF_ID_t* globaln3iwfid) {
  if (!plmnId.encode(globaln3iwfid->pLMNIdentity)) return false;
  if (!n3iwfId.encode(globaln3iwfid->n3IWF_ID)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool GlobalN3iwfId::decode(Ngap_GlobalN3IWF_ID_t* globaln3iwfid) {
  if (!plmnId.decode(globaln3iwfid->pLMNIdentity)) return false;
  if (!n3iwfId.decode(globaln3iwfid->n3IWF_ID)) return false;

  return true;
}
}  // namespace ngap
