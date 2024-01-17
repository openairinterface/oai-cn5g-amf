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

#include "PduSessionResourceItemCxtRelCpl.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceItemCxtRelCpl::PduSessionResourceItemCxtRelCpl() {}

//------------------------------------------------------------------------------
PduSessionResourceItemCxtRelCpl::~PduSessionResourceItemCxtRelCpl() {}

//------------------------------------------------------------------------------
void PduSessionResourceItemCxtRelCpl::set(const PduSessionId& pdu_session_id) {
  pdu_session_id_ = pdu_session_id;
}

//------------------------------------------------------------------------------
void PduSessionResourceItemCxtRelCpl::get(PduSessionId& pdu_session_id) const {
  pdu_session_id = pdu_session_id_;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemCxtRelCpl::encode(
    Ngap_PDUSessionResourceItemCxtRelCpl_t&
        pdu_session_resource_item_cxt_rel_cpl) {
  if (!pdu_session_id_.encode(
          pdu_session_resource_item_cxt_rel_cpl.pDUSessionID))
    return false;
  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceItemCxtRelCpl::decode(
    const Ngap_PDUSessionResourceItemCxtRelCpl_t&
        pdu_session_resource_item_cxt_rel_cpl) {
  pdu_session_id_.set(pdu_session_resource_item_cxt_rel_cpl.pDUSessionID);
  return true;
}

}  // namespace ngap
