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

#include "PduSessionType.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionType::PduSessionType() {
  pdu_session_type_ = 0;
}

//------------------------------------------------------------------------------
PduSessionType::~PduSessionType() {}

//------------------------------------------------------------------------------
void PduSessionType::set(e_Ngap_PDUSessionType pdu_session_type) {
  pdu_session_type_ = pdu_session_type;
}

//------------------------------------------------------------------------------
bool PduSessionType::get(long& pdu_session_type) const {
  pdu_session_type = pdu_session_type_;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionType::encode(Ngap_PDUSessionType_t& type) {
  type = pdu_session_type_;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionType::decode(Ngap_PDUSessionType_t type) {
  pdu_session_type_ = type;

  return true;
}

}  // namespace ngap
