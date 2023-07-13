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

#include "_5gsDeregistrationType.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsDeregistrationType::_5gsDeregistrationType() : Type1NasIeFormatTv() {
  u1.b = 0;
}

//------------------------------------------------------------------------------
_5gsDeregistrationType::_5gsDeregistrationType(uint8_t iei)
    : Type1NasIeFormatTv(iei) {
  u1.bf.iei = iei;
}

//------------------------------------------------------------------------------
_5gsDeregistrationType::_5gsDeregistrationType(uint8_t iei, uint8_t value) {
  u1.b = (iei << 4) | (value && 0x0f);
  SetValue(value && 0x0f);
}
//------------------------------------------------------------------------------
_5gsDeregistrationType::_5gsDeregistrationType(
    _5gs_deregistration_type_t type) {
  u1.b                           = 0;
  u1.bf.switch_off               = type.switch_off;
  u1.bf.re_registration_required = type.re_registration_required;
  u1.bf.access_type              = type.access_type;
  u1.bf.iei                      = type.iei;
  if (type.iei != 0) {
    SetIei(type.iei);
  }
  SetValue(u1.b && 0x0f);
}

//------------------------------------------------------------------------------
_5gsDeregistrationType::~_5gsDeregistrationType() {}

//------------------------------------------------------------------------------
void _5gsDeregistrationType::setValue() {
  Type1NasIeFormatTv::SetValue(u1.b && 0x0f);
}

//------------------------------------------------------------------------------
void _5gsDeregistrationType::getValue() {
  u1.b = Type1NasIeFormatTv::GetValue() && 0x0f;
  if (iei_.has_value()) {
    u1.b |= iei_.value() && 0xf0;
  }
}

//------------------------------------------------------------------------------
void _5gsDeregistrationType::set(_5gs_deregistration_type_t type) {
  u1.b                           = 0;
  u1.bf.switch_off               = type.switch_off;
  u1.bf.re_registration_required = type.re_registration_required;
  u1.bf.access_type              = type.access_type;
  u1.bf.iei                      = type.iei;
  if (type.iei != 0) {
    SetIei(type.iei);
  }
  SetValue(u1.b && 0x0f);
}

//------------------------------------------------------------------------------
void _5gsDeregistrationType::get(_5gs_deregistration_type_t& type) const {
  type.switch_off               = u1.bf.switch_off;
  type.re_registration_required = u1.bf.re_registration_required;
  type.access_type              = u1.bf.access_type;
  type.iei                      = u1.bf.iei;
}

//------------------------------------------------------------------------------
void _5gsDeregistrationType::set(uint8_t type) {
  u1.b = type;
}

//------------------------------------------------------------------------------
void _5gsDeregistrationType::get(uint8_t& type) const {
  type = u1.b;
}

/*
//------------------------------------------------------------------------------
int _5gsDeregistrationType::Encode(uint8_t* buf, int len) {
  Logger::nas_mm().error("Encoding 5GSDeregistrationType IE");
  if (len < 1) {
    Logger::nas_mm().error(
        "Encoding 5GSDeregistrationType error (len is less than one)");
    return -1;
  }
  *buf = u1.b;
  return 0;
}

//------------------------------------------------------------------------------
int _5gsDeregistrationType::Decode(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding 5GSDeregistrationType");
  u1.b = *buf;

  Logger::nas_mm().debug("Decoded 5GSDeRegistrationType");
  return 0;
}
*/
