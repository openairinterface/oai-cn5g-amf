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

#ifndef _USER_LOCATION_INFORMATION_N3IWF_H_
#define _USER_LOCATION_INFORMATION_N3IWF_H_

#include "TransportLayerAddress.hpp"

extern "C" {
#include "Ngap_UserLocationInformationN3IWF.h"
}

namespace ngap {
class UserLocationInformationN3IWF {
 public:
  UserLocationInformationN3IWF();
  virtual ~UserLocationInformationN3IWF();

  void set(const Ngap_TransportLayerAddress_t&, const Ngap_PortNumber_t&);
  void get(Ngap_TransportLayerAddress_t&, Ngap_PortNumber_t&);

  bool encode(Ngap_UserLocationInformationN3IWF_t*);
  bool decode(Ngap_UserLocationInformationN3IWF_t*);

 private:
  Ngap_TransportLayerAddress_t iPAddress;  // Mandatory
  Ngap_PortNumber_t portNumber;            // Mandatory
  TransportLayerAddress* transportLayerAddress;
};

}  // namespace ngap

#endif
