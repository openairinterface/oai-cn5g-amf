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

#ifndef _DRB_STATUS_UL12_H_
#define _DRB_STATUS_UL12_H_

#include "CountValueForPdcpSn12.hpp"

extern "C" {
#include "Ngap_DRBStatusUL12.h"
}

namespace ngap {

class DrbStatusUl12 {
 public:
  DrbStatusUl12();
  virtual ~DrbStatusUl12();

  void get(CountValueForPdcpSn12& count_value) const;
  void set(const CountValueForPdcpSn12& count_value);

  bool encode(Ngap_DRBStatusUL12_t& ul12);
  bool decode(const Ngap_DRBStatusUL12_t& ul12);

 private:
  CountValueForPdcpSn12 pdcp_value;  // Mandatory
  // TODO: Receive Status of UL PDCP SDUs //Optional
};

}  // namespace ngap
#endif
