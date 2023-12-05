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

#ifndef _QOS_FLOW_PER_TNL_INFORMATION_H_
#define _QOS_FLOW_PER_TNL_INFORMATION_H_

#include "AssociatedQosFlowList.hpp"
#include "UPTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_QosFlowPerTNLInformation.h"
}

namespace ngap {

class QosFlowPerTnlInformation {
 public:
  QosFlowPerTnlInformation();
  virtual ~QosFlowPerTnlInformation();

  void setQoSFlowPerTNLInformation(
      const UpTransportLayerInformation& m_uPTransportLayerInformation,
      const AssociatedQosFlowList& m_associatedQosFlowList);
  void getQoSFlowPerTNLInformation(
      UpTransportLayerInformation& m_uPTransportLayerInformation,
      AssociatedQosFlowList& m_associatedQosFlowList) const;

  bool encode(Ngap_QosFlowPerTNLInformation_t& qosFlowPerTNLInformation);
  bool decode(const Ngap_QosFlowPerTNLInformation_t& qosFlowPerTNLInformation);

 private:
  UpTransportLayerInformation uPTransportLayerInformation;  // Mandatory
  AssociatedQosFlowList associatedQosFlowList;              // Mandatory
};

}  // namespace ngap

#endif
