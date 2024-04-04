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

#include "AmfRegionId.hpp"

#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
AmfRegionId::AmfRegionId() {
  m_RegionId = 0;
}

//------------------------------------------------------------------------------
AmfRegionId::~AmfRegionId() {}

//------------------------------------------------------------------------------
void AmfRegionId::set(const std::string& id) {
  m_RegionId = utils::fromString<int>(id);
}

//------------------------------------------------------------------------------
void AmfRegionId::set(const uint8_t& id) {
  m_RegionId = id;
}

//------------------------------------------------------------------------------
void AmfRegionId::get(std::string& id) const {
  id = std::to_string(m_RegionId);
}

//------------------------------------------------------------------------------
void AmfRegionId::get(uint8_t& id) const {
  id = m_RegionId;
}

//------------------------------------------------------------------------------
bool AmfRegionId::encode(Ngap_AMFRegionID_t& id) const {
  id.size         = 1;
  uint8_t* buffer = (uint8_t*) calloc(1, sizeof(uint8_t));
  if (!buffer) return false;
  *buffer        = m_RegionId;
  id.buf         = buffer;
  id.bits_unused = 0;

  return true;
}

//------------------------------------------------------------------------------
bool AmfRegionId::decode(const Ngap_AMFRegionID_t& id) {
  if (!id.buf) return false;
  m_RegionId = *id.buf;

  return true;
}
}  // namespace ngap
