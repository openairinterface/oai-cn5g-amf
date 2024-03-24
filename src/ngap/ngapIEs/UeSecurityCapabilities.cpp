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

#include "UeSecurityCapabilities.hpp"

#include "conversions.h"

namespace ngap {

//------------------------------------------------------------------------------
UeSecurityCapabilities::UeSecurityCapabilities() {
  m_NrEncryptionAlgorithms             = 0;
  m_IntegrityProtectionAlgorithms      = 0;
  m_EutraEncryptionAlgorithms          = 0;
  m_EutraIntegrityProtectionAlgorithms = 0;
}

//------------------------------------------------------------------------------
UeSecurityCapabilities::~UeSecurityCapabilities() {}

//------------------------------------------------------------------------------
void UeSecurityCapabilities::set(
    uint16_t nrEncryptionAlgorithms, uint16_t nrIntegrityProtectionAlgorithms,
    uint16_t eutraEncryptionAlgorithms,
    uint16_t eutraIntegrityProtectionAlgorithms) {
  m_NrEncryptionAlgorithms             = nrEncryptionAlgorithms;
  m_IntegrityProtectionAlgorithms      = nrIntegrityProtectionAlgorithms;
  m_EutraEncryptionAlgorithms          = eutraEncryptionAlgorithms;
  m_EutraIntegrityProtectionAlgorithms = eutraIntegrityProtectionAlgorithms;
}

//------------------------------------------------------------------------------
bool UeSecurityCapabilities::get(
    uint16_t& nrEncryptionAlgorithms, uint16_t& nrIntegrityProtectionAlgorithms,
    uint16_t& eutraEncryptionAlgorithms,
    uint16_t& eutraIntegrityProtectionAlgorithms) const {
  nrEncryptionAlgorithms             = m_NrEncryptionAlgorithms;
  nrIntegrityProtectionAlgorithms    = m_IntegrityProtectionAlgorithms;
  eutraEncryptionAlgorithms          = m_EutraEncryptionAlgorithms;
  eutraIntegrityProtectionAlgorithms = m_EutraIntegrityProtectionAlgorithms;

  return true;
}

//------------------------------------------------------------------------------
bool UeSecurityCapabilities::encode(
    Ngap_UESecurityCapabilities_t& ueSecurityCapabilities) const {
  ueSecurityCapabilities.nRencryptionAlgorithms.bits_unused = 0;
  ueSecurityCapabilities.nRencryptionAlgorithms.size        = sizeof(uint16_t);
  ueSecurityCapabilities.nRencryptionAlgorithms.buf =
      (uint8_t*) calloc(1, ueSecurityCapabilities.nRencryptionAlgorithms.size);
  if (!ueSecurityCapabilities.nRencryptionAlgorithms.buf) return false;
  for (int i = 0; i < ueSecurityCapabilities.nRencryptionAlgorithms.size; i++) {
    ueSecurityCapabilities.nRencryptionAlgorithms.buf[i] =
        (m_NrEncryptionAlgorithms & (0xff00 >> i * 8)) >>
        ((ueSecurityCapabilities.nRencryptionAlgorithms.size - i - 1) * 8);
  }

  ueSecurityCapabilities.nRintegrityProtectionAlgorithms.bits_unused = 0;
  ueSecurityCapabilities.nRintegrityProtectionAlgorithms.size =
      sizeof(uint16_t);
  ueSecurityCapabilities.nRintegrityProtectionAlgorithms.buf =
      (uint8_t*) calloc(
          1, ueSecurityCapabilities.nRintegrityProtectionAlgorithms.size);
  if (!ueSecurityCapabilities.nRintegrityProtectionAlgorithms.buf) return false;
  for (int i = 0;
       i < ueSecurityCapabilities.nRintegrityProtectionAlgorithms.size; i++) {
    ueSecurityCapabilities.nRintegrityProtectionAlgorithms.buf[i] =
        (m_IntegrityProtectionAlgorithms & (0xff00 >> i * 8)) >>
        ((ueSecurityCapabilities.nRintegrityProtectionAlgorithms.size - i - 1) *
         8);
  }

  ueSecurityCapabilities.eUTRAencryptionAlgorithms.bits_unused = 0;
  ueSecurityCapabilities.eUTRAencryptionAlgorithms.size = sizeof(uint16_t);
  ueSecurityCapabilities.eUTRAencryptionAlgorithms.buf  = (uint8_t*) calloc(
      1, ueSecurityCapabilities.eUTRAencryptionAlgorithms.size);
  if (!ueSecurityCapabilities.eUTRAencryptionAlgorithms.buf) return false;
  for (int i = 0; i < ueSecurityCapabilities.eUTRAencryptionAlgorithms.size;
       i++) {
    ueSecurityCapabilities.eUTRAencryptionAlgorithms.buf[i] =
        (m_EutraEncryptionAlgorithms & (0xff00 >> i * 8)) >>
        ((ueSecurityCapabilities.eUTRAencryptionAlgorithms.size - i - 1) * 8);
  }

  ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.bits_unused = 0;
  ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.size =
      sizeof(uint16_t);
  ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.buf =
      (uint8_t*) calloc(
          1, ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.size);
  if (!ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.buf)
    return false;
  for (int i = 0;
       i < ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.size;
       i++) {
    ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.buf[i] =
        (m_EutraIntegrityProtectionAlgorithms & (0xff00 >> i * 8)) >>
        ((ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.size - i -
          1) *
         8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool UeSecurityCapabilities::decode(
    const Ngap_UESecurityCapabilities_t& ueSecurityCapabilities) {
  if (!ueSecurityCapabilities.nRencryptionAlgorithms.buf) return false;
  if (!ueSecurityCapabilities.nRintegrityProtectionAlgorithms.buf) return false;
  if (!ueSecurityCapabilities.eUTRAencryptionAlgorithms.buf) return false;
  if (!ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.buf)
    return false;

  m_NrEncryptionAlgorithms             = 0;
  m_IntegrityProtectionAlgorithms      = 0;
  m_EutraEncryptionAlgorithms          = 0;
  m_EutraIntegrityProtectionAlgorithms = 0;

  for (int i = 0; i < ueSecurityCapabilities.nRencryptionAlgorithms.size; i++) {
    m_NrEncryptionAlgorithms = m_NrEncryptionAlgorithms << 8;
    m_NrEncryptionAlgorithms |=
        ueSecurityCapabilities.nRencryptionAlgorithms.buf[i];
  }
  for (int i = 0;
       i < ueSecurityCapabilities.nRintegrityProtectionAlgorithms.size; i++) {
    m_IntegrityProtectionAlgorithms = m_IntegrityProtectionAlgorithms << 8;
    m_IntegrityProtectionAlgorithms |=
        ueSecurityCapabilities.nRintegrityProtectionAlgorithms.buf[i];
  }
  for (int i = 0; i < ueSecurityCapabilities.eUTRAencryptionAlgorithms.size;
       i++) {
    m_EutraEncryptionAlgorithms = m_EutraEncryptionAlgorithms << 8;
    m_EutraEncryptionAlgorithms |=
        ueSecurityCapabilities.eUTRAencryptionAlgorithms.buf[i];
  }
  for (int i = 0;
       i < ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.size;
       i++) {
    m_EutraIntegrityProtectionAlgorithms = m_EutraIntegrityProtectionAlgorithms
                                           << 8;
    m_EutraIntegrityProtectionAlgorithms |=
        ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.buf[i];
  }

  return true;
}

}  // namespace ngap
