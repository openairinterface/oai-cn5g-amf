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

#ifndef _PLMN_ID_H
#define _PLMN_ID_H

extern "C" {
#include "Ngap_PLMNIdentity.h"
}

#include <string>

namespace ngap {

class PlmnId {
 public:
  PlmnId();
  virtual ~PlmnId();

  void set(const std::string& mcc, const std::string& mnc);
  void get(std::string& mcc, std::string& mnc) const;

  void getMcc(std::string& mcc) const;
  void getMnc(std::string& mnc) const;

  bool encode(Ngap_PLMNIdentity_t&);
  bool decode(const Ngap_PLMNIdentity_t&);

 private:
  uint8_t mcc_digit2;
  uint8_t mcc_digit1;
  uint8_t mnc_digit3;  // in case of 2 digit MNC, it should be 0xf
  uint8_t mcc_digit3;
  uint8_t mnc_digit2;
  uint8_t mnc_digit1;
};

}  // namespace ngap

#endif
