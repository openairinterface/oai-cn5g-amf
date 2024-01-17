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

#include "DrbStatusDl12.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DrbStatusDl12::DrbStatusDl12() {}

//------------------------------------------------------------------------------
DrbStatusDl12::~DrbStatusDl12() {}

//------------------------------------------------------------------------------
void DrbStatusDl12::get(CountValueForPdcpSn12& value) const {
  value = dl_count_value_;
}

//------------------------------------------------------------------------------
void DrbStatusDl12::set(const CountValueForPdcpSn12& value) {
  dl_count_value_ = value;
}

//------------------------------------------------------------------------------
bool DrbStatusDl12::encode(Ngap_DRBStatusDL12_t& dl12) const {
  if (!dl_count_value_.encode(dl12.dL_COUNTValue)) {
    Logger::ngap().error("Encode DrbStatusDl12 IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool DrbStatusDl12::decode(const Ngap_DRBStatusDL12_t& dl12) {
  if (!dl_count_value_.decode(dl12.dL_COUNTValue)) {
    Logger::ngap().error("Decode DrbStatusDl12 IE error");
    return false;
  }
  return true;
}
}  // namespace ngap
