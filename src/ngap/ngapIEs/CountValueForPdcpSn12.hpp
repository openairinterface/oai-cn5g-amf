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

#ifndef _COUNT_VALUE_FOR_PDCP_SN_12_H_
#define _COUNT_VALUE_FOR_PDCP_SN_12_H_

extern "C" {
#include "Ngap_COUNTValueForPDCP-SN12.h"
}

namespace ngap {
class CountValueForPdcpSn12 {
 public:
  CountValueForPdcpSn12();
  virtual ~CountValueForPdcpSn12();

  void setValue(long pDCP, long hfn_PDCP);
  void getValue(long& pDCP, long& hFN_PDCP) const;

  bool encode(Ngap_COUNTValueForPDCP_SN12_t& value) const;
  bool decode(const Ngap_COUNTValueForPDCP_SN12_t& value);

 private:
  long pdcp;      // Mandatory (12 bits)
  long hfn_pdcp;  // Mandatory (20 bits?)
};

}  // namespace ngap
#endif
