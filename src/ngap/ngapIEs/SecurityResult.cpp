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
void SecurityResult::setSecurityResult(
    const IntegrityProtectionResult& m_integrityProtectionResult,
    const ConfidentialityProtectionResult& m_confidentialityProtectionResult) {
  integrityProtectionResult       = m_integrityProtectionResult;
  confidentialityProtectionResult = m_confidentialityProtectionResult;
}

//------------------------------------------------------------------------------
bool SecurityResult::getSecurityResult(
    IntegrityProtectionResult& m_integrityProtectionResult,
    ConfidentialityProtectionResult& m_confidentialityProtectionResult) const {
  m_integrityProtectionResult       = integrityProtectionResult;
  m_confidentialityProtectionResult = confidentialityProtectionResult;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityResult::encode(Ngap_SecurityResult_t& securityResult) {
  if (!integrityProtectionResult.encode(
          securityResult.integrityProtectionResult))
    return false;
  if (!confidentialityProtectionResult.encode(
          securityResult.confidentialityProtectionResult))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityResult::decode(const Ngap_SecurityResult_t& securityResult) {
  if (!integrityProtectionResult.decode(
          securityResult.integrityProtectionResult))
    return false;
  if (!confidentialityProtectionResult.decode(
          securityResult.confidentialityProtectionResult))
    return false;

  return true;
}
}  // namespace ngap
