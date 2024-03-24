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

#include "GlobalNgEnbId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GlobalNgEnbId::GlobalNgEnbId() {}

//------------------------------------------------------------------------------
GlobalNgEnbId::~GlobalNgEnbId() {}

//------------------------------------------------------------------------------
void GlobalNgEnbId::set(const PlmnId& plmnId, const NgEnbId& ngEnbId) {
  m_PlmnId  = plmnId;
  m_NgEnbId = ngEnbId;
}

//------------------------------------------------------------------------------
void GlobalNgEnbId::get(PlmnId& plmnId, NgEnbId& ngEnbId) const {
  plmnId  = m_PlmnId;
  ngEnbId = m_NgEnbId;
}

//------------------------------------------------------------------------------
bool GlobalNgEnbId::encode(Ngap_GlobalNgENB_ID_t& globalNgEnbId) const {
  if (!m_PlmnId.encode(globalNgEnbId.pLMNIdentity)) return false;
  if (!m_NgEnbId.encode(globalNgEnbId.ngENB_ID)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool GlobalNgEnbId::decode(const Ngap_GlobalNgENB_ID_t& globalNgEnbId) {
  if (!m_PlmnId.decode(globalNgEnbId.pLMNIdentity)) return false;
  if (!m_NgEnbId.decode(globalNgEnbId.ngENB_ID)) return false;
  return true;
}
}  // namespace ngap
