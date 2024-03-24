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

#ifndef _UE_SECURITY_CAPABILITIES_H_
#define _UE_SECURITY_CAPABILITIES_H_

extern "C" {
#include "Ngap_UESecurityCapabilities.h"
}

namespace ngap {

class UeSecurityCapabilities {
 public:
  UeSecurityCapabilities();
  virtual ~UeSecurityCapabilities();

  void set(
      uint16_t nr_encryption_algs, uint16_t integrityProtectionAlgorithms,
      uint16_t eutraEncryptionAlgorithms,
      uint16_t eutraIntegrityProtectionAlgorithms);
  bool get(
      uint16_t& nr_encryption_algs, uint16_t& integrityProtectionAlgorithms,
      uint16_t& eutraEncryptionAlgorithms,
      uint16_t& eutraIntegrityProtectionAlgorithms) const;

  bool encode(Ngap_UESecurityCapabilities_t&) const;
  bool decode(const Ngap_UESecurityCapabilities_t&);

 private:
  uint16_t m_NrEncryptionAlgorithms;              // Mandatory
  uint16_t m_IntegrityProtectionAlgorithms;       // Mandatory
  uint16_t m_EutraEncryptionAlgorithms;           // Mandatory
  uint16_t m_EutraIntegrityProtectionAlgorithms;  // Mandatory
};

}  // namespace ngap

#endif
