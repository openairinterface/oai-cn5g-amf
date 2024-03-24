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

#include "PduSessionResourceReleasedItemRelRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleasedItemRelRes::PduSessionResourceReleasedItemRelRes() {}

//------------------------------------------------------------------------------
PduSessionResourceReleasedItemRelRes::~PduSessionResourceReleasedItemRelRes() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleasedItemRelRes::set(
    const PduSessionId& pduSessionId,
    const OCTET_STRING_t& pduSessionResourceReleaseResponseTransfer) {
  m_PduSessionId = pduSessionId;
  m_PduSessionResourceReleaseResponseTransfer =
      pduSessionResourceReleaseResponseTransfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleasedItemRelRes::get(
    PduSessionId& pduSessionId,
    OCTET_STRING_t& pduSessionResourceReleaseResponseTransfer) const {
  pduSessionId = m_PduSessionId;
  pduSessionResourceReleaseResponseTransfer =
      m_PduSessionResourceReleaseResponseTransfer;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleasedItemRelRes::encode(
    Ngap_PDUSessionResourceReleasedItemRelRes_t& pduSessionResourceItem) const {
  if (!m_PduSessionId.encode(pduSessionResourceItem.pDUSessionID)) return false;

  pduSessionResourceItem.pDUSessionResourceReleaseResponseTransfer =
      m_PduSessionResourceReleaseResponseTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleasedItemRelRes::decode(
    const Ngap_PDUSessionResourceReleasedItemRelRes_t& pduSessionResourceItem) {
  m_PduSessionId.set(pduSessionResourceItem.pDUSessionID);
  m_PduSessionResourceReleaseResponseTransfer =
      pduSessionResourceItem.pDUSessionResourceReleaseResponseTransfer;

  return true;
}

}  // namespace ngap
