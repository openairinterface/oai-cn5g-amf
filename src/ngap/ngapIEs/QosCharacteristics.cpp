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

#include "QosCharacteristics.hpp"

namespace ngap {

//------------------------------------------------------------------------------
QosCharacteristics::QosCharacteristics() {
  nonDynamic5QIDescriptor = std::nullopt;
  dynamic5QIDescriptor    = std::nullopt;
}

//------------------------------------------------------------------------------
QosCharacteristics::~QosCharacteristics() {}

//------------------------------------------------------------------------------
void QosCharacteristics::setQosCharacteristics(
    const NonDynamic5QIDescriptor& m_nonDynamic5QIDescriptor) {
  nonDynamic5QIDescriptor =
      std::make_optional<NonDynamic5QIDescriptor>(m_nonDynamic5QIDescriptor);
  dynamic5QIDescriptor = std::nullopt;
}

//------------------------------------------------------------------------------
void QosCharacteristics::getQosCharacteristics(
    std::optional<NonDynamic5QIDescriptor>& m_nonDynamic5QIDescriptor) const {
  m_nonDynamic5QIDescriptor = nonDynamic5QIDescriptor;
}

//------------------------------------------------------------------------------
void QosCharacteristics::setQosCharacteristics(
    const Dynamic5QIDescriptor& m_dynamic5QIDescriptor) {
  dynamic5QIDescriptor =
      std::make_optional<Dynamic5QIDescriptor>(m_dynamic5QIDescriptor);
  nonDynamic5QIDescriptor = std::nullopt;
}

//------------------------------------------------------------------------------
void QosCharacteristics::getQosCharacteristics(
    std::optional<Dynamic5QIDescriptor>& m_dynamic5QIDescriptor) const {
  m_dynamic5QIDescriptor = dynamic5QIDescriptor;
}

//------------------------------------------------------------------------------
int QosCharacteristics::QosCharacteristicsPresent() {
  if (nonDynamic5QIDescriptor.has_value())
    return Ngap_QosCharacteristics_PR_nonDynamic5QI;
  else if (dynamic5QIDescriptor.has_value())
    return Ngap_QosCharacteristics_PR_dynamic5QI;
  else
    return Ngap_QosCharacteristics_PR_NOTHING;
}

//------------------------------------------------------------------------------
bool QosCharacteristics::encode(Ngap_QosCharacteristics_t& qosCharacteristics) {
  if (nonDynamic5QIDescriptor.has_value()) {
    qosCharacteristics.present = Ngap_QosCharacteristics_PR_nonDynamic5QI;
    Ngap_NonDynamic5QIDescriptor_t* non_dynamic_descriptor =
        (Ngap_NonDynamic5QIDescriptor_t*) calloc(
            1, sizeof(Ngap_NonDynamic5QIDescriptor_t));
    if (!non_dynamic_descriptor) return false;
    if (!nonDynamic5QIDescriptor.value().encode(*non_dynamic_descriptor))
      return false;
    qosCharacteristics.choice.nonDynamic5QI = non_dynamic_descriptor;
  } else if (dynamic5QIDescriptor.has_value()) {
    qosCharacteristics.present = Ngap_QosCharacteristics_PR_dynamic5QI;
    Ngap_Dynamic5QIDescriptor_t* dynamic_descriptor =
        (Ngap_Dynamic5QIDescriptor_t*) calloc(
            1, sizeof(Ngap_Dynamic5QIDescriptor_t));
    if (!dynamic_descriptor) return false;
    if (!dynamic5QIDescriptor.value().encode(*dynamic_descriptor)) return false;
    qosCharacteristics.choice.dynamic5QI = dynamic_descriptor;
  } else {
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool QosCharacteristics::decode(
    const Ngap_QosCharacteristics_t& qosCharacteristics) {
  if (qosCharacteristics.present == Ngap_QosCharacteristics_PR_nonDynamic5QI) {
    NonDynamic5QIDescriptor tmp = {};
    if (!tmp.decode(*qosCharacteristics.choice.nonDynamic5QI)) return false;
    nonDynamic5QIDescriptor = std::make_optional<NonDynamic5QIDescriptor>(tmp);
  } else if (
      qosCharacteristics.present == Ngap_QosCharacteristics_PR_dynamic5QI) {
    Dynamic5QIDescriptor tmp = {};
    if (!tmp.decode(*qosCharacteristics.choice.dynamic5QI)) return false;
    dynamic5QIDescriptor = std::make_optional<Dynamic5QIDescriptor>(tmp);
  } else {
    return false;
  }

  return true;
}
}  // namespace ngap
