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

#include "GlobalGnbId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GlobalGnbId::GlobalGnbId() {
  m_PlmnId = {};
  m_GnbId  = {};
}

//------------------------------------------------------------------------------
GlobalGnbId::~GlobalGnbId() {}

//------------------------------------------------------------------------------
void GlobalGnbId::set(const PlmnId& plmn, const GnbId& gnbId) {
  m_PlmnId = plmn;
  m_GnbId  = gnbId;
}

//------------------------------------------------------------------------------
void GlobalGnbId::get(PlmnId& plmn, GnbId& gnbId) const {
  plmn  = m_PlmnId;
  gnbId = m_GnbId;
}

//------------------------------------------------------------------------------
bool GlobalGnbId::encode(Ngap_GlobalGNB_ID_t& globalGnbId) const {
  if (!m_PlmnId.encode(globalGnbId.pLMNIdentity)) return false;
  if (!m_GnbId.encode(globalGnbId.gNB_ID)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool GlobalGnbId::decode(const Ngap_GlobalGNB_ID_t& globalGnbId) {
  if (!m_PlmnId.decode(globalGnbId.pLMNIdentity)) return false;
  if (!m_GnbId.decode(globalGnbId.gNB_ID)) return false;

  return true;
}
}  // namespace ngap
