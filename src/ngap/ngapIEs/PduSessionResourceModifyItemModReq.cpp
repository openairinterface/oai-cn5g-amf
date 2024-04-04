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

#include "PduSessionResourceModifyItemModReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceModifyItemModReq::PduSessionResourceModifyItemModReq() {
  m_NasPdu = std::nullopt;
  m_SNssai = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceModifyItemModReq::~PduSessionResourceModifyItemModReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceModifyItemModReq::set(
    const PduSessionId& pduSessionId, const std::optional<NasPdu>& nasPdu,
    const OCTET_STRING_t& pduSessionResourceModifyRequestTransfer,
    const std::optional<SNssai>& sNssai) {
  m_PduSessionId = pduSessionId;
  if (nasPdu.has_value()) {
    NasPdu tmp = {};
    tmp.set(nasPdu.value());
    m_NasPdu = std::optional<NasPdu>(tmp);
  }

  m_PduSessionResourceModifyRequestTransfer =
      pduSessionResourceModifyRequestTransfer;
  m_SNssai = sNssai;
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyItemModReq::encode(
    Ngap_PDUSessionResourceModifyItemModReq_t&
        pduSessionResourceModifyItemModReq) const {
  if (!m_PduSessionId.encode(pduSessionResourceModifyItemModReq.pDUSessionID))
    return false;
  if (m_NasPdu.has_value()) {
    pduSessionResourceModifyItemModReq.nAS_PDU =
        (Ngap_NAS_PDU_t*) calloc(1, sizeof(Ngap_NAS_PDU_t));
    if (!pduSessionResourceModifyItemModReq.nAS_PDU) return false;
    if (!m_NasPdu.value().encode(*pduSessionResourceModifyItemModReq.nAS_PDU)) {
      if (pduSessionResourceModifyItemModReq.nAS_PDU != nullptr)
        free(pduSessionResourceModifyItemModReq.nAS_PDU);
      return false;
    }
  }

  pduSessionResourceModifyItemModReq.pDUSessionResourceModifyRequestTransfer =
      m_PduSessionResourceModifyRequestTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceModifyItemModReq::decode(
    const Ngap_PDUSessionResourceModifyItemModReq_t&
        pduSessionResourceModifyItemModReq) {
  if (!m_PduSessionId.decode(pduSessionResourceModifyItemModReq.pDUSessionID))
    return false;

  if (pduSessionResourceModifyItemModReq.nAS_PDU) {
    NasPdu tmp = {};
    if (!tmp.decode(*pduSessionResourceModifyItemModReq.nAS_PDU)) return false;
    m_NasPdu = std::optional<NasPdu>(tmp);
  }

  m_PduSessionResourceModifyRequestTransfer =
      pduSessionResourceModifyItemModReq
          .pDUSessionResourceModifyRequestTransfer;

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceModifyItemModReq::get(
    PduSessionId& pduSessionId, std::optional<NasPdu>& nasPdu,
    OCTET_STRING_t& pduSessionResourceModifyRequestTransfer,
    std::optional<SNssai>& sNssai) const {
  pduSessionId = m_PduSessionId;
  nasPdu       = *m_NasPdu;
  if (m_NasPdu.has_value()) {
    NasPdu tmp = {};
    tmp.set(nasPdu.value());
    nasPdu = std::optional<NasPdu>(tmp);
  }

  pduSessionResourceModifyRequestTransfer =
      m_PduSessionResourceModifyRequestTransfer;
  sNssai = m_SNssai;
}

}  // namespace ngap
