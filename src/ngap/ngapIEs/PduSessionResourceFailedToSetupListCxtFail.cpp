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

#include "PduSessionResourceFailedToSetupListCxtFail.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceFailedToSetupListCxtFail::
    PduSessionResourceFailedToSetupListCxtFail() {}

//------------------------------------------------------------------------------
PduSessionResourceFailedToSetupListCxtFail::
    ~PduSessionResourceFailedToSetupListCxtFail() {}

//------------------------------------------------------------------------------
void PduSessionResourceFailedToSetupListCxtFail::set(
    const std::vector<PduSessionResourceFailedToSetupItemCxtFail>& list) {
  m_ItemList = list;
}

//------------------------------------------------------------------------------
void PduSessionResourceFailedToSetupListCxtFail::get(
    std::vector<PduSessionResourceFailedToSetupItemCxtFail>& list) const {
  list = m_ItemList;
}

//------------------------------------------------------------------------------
bool PduSessionResourceFailedToSetupListCxtFail::encode(
    Ngap_PDUSessionResourceFailedToSetupListCxtFail_t&
        pduSessionResourceFailedToSetupListCxtFail) const {
  for (std::vector<PduSessionResourceFailedToSetupItemCxtFail>::const_iterator
           it = m_ItemList.begin();
       it < m_ItemList.end(); ++it) {
    Ngap_PDUSessionResourceFailedToSetupItemCxtFail_t* failedToFailure =
        (Ngap_PDUSessionResourceFailedToSetupItemCxtFail_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceFailedToSetupItemCxtFail_t));
    if (!failedToFailure) return false;
    if (!it->encode(*failedToFailure)) return false;
    if (ASN_SEQUENCE_ADD(
            &pduSessionResourceFailedToSetupListCxtFail.list,
            failedToFailure) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceFailedToSetupListCxtFail::decode(
    const Ngap_PDUSessionResourceFailedToSetupListCxtFail_t&
        pduSessionResourceFailedToSetupListCxtFail) {
  m_ItemList.reserve(pduSessionResourceFailedToSetupListCxtFail.list.count);

  for (int i = 0; i < pduSessionResourceFailedToSetupListCxtFail.list.count;
       i++) {
    PduSessionResourceFailedToSetupItemCxtFail itemCxtFail = {};
    if (!itemCxtFail.decode(
            *pduSessionResourceFailedToSetupListCxtFail.list.array[i]))
      return false;
    m_ItemList.push_back(itemCxtFail);
  }

  return true;
}

}  // namespace ngap
