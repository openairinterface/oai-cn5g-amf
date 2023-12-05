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
  region_id_ = 0;
}

//------------------------------------------------------------------------------
AmfRegionId::~AmfRegionId() {}

//------------------------------------------------------------------------------
void AmfRegionId::setAMFRegionID(const std::string& id) {
  region_id_ = utils::fromString<int>(id);
}

//------------------------------------------------------------------------------
void AmfRegionId::setAMFRegionID(const uint8_t& id) {
  region_id_ = id;
}

//------------------------------------------------------------------------------
void AmfRegionId::getAMFRegionID(std::string& id) const {
  id = std::to_string(region_id_);
}

//------------------------------------------------------------------------------
void AmfRegionId::getAMFRegionID(uint8_t& id) const {
  id = region_id_;
}

//------------------------------------------------------------------------------
bool AmfRegionId::encode(Ngap_AMFRegionID_t& id) const {
  id.size         = 1;
  uint8_t* buffer = (uint8_t*) calloc(1, sizeof(uint8_t));
  if (!buffer) return false;
  *buffer        = region_id_;
  id.buf         = buffer;
  id.bits_unused = 0;

  return true;
}

//------------------------------------------------------------------------------
bool AmfRegionId::decode(Ngap_AMFRegionID_t id) {
  if (!id.buf) return false;
  region_id_ = *id.buf;

  return true;
}
}  // namespace ngap
