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

#include "DrbStatusUl.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DrbStatusUl::DrbStatusUl() {
  ul18_ = std::nullopt;
  ul12_ = std::nullopt;
}

//------------------------------------------------------------------------------
DrbStatusUl::~DrbStatusUl() {}

//------------------------------------------------------------------------------
void DrbStatusUl::setdRBStatusUL(const DrbStatusUl18& ul18) {
  ul18_ = std::make_optional<DrbStatusUl18>(ul18);
  ul12_ = std::nullopt;
}

//------------------------------------------------------------------------------
void DrbStatusUl::getdRBStatusUL(std::optional<DrbStatusUl18>& ul18) const {
  ul18 = ul18_;
}

//------------------------------------------------------------------------------
void DrbStatusUl::setdRBStatusUL(const DrbStatusUl12& ul12) {
  ul18_ = std::nullopt;
  ul12_ = std::make_optional<DrbStatusUl12>(ul12);
}

//------------------------------------------------------------------------------
void DrbStatusUl::getdRBStatusUL(std::optional<DrbStatusUl12>& ul12) const {
  ul12 = ul12_;
}

//------------------------------------------------------------------------------
bool DrbStatusUl::encode(Ngap_DRBStatusUL_t& uL) {
  if (ul18_.has_value()) {
    uL.present = Ngap_DRBStatusUL_PR_dRBStatusUL18;
    uL.choice.dRBStatusUL18 =
        (Ngap_DRBStatusUL18_t*) calloc(1, sizeof(Ngap_DRBStatusUL18_t));
    if (!ul18_.value().encode(*uL.choice.dRBStatusUL18)) {
      Logger::ngap().error("Encode DRBStatusUL18 IE error");
      return false;
    }
  } else if (ul12_.has_value()) {
    uL.present = Ngap_DRBStatusUL_PR_dRBStatusUL12;
    uL.choice.dRBStatusUL12 =
        (Ngap_DRBStatusUL12_t*) calloc(1, sizeof(Ngap_DRBStatusUL12_t));
    if (!ul12_.value().encode(*uL.choice.dRBStatusUL12)) {
      Logger::ngap().error("Encode DRBStatusUL18 IE error");
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool DrbStatusUl::decode(const Ngap_DRBStatusUL_t& uL) {
  if (uL.present == Ngap_DRBStatusUL_PR_dRBStatusUL18) {
    DrbStatusUl18 item = {};
    if (!item.decode(*uL.choice.dRBStatusUL18)) {
      Logger::ngap().error("Decode DRBStatusUL18 IE error");
      return false;
    }
    ul18_ = std::make_optional<DrbStatusUl18>(item);
  } else if (uL.present == Ngap_DRBStatusUL_PR_dRBStatusUL12) {
    DrbStatusUl12 item = {};
    if (!item.decode(*uL.choice.dRBStatusUL12)) {
      Logger::ngap().error("Decode DRBStatusUL12 IE error");
      return false;
    }
    ul12_ = std::make_optional<DrbStatusUl12>(item);
  }

  return true;
}
}  // namespace ngap
