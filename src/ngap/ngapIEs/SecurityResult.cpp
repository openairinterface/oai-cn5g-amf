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

#include "SecurityResult.hpp"

namespace ngap {

//------------------------------------------------------------------------------
SecurityResult::SecurityResult() {}

//------------------------------------------------------------------------------
SecurityResult::~SecurityResult() {}

//------------------------------------------------------------------------------
void SecurityResult::set(
    const IntegrityProtectionResult& integrityProtectionResult,
    const ConfidentialityProtectionResult& confidentialityProtectionResult) {
  m_IntegrityProtectionResult       = integrityProtectionResult;
  m_ConfidentialityProtectionResult = confidentialityProtectionResult;
}

//------------------------------------------------------------------------------
bool SecurityResult::get(
    IntegrityProtectionResult& integrityProtectionResult,
    ConfidentialityProtectionResult& confidentialityProtectionResult) const {
  integrityProtectionResult       = m_IntegrityProtectionResult;
  confidentialityProtectionResult = m_ConfidentialityProtectionResult;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityResult::encode(Ngap_SecurityResult_t& securityResult) const {
  if (!m_IntegrityProtectionResult.encode(
          securityResult.integrityProtectionResult))
    return false;
  if (!m_ConfidentialityProtectionResult.encode(
          securityResult.confidentialityProtectionResult))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityResult::decode(const Ngap_SecurityResult_t& securityResult) {
  if (!m_IntegrityProtectionResult.decode(
          securityResult.integrityProtectionResult))
    return false;
  if (!m_ConfidentialityProtectionResult.decode(
          securityResult.confidentialityProtectionResult))
    return false;

  return true;
}
}  // namespace ngap
