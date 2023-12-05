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
#include "DrbStatusDl.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DrbStatusDl::DrbStatusDl() {
  dl18_ = std::nullopt;
  dl12_ = std::nullopt;
}

//------------------------------------------------------------------------------
DrbStatusDl::~DrbStatusDl() {}

//------------------------------------------------------------------------------
void DrbStatusDl::setDRBStatusDL18(const DrbStatusDl18& dl18) {
  dl18_ = std::make_optional<DrbStatusDl18>(dl18);
  dl12_ = std::nullopt;
}

//------------------------------------------------------------------------------
void DrbStatusDl::getDRBStatusDL18(std::optional<DrbStatusDl18>& dl18) const {
  dl18 = dl18_;
}

//------------------------------------------------------------------------------
void DrbStatusDl::setDRBStatusDL12(const DrbStatusDl12& dl12) {
  dl18_ = std::nullopt;
  dl12_ = std::make_optional<DrbStatusDl12>(dl12);
}

//------------------------------------------------------------------------------
void DrbStatusDl::getDRBStatusDL12(std::optional<DrbStatusDl12>& dl12) const {
  dl12 = dl12_;
}

//------------------------------------------------------------------------------
bool DrbStatusDl::encode(Ngap_DRBStatusDL_t& dL) {
  if (dl18_.has_value()) {
    dL.present = Ngap_DRBStatusDL_PR_dRBStatusDL18;
    dL.choice.dRBStatusDL18 =
        (Ngap_DRBStatusDL18_t*) calloc(1, sizeof(Ngap_DRBStatusDL18_t));
    if (!dl18_.value().encode(*dL.choice.dRBStatusDL18)) {
      Logger::ngap().error("Encode DRBStatusDL18 IE error");
      return false;
    }
  } else if (dl12_.has_value()) {
    dL.present = Ngap_DRBStatusDL_PR_dRBStatusDL12;
    dL.choice.dRBStatusDL12 =
        (Ngap_DRBStatusDL12_t*) calloc(1, sizeof(Ngap_DRBStatusDL12_t));
    if (!dl12_.value().encode(*dL.choice.dRBStatusDL12)) {
      Logger::ngap().error("Encode DRBStatusDL12 IE error");
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool DrbStatusDl::decode(const Ngap_DRBStatusDL_t& dL) {
  if (dL.present == Ngap_DRBStatusDL_PR_dRBStatusDL18) {
    DrbStatusDl18 dl18 = {};
    if (!dl18.decode(*dL.choice.dRBStatusDL18)) {
      Logger::ngap().error("Decode DRBStatusDL18 IE error");
      return false;
    }
    dl18_ = std::make_optional<DrbStatusDl18>(dl18);
  } else if (dL.present == Ngap_DRBStatusDL_PR_dRBStatusDL12) {
    DrbStatusDl12 dl12 = {};
    if (!dl12.decode(*dL.choice.dRBStatusDL12)) {
      Logger::ngap().error("Decode DRBStatusDL12 IE error");
      return false;
    }
    dl12_ = std::make_optional<DrbStatusDl12>(dl12);
  }
  return true;
}
}  // namespace ngap
