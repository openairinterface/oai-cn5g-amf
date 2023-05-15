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

#include "NetworkSlicingIndication.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
NetworkSlicingIndication::NetworkSlicingIndication()
    : Type1NasIeFormatTv(), dcni_(), nssci_() {}

//------------------------------------------------------------------------------
NetworkSlicingIndication::NetworkSlicingIndication(uint8_t iei)
    : Type1NasIeFormatTv(iei) {
  dcni_  = false;
  nssci_ = false;
}

//------------------------------------------------------------------------------
NetworkSlicingIndication::NetworkSlicingIndication(bool dcni, bool nssci)
    : Type1NasIeFormatTv() {
  dcni_  = dcni;
  nssci_ = nssci;
}

//------------------------------------------------------------------------------
NetworkSlicingIndication::NetworkSlicingIndication(
    const uint8_t iei, bool dcni, bool nssci)
    : Type1NasIeFormatTv(iei) {
  dcni_  = dcni;
  nssci_ = nssci;
}

//------------------------------------------------------------------------------
NetworkSlicingIndication::~NetworkSlicingIndication() {}

//------------------------------------------------------------------------------
void NetworkSlicingIndication::SetValue() {
  value_ = 0x0f | (dcni_ << 1) | nssci_;
}

//------------------------------------------------------------------------------
void NetworkSlicingIndication::GetValue() {
  dcni_  = value_ & 0x02;
  nssci_ = value_ & 0x01;
}

//------------------------------------------------------------------------------
void NetworkSlicingIndication::SetDcni(bool value) {
  dcni_ = value;
}

//------------------------------------------------------------------------------
void NetworkSlicingIndication::SetNssci(bool value) {
  nssci_ = value;
}

//------------------------------------------------------------------------------
bool NetworkSlicingIndication::GetDcni() const {
  return dcni_;
}

//------------------------------------------------------------------------------
bool NetworkSlicingIndication::GetNssci() const {
  return nssci_;
}

//------------------------------------------------------------------------------
int NetworkSlicingIndication::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding %s", GetIeName().c_str());

  int encoded_size = 0;
  SetValue();  // Update Value in Type1NasIeFormatTv
  encoded_size = Type1NasIeFormatTv::Encode(buf, len);

  Logger::nas_mm().debug(
      "Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NetworkSlicingIndication::Decode(uint8_t* buf, int len, bool is_iei) {
  Logger::nas_mm().debug("Decoding %s", GetIeName().c_str());
  int decoded_size = 0;
  decoded_size     = Type1NasIeFormatTv::Decode(buf, len, is_iei);
  // Get DCNI/NSSCI from value
  GetValue();

  Logger::nas_mm().debug("DCNI 0x%x, NSSCI 0x%x", dcni_, nssci_);
  Logger::nas_mm().debug(
      "Decoded %s, len (%d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}
