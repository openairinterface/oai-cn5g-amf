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

#include "UePagingIdentity.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UePagingIdentity::UePagingIdentity() {}

//------------------------------------------------------------------------------
UePagingIdentity::~UePagingIdentity() {}

//------------------------------------------------------------------------------
void UePagingIdentity::set(
    const std::string& set_id, const std::string& pointer,
    const std::string& tmsi) {
  m_FiveGSTmsi.set(set_id, pointer, tmsi);
}

//------------------------------------------------------------------------------
void UePagingIdentity::get(std::string& fiveGsTmsi) const {
  m_FiveGSTmsi.getTmsi(fiveGsTmsi);
}

//------------------------------------------------------------------------------
void UePagingIdentity::get(
    std::string& set_id, std::string& pointer, std::string& tmsi) const {
  m_FiveGSTmsi.get(set_id, pointer, tmsi);
}

//------------------------------------------------------------------------------
bool UePagingIdentity::encode(Ngap_UEPagingIdentity_t& pdu) const {
  pdu.present = Ngap_UEPagingIdentity_PR_fiveG_S_TMSI;
  Ngap_FiveG_S_TMSI_t* ie =
      (Ngap_FiveG_S_TMSI_t*) calloc(1, sizeof(Ngap_FiveG_S_TMSI_t));
  pdu.choice.fiveG_S_TMSI = ie;
  if (!m_FiveGSTmsi.encode(*pdu.choice.fiveG_S_TMSI)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool UePagingIdentity::decode(const Ngap_UEPagingIdentity_t& pdu) {
  if (pdu.present != Ngap_UEPagingIdentity_PR_fiveG_S_TMSI) return false;
  if (!m_FiveGSTmsi.decode(*pdu.choice.fiveG_S_TMSI)) return false;

  return true;
}
}  // namespace ngap
