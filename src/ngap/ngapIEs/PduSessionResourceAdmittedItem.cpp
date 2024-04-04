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

#include "PduSessionResourceAdmittedItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceAdmittedItem::PduSessionResourceAdmittedItem() {}

//------------------------------------------------------------------------------
PduSessionResourceAdmittedItem::~PduSessionResourceAdmittedItem() {}

//------------------------------------------------------------------------------
void PduSessionResourceAdmittedItem::set(
    const PduSessionId& pduSessionID,
    const OCTET_STRING_t& handoverRequestAckTransfer) {
  m_PduSessionId               = pduSessionID;
  m_HandoverRequestAckTransfer = handoverRequestAckTransfer;
}
//------------------------------------------------------------------------------
void PduSessionResourceAdmittedItem::get(
    PduSessionId& pduSessionID,
    OCTET_STRING_t& handoverRequestAckTransfer) const {
  pduSessionID               = m_PduSessionId;
  handoverRequestAckTransfer = m_HandoverRequestAckTransfer;
}

//------------------------------------------------------------------------------
bool PduSessionResourceAdmittedItem::encode(
    Ngap_PDUSessionResourceAdmittedItem_t& pduItem) const {
  if (!m_PduSessionId.encode(pduItem.pDUSessionID)) return false;
  pduItem.handoverRequestAcknowledgeTransfer = m_HandoverRequestAckTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceAdmittedItem::decode(
    const Ngap_PDUSessionResourceAdmittedItem_t& pduItem) {
  if (!m_PduSessionId.decode(pduItem.pDUSessionID)) return false;
  m_HandoverRequestAckTransfer = pduItem.handoverRequestAcknowledgeTransfer;

  return true;
}
}  // namespace ngap
