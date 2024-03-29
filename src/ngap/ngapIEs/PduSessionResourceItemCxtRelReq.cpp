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

#include "PduSessionResourceItemCxtRelReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceItemCxtRelReq::PduSessionResourceItemCxtRelReq() {}

//------------------------------------------------------------------------------
PduSessionResourceItemCxtRelReq::~PduSessionResourceItemCxtRelReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceItemCxtRelReq::set(const PduSessionId& pduSessionId) {
  m_PduSessionId = pduSessionId;
}

//------------------------------------------------------------------------------
void PduSessionResourceItemCxtRelReq::get(PduSessionId& pduSessionId) const {
  pduSessionId = m_PduSessionId;
}

//------------------------------------------------------------------------------
PduSessionId PduSessionResourceItemCxtRelReq::get() const {
  return m_PduSessionId;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemCxtRelReq::encode(
    Ngap_PDUSessionResourceItemCxtRelReq_t& pduSessionResourceItem) const {
  if (!m_PduSessionId.encode(pduSessionResourceItem.pDUSessionID)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemCxtRelReq::decode(
    const Ngap_PDUSessionResourceItemCxtRelReq_t& pduSessionResourceItem) {
  m_PduSessionId.set(pduSessionResourceItem.pDUSessionID);
  return true;
}

}  // namespace ngap
