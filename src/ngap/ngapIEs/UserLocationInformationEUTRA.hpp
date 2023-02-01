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

#ifndef _USERLOCATIONINFORMATIONEUTRA_H_
#define _USERLOCATIONINFORMATIONEUTRA_H_

#include "EUTRA-CGI.hpp"
#include "TAI.hpp"
//#include "TimeStamp.hpp"

extern "C" {
#include "Ngap_UserLocationInformationEUTRA.h"
}

namespace ngap {
class UserLocationInformationEUTRA {
 public:
  UserLocationInformationEUTRA();
  virtual ~UserLocationInformationEUTRA();

  void set(const EUTRA_CGI& m_eUTRA_CGI, const TAI& m_tAI);
  void get(EUTRA_CGI& m_eUTRA_CGI, TAI& m_tAI);

  // bool getTimeStampPresence();

  bool encode(Ngap_UserLocationInformationEUTRA_t* userLocationInformation);
  bool decode(Ngap_UserLocationInformationEUTRA_t* userLocationInformation);

 private:
  EUTRA_CGI eUTRA_CGI;  // Mandatory
  TAI tAI;              // Mandatory
  // TODO: TimeStamp *timeStamp; //Age of Location (Optional)
  // TODO: NG-RAN CGI (PSCell Information) (Optional)
};

}  // namespace ngap

#endif
