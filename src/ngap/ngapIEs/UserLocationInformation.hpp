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

#include "UserLocationInformationEutra.hpp"
#include "UserLocationInformationN3iwf.hpp"
#include "UserLocationInformationNr.hpp"

extern "C" {
#include "Ngap_UserLocationInformation.h"
}

namespace ngap {
class UserLocationInformation {
 public:
  UserLocationInformation();
  virtual ~UserLocationInformation();

  void set(const UserLocationInformationEutra&);
  bool get(UserLocationInformationEutra&) const;

  void set(const UserLocationInformationNr&);
  bool get(UserLocationInformationNr&) const;

  // void set(const UserLocationInformationN3IWF&);
  // void get(UserLocationInformationN3IWF&);

  Ngap_UserLocationInformation_PR getChoiceOfUserLocationInformation() const;

  bool encode(Ngap_UserLocationInformation_t& userLocationInformation) const;
  bool decode(const Ngap_UserLocationInformation_t& userLocationInformation);

 private:
  Ngap_UserLocationInformation_PR m_Present;
  std::optional<UserLocationInformationEutra> m_UserLocationInformationEutra;
  std::optional<UserLocationInformationNr> m_UserLocationInformationNr;
  // TODO: UserLocationInformationN3IWF *userLocationInformationN3IWF;
};

}  // namespace ngap

#endif
