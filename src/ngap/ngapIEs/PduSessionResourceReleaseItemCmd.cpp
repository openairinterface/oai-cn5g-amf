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

#include "PduSessionResourceReleaseItemCmd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseItemCmd::PduSessionResourceReleaseItemCmd() {}

//------------------------------------------------------------------------------
PduSessionResourceReleaseItemCmd::~PduSessionResourceReleaseItemCmd() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseItemCmd::setPDUSessionResourceReleaseItemCmd(
    const PduSessionId& pdu_session_id,
    const OCTET_STRING_t& pdu_session_resource_release) {
  pdu_session_id_                                = pdu_session_id;
  pdu_session_resource_release_command_transfer_ = pdu_session_resource_release;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseItemCmd::encode(
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
        pduSessionResourceReleaseCommandTransfer) {
  if (!pdu_session_id_.encode(
          pduSessionResourceReleaseCommandTransfer.pDUSessionID))
    return false;
  pduSessionResourceReleaseCommandTransfer
      .pDUSessionResourceReleaseCommandTransfer =
      pdu_session_resource_release_command_transfer_;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseItemCmd::decode(
    const Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
        pduSessionResourceReleaseCommandTransfer) {
  if (!pdu_session_id_.decode(
          pduSessionResourceReleaseCommandTransfer.pDUSessionID))
    return false;
  pdu_session_resource_release_command_transfer_ =
      pduSessionResourceReleaseCommandTransfer
          .pDUSessionResourceReleaseCommandTransfer;

  return true;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseItemCmd::getPDUSessionResourceReleaseItemCmd(
    PduSessionId& pdu_session_id,
    OCTET_STRING_t& pdu_session_resource_release) const {
  pdu_session_id               = pdu_session_id_;
  pdu_session_resource_release = pdu_session_resource_release_command_transfer_;
}

}  // namespace ngap
