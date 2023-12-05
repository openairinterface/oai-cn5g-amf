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

#include "PduSessionResourceSetupItemCxtReq.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupItemCxtReq::PduSessionResourceSetupItemCxtReq() {
  nas_pdu_ = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupItemCxtReq::~PduSessionResourceSetupItemCxtReq() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupItemCxtReq::set(
    const PduSessionId& pdu_session_id, const std::optional<NasPdu>& nas_pdu,
    const S_NSSAI& s_nssai,
    const OCTET_STRING_t& pdu_session_resource_setup_request_transfer) {
  pdu_session_id_ = pdu_session_id;
  nas_pdu_        = nas_pdu;
  s_nssai_        = s_nssai;
  pdu_session_resource_setup_request_transfer_ =
      pdu_session_resource_setup_request_transfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupItemCxtReq::get(
    PduSessionId& pdu_session_id, std::optional<NasPdu>& nas_pdu,
    S_NSSAI& s_nssai,
    OCTET_STRING_t& pdu_session_resource_setup_request_transfer) const {
  pdu_session_id = pdu_session_id_;
  nas_pdu        = nas_pdu_;
  s_nssai        = s_nssai_;
  pdu_session_resource_setup_request_transfer =
      pdu_session_resource_setup_request_transfer_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemCxtReq::encode(
    Ngap_PDUSessionResourceSetupItemCxtReq_t&
        pduSessionResourceSetupItemCxtReq) {
  if (!pdu_session_id_.encode(pduSessionResourceSetupItemCxtReq.pDUSessionID))
    return false;

  if (nas_pdu_.has_value()) {
    Ngap_NAS_PDU_t* naspdu =
        (Ngap_NAS_PDU_t*) calloc(1, sizeof(Ngap_NAS_PDU_t));
    if (!naspdu) return false;
    if (!nas_pdu_.value().encode(*naspdu)) {
      free_wrapper((void**) &naspdu);
      return false;
    }
    pduSessionResourceSetupItemCxtReq.nAS_PDU = naspdu;
  }

  if (!s_nssai_.encode(pduSessionResourceSetupItemCxtReq.s_NSSAI)) return false;
  pduSessionResourceSetupItemCxtReq.pDUSessionResourceSetupRequestTransfer =
      pdu_session_resource_setup_request_transfer_;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemCxtReq::decode(
    const Ngap_PDUSessionResourceSetupItemCxtReq_t&
        pduSessionResourceSetupItemCxtReq) {
  if (!pdu_session_id_.decode(pduSessionResourceSetupItemCxtReq.pDUSessionID))
    return false;
  if (!s_nssai_.decode(pduSessionResourceSetupItemCxtReq.s_NSSAI)) return false;

  if (pduSessionResourceSetupItemCxtReq.nAS_PDU) {
    NasPdu tmp = {};
    if (!tmp.decode(*pduSessionResourceSetupItemCxtReq.nAS_PDU)) return false;
    nas_pdu_ = std::optional<NasPdu>(tmp);
  }

  pdu_session_resource_setup_request_transfer_ =
      pduSessionResourceSetupItemCxtReq.pDUSessionResourceSetupRequestTransfer;

  return true;
}

}  // namespace ngap
