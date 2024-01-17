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

#ifndef _USER_LOCATION_INFORMATION_NR_H_
#define _USER_LOCATION_INFORMATION_NR_H_

#include "NrCgi.hpp"
#include "TAI.hpp"

extern "C" {
#include "Ngap_UserLocationInformationNR.h"
}

namespace ngap {
class UserLocationInformationNR {
 public:
  UserLocationInformationNR();
  virtual ~UserLocationInformationNR();

  void set(const NrCgi&, const TAI&);
  void get(NrCgi&, TAI&);

  bool encode(Ngap_UserLocationInformationNR_t&);
  bool decode(const Ngap_UserLocationInformationNR_t&);

 private:
  NrCgi nR_CGI;  // Mandatory
  TAI tAI;       // Mandatory
  // bool timeStampIsSet;
  // TODO: TimeStamp timeStamp; //Age of Location (Optional)
  // TODO: PSCell Information
};

}  // namespace ngap

#endif
