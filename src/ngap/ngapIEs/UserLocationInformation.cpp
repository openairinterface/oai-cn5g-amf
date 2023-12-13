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

#include "UserLocationInformation.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformation::UserLocationInformation() {
  present_                         = Ngap_UserLocationInformation_PR_NOTHING;
  user_location_information_eutra_ = std::nullopt;
  user_location_information_nr_    = std::nullopt;
  // userLocationInformationN3IWF = std::nullopt;
}

//------------------------------------------------------------------------------
UserLocationInformation::~UserLocationInformation() {}

//------------------------------------------------------------------------------
Ngap_UserLocationInformation_PR
UserLocationInformation::getChoiceOfUserLocationInformation() {
  return present_;
}

//------------------------------------------------------------------------------
bool UserLocationInformation::getInformation(
    UserLocationInformationEUTRA& information_eutra) {
  if (!user_location_information_eutra_.has_value()) return false;
  information_eutra = user_location_information_eutra_.value();
  return true;
}

//------------------------------------------------------------------------------
void UserLocationInformation::setInformation(
    const UserLocationInformationEUTRA& information_eutra) {
  present_ = Ngap_UserLocationInformation_PR_userLocationInformationEUTRA;
  user_location_information_eutra_ =
      std::optional<UserLocationInformationEUTRA>(information_eutra);
}

//------------------------------------------------------------------------------
bool UserLocationInformation::getInformation(
    UserLocationInformationNR& information_nr) {
  if (!user_location_information_nr_.has_value()) return false;
  information_nr = user_location_information_nr_.value();
  return true;
}

//------------------------------------------------------------------------------
void UserLocationInformation::setInformation(
    const UserLocationInformationNR& information_nr) {
  present_ = Ngap_UserLocationInformation_PR_userLocationInformationNR;
  user_location_information_nr_ =
      std::optional<UserLocationInformationNR>(information_nr);
}

//------------------------------------------------------------------------------
bool UserLocationInformation::encode(
    Ngap_UserLocationInformation_t& user_location_information) {
  user_location_information.present = present_;
  switch (present_) {
    case Ngap_UserLocationInformation_PR_userLocationInformationEUTRA: {
      Ngap_UserLocationInformationEUTRA* ie_eutra =
          (Ngap_UserLocationInformationEUTRA*) calloc(
              1, sizeof(Ngap_UserLocationInformationEUTRA));
      user_location_information_eutra_.value().encode(*ie_eutra);
      user_location_information.choice.userLocationInformationEUTRA = ie_eutra;
      break;
    }
    case Ngap_UserLocationInformation_PR_userLocationInformationNR: {
      Ngap_UserLocationInformationNR* ie_nr =
          (Ngap_UserLocationInformationNR*) calloc(
              1, sizeof(Ngap_UserLocationInformationNR));
      user_location_information_nr_.value().encode(*ie_nr);
      user_location_information.choice.userLocationInformationNR = ie_nr;
      break;
    }
    default:
      Logger::ngap().warn("UserLocationInformation encode error!");
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformation::decode(
    const Ngap_UserLocationInformation_t& user_location_information) {
  present_ = user_location_information.present;
  switch (present_) {
    case Ngap_UserLocationInformation_PR_userLocationInformationEUTRA: {
      UserLocationInformationEUTRA user_location_information_eutra = {};
      user_location_information_eutra.decode(
          *user_location_information.choice.userLocationInformationEUTRA);
      user_location_information_eutra_ =
          std::optional<UserLocationInformationEUTRA>(
              user_location_information_eutra);
      break;
    }
    case Ngap_UserLocationInformation_PR_userLocationInformationNR: {
      UserLocationInformationNR user_location_information_nr = {};
      user_location_information_nr.decode(
          *user_location_information.choice.userLocationInformationNR);
      user_location_information_nr_ = std::optional<UserLocationInformationNR>(
          user_location_information_nr);
      break;
    }
    default:
      Logger::ngap().warn("UserLocationInformation decode error!");
      return false;
  }
  return true;
}

}  // namespace ngap
