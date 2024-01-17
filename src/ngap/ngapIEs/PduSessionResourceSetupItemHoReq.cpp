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
    const PduSessionId& pdu_session_id, const S_NSSAI& s_nssai,
    const OCTET_STRING_t& handover_request_transfer) {
  PduSessionResourceItem::set(pdu_session_id, handover_request_transfer);
  s_NSSAI = s_nssai;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupItemHoReq::get(
    PduSessionId& pdu_session_id, S_NSSAI& s_nssai,
    OCTET_STRING_t& handover_request_transfer) const {
  PduSessionResourceItem::get(pdu_session_id, handover_request_transfer);
  s_nssai = s_NSSAI;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemHoReq::encode(
    Ngap_PDUSessionResourceSetupItemHOReq_t& resource_setup_item) {
  if (!PduSessionResourceItem::encode(
          resource_setup_item.pDUSessionID,
          resource_setup_item.handoverRequestTransfer))
    return false;

  if (!s_NSSAI.encode(resource_setup_item.s_NSSAI)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupItemHoReq::decode(
    const Ngap_PDUSessionResourceSetupItemHOReq_t& resource_setup_item) {
  if (!PduSessionResourceItem::decode(
          resource_setup_item.pDUSessionID,
          resource_setup_item.handoverRequestTransfer))
    return false;

  if (!s_NSSAI.decode(resource_setup_item.s_NSSAI)) return false;

  return true;
}

}  // namespace ngap
