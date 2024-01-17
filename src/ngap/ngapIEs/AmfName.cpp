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

#include "AmfName.hpp"

#include "amf_conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
AmfName::AmfName() {}

//------------------------------------------------------------------------------
AmfName::~AmfName() {}

//------------------------------------------------------------------------------
bool AmfName::setValue(const std::string& amf_name) {
  if (amf_name.size() > AMF_NAME_SIZE_MAX) return false;
  amf_name_ = amf_name;
  return true;
}

//------------------------------------------------------------------------------
void AmfName::getValue(std::string& amf_name) const {
  amf_name = amf_name_;
}

//------------------------------------------------------------------------------
bool AmfName::encode(Ngap_AMFName_t& amf_name_ie) const {
  amf_conv::string_2_octet_string(amf_name_, amf_name_ie);
  return true;
}

//------------------------------------------------------------------------------
bool AmfName::decode(const Ngap_AMFName_t& amf_name_ie) {
  if (!amf_name_ie.buf) return false;
  amf_conv::octet_string_2_string(amf_name_ie, amf_name_);
  return true;
}

}  // namespace ngap
