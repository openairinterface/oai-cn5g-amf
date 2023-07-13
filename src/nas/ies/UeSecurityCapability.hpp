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

#ifndef _UE_SECURITY_CAPABILITY_H
#define _UE_SECURITY_CAPABILITY_H

#include "Type4NasIe.hpp"

constexpr uint8_t kUeSecurityCapabilityMinimumLength = 4;
constexpr uint8_t kUeSecurityCapabilityMaximumLength = 10;
constexpr auto kUeSecurityCapabilityIeName           = "UE Security Capability";

namespace nas {

class UeSecurityCapability : public Type4NasIe {
 public:
  UeSecurityCapability();
  UeSecurityCapability(uint8_t iei);
  UeSecurityCapability(uint8_t _5g_ea, uint8_t _5g_ia);
  UeSecurityCapability(uint8_t iei, uint8_t _5g_ea, uint8_t _5g_ia);
  UeSecurityCapability(
      uint8_t iei, uint8_t _5g_ea, uint8_t _5g_ia, uint8_t eea, uint8_t eia);
  UeSecurityCapability(
      uint8_t _5g_ea, uint8_t _5g_ia, uint8_t eea, uint8_t eia);
  ~UeSecurityCapability();
  void operator=(const UeSecurityCapability& ue_security_capability);

  static std::string GetIeName() { return kUeSecurityCapabilityIeName; }

  void SetEa(uint8_t value);
  uint8_t GetEa() const;

  void SetIa(uint8_t value);
  uint8_t GetIa() const;

  void SetEea(uint8_t value);
  bool GetEea(uint8_t& value) const;

  void SetEia(uint8_t value);
  bool GetEia(uint8_t& value) const;

  void Set(uint8_t _5g_ea, uint8_t _5g_ia);
  void Set(uint8_t _5g_ea, uint8_t _5g_ia, uint8_t eea, uint8_t eia);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

 private:
  uint8_t _5g_ea_;              // 3rd octet, Mandatory
  uint8_t _5g_ia_;              // 4th octet, Mandatory
  std::optional<uint8_t> eea_;  // 5th octet, Optional
  std::optional<uint8_t> eia_;  // 6th octet, Optional
  // Spare
};

}  // namespace nas

#endif
