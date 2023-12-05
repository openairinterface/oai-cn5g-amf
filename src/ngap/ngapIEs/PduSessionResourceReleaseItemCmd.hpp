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

#ifndef _PDU_SESSION_RESOURCE_RELEASE_ITEM_CMD_H_
#define _PDU_SESSION_RESOURCE_RELEASE_ITEM_CMD_H_

#include "NasPdu.hpp"
#include "PduSessionId.hpp"
#include "PduSessionResourceReleaseCommandTransfer.hpp"
#include "S-NSSAI.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceToReleaseItemRelCmd.h"
}

namespace ngap {

class PduSessionResourceReleaseItemCmd {
 public:
  PduSessionResourceReleaseItemCmd();
  virtual ~PduSessionResourceReleaseItemCmd();

  void setPDUSessionResourceReleaseItemCmd(
      const PduSessionId& pdu_session_id,
      const OCTET_STRING_t& pdu_session_resource_release);
  void getPDUSessionResourceReleaseItemCmd(
      PduSessionId& pdu_session_id,
      OCTET_STRING_t& pdu_session_resource_release) const;

  bool encode(Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
                  pdu_session_resource_release_item);
  bool decode(const Ngap_PDUSessionResourceToReleaseItemRelCmd_t&
                  pdu_session_resource_release_item);

 private:
  PduSessionId pdu_session_id_;                                   // Mandatory
  OCTET_STRING_t pdu_session_resource_release_command_transfer_;  // Mandatory
};

}  // namespace ngap

#endif
