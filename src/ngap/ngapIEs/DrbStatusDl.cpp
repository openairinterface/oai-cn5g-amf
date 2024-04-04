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
  m_Dl18 = std::nullopt;
  m_Dl12 = std::nullopt;
}

//------------------------------------------------------------------------------
DrbStatusDl::~DrbStatusDl() {}

//------------------------------------------------------------------------------
void DrbStatusDl::setDrbStatusDl18(const DrbStatusDl18& dl18) {
  m_Dl18 = std::make_optional<DrbStatusDl18>(dl18);
  m_Dl12 = std::nullopt;
}

//------------------------------------------------------------------------------
void DrbStatusDl::getDrbStatusDl18(std::optional<DrbStatusDl18>& dl18) const {
  dl18 = m_Dl18;
}

//------------------------------------------------------------------------------
void DrbStatusDl::setDrbStatusDl12(const DrbStatusDl12& dl12) {
  m_Dl18 = std::nullopt;
  m_Dl12 = std::make_optional<DrbStatusDl12>(dl12);
}

//------------------------------------------------------------------------------
void DrbStatusDl::getDrbStatusDl12(std::optional<DrbStatusDl12>& dl12) const {
  dl12 = m_Dl12;
}

//------------------------------------------------------------------------------
bool DrbStatusDl::encode(Ngap_DRBStatusDL_t& dl) const {
  if (m_Dl18.has_value()) {
    dl.present = Ngap_DRBStatusDL_PR_dRBStatusDL18;
    dl.choice.dRBStatusDL18 =
        (Ngap_DRBStatusDL18_t*) calloc(1, sizeof(Ngap_DRBStatusDL18_t));
    if (!m_Dl18.value().encode(*dl.choice.dRBStatusDL18)) {
      Logger::ngap().error("Encode DRBStatusDL18 IE error");
      return false;
    }
  } else if (m_Dl12.has_value()) {
    dl.present = Ngap_DRBStatusDL_PR_dRBStatusDL12;
    dl.choice.dRBStatusDL12 =
        (Ngap_DRBStatusDL12_t*) calloc(1, sizeof(Ngap_DRBStatusDL12_t));
    if (!m_Dl12.value().encode(*dl.choice.dRBStatusDL12)) {
      Logger::ngap().error("Encode DRBStatusDL12 IE error");
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool DrbStatusDl::decode(const Ngap_DRBStatusDL_t& dl) {
  if (dl.present == Ngap_DRBStatusDL_PR_dRBStatusDL18) {
    DrbStatusDl18 dl18 = {};
    if (!dl18.decode(*dl.choice.dRBStatusDL18)) {
      Logger::ngap().error("Decode DRBStatusDL18 IE error");
      return false;
    }
    m_Dl18 = std::make_optional<DrbStatusDl18>(dl18);
  } else if (dl.present == Ngap_DRBStatusDL_PR_dRBStatusDL12) {
    DrbStatusDl12 dl12 = {};
    if (!dl12.decode(*dl.choice.dRBStatusDL12)) {
      Logger::ngap().error("Decode DRBStatusDL12 IE error");
      return false;
    }
    m_Dl12 = std::make_optional<DrbStatusDl12>(dl12);
  }
  return true;
}
}  // namespace ngap
