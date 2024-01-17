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

#include "UserLocationInformationEUTRA.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformationEUTRA::UserLocationInformationEUTRA() {}

//------------------------------------------------------------------------------
UserLocationInformationEUTRA::~UserLocationInformationEUTRA() {}

//------------------------------------------------------------------------------
void UserLocationInformationEUTRA::set(
    const EUtraCgi& m_eUTRA_CGI, const TAI& m_tAI) {
  eUTRA_CGI = m_eUTRA_CGI;
  tAI       = m_tAI;
}

//------------------------------------------------------------------------------
void UserLocationInformationEUTRA::get(EUtraCgi& m_eUTRA_CGI, TAI& m_tAI) {
  m_eUTRA_CGI = eUTRA_CGI;
  m_tAI       = tAI;
}

//------------------------------------------------------------------------------
bool UserLocationInformationEUTRA::encode(
    Ngap_UserLocationInformationEUTRA_t& user_location_info_eutra) {
  if (!eUTRA_CGI.encode(user_location_info_eutra.eUTRA_CGI)) {
    Logger::ngap().warn("Encode eUTRA_CGI IE error");
    return false;
  }
  if (!tAI.encode(user_location_info_eutra.tAI)) {
    Logger::ngap().warn("Encode TAI IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationEUTRA::decode(
    const Ngap_UserLocationInformationEUTRA_t& user_location_info_eutra) {
  if (!eUTRA_CGI.decode(user_location_info_eutra.eUTRA_CGI)) {
    Logger::ngap().warn("Decode eUTRA_CGI IE error");
    return false;
  }

  if (!tAI.decode(user_location_info_eutra.tAI)) {
    Logger::ngap().warn("Decode TAI IE error");
    return false;
  }
  return true;
}
}  // namespace ngap
