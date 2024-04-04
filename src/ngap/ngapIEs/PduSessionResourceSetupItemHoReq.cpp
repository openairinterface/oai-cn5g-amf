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

#include "PduSessionResourceSetupItemHoReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupItemHoReq::PduSessionResourceSetupItemHoReq()
    : PduSessionResourceItem() {}

//------------------------------------------------------------------------------
PduSessionResourceSetupItemHoReq::~PduSessionResourceSetupItemHoReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupItemHoReq::set(
    const PduSessionId& pduSessionId, const SNssai& sNssai,
    const OCTET_STRING_t& handoverRequestTransfer) {
  PduSessionResourceItem::set(pduSessionId, handoverRequestTransfer);
  m_SNssai = sNssai;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupItemHoReq::get(
    PduSessionId& pduSessionId, SNssai& sNssai,
    OCTET_STRING_t& handoverRequestTransfer) const {
  PduSessionResourceItem::get(pduSessionId, handoverRequestTransfer);
  sNssai = m_SNssai;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemHoReq::encode(
    Ngap_PDUSessionResourceSetupItemHOReq_t& resourceSetupItem) const {
  if (!PduSessionResourceItem::encode(
          resourceSetupItem.pDUSessionID,
          resourceSetupItem.handoverRequestTransfer))
    return false;

  if (!m_SNssai.encode(resourceSetupItem.s_NSSAI)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemHoReq::decode(
    const Ngap_PDUSessionResourceSetupItemHOReq_t& resourceSetupItem) {
  if (!PduSessionResourceItem::decode(
          resourceSetupItem.pDUSessionID,
          resourceSetupItem.handoverRequestTransfer))
    return false;

  if (!m_SNssai.decode(resourceSetupItem.s_NSSAI)) return false;

  return true;
}

}  // namespace ngap
