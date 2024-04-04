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

#include "PduSessionResourceSetupItemSURes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupItemSURes::PduSessionResourceSetupItemSURes() {}

//------------------------------------------------------------------------------
PduSessionResourceSetupItemSURes::~PduSessionResourceSetupItemSURes() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupItemSURes::set(
    const PduSessionId& pduSessionId,
    const OCTET_STRING_t& pduSessionResourceSetupResponseTransfer) {
  m_PduSessionId = pduSessionId;
  m_PduSessionResourceSetupResponseTransfer =
      pduSessionResourceSetupResponseTransfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupItemSURes::get(
    PduSessionId& pduSessionId,
    OCTET_STRING_t& pduSessionResourceSetupResponseTransfer) const {
  pduSessionId = m_PduSessionId;
  pduSessionResourceSetupResponseTransfer =
      m_PduSessionResourceSetupResponseTransfer;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemSURes::encode(
    Ngap_PDUSessionResourceSetupItemSURes_t& pduSessionResourceItem) const {
  if (!m_PduSessionId.encode(pduSessionResourceItem.pDUSessionID)) return false;
  pduSessionResourceItem.pDUSessionResourceSetupResponseTransfer =
      m_PduSessionResourceSetupResponseTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemSURes::decode(
    const Ngap_PDUSessionResourceSetupItemSURes_t& pduSessionResourceItem) {
  if (!m_PduSessionId.decode(pduSessionResourceItem.pDUSessionID)) return false;
  m_PduSessionResourceSetupResponseTransfer =
      pduSessionResourceItem.pDUSessionResourceSetupResponseTransfer;

  return true;
}

}  // namespace ngap
