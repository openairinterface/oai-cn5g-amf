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

#include "PDUSessionResourceItemHORqd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceItemHORqd::PDUSessionResourceItemHORqd() {}

//------------------------------------------------------------------------------
PDUSessionResourceItemHORqd::~PDUSessionResourceItemHORqd() {}

//------------------------------------------------------------------------------
void PDUSessionResourceItemHORqd::setPDUSessionResourceItemHORqd(
    const PDUSessionID& m_pDUSessionID,
    const OCTET_STRING_t& m_handoverRequiredTransfer) {
  pDUSessionID             = m_pDUSessionID;
  handoverRequiredTransfer = m_handoverRequiredTransfer;
}

//------------------------------------------------------------------------------
void PDUSessionResourceItemHORqd::getPDUSessionResourceItemHORqd(
    PDUSessionID& m_pDUSessionID, OCTET_STRING_t& m_handoverRequiredTransfer) {
  m_pDUSessionID             = pDUSessionID;
  m_handoverRequiredTransfer = handoverRequiredTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceItemHORqd::encode2PDUSessionResourceItemHORqd(
    Ngap_PDUSessionResourceItemHORqd_t* pdUSessionResourceItemHORqd) {
  if (!pDUSessionID.encode(pdUSessionResourceItemHORqd->pDUSessionID))
    return false;
  pdUSessionResourceItemHORqd->handoverRequiredTransfer =
      handoverRequiredTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceItemHORqd::decodefromPDUSessionResourceItemHORqd(
    Ngap_PDUSessionResourceItemHORqd_t* pdUSessionResourceItemHORqd) {
  if (!pDUSessionID.decode(pdUSessionResourceItemHORqd->pDUSessionID))
    return false;
  handoverRequiredTransfer =
      pdUSessionResourceItemHORqd->handoverRequiredTransfer;

  return true;
}

}  // namespace ngap
