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

#include "PduSessionResourceHandoverItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceHandoverItem::PduSessionResourceHandoverItem() {}

//------------------------------------------------------------------------------
PduSessionResourceHandoverItem::~PduSessionResourceHandoverItem() {}

//------------------------------------------------------------------------------
void PduSessionResourceHandoverItem::set(
    const PduSessionId& sessionId, const OCTET_STRING_t& commandTransfer) {
  m_PduSessionId            = sessionId;
  m_HandoverCommandTransfer = commandTransfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceHandoverItem::get(
    PduSessionId& sessionId, OCTET_STRING_t& commandTransfer) const {
  sessionId       = m_PduSessionId;
  commandTransfer = m_HandoverCommandTransfer;
}

//------------------------------------------------------------------------------
bool PduSessionResourceHandoverItem::encode(
    Ngap_PDUSessionResourceHandoverItem_t& item) const {
  if (!m_PduSessionId.encode(item.pDUSessionID)) return false;
  item.handoverCommandTransfer = m_HandoverCommandTransfer;
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceHandoverItem::decode(
    const Ngap_PDUSessionResourceHandoverItem_t& item) {
  if (!m_PduSessionId.decode(item.pDUSessionID)) return false;
  m_HandoverCommandTransfer = item.handoverCommandTransfer;
  return true;
}
}  // namespace ngap
