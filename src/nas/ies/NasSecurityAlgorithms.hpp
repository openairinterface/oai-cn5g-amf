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

#ifndef _NAS_SECURITY_ALGORITHS_H
#define _NAS_SECURITY_ALGORITHS_H

#include "Type3NasIe.hpp"

constexpr uint8_t kNasSecurityAlgorithmsLength = 2;
constexpr auto kNasSecurityAlgorithmsIeName    = "NAS Security Algorithms";

namespace nas {

class NasSecurityAlgorithms : public Type3NasIe {
 public:
  NasSecurityAlgorithms();
  NasSecurityAlgorithms(uint8_t iei);
  NasSecurityAlgorithms(uint8_t ciphering, uint8_t integrity_protection);
  NasSecurityAlgorithms(
      uint8_t iei, uint8_t ciphering, uint8_t integrity_protection);
  ~NasSecurityAlgorithms();

  static std::string GetIeName() { return kNasSecurityAlgorithmsIeName; }

  void SetTypeOfCipheringAlgorithm(uint8_t value);
  uint8_t GetTypeOfCipheringAlgorithm() const;

  void SetTypeOfIntegrityProtectionAlgorithm(uint8_t value);
  uint8_t GetTypeOfIntegrityProtectionAlgorithm() const;

  void Set(uint8_t ciphering, uint8_t integrity_protection);
  void Get(uint8_t& ciphering, uint8_t& integrity_protection) const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

 private:
  uint8_t type_of_ciphering_algorithm_;
  uint8_t type_of_integrity_protection_algorithm_;
};

}  // namespace nas

#endif
