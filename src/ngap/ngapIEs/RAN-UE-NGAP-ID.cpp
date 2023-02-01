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

#include "RAN-UE-NGAP-ID.hpp"
//#include "String2Value.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
RAN_UE_NGAP_ID::RAN_UE_NGAP_ID() {
  id_ = 0;
}

//------------------------------------------------------------------------------
RAN_UE_NGAP_ID::RAN_UE_NGAP_ID(uint32_t id) : id_(id) {}

//------------------------------------------------------------------------------
RAN_UE_NGAP_ID::~RAN_UE_NGAP_ID() {}

//------------------------------------------------------------------------------
void RAN_UE_NGAP_ID::set(const uint32_t& id) {
  id_ = id;
}

//------------------------------------------------------------------------------
uint32_t RAN_UE_NGAP_ID::get() const {
  return id_;
}

//------------------------------------------------------------------------------
bool RAN_UE_NGAP_ID::encode(Ngap_RAN_UE_NGAP_ID_t& ran_ue_ngap_id) {
  ran_ue_ngap_id = id_;
  return true;
}

//------------------------------------------------------------------------------
bool RAN_UE_NGAP_ID::decode(Ngap_RAN_UE_NGAP_ID_t& ran_ue_ngap_id) {
  id_ = ran_ue_ngap_id;
  return true;
}
}  // namespace ngap
