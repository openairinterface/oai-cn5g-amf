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

#include "amf_conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RanNodeName::RanNodeName() {
  m_RanNodeName = {};
}

//------------------------------------------------------------------------------
RanNodeName::~RanNodeName() {}

//------------------------------------------------------------------------------
bool RanNodeName::set(const std::string& value) {
  if (value.size() > RAN_NODE_NAME_SIZE_MAX) return false;
  m_RanNodeName = value;
  return true;
}

//------------------------------------------------------------------------------
void RanNodeName::get(std::string& value) const {
  value = m_RanNodeName;
}

//------------------------------------------------------------------------------
bool RanNodeName::encode(Ngap_RANNodeName_t& ranNodeName) const {
  amf_conv::string_2_octet_string(m_RanNodeName, ranNodeName);
  return true;
}

//------------------------------------------------------------------------------
bool RanNodeName::decode(const Ngap_RANNodeName_t& ranNodeName) {
  if (!ranNodeName.buf) return false;
  amf_conv::octet_string_2_string(ranNodeName, m_RanNodeName);
  return true;
}

}  // namespace ngap
