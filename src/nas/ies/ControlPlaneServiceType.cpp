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

#include "ControlPlaneServiceType.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ControlPlaneServiceType::ControlPlaneServiceType()
    : Type1NasIe(true), service_type_value_() {}

//------------------------------------------------------------------------------
ControlPlaneServiceType::ControlPlaneServiceType(uint8_t value)
    : Type1NasIe(true) {
  service_type_value_ = value & 0x07;
  Type1NasIe::SetValue(service_type_value_);
}

//------------------------------------------------------------------------------
ControlPlaneServiceType::~ControlPlaneServiceType() {}

//------------------------------------------------------------------------------
void ControlPlaneServiceType::SetValue(uint8_t value) {
  service_type_value_ = value & 0x07;
  Type1NasIe::SetValue(service_type_value_);
}

//------------------------------------------------------------------------------
void ControlPlaneServiceType::GetValue(uint8_t& value) const {
  value = service_type_value_;
}

//------------------------------------------------------------------------------
void ControlPlaneServiceType::SetValue() {
  Type1NasIe::SetValue(service_type_value_ & 0x07);
}

//------------------------------------------------------------------------------
void ControlPlaneServiceType::GetValue() {
  service_type_value_ = value_ & 0x07;
}
