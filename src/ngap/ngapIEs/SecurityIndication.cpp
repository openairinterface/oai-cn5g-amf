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

#include "SecurityIndication.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {

//------------------------------------------------------------------------------
SecurityIndication::SecurityIndication() {
  maximumIntegrityProtectedDataRateUL = std::nullopt;
  maximumIntegrityProtectedDataRateDL = std::nullopt;
}

//------------------------------------------------------------------------------
SecurityIndication::SecurityIndication(
    const IntegrityProtectionIndication& m_integrityProtectionIndication,
    const ConfidentialityProtectionIndication&
        m_confidentialityProtectionIndication,
    const std::optional<MaximumIntegrityProtectedDataRate>&
        m_maximumIntegrityProtectedDataRateUL,
    const std::optional<MaximumIntegrityProtectedDataRate>&
        m_maximumIntegrityProtectedDataRateDL) {
  integrityProtectionIndication       = m_integrityProtectionIndication;
  confidentialityProtectionIndication = m_confidentialityProtectionIndication;
  maximumIntegrityProtectedDataRateUL = m_maximumIntegrityProtectedDataRateUL;
  maximumIntegrityProtectedDataRateDL = m_maximumIntegrityProtectedDataRateDL;
}

//------------------------------------------------------------------------------
SecurityIndication::~SecurityIndication() {}

//------------------------------------------------------------------------------
void SecurityIndication::setSecurityIndication(
    const IntegrityProtectionIndication& m_integrityProtectionIndication,
    const ConfidentialityProtectionIndication&
        m_confidentialityProtectionIndication,
    const std::optional<MaximumIntegrityProtectedDataRate>&
        m_maximumIntegrityProtectedDataRateUL,
    const std::optional<MaximumIntegrityProtectedDataRate>&
        m_maximumIntegrityProtectedDataRateDL) {
  integrityProtectionIndication       = m_integrityProtectionIndication;
  confidentialityProtectionIndication = m_confidentialityProtectionIndication;
  maximumIntegrityProtectedDataRateUL = m_maximumIntegrityProtectedDataRateUL;
  maximumIntegrityProtectedDataRateDL = m_maximumIntegrityProtectedDataRateDL;
}

//------------------------------------------------------------------------------
void SecurityIndication::getSecurityIndication(
    IntegrityProtectionIndication& m_integrityProtectionIndication,
    ConfidentialityProtectionIndication& m_confidentialityProtectionIndication,
    std::optional<MaximumIntegrityProtectedDataRate>&
        m_maximumIntegrityProtectedDataRateUL,
    std::optional<MaximumIntegrityProtectedDataRate>&
        m_maximumIntegrityProtectedDataRateDL) const {
  m_integrityProtectionIndication       = integrityProtectionIndication;
  m_confidentialityProtectionIndication = confidentialityProtectionIndication;
  m_maximumIntegrityProtectedDataRateUL = maximumIntegrityProtectedDataRateUL;
  m_maximumIntegrityProtectedDataRateDL = maximumIntegrityProtectedDataRateDL;
}

//------------------------------------------------------------------------------
bool SecurityIndication::encode(Ngap_SecurityIndication_t& securityIndication) {
  if (!integrityProtectionIndication.encode(
          securityIndication.integrityProtectionIndication))
    return false;
  if (!confidentialityProtectionIndication.encode(
          securityIndication.confidentialityProtectionIndication))
    return false;
  if (maximumIntegrityProtectedDataRateUL.has_value()) {
    Ngap_MaximumIntegrityProtectedDataRate_t* maxIPDataRate =
        (Ngap_MaximumIntegrityProtectedDataRate_t*) calloc(
            1, sizeof(Ngap_MaximumIntegrityProtectedDataRate_t));
    if (!maxIPDataRate) return false;
    if (!maximumIntegrityProtectedDataRateUL.value().encode(*maxIPDataRate)) {
      free_wrapper((void**) &maxIPDataRate);
      return false;
    }

    securityIndication.maximumIntegrityProtectedDataRate_UL = maxIPDataRate;
    // free_wrapper((void**) &maxIPDataRate);
  }
  // TODO: check maximumIntegrityProtectedDataRateDL

  return true;
}

//------------------------------------------------------------------------------
bool SecurityIndication::decode(
    const Ngap_SecurityIndication_t& securityIndication) {
  if (!integrityProtectionIndication.decode(
          securityIndication.integrityProtectionIndication))
    return false;
  if (!confidentialityProtectionIndication.decode(
          securityIndication.confidentialityProtectionIndication))
    return false;

  // TODO: verify maximumIntegrityProtectedDataRate
  if (securityIndication.maximumIntegrityProtectedDataRate_UL) {
    MaximumIntegrityProtectedDataRate tmp = {};

    if (!tmp.decode(*securityIndication.maximumIntegrityProtectedDataRate_UL))
      return false;
    maximumIntegrityProtectedDataRateUL =
        std::make_optional<MaximumIntegrityProtectedDataRate>(tmp);
  }
  // TODO: verify maximumIntegrityProtectedDataRateDL

  return true;
}

}  // namespace ngap
