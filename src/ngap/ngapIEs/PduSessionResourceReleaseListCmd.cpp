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
void PduSessionResourceReleaseListCmd::set(
    const std::vector<PduSessionResourceReleaseItemCmd>& itemList) {
  uint8_t number_items = (itemList.size() > kMaxNoOfPduSessions) ?
                             kMaxNoOfPduSessions :
                             itemList.size();
  m_ItemList.insert(
      m_ItemList.begin(), itemList.begin(), itemList.begin() + number_items);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseListCmd::get(
    std::vector<PduSessionResourceReleaseItemCmd>& itemList) const {
  itemList = m_ItemList;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseListCmd::encode(
    Ngap_PDUSessionResourceToReleaseListRelCmd_t&
        pduSessionResourceReleaseListCmd) const {
  for (int i = 0; i < m_ItemList.size(); i++) {
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t* item =
        (Ngap_PDUSessionResourceToReleaseItemRelCmd_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceToReleaseItemRelCmd_t));
    if (!item) return false;
    if (!m_ItemList[i].encode(*item)) return false;
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
    m_ItemList.push_back(item);
  }

  return true;
}
}  // namespace ngap
