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

#ifndef _ASSOCIATED_QOS_FLOW_ITEM_H_
#define _ASSOCIATED_QOS_FLOW_ITEM_H_

#include "QosFlowIdentifier.hpp"

extern "C" {
#include "Ngap_AssociatedQosFlowItem.h"
}

namespace ngap {

class AssociatedQosFlowItem {
 public:
  AssociatedQosFlowItem();
  virtual ~AssociatedQosFlowItem();

  void setAssociatedQosFlowItem(
      const e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication&
          m_qosFlowMappingIndication,
      const QosFlowIdentifier& m_qosFlowIdentifier);
  bool getAssociatedQosFlowItem(
      long& m_qosFlowMappingIndication,
      QosFlowIdentifier& m_qosFlowIdentifier) const;

  void setAssociatedQosFlowItem(const QosFlowIdentifier& m_qosFlowIdentifier);
  // TODO: Get

  bool encode(Ngap_AssociatedQosFlowItem_t& associatedQosFlowItem);
  bool decode(Ngap_AssociatedQosFlowItem_t& associatedQosFlowItem);

 private:
  QosFlowIdentifier qosFlowIdentifier;  // Mandatory
  long qosFlowMappingIndication;        // TODO: Optional
};

}  // namespace ngap

#endif
