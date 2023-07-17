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

#ifndef _UE_NETWORK_CAPABILITY_H
#define _UE_NETWORK_CAPABILITY_H

#include "Type4NasIe.hpp"

constexpr uint8_t kUeNetworkCapabilityMinimumLength = 4;
constexpr uint8_t kUeNetworkCapabilityMaximumLength = 15;
constexpr auto kUeNetworkCapabilityIeName           = "UE Network Capability";

namespace nas {

class UeNetworkCapability : public Type4NasIe {
 public:
  UeNetworkCapability();
  UeNetworkCapability(uint8_t iei);
  UeNetworkCapability(const uint8_t iei, uint8_t eea, uint8_t eia);
  ~UeNetworkCapability();

  static std::string GetIeName() { return kUeNetworkCapabilityIeName; }

  void SetEea(uint8_t value);
  void SetEia(uint8_t value);

  uint8_t GetEea() const;
  uint8_t GetEia() const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_option = true);

 private:
  uint8_t eea_;  // Mandatory
  uint8_t eia_;  // Mandatory
  // TODO: uint8_t uea_; //Optional
  // TODO: uint8_t uia_; //Optional
  // TODO: uint8_t octet7_; //Optional
  // TODO: uint8_t octet8_;//Optional
  // TODO: uint8_t octet9_; //Optional
  // TODO: spare octet 10-15
};

}  // namespace nas

#endif
