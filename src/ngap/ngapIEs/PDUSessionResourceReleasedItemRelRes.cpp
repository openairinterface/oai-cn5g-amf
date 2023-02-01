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

#include "PDUSessionResourceReleasedItemRelRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceReleasedItemRelRes::PDUSessionResourceReleasedItemRelRes() {}

//------------------------------------------------------------------------------
PDUSessionResourceReleasedItemRelRes::~PDUSessionResourceReleasedItemRelRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceReleasedItemRelRes::set(
    const PDUSessionID& pdu_session_id,
    const OCTET_STRING_t& pdu_session_resource_release_response_transfer) {
  pdu_session_id_ = pdu_session_id;
  pdu_session_resource_release_response_transfer_ =
      pdu_session_resource_release_response_transfer;
}

//------------------------------------------------------------------------------
void PDUSessionResourceReleasedItemRelRes::get(
    PDUSessionID& pdu_session_id,
    OCTET_STRING_t& pdu_session_resource_release_response_transfer) {
  pdu_session_id = pdu_session_id_;
  pdu_session_resource_release_response_transfer =
      pdu_session_resource_release_response_transfer_;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceReleasedItemRelRes::encode(
    Ngap_PDUSessionResourceReleasedItemRelRes_t*
        pdu_session_resource_released_item_rel_res) {
  if (!pdu_session_id_.encode(
          pdu_session_resource_released_item_rel_res->pDUSessionID))
    return false;

  pdu_session_resource_released_item_rel_res
      ->pDUSessionResourceReleaseResponseTransfer =
      pdu_session_resource_release_response_transfer_;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceReleasedItemRelRes::decode(
    Ngap_PDUSessionResourceReleasedItemRelRes_t*
        pdu_session_resource_released_item_rel_res) {
  pdu_session_id_.set(pdu_session_resource_released_item_rel_res->pDUSessionID);
  pdu_session_resource_release_response_transfer_ =
      pdu_session_resource_released_item_rel_res
          ->pDUSessionResourceReleaseResponseTransfer;

  return true;
}

}  // namespace ngap
