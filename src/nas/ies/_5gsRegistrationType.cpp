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

#include "_5gsRegistrationType.hpp"

#include "3gpp_24.501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5gsRegistrationType::_5gsRegistrationType()
    : Type1NasIeFormatTv(), follow_on_req_(false), reg_type_(0) {}

//------------------------------------------------------------------------------
_5gsRegistrationType::_5gsRegistrationType(
    const bool& follow_on_req, const uint8_t& type)
    : Type1NasIeFormatTv(), follow_on_req_(follow_on_req) {
  if (validateValue(follow_on_req, type)) reg_type_ = type;
  setValue();
}

//------------------------------------------------------------------------------
_5gsRegistrationType::_5gsRegistrationType(
    const uint8_t& iei, const bool& follow_on_req, const uint8_t& type)
    : Type1NasIeFormatTv(iei) {
  follow_on_req_ = follow_on_req;
  if (validateValue(follow_on_req, type)) reg_type_ = type;
  setValue();
}

//------------------------------------------------------------------------------
_5gsRegistrationType::~_5gsRegistrationType() {}

//------------------------------------------------------------------------------
void _5gsRegistrationType::setValue() {
  if (follow_on_req_)
    value_ = 0b1000 | (0x07 & reg_type_);
  else
    value_ = 0x07 & reg_type_;
}

//------------------------------------------------------------------------------
void _5gsRegistrationType::getValue() {
  follow_on_req_ = (0b1000 & value_) >> 3;
  reg_type_      = value_ & 0b00000111;
}

//------------------------------------------------------------------------------
bool _5gsRegistrationType::validateValue(
    const bool& follow_on_req, const uint8_t& type) {
  if (type > static_cast<uint8_t>(_5gsMobileIdentityEnum::MAX_VALUE))
    return false;
  return true;
}

//------------------------------------------------------------------------------
void _5gsRegistrationType::set(
    const bool& follow_on_req, const uint8_t& type, const uint8_t& iei) {
  follow_on_req_ = follow_on_req;
  if (validateValue(follow_on_req, type)) reg_type_ = type;
  setValue();
  SetIei(iei);
}

//------------------------------------------------------------------------------
void _5gsRegistrationType::set(const bool& follow_on_req, const uint8_t& type) {
  follow_on_req_ = follow_on_req;
  if (validateValue(follow_on_req, type)) reg_type_ = type;
  setValue();
}

//------------------------------------------------------------------------------
bool _5gsRegistrationType::isFollowOnReq() {
  getValue();
  return follow_on_req_;
}

//------------------------------------------------------------------------------
uint8_t _5gsRegistrationType::getRegType() {
  getValue();
  return reg_type_;
}
