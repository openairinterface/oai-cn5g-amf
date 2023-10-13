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

#include "UESecurityCapabilities.hpp"

#include "conversions.h"

namespace ngap {

//------------------------------------------------------------------------------
UESecurityCapabilities::UESecurityCapabilities() {
  nr_encryption_algs_               = 0;
  nr_integrity_protection_algs_     = 0;
  e_utra_encryption_algs_           = 0;
  e_utra_integrity_protection_algs_ = 0;
}

//------------------------------------------------------------------------------
UESecurityCapabilities::~UESecurityCapabilities() {}

//------------------------------------------------------------------------------
void UESecurityCapabilities::set(
    const uint16_t& nr_encryption_algs,
    const uint16_t& nr_integrity_protection_algs,
    const uint16_t& e_utra_encryption_algs,
    const uint16_t& e_utra_integrity_protection_algs) {
  nr_encryption_algs_               = nr_encryption_algs;
  nr_integrity_protection_algs_     = nr_integrity_protection_algs;
  e_utra_encryption_algs_           = e_utra_encryption_algs;
  e_utra_integrity_protection_algs_ = e_utra_integrity_protection_algs;
}

//------------------------------------------------------------------------------
bool UESecurityCapabilities::get(
    uint16_t& nr_encryption_algs, uint16_t& nr_integrity_protection_algs,
    uint16_t& e_utra_encryption_algs,
    uint16_t& e_utra_integrity_protection_algs) {
  nr_encryption_algs               = nr_encryption_algs_;
  nr_integrity_protection_algs     = nr_integrity_protection_algs_;
  e_utra_encryption_algs           = e_utra_encryption_algs_;
  e_utra_integrity_protection_algs = e_utra_integrity_protection_algs_;

  return true;
}

//------------------------------------------------------------------------------
bool UESecurityCapabilities::encode(
    Ngap_UESecurityCapabilities_t& ueSecurityCapabilities) {
  ueSecurityCapabilities.nRencryptionAlgorithms.bits_unused = 0;
  ueSecurityCapabilities.nRencryptionAlgorithms.size        = sizeof(uint16_t);
  ueSecurityCapabilities.nRencryptionAlgorithms.buf =
      (uint8_t*) calloc(1, ueSecurityCapabilities.nRencryptionAlgorithms.size);
  if (!ueSecurityCapabilities.nRencryptionAlgorithms.buf) return false;
  for (int i = 0; i < ueSecurityCapabilities.nRencryptionAlgorithms.size; i++) {
    ueSecurityCapabilities.nRencryptionAlgorithms.buf[i] =
        (nr_encryption_algs_ & (0xff00 >> i * 8)) >>
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
        (nr_integrity_protection_algs_ & (0xff00 >> i * 8)) >>
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
        (e_utra_encryption_algs_ & (0xff00 >> i * 8)) >>
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
        (e_utra_integrity_protection_algs_ & (0xff00 >> i * 8)) >>
        ((ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.size - i -
          1) *
         8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool UESecurityCapabilities::decode(
    const Ngap_UESecurityCapabilities_t& ueSecurityCapabilities) {
  if (!ueSecurityCapabilities.nRencryptionAlgorithms.buf) return false;
  if (!ueSecurityCapabilities.nRintegrityProtectionAlgorithms.buf) return false;
  if (!ueSecurityCapabilities.eUTRAencryptionAlgorithms.buf) return false;
  if (!ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.buf)
    return false;

  nr_encryption_algs_               = 0;
  nr_integrity_protection_algs_     = 0;
  e_utra_encryption_algs_           = 0;
  e_utra_integrity_protection_algs_ = 0;

  for (int i = 0; i < ueSecurityCapabilities.nRencryptionAlgorithms.size; i++) {
    nr_encryption_algs_ = nr_encryption_algs_ << 8;
    nr_encryption_algs_ |= ueSecurityCapabilities.nRencryptionAlgorithms.buf[i];
  }
  for (int i = 0;
       i < ueSecurityCapabilities.nRintegrityProtectionAlgorithms.size; i++) {
    nr_integrity_protection_algs_ = nr_integrity_protection_algs_ << 8;
    nr_integrity_protection_algs_ |=
        ueSecurityCapabilities.nRintegrityProtectionAlgorithms.buf[i];
  }
  for (int i = 0; i < ueSecurityCapabilities.eUTRAencryptionAlgorithms.size;
       i++) {
    e_utra_encryption_algs_ = e_utra_encryption_algs_ << 8;
    e_utra_encryption_algs_ |=
        ueSecurityCapabilities.eUTRAencryptionAlgorithms.buf[i];
  }
  for (int i = 0;
       i < ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.size;
       i++) {
    e_utra_integrity_protection_algs_ = e_utra_integrity_protection_algs_ << 8;
    e_utra_integrity_protection_algs_ |=
        ueSecurityCapabilities.eUTRAintegrityProtectionAlgorithms.buf[i];
  }

  return true;
}

}  // namespace ngap
