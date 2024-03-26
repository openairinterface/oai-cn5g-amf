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

#include "PduSessionResourceToReleaseItemRelCmd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceToReleaseItemRelCmd::PduSessionResourceToReleaseItemRelCmd() {
}

//------------------------------------------------------------------------------
PduSessionResourceToReleaseItemRelCmd::
    ~PduSessionResourceToReleaseItemRelCmd() {}

//------------------------------------------------------------------------------
void PduSessionResourceToReleaseItemRelCmd::set(
    const PduSessionId& pduSessionId,
    const OCTET_STRING_t& pduSessionResourceReleaseCommandTransfer) {
  m_PduSessionId = pduSessionId;
  m_PduSessionResourceReleaseCommandTransfer =
      pduSessionResourceReleaseCommandTransfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceToReleaseItemRelCmd::get(
    PduSessionId& pduSessionId,
    OCTET_STRING_t& pduSessionResourceReleaseCommandTransfer) const {
  pduSessionId = m_PduSessionId;
  pduSessionResourceReleaseCommandTransfer =
      m_PduSessionResourceReleaseCommandTransfer;
}

//------------------------------------------------------------------------------
bool PduSessionResourceToReleaseItemRelCmd::encode(
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t& pduSessionResourceItem)
    const {
  if (!m_PduSessionId.encode(pduSessionResourceItem.pDUSessionID)) return false;

  pduSessionResourceItem.pDUSessionResourceReleaseCommandTransfer =
      m_PduSessionResourceReleaseCommandTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceToReleaseItemRelCmd::decode(
    const Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
        pduSessionResourceItem) {
  m_PduSessionResourceReleaseCommandTransfer =
      pduSessionResourceItem.pDUSessionResourceReleaseCommandTransfer;

  return true;
}

}  // namespace ngap
