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

#include "UserLocationInformationNr.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformationNr::UserLocationInformationNr() {}

//------------------------------------------------------------------------------
UserLocationInformationNr::~UserLocationInformationNr() {}

//------------------------------------------------------------------------------
void UserLocationInformationNr::set(const NrCgi& nrCgi, const Tai& tai) {
  m_NrCgi = nrCgi;
  m_Tai   = tai;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNr::encode(
    Ngap_UserLocationInformationNR_t& userLocationInformation) const {
  if (!m_NrCgi.encode(userLocationInformation.nR_CGI)) {
    return false;
  }
  if (!m_Tai.encode(userLocationInformation.tAI)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNr::decode(
    const Ngap_UserLocationInformationNR_t& userLocationInformation) {
  if (!m_NrCgi.decode(userLocationInformation.nR_CGI)) {
    return false;
  }

  if (!m_Tai.decode(userLocationInformation.tAI)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void UserLocationInformationNr::get(NrCgi& nrCgi, Tai& tai) const {
  nrCgi = m_NrCgi;
  tai   = m_Tai;
}
}  // namespace ngap
