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

#include "PduSessionId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionId::PduSessionId() {
  m_PduSessionId = 0;
}

//------------------------------------------------------------------------------
PduSessionId::~PduSessionId() {}

//------------------------------------------------------------------------------
void PduSessionId::set(const uint8_t& id) {
  m_PduSessionId = id;
}

//------------------------------------------------------------------------------
void PduSessionId::get(uint8_t& id) const {
  id = m_PduSessionId;
}

//------------------------------------------------------------------------------
bool PduSessionId::encode(Ngap_PDUSessionID_t& pduSessionId) const {
  pduSessionId = m_PduSessionId;
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionId::decode(Ngap_PDUSessionID_t pduSessionId) {
  m_PduSessionId = pduSessionId;
  return true;
}

}  // namespace ngap
