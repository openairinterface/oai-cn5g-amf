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

#include "RanNodeName.hpp"

#include "conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RanNodeName::RanNodeName() {
  ran_node_name_ = {};
}

//------------------------------------------------------------------------------
RanNodeName::~RanNodeName() {}

//------------------------------------------------------------------------------
bool RanNodeName::setValue(const std::string& value) {
  if (value.size() > RAN_NODE_NAME_SIZE_MAX) return false;
  ran_node_name_ = value;
  return true;
}

//------------------------------------------------------------------------------
void RanNodeName::getValue(std::string& value) const {
  value = ran_node_name_;
}

//------------------------------------------------------------------------------
bool RanNodeName::encode(Ngap_RANNodeName_t& ran_node_name) {
  conv::string_2_octet_string(ran_node_name_, ran_node_name);
  return true;
}

//------------------------------------------------------------------------------
bool RanNodeName::decode(const Ngap_RANNodeName_t& ran_node_name) {
  if (!ran_node_name.buf) return false;
  conv::octet_string_2_string(ran_node_name, ran_node_name_);
  return true;
}

}  // namespace ngap
