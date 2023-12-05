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

#include "PduSessionResourceReleaseListCmd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseListCmd::PduSessionResourceReleaseListCmd() {}

//------------------------------------------------------------------------------
PduSessionResourceReleaseListCmd::~PduSessionResourceReleaseListCmd() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseListCmd::setPDUSessionResourceReleaseListCmd(
    const std::vector<PduSessionResourceReleaseItemCmd>& list_item) {
  uint8_t number_items = (list_item.size() > kMaxNoOfPduSessions) ?
                             kMaxNoOfPduSessions :
                             list_item.size();
  list_.insert(
      list_.begin(), list_item.begin(), list_item.begin() + number_items);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseListCmd::getPDUSessionResourceReleaseListCmd(
    std::vector<PduSessionResourceReleaseItemCmd>& list_item) const {
  list_item = list_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseListCmd::encode(
    Ngap_PDUSessionResourceToReleaseListRelCmd_t&
        pduSessionResourceReleaseListCmd) {
  for (int i = 0; i < list_.size(); i++) {
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t* item =
        (Ngap_PDUSessionResourceToReleaseItemRelCmd_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceToReleaseItemRelCmd_t));
    if (!item) return false;
    if (!list_[i].encode(*item)) return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceReleaseListCmd.list, item) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseListCmd::decode(
    const Ngap_PDUSessionResourceToReleaseListRelCmd_t&
        pduSessionResourceReleaseListCmd) {
  for (int i = 0; i < pduSessionResourceReleaseListCmd.list.count; i++) {
    PduSessionResourceReleaseItemCmd item = {};
    if (!item.decode(*pduSessionResourceReleaseListCmd.list.array[i]))
      return false;
    list_.push_back(item);
  }

  return true;
}
}  // namespace ngap
