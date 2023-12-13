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

#ifndef _QOS_FLOW_PER_TNL_INFORMATION_LIST_H_
#define _QOS_FLOW_PER_TNL_INFORMATION_LIST_H_

#include <vector>

#include "QosFlowPerTnlInformationItem.hpp"

extern "C" {
#include "Ngap_QosFlowPerTNLInformationList.h"
}

namespace ngap {

class QosFlowPerTnlInformationList {
 public:
  QosFlowPerTnlInformationList();
  virtual ~QosFlowPerTnlInformationList();

  void set(const std::vector<QosFlowPerTnlInformationItem>& list);
  void get(std::vector<QosFlowPerTnlInformationItem>& list) const;

  bool encode(
      Ngap_QosFlowPerTNLInformationList_t& qosFlowPerTnlInformationList);
  bool decode(
      const Ngap_QosFlowPerTNLInformationList_t& qosFlowPerTnlInformationList);

 private:
  std::vector<QosFlowPerTnlInformationItem> list_;
  constexpr static uint8_t KMaxNoOfMultiConnectivityMinusOne = 3;
};
}  // namespace ngap
#endif
