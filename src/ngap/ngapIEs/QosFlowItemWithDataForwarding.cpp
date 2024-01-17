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

#include "QosFlowItemWithDataForwarding.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosFlowItemWithDataForWarding::QosFlowItemWithDataForWarding() {
  data_forwarding_accepted_ = std::nullopt;
}

//------------------------------------------------------------------------------
QosFlowItemWithDataForWarding::~QosFlowItemWithDataForWarding() {}

void QosFlowItemWithDataForWarding::set(
    const QosFlowIdentifier& qfi,
    const std::optional<long>& data_forwarding_accepted) {
  qfi_                      = qfi;
  data_forwarding_accepted_ = data_forwarding_accepted;
}
//------------------------------------------------------------------------------
void QosFlowItemWithDataForWarding::getQosFlowIdentifier(
    Ngap_QosFlowIdentifier_t& qfi) {
  long value = {};
  if (qfi_.getQosFlowIdentifier(value)) {
    qfi = (Ngap_QosFlowIdentifier_t) value;
  }
}

//------------------------------------------------------------------------------
bool QosFlowItemWithDataForWarding::decode(
    const Ngap_QosFlowItemWithDataForwarding_t& qos_flow_item) {
  if (!qfi_.decode(qos_flow_item.qosFlowIdentifier)) {
    return false;
  }
  if (qos_flow_item.dataForwardingAccepted)
    data_forwarding_accepted_ =
        std::make_optional<long>(*qos_flow_item.dataForwardingAccepted);
  return true;
}
}  // namespace ngap
