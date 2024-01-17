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

#ifndef _DRB_STATUS_DL12_H_
#define _DRB_STATUS_DL12_H_

#include "CountValueForPdcpSn12.hpp"
#include "logger.hpp"

extern "C" {
#include "Ngap_DRBStatusDL12.h"
}

namespace ngap {

class DrbStatusDl12 {
 public:
  DrbStatusDl12();
  virtual ~DrbStatusDl12();

  void get(CountValueForPdcpSn12& value) const;
  void set(const CountValueForPdcpSn12& value);

  bool encode(Ngap_DRBStatusDL12_t& dl12) const;
  bool decode(const Ngap_DRBStatusDL12_t& dl12);

 private:
  CountValueForPdcpSn12 dl_count_value_;  // Mandatory
};
}  // namespace ngap
#endif
