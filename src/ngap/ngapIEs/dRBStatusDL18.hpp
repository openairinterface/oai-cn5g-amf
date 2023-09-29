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

#ifndef _DRB_STATUS_DL18_H_
#define _DRB_STATUS_DL18_H_

#include "COUNTValueForPDCP_SN18.hpp"
#include "logger.hpp"

extern "C" {
#include "Ngap_DRBStatusDL18.h"
}
namespace ngap {
class DRBStatusDL18 {
 public:
  DRBStatusDL18();
  virtual ~DRBStatusDL18();

  void getcountvalue(COUNTValueForPDCP_SN18& value);
  void setcountvalue(const COUNTValueForPDCP_SN18& value);

  bool encodeddRBStatusDL18(Ngap_DRBStatusDL18_t* DL18);
  bool decodeddRBStatusDL18(Ngap_DRBStatusDL18_t* DL18);

 private:
  COUNTValueForPDCP_SN18 pdcp_value;  // Mandatory
  // TODO: Old Associated QoS Flow List - UL End Marker Expected (Optional)
};
}  // namespace ngap
#endif
