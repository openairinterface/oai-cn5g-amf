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

#include "PduSessionResourceToReleaseItemRelCmd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceToReleaseItemRelCmd::PduSessionResourceToReleaseItemRelCmd() {
}

//------------------------------------------------------------------------------
PduSessionResourceToReleaseItemRelCmd::
    ~PduSessionResourceToReleaseItemRelCmd() {}

//------------------------------------------------------------------------------
void PduSessionResourceToReleaseItemRelCmd::set(
    const PduSessionId& pdu_session_id,
    const OCTET_STRING_t& pdu_session_resource_release_command_transfer) {
  pdu_session_id_ = pdu_session_id;
  pdu_session_resource_release_command_transfer_ =
      pdu_session_resource_release_command_transfer;
}

//------------------------------------------------------------------------------
void PduSessionResourceToReleaseItemRelCmd::get(
    PduSessionId& pdu_session_id,
    OCTET_STRING_t& pdu_session_resource_release_command_transfer) {
  pdu_session_id = pdu_session_id_;
  pdu_session_resource_release_command_transfer =
      pdu_session_resource_release_command_transfer_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceToReleaseItemRelCmd::encode(
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
        pdu_session_resource_to_release_item_rel_cmd) {
  if (!pdu_session_id_.encode(
          pdu_session_resource_to_release_item_rel_cmd.pDUSessionID))
    return false;

  pdu_session_resource_to_release_item_rel_cmd
      .pDUSessionResourceReleaseCommandTransfer =
      pdu_session_resource_release_command_transfer_;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceToReleaseItemRelCmd::decode(
    const Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
        pdu_session_resource_to_release_item_rel_cmd) {
  pdu_session_resource_release_command_transfer_ =
      pdu_session_resource_to_release_item_rel_cmd
          .pDUSessionResourceReleaseCommandTransfer;

  return true;
}

}  // namespace ngap
