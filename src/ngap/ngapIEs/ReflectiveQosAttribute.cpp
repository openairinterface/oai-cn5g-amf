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

#include "ReflectiveQosAttribute.hpp"

namespace ngap {

//------------------------------------------------------------------------------
ReflectiveQosAttribute::ReflectiveQosAttribute() {
  attribute_ = 0;
}

//------------------------------------------------------------------------------
ReflectiveQosAttribute::ReflectiveQosAttribute(
    e_Ngap_ReflectiveQosAttribute value) {
  attribute_ = value;
}
//------------------------------------------------------------------------------
ReflectiveQosAttribute::~ReflectiveQosAttribute() {}

//------------------------------------------------------------------------------
void ReflectiveQosAttribute::set(e_Ngap_ReflectiveQosAttribute value) {
  attribute_ = value;
}

//------------------------------------------------------------------------------
bool ReflectiveQosAttribute::get(e_Ngap_ReflectiveQosAttribute& value) {
  if (attribute_ == 0)
    value = (e_Ngap_ReflectiveQosAttribute) attribute_;
  else
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool ReflectiveQosAttribute::encode(
    Ngap_ReflectiveQosAttribute_t& value) const {
  value = attribute_;

  return true;
}

//------------------------------------------------------------------------------
bool ReflectiveQosAttribute::decode(Ngap_ReflectiveQosAttribute_t value) {
  attribute_ = value;

  return true;
}
}  // namespace ngap
