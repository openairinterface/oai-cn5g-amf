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

#include "PduSessionResourceReleaseItemCmd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseItemCmd::PduSessionResourceReleaseItemCmd() {}

//------------------------------------------------------------------------------
PduSessionResourceReleaseItemCmd::~PduSessionResourceReleaseItemCmd() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseItemCmd::set(
    const PduSessionId& pduSessionId,
    const OCTET_STRING_t& pduSessionResourceRelease) {
  m_PduSessionId                             = pduSessionId;
  m_PduSessionResourceReleaseCommandTransfer = pduSessionResourceRelease;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseItemCmd::encode(
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
        pduSessionResourceReleaseCommandTransfer) const {
  if (!m_PduSessionId.encode(
          pduSessionResourceReleaseCommandTransfer.pDUSessionID))
    return false;
  pduSessionResourceReleaseCommandTransfer
      .pDUSessionResourceReleaseCommandTransfer =
      m_PduSessionResourceReleaseCommandTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseItemCmd::decode(
    const Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
        pduSessionResourceReleaseCommandTransfer) {
  if (!m_PduSessionId.decode(
          pduSessionResourceReleaseCommandTransfer.pDUSessionID))
    return false;
  m_PduSessionResourceReleaseCommandTransfer =
      pduSessionResourceReleaseCommandTransfer
          .pDUSessionResourceReleaseCommandTransfer;

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseItemCmd::get(
    PduSessionId& pduSessionId,
    OCTET_STRING_t& pduSessionResourceRelease) const {
  pduSessionId              = m_PduSessionId;
  pduSessionResourceRelease = m_PduSessionResourceReleaseCommandTransfer;
}

}  // namespace ngap
