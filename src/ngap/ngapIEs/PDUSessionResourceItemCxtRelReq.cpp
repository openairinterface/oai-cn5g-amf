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

#include "PDUSessionResourceItemCxtRelReq.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceItemCxtRelReq::PDUSessionResourceItemCxtRelReq() {}

//------------------------------------------------------------------------------
PDUSessionResourceItemCxtRelReq::~PDUSessionResourceItemCxtRelReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceItemCxtRelReq::set(const PDUSessionID& pdu_session_id) {
  pdu_session_id_ = pdu_session_id;
}

//------------------------------------------------------------------------------
void PDUSessionResourceItemCxtRelReq::get(PDUSessionID& pdu_session_id) {
  pdu_session_id = pdu_session_id_;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceItemCxtRelReq::encode(
    Ngap_PDUSessionResourceItemCxtRelReq_t*
        pdu_session_resource_item_cxt_rel_req) {
  if (!pdu_session_id_.encode(
          pdu_session_resource_item_cxt_rel_req->pDUSessionID))
    return false;
  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceItemCxtRelReq::decode(
    Ngap_PDUSessionResourceItemCxtRelReq_t*
        pdu_session_resource_item_cxt_rel_req) {
  pdu_session_id_.set(pdu_session_resource_item_cxt_rel_req->pDUSessionID);
  return true;
}

}  // namespace ngap
