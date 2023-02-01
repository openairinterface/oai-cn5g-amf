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

#include "PDUSessionResourceSetupItemHOReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemHOReq::PDUSessionResourceSetupItemHOReq()
    : PDUSessionResourceItem() {}

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemHOReq::~PDUSessionResourceSetupItemHOReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemHOReq::set(
    const PDUSessionID& pdu_session_id, const S_NSSAI& s_nssai,
    const OCTET_STRING_t& handover_request_transfer) {
  PDUSessionResourceItem::set(pdu_session_id, handover_request_transfer);
  s_NSSAI = s_nssai;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemHOReq::get(
    PDUSessionID& pdu_session_id, S_NSSAI& s_nssai,
    OCTET_STRING_t& handover_request_transfer) {
  PDUSessionResourceItem::get(pdu_session_id, handover_request_transfer);
  s_nssai = s_NSSAI;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemHOReq::encode(
    Ngap_PDUSessionResourceSetupItemHOReq_t* resource_setup_item) {
  if (!PDUSessionResourceItem::encode(
          resource_setup_item->pDUSessionID,
          resource_setup_item->handoverRequestTransfer))
    return false;

  if (!s_NSSAI.encode(&resource_setup_item->s_NSSAI)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemHOReq::decode(
    Ngap_PDUSessionResourceSetupItemHOReq_t* resource_setup_item) {
  if (!PDUSessionResourceItem::decode(
          resource_setup_item->pDUSessionID,
          resource_setup_item->handoverRequestTransfer))
    return false;

  if (!s_NSSAI.decode(&resource_setup_item->s_NSSAI)) return false;

  return true;
}

}  // namespace ngap
