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

#ifndef _SECURITY_INDICATION_H_
#define _SECURITY_INDICATION_H_

#include <optional>

#include "ConfidentialityProtectionIndication.hpp"
#include "IntegrityProtectionIndication.hpp"
#include "MaximumIntegrityProtectedDataRate.hpp"

extern "C" {
#include "Ngap_SecurityIndication.h"
}

namespace ngap {

class SecurityIndication {
 public:
  SecurityIndication();
  SecurityIndication(
      const IntegrityProtectionIndication& integrityProtectionIndication,
      const ConfidentialityProtectionIndication&
          confidentialityProtectionIndication,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          maximumIntegrityProtectedDataRateUl,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          maximumIntegrityProtectedDataRateDl);
  virtual ~SecurityIndication();

  void set(
      const IntegrityProtectionIndication& integrityProtectionIndication,
      const ConfidentialityProtectionIndication&
          confidentialityProtectionIndication,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          maximumIntegrityProtectedDataRateUl,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          maximumIntegrityProtectedDataRateDl);

  void get(
      IntegrityProtectionIndication& integrityProtectionIndication,
      ConfidentialityProtectionIndication& confidentialityProtectionIndication,
      std::optional<MaximumIntegrityProtectedDataRate>&
          maximumIntegrityProtectedDataRateUl,
      std::optional<MaximumIntegrityProtectedDataRate>&
          maximumIntegrityProtectedDataRateDl) const;

  bool encode(Ngap_SecurityIndication_t& securityIndication) const;
  bool decode(const Ngap_SecurityIndication_t& securityIndication);

 private:
  IntegrityProtectionIndication m_IntegrityProtectionIndication;  // Mandatory
  ConfidentialityProtectionIndication
      m_ConfidentialityProtectionIndication;  // Mandatory
  std::optional<MaximumIntegrityProtectedDataRate>
      m_MaximumIntegrityProtectedDataRateUl;  // Conditional
  std::optional<MaximumIntegrityProtectedDataRate>
      m_MaximumIntegrityProtectedDataRateDl;  // Optional
};

}  // namespace ngap

#endif
