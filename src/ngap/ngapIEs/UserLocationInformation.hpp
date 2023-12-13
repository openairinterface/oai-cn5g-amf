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

#ifndef _USER_LOCATION_INFORMATION_H_
#define _USER_LOCATION_INFORMATION_H_

#include <optional>

#include "UserLocationInformationEUTRA.hpp"
#include "UserLocationInformationN3IWF.hpp"
#include "UserLocationInformationNR.hpp"

extern "C" {
#include "Ngap_UserLocationInformation.h"
}

namespace ngap {
class UserLocationInformation {
 public:
  UserLocationInformation();
  virtual ~UserLocationInformation();

  void setInformation(const UserLocationInformationEUTRA&);
  bool getInformation(UserLocationInformationEUTRA&);

  void setInformation(const UserLocationInformationNR&);
  bool getInformation(UserLocationInformationNR&);

  // void setInformation(const UserLocationInformationN3IWF&);
  // void getInformation(UserLocationInformationN3IWF&);

  Ngap_UserLocationInformation_PR getChoiceOfUserLocationInformation();

  bool encode(Ngap_UserLocationInformation_t& user_location_information);
  bool decode(const Ngap_UserLocationInformation_t& user_location_information);

 private:
  Ngap_UserLocationInformation_PR present_;
  std::optional<UserLocationInformationEUTRA> user_location_information_eutra_;
  std::optional<UserLocationInformationNR> user_location_information_nr_;
  // TODO: UserLocationInformationN3IWF *userLocationInformationN3IWF;
};

}  // namespace ngap

#endif
