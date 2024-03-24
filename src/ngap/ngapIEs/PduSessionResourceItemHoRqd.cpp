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

#include "PduSessionResourceItemHoRqd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceItemHoRqd::PduSessionResourceItemHoRqd() {}

//------------------------------------------------------------------------------
PduSessionResourceItemHoRqd::~PduSessionResourceItemHoRqd() {}

//------------------------------------------------------------------------------
void PduSessionResourceItemHoRqd::set(
    const PduSessionId& pduSessionId,
    const OCTET_STRING_t& m_handoverRequiredTransfer) {
  m_PduSessionId             = pduSessionId;
  m_HandoverRequiredTransfer = m_handoverRequiredTransfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceItemHoRqd::get(
    PduSessionId& pduSessionId,
    OCTET_STRING_t& m_handoverRequiredTransfer) const {
  pduSessionId               = m_PduSessionId;
  m_handoverRequiredTransfer = m_HandoverRequiredTransfer;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemHoRqd::encode(
    Ngap_PDUSessionResourceItemHORqd_t& pduSessionResourceItemHORqd) const {
  if (!m_PduSessionId.encode(pduSessionResourceItemHORqd.pDUSessionID))
    return false;
  pduSessionResourceItemHORqd.handoverRequiredTransfer =
      m_HandoverRequiredTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemHoRqd::decode(
    const Ngap_PDUSessionResourceItemHORqd_t& pduSessionResourceItemHORqd) {
  if (!m_PduSessionId.decode(pduSessionResourceItemHORqd.pDUSessionID))
    return false;
  m_HandoverRequiredTransfer =
      pduSessionResourceItemHORqd.handoverRequiredTransfer;

  return true;
}

}  // namespace ngap
