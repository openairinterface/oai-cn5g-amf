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
    const PduSessionId& m_pDUSessionID,
    const OCTET_STRING_t& m_handoverRequiredTransfer) {
  pDUSessionID             = m_pDUSessionID;
  handoverRequiredTransfer = m_handoverRequiredTransfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceItemHoRqd::get(
    PduSessionId& m_pDUSessionID,
    OCTET_STRING_t& m_handoverRequiredTransfer) const {
  m_pDUSessionID             = pDUSessionID;
  m_handoverRequiredTransfer = handoverRequiredTransfer;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemHoRqd::encode(
    Ngap_PDUSessionResourceItemHORqd_t& pdUSessionResourceItemHORqd) {
  if (!pDUSessionID.encode(pdUSessionResourceItemHORqd.pDUSessionID))
    return false;
  pdUSessionResourceItemHORqd.handoverRequiredTransfer =
      handoverRequiredTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemHoRqd::decode(
    const Ngap_PDUSessionResourceItemHORqd_t& pdUSessionResourceItemHORqd) {
  if (!pDUSessionID.decode(pdUSessionResourceItemHORqd.pDUSessionID))
    return false;
  handoverRequiredTransfer =
      pdUSessionResourceItemHORqd.handoverRequiredTransfer;

  return true;
}

}  // namespace ngap
