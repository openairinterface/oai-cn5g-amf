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
  m_Ul18 = std::nullopt;
  m_Ul12 = std::nullopt;
}

//------------------------------------------------------------------------------
DrbStatusUl::~DrbStatusUl() {}

//------------------------------------------------------------------------------
void DrbStatusUl::setDrbStatusUl(const DrbStatusUl18& ul18) {
  m_Ul18 = std::make_optional<DrbStatusUl18>(ul18);
  m_Ul12 = std::nullopt;
}

//------------------------------------------------------------------------------
void DrbStatusUl::getDrbStatusUl(std::optional<DrbStatusUl18>& ul18) const {
  ul18 = m_Ul18;
}

//------------------------------------------------------------------------------
void DrbStatusUl::setDrbStatusUl(const DrbStatusUl12& ul12) {
  m_Ul18 = std::nullopt;
  m_Ul12 = std::make_optional<DrbStatusUl12>(ul12);
}

//------------------------------------------------------------------------------
void DrbStatusUl::getDrbStatusUl(std::optional<DrbStatusUl12>& ul12) const {
  ul12 = m_Ul12;
}

//------------------------------------------------------------------------------
bool DrbStatusUl::encode(Ngap_DRBStatusUL_t& ul) const {
  if (m_Ul18.has_value()) {
    ul.present = Ngap_DRBStatusUL_PR_dRBStatusUL18;
    ul.choice.dRBStatusUL18 =
        (Ngap_DRBStatusUL18_t*) calloc(1, sizeof(Ngap_DRBStatusUL18_t));
    if (!m_Ul18.value().encode(*ul.choice.dRBStatusUL18)) {
      Logger::ngap().error("Encode DRBStatusUL18 IE error");
      return false;
    }
  } else if (m_Ul12.has_value()) {
    ul.present = Ngap_DRBStatusUL_PR_dRBStatusUL12;
    ul.choice.dRBStatusUL12 =
        (Ngap_DRBStatusUL12_t*) calloc(1, sizeof(Ngap_DRBStatusUL12_t));
    if (!m_Ul12.value().encode(*ul.choice.dRBStatusUL12)) {
      Logger::ngap().error("Encode DRBStatusUL18 IE error");
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool DrbStatusUl::decode(const Ngap_DRBStatusUL_t& ul) {
  if (ul.present == Ngap_DRBStatusUL_PR_dRBStatusUL18) {
    DrbStatusUl18 item = {};
    if (!item.decode(*ul.choice.dRBStatusUL18)) {
      Logger::ngap().error("Decode DRBStatusUL18 IE error");
      return false;
    }
    m_Ul18 = std::make_optional<DrbStatusUl18>(item);
  } else if (ul.present == Ngap_DRBStatusUL_PR_dRBStatusUL12) {
    DrbStatusUl12 item = {};
    if (!item.decode(*ul.choice.dRBStatusUL12)) {
      Logger::ngap().error("Decode DRBStatusUL12 IE error");
      return false;
    }
    m_Ul12 = std::make_optional<DrbStatusUl12>(item);
  }

  return true;
}
}  // namespace ngap
