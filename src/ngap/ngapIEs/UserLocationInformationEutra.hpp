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

#ifndef _USER_LOCATION_INFORMATION_EUTRA_H_
#define _USER_LOCATION_INFORMATION_EUTRA_H_

#include "EutraCgi.hpp"
#include "Tai.hpp"

extern "C" {
#include "Ngap_UserLocationInformationEUTRA.h"
}

namespace ngap {
class UserLocationInformationEutra {
 public:
  UserLocationInformationEutra();
  virtual ~UserLocationInformationEutra();

  void set(const EutraCgi& eutraCgi, const Tai& tai);
  void get(EutraCgi& eutraCgi, Tai& tai) const;

  // bool getTimeStampPresence();

  bool encode(
      Ngap_UserLocationInformationEUTRA_t& userLocationInformation) const;
  bool decode(
      const Ngap_UserLocationInformationEUTRA_t& userLocationInformation);

 private:
  EutraCgi m_EutraCgi;  // Mandatory
  Tai m_Tai;            // Mandatory
  // TODO: TimeStamp *timeStamp; //Age of Location (Optional)
  // TODO: NG-RAN CGI (PSCell Information) (Optional)
};

}  // namespace ngap

#endif
