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

#include "PduSessionResourceFailedToSetupItemSURes.hpp"

#include "amf_conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceFailedToSetupItemSURes::
    PduSessionResourceFailedToSetupItemSURes() {}

//------------------------------------------------------------------------------
PduSessionResourceFailedToSetupItemSURes::
    ~PduSessionResourceFailedToSetupItemSURes() {}

//------------------------------------------------------------------------------
void PduSessionResourceFailedToSetupItemSURes::set(
    const PduSessionId& pdu_session_id,
    const OCTET_STRING_t& pdu_session_resource) {
  pdu_session_id_                                   = pdu_session_id;
  pdu_session_resource_setup_unsuccessful_transfer_ = pdu_session_resource;
}

//------------------------------------------------------------------------------
void PduSessionResourceFailedToSetupItemSURes::get(
    PduSessionId& pdu_session_id, OCTET_STRING_t& pdu_session_resource) {
  pdu_session_id       = pdu_session_id_;
  pdu_session_resource = pdu_session_resource_setup_unsuccessful_transfer_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceFailedToSetupItemSURes::encode(
    Ngap_PDUSessionResourceFailedToSetupItemSURes_t&
        pdu_session_resource_item) {
  if (!pdu_session_id_.encode(pdu_session_resource_item.pDUSessionID))
    return false;
  amf_conv::octet_string_copy(
      pdu_session_resource_item.pDUSessionResourceSetupUnsuccessfulTransfer,
      pdu_session_resource_setup_unsuccessful_transfer_);

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceFailedToSetupItemSURes::decode(
    const Ngap_PDUSessionResourceFailedToSetupItemSURes_t&
        pdu_session_resource_item) {
  if (!pdu_session_id_.decode(pdu_session_resource_item.pDUSessionID))
    return false;
  amf_conv::octet_string_copy(
      pdu_session_resource_setup_unsuccessful_transfer_,
      pdu_session_resource_item.pDUSessionResourceSetupUnsuccessfulTransfer);
  return true;
}

}  // namespace ngap
