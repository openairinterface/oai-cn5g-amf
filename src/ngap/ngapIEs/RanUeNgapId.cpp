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

#include "RanUeNgapId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RanUeNgapId::RanUeNgapId() {
  id_ = 0;
}

//------------------------------------------------------------------------------
RanUeNgapId::RanUeNgapId(uint32_t id) : id_(id) {}

//------------------------------------------------------------------------------
RanUeNgapId::~RanUeNgapId() {}

//------------------------------------------------------------------------------
void RanUeNgapId::set(const uint32_t& id) {
  id_ = id;
}

//------------------------------------------------------------------------------
uint32_t RanUeNgapId::get() const {
  return id_;
}

//------------------------------------------------------------------------------
bool RanUeNgapId::encode(Ngap_RAN_UE_NGAP_ID_t& ran_ue_ngap_id) {
  ran_ue_ngap_id = id_;
  return true;
}

//------------------------------------------------------------------------------
bool RanUeNgapId::decode(Ngap_RAN_UE_NGAP_ID_t ran_ue_ngap_id) {
  id_ = ran_ue_ngap_id;
  return true;
}
}  // namespace ngap
