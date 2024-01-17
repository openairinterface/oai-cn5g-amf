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
      const IntegrityProtectionIndication& m_integrityProtectionIndication,
      const ConfidentialityProtectionIndication&
          m_confidentialityProtectionIndication,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          m_maximumIntegrityProtectedDataRateUL,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          m_maximumIntegrityProtectedDataRateDL);
  virtual ~SecurityIndication();

  void setSecurityIndication(
      const IntegrityProtectionIndication& m_integrityProtectionIndication,
      const ConfidentialityProtectionIndication&
          m_confidentialityProtectionIndication,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          m_maximumIntegrityProtectedDataRateUL,
      const std::optional<MaximumIntegrityProtectedDataRate>&
          m_maximumIntegrityProtectedDataRateDL);

  void getSecurityIndication(
      IntegrityProtectionIndication& m_integrityProtectionIndication,
      ConfidentialityProtectionIndication&
          m_confidentialityProtectionIndication,
      std::optional<MaximumIntegrityProtectedDataRate>&
          m_maximumIntegrityProtectedDataRateUL,
      std::optional<MaximumIntegrityProtectedDataRate>&
          m_maximumIntegrityProtectedDataRateDL) const;

  bool encode(Ngap_SecurityIndication_t& securityIndication);
  bool decode(const Ngap_SecurityIndication_t& securityIndication);

 private:
  IntegrityProtectionIndication integrityProtectionIndication;  // Mandatory
  ConfidentialityProtectionIndication
      confidentialityProtectionIndication;  // Mandatory
  std::optional<MaximumIntegrityProtectedDataRate>
      maximumIntegrityProtectedDataRateUL;  // Conditional
  std::optional<MaximumIntegrityProtectedDataRate>
      maximumIntegrityProtectedDataRateDL;  // Optional
};

}  // namespace ngap

#endif
