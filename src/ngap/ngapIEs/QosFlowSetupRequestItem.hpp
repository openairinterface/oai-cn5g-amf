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

#ifndef _QOS_FLOW_SETUP_REQUEST_ITEM_H_
#define _QOS_FLOW_SETUP_REQUEST_ITEM_H_

#include "QosFlowIdentifier.hpp"
#include "QosFlowLevelQosParameters.hpp"

extern "C" {
#include "Ngap_QosFlowSetupRequestItem.h"
}

namespace ngap {

class QosFlowSetupRequestItem {
 public:
  QosFlowSetupRequestItem();
  virtual ~QosFlowSetupRequestItem();

  void set(
      const QosFlowIdentifier& m_qosFlowIdentifier,
      const QosFlowLevelQosParameters& m_qosFlowLevelQosParameters);
  bool get(
      QosFlowIdentifier& m_qosFlowIdentifier,
      QosFlowLevelQosParameters& m_qosFlowLevelQosParameters) const;

  bool encode(Ngap_QosFlowSetupRequestItem_t&);
  bool decode(const Ngap_QosFlowSetupRequestItem_t&);

 private:
  QosFlowIdentifier qosFlowIdentifier;                  // Mandatory
  QosFlowLevelQosParameters qosFlowLevelQosParameters;  // Mandatory
  // TODO: E-RAB ID //Optional
};

}  // namespace ngap

#endif
