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

#ifndef _QOS_FLOW_PER_TNL_INFORMATION_ITEM_H_
#define _QOS_FLOW_PER_TNL_INFORMATION_ITEM_H_

#include "QosFlowPerTnlInformation.hpp"

extern "C" {
#include "Ngap_QosFlowPerTNLInformationItem.h"
}

namespace ngap {

class QosFlowPerTnlInformationItem {
 public:
  QosFlowPerTnlInformationItem();
  virtual ~QosFlowPerTnlInformationItem();

  void set(const QosFlowPerTnlInformation& qosFlowPerTnlInformation);
  void get(QosFlowPerTnlInformation& qosFlowPerTnlInformation) const;

  bool encode(
      Ngap_QosFlowPerTNLInformationItem_t& qosFlowPerTnlInformationItem) const;
  bool decode(
      const Ngap_QosFlowPerTNLInformationItem_t& qosFlowPerTnlInformationItem);

 private:
  QosFlowPerTnlInformation m_QosFlowPerTnlInformation;  // Mandatory
  // TODO: Ngap_ProtocolExtensionContainer
};

}  // namespace ngap

#endif
