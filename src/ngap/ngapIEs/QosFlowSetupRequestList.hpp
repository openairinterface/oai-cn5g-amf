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

#ifndef _QOS_FLOW_SETUP_REQUEST_LIST_H_
#define _QOS_FLOW_SETUP_REQUEST_LIST_H_

#include <vector>

#include "QosFlowSetupRequestItem.hpp"

extern "C" {
#include "Ngap_QosFlowSetupRequestList.h"
}

namespace ngap {

class QosFlowSetupRequestList {
 public:
  QosFlowSetupRequestList();
  virtual ~QosFlowSetupRequestList();

  void set(const std::vector<QosFlowSetupRequestItem>& list);
  void get(std::vector<QosFlowSetupRequestItem>& list) const;

  bool encode(Ngap_QosFlowSetupRequestList_t& qosFlowSetupRequestList);
  bool decode(const Ngap_QosFlowSetupRequestList_t& qosFlowSetupRequestList);

 private:
  std::vector<QosFlowSetupRequestItem> list_;
  constexpr static uint8_t kMaxNoOfQoSFlows = 64;
};
}  // namespace ngap
#endif
