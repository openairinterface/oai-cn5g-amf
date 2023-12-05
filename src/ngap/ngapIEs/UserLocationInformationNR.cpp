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

#include "UserLocationInformationNR.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformationNR::UserLocationInformationNR() {}

//------------------------------------------------------------------------------
UserLocationInformationNR::~UserLocationInformationNR() {}

//------------------------------------------------------------------------------
void UserLocationInformationNR::set(const NrCgi& m_nR_CGI, const TAI& m_tAI) {
  nR_CGI = m_nR_CGI;
  tAI    = m_tAI;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNR::encode(
    Ngap_UserLocationInformationNR_t& user_location_info_nr) {
  if (!nR_CGI.encode(user_location_info_nr.nR_CGI)) {
    return false;
  }
  if (!tAI.encode(user_location_info_nr.tAI)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNR::decode(
    const Ngap_UserLocationInformationNR_t& user_location_info_nr) {
  if (!nR_CGI.decode(user_location_info_nr.nR_CGI)) {
    return false;
  }

  if (!tAI.decode(user_location_info_nr.tAI)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void UserLocationInformationNR::get(NrCgi& m_nR_CGI, TAI& m_tAI) {
  m_nR_CGI = nR_CGI;
  m_tAI    = tAI;
}
}  // namespace ngap
