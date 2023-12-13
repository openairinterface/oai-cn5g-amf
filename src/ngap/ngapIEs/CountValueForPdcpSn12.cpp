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

#include "CountValueForPdcpSn12.hpp"

namespace ngap {
//------------------------------------------------------------------------------
CountValueForPdcpSn12::CountValueForPdcpSn12() {
  pdcp     = 0;
  hfn_pdcp = 0;
}

//------------------------------------------------------------------------------
CountValueForPdcpSn12::~CountValueForPdcpSn12() {}
void CountValueForPdcpSn12::setValue(long pDCP, long hfn_PDCP) {
  pdcp     = pDCP;
  hfn_pdcp = hfn_PDCP;
}

//------------------------------------------------------------------------------
void CountValueForPdcpSn12::getValue(long& pDCP, long& hFN_PDCP) const {
  pDCP     = pdcp;
  hFN_PDCP = hfn_pdcp;
}

//------------------------------------------------------------------------------
bool CountValueForPdcpSn12::encode(Ngap_COUNTValueForPDCP_SN12_t& value) const {
  value.pDCP_SN12     = pdcp;
  value.hFN_PDCP_SN12 = hfn_pdcp;
  return true;
}

//------------------------------------------------------------------------------
bool CountValueForPdcpSn12::decode(const Ngap_COUNTValueForPDCP_SN12_t& value) {
  pdcp     = value.pDCP_SN12;
  hfn_pdcp = value.hFN_PDCP_SN12;
  return true;
}
}  // namespace ngap
