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

#include "PDUSessionResourceAdmittedItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceAdmittedItem::PDUSessionResourceAdmittedItem() {}

//------------------------------------------------------------------------------
PDUSessionResourceAdmittedItem::~PDUSessionResourceAdmittedItem() {}

//------------------------------------------------------------------------------
void PDUSessionResourceAdmittedItem::set(
    const PDUSessionID& m_pDUSessionID,
    const OCTET_STRING_t& m_handoverRequestAckTransfer) {
  pDUSessionID               = m_pDUSessionID;
  handoverRequestAckTransfer = m_handoverRequestAckTransfer;
}
//------------------------------------------------------------------------------
void PDUSessionResourceAdmittedItem::get(
    PDUSessionID& m_pDUSessionID,
    OCTET_STRING_t& m_handoverRequestAckTransfer) {
  m_pDUSessionID               = pDUSessionID;
  m_handoverRequestAckTransfer = handoverRequestAckTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceAdmittedItem::encode(
    Ngap_PDUSessionResourceAdmittedItem_t* pduItem) {
  if (!pDUSessionID.encode(pduItem->pDUSessionID)) return false;
  pduItem->handoverRequestAcknowledgeTransfer = handoverRequestAckTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceAdmittedItem::decode(
    Ngap_PDUSessionResourceAdmittedItem_t* pduItem) {
  if (!pDUSessionID.decode(pduItem->pDUSessionID)) return false;
  handoverRequestAckTransfer = pduItem->handoverRequestAcknowledgeTransfer;

  return true;
}
}  // namespace ngap
