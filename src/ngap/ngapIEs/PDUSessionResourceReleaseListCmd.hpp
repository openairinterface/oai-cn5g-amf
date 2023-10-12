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

#ifndef _PDU_SESSION_RESOURCE_RELEASE_LIST_CMD_H_
#define _PDU_SESSION_RESOURCE_RELEASE_LIST_CMD_H_

#include <vector>

#include "PDUSessionResourceReleaseItemCmd.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceToReleaseListRelCmd.h"
}

constexpr uint16_t kMaxNoOfPduSessions = 256;

namespace ngap {

class PDUSessionResourceReleaseListCmd {
 public:
  PDUSessionResourceReleaseListCmd();
  virtual ~PDUSessionResourceReleaseListCmd();

  void setPDUSessionResourceReleaseListCmd(
      const std::vector<PDUSessionResourceReleaseItemCmd>& list_item);
  void getPDUSessionResourceReleaseListCmd(
      std::vector<PDUSessionResourceReleaseItemCmd>& list_item) const;

  bool encode(Ngap_PDUSessionResourceToReleaseListRelCmd_t*
                  pduSessionResourceReleaseListCmd);
  bool decode(Ngap_PDUSessionResourceToReleaseListRelCmd_t*
                  pduSessionResourceReleaseListCmd);

 private:
  std::vector<PDUSessionResourceReleaseItemCmd> list_;
};

}  // namespace ngap

#endif
