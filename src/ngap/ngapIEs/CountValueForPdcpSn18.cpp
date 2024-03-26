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

#include "CountValueForPdcpSn18.hpp"

namespace ngap {
//------------------------------------------------------------------------------
CountValueForPdcpSn18::CountValueForPdcpSn18() {
  m_Pdcp    = 0;
  m_HfnPdcp = 0;
}

//------------------------------------------------------------------------------
CountValueForPdcpSn18::~CountValueForPdcpSn18() {}
void CountValueForPdcpSn18::set(const long& pDCP, const long& hfn_PDCP) {
  m_Pdcp    = pDCP;
  m_HfnPdcp = hfn_PDCP;
}

//------------------------------------------------------------------------------
void CountValueForPdcpSn18::get(long& pDCP, long& hFN_PDCP) const {
  pDCP     = m_Pdcp;
  hFN_PDCP = m_HfnPdcp;
}

//------------------------------------------------------------------------------
bool CountValueForPdcpSn18::encode(
    Ngap_COUNTValueForPDCP_SN18_t& countvalue) const {
  countvalue.pDCP_SN18     = m_Pdcp;
  countvalue.hFN_PDCP_SN18 = m_HfnPdcp;
  return true;
}

//------------------------------------------------------------------------------
bool CountValueForPdcpSn18::decode(
    const Ngap_COUNTValueForPDCP_SN18_t& countValue) {
  m_Pdcp    = countValue.pDCP_SN18;
  m_HfnPdcp = countValue.hFN_PDCP_SN18;
  return true;
}
}  // namespace ngap
