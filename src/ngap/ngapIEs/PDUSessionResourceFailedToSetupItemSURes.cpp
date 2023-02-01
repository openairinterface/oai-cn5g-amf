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

#include "PDUSessionResourceFailedToSetupItemSURes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupItemSURes::
    PDUSessionResourceFailedToSetupItemSURes() {}

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupItemSURes::
    ~PDUSessionResourceFailedToSetupItemSURes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupItemSURes::set(
    const PDUSessionID& pdu_session_id,
    const OCTET_STRING_t& pdu_session_resource_setup_unsuccessful_transfer) {
  pdu_session_id_ = pdu_session_id;
  pdu_session_resource_setup_unsuccessful_transfer_ =
      pdu_session_resource_setup_unsuccessful_transfer;
}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupItemSURes::get(
    PDUSessionID& pdu_session_id,
    OCTET_STRING_t& pdu_session_resource_setup_unsuccessful_transfer) {
  pdu_session_id = pdu_session_id_;
  pdu_session_resource_setup_unsuccessful_transfer =
      pdu_session_resource_setup_unsuccessful_transfer_;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupItemSURes::encode(
    Ngap_PDUSessionResourceFailedToSetupItemSURes_t*
        pdu_session_resource_failed_to_setup_item_su_res) {
  if (!pdu_session_id_.encode(
          pdu_session_resource_failed_to_setup_item_su_res->pDUSessionID))
    return false;
  pdu_session_resource_failed_to_setup_item_su_res
      ->pDUSessionResourceSetupUnsuccessfulTransfer =
      pdu_session_resource_setup_unsuccessful_transfer_;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupItemSURes::decode(
    Ngap_PDUSessionResourceFailedToSetupItemSURes_t*
        pdu_session_resource_failed_to_setup_item_su_res) {
  if (!pdu_session_id_.decode(
          pdu_session_resource_failed_to_setup_item_su_res->pDUSessionID))
    return false;
  pdu_session_resource_setup_unsuccessful_transfer_ =
      pdu_session_resource_failed_to_setup_item_su_res
          ->pDUSessionResourceSetupUnsuccessfulTransfer;

  return true;
}

}  // namespace ngap
