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

#include "TAI.hpp"

namespace ngap {

//------------------------------------------------------------------------------
TAI::TAI() {}

//------------------------------------------------------------------------------
TAI::~TAI() {}

//------------------------------------------------------------------------------
void TAI::setTAI(const PlmnId& plmn_id, const TAC& tac) {
  plmn_id_ = plmn_id;
  tac_     = tac;
}

//------------------------------------------------------------------------------
void TAI::setTAI(
    const std::string& mcc, const std::string& mnc, const uint32_t& tac) {
  plmn_id_.set(mcc, mnc);
  tac_.set(tac);
}

//------------------------------------------------------------------------------
void TAI::getTAI(std::string& mcc, std::string& mnc, uint32_t& tac) {
  plmn_id_.getMcc(mcc);
  plmn_id_.getMnc(mnc);
  tac = tac_.get();
}

//------------------------------------------------------------------------------
void TAI::setTAI(const Tai_t& tai) {
  plmn_id_.set(tai.mcc, tai.mnc);
  tac_.set(tai.tac);
}
//------------------------------------------------------------------------------
bool TAI::encode(Ngap_TAI_t& tai) {
  if (!plmn_id_.encode(tai.pLMNIdentity)) return false;
  if (!tac_.encode(tai.tAC)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool TAI::decode(const Ngap_TAI_t& tai) {
  if (!plmn_id_.decode(tai.pLMNIdentity)) return false;
  if (!tac_.decode(tai.tAC)) return false;

  return true;
}

//------------------------------------------------------------------------------
void TAI::getTAI(PlmnId& plmn_id, TAC& tac) {
  plmn_id = plmn_id_;
  tac     = tac_;
}

//------------------------------------------------------------------------------
void TAI::getTAI(Tai_t& tai) {
  plmn_id_.getMcc(tai.mcc);
  plmn_id_.getMnc(tai.mnc);
  tai.tac = tac_.get();
}
}  // namespace ngap
