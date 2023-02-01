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

#ifndef _S1_UE_SECURITY_CAPABILITY_H
#define _S1_UE_SECURITY_CAPABILITY_H

#include "Type4NasIe.hpp"

constexpr uint8_t kS1UeSecurityCapabilityMinimumLength = 4;
constexpr uint8_t kS1UeSecurityCapabilityMaximumLength = 7;
constexpr auto kS1UeSecurityCapabilityIeName = "S1 UE Security Capability";

namespace nas {

class S1UeSecurityCapability : public Type4NasIe {
 public:
  S1UeSecurityCapability();
  S1UeSecurityCapability(uint8_t iei);
  S1UeSecurityCapability(const uint8_t iei, uint8_t eea, uint8_t eia);
  S1UeSecurityCapability(
      const uint8_t iei, uint8_t eea, uint8_t eia, uint8_t uea, uint8_t uia);
  ~S1UeSecurityCapability();

  static std::string GetIeName() { return kS1UeSecurityCapabilityIeName; }

  void SetEea(uint8_t sel);
  uint8_t GetEea() const;

  void SetEia(uint8_t sel);
  uint8_t GetEia() const;

  void Set(uint8_t eea, uint8_t eia);
  void Get(uint8_t& eea, uint8_t& eia) const;

  void Set(uint8_t eea, uint8_t eia, uint8_t uea, uint8_t uia);
  void Get(uint8_t& eea, uint8_t& eia, uint8_t& uea, uint8_t& uia) const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

 private:
  uint8_t eea_;
  uint8_t eia_;
  std::optional<uint8_t> uea_;  // 5th octet, Optional
  std::optional<uint8_t> uia_;  // 6th octet, Optional
  std::optional<uint8_t> gea_;  // 7th octet, Optional
};

}  // namespace nas

#endif
