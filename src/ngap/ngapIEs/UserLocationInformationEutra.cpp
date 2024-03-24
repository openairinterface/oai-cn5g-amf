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

#include "UserLocationInformationEutra.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformationEutra::UserLocationInformationEutra() {}

//------------------------------------------------------------------------------
UserLocationInformationEutra::~UserLocationInformationEutra() {}

//------------------------------------------------------------------------------
void UserLocationInformationEutra::set(
    const EutraCgi& eutraCgi, const Tai& tai) {
  m_EutraCgi = eutraCgi;
  m_Tai      = tai;
}

//------------------------------------------------------------------------------
void UserLocationInformationEutra::get(EutraCgi& eutraCgi, Tai& tai) const {
  eutraCgi = m_EutraCgi;
  tai      = m_Tai;
}

//------------------------------------------------------------------------------
bool UserLocationInformationEutra::encode(
    Ngap_UserLocationInformationEUTRA_t& userLocationInformation) const {
  if (!m_EutraCgi.encode(userLocationInformation.eUTRA_CGI)) {
    Logger::ngap().warn("Encode eUTRA_CGI IE error");
    return false;
  }
  if (!m_Tai.encode(userLocationInformation.tAI)) {
    Logger::ngap().warn("Encode Tai IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationEutra::decode(
    const Ngap_UserLocationInformationEUTRA_t& userLocationInformation) {
  if (!m_EutraCgi.decode(userLocationInformation.eUTRA_CGI)) {
    Logger::ngap().warn("Decode eUTRA_CGI IE error");
    return false;
  }

  if (!m_Tai.decode(userLocationInformation.tAI)) {
    Logger::ngap().warn("Decode Tai IE error");
    return false;
  }
  return true;
}
}  // namespace ngap
