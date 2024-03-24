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
  m_NonDynamic5qiDescriptor = std::nullopt;
  m_Dynamic5qiDescriptor    = std::nullopt;
}

//------------------------------------------------------------------------------
QosCharacteristics::~QosCharacteristics() {}

//------------------------------------------------------------------------------
void QosCharacteristics::set(
    const NonDynamic5qiDescriptor& nonDynamic5qiDescriptor) {
  m_NonDynamic5qiDescriptor =
      std::make_optional<NonDynamic5qiDescriptor>(nonDynamic5qiDescriptor);
  m_Dynamic5qiDescriptor = std::nullopt;
}

//------------------------------------------------------------------------------
void QosCharacteristics::get(
    std::optional<NonDynamic5qiDescriptor>& nonDynamic5qiDescriptor) const {
  nonDynamic5qiDescriptor = m_NonDynamic5qiDescriptor;
}

//------------------------------------------------------------------------------
void QosCharacteristics::set(const Dynamic5qiDescriptor& dynamic5qiDescriptor) {
  m_Dynamic5qiDescriptor =
      std::make_optional<Dynamic5qiDescriptor>(dynamic5qiDescriptor);
  m_NonDynamic5qiDescriptor = std::nullopt;
}

//------------------------------------------------------------------------------
void QosCharacteristics::get(
    std::optional<Dynamic5qiDescriptor>& dynamic5qiDescriptor) const {
  dynamic5qiDescriptor = m_Dynamic5qiDescriptor;
}

//------------------------------------------------------------------------------
int QosCharacteristics::QosCharacteristicsPresent() {
  if (m_NonDynamic5qiDescriptor.has_value())
    return Ngap_QosCharacteristics_PR_nonDynamic5QI;
  else if (m_Dynamic5qiDescriptor.has_value())
    return Ngap_QosCharacteristics_PR_dynamic5QI;
  else
    return Ngap_QosCharacteristics_PR_NOTHING;
}

//------------------------------------------------------------------------------
bool QosCharacteristics::encode(
    Ngap_QosCharacteristics_t& qosCharacteristics) const {
  if (m_NonDynamic5qiDescriptor.has_value()) {
    qosCharacteristics.present = Ngap_QosCharacteristics_PR_nonDynamic5QI;
    Ngap_NonDynamic5QIDescriptor_t* nonDynamic5qiDescriptor =
        (Ngap_NonDynamic5QIDescriptor_t*) calloc(
            1, sizeof(Ngap_NonDynamic5QIDescriptor_t));
    if (!nonDynamic5qiDescriptor) return false;
    if (!m_NonDynamic5qiDescriptor.value().encode(*nonDynamic5qiDescriptor))
      return false;
    qosCharacteristics.choice.nonDynamic5QI = nonDynamic5qiDescriptor;
  } else if (m_Dynamic5qiDescriptor.has_value()) {
    qosCharacteristics.present = Ngap_QosCharacteristics_PR_dynamic5QI;
    Ngap_Dynamic5QIDescriptor_t* dynamic5qiDescriptor =
        (Ngap_Dynamic5QIDescriptor_t*) calloc(
            1, sizeof(Ngap_Dynamic5QIDescriptor_t));
    if (!dynamic5qiDescriptor) return false;
    if (!m_Dynamic5qiDescriptor.value().encode(*dynamic5qiDescriptor))
      return false;
    qosCharacteristics.choice.dynamic5QI = dynamic5qiDescriptor;
  } else {
    return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool QosCharacteristics::decode(
    const Ngap_QosCharacteristics_t& qosCharacteristics) {
  if (qosCharacteristics.present == Ngap_QosCharacteristics_PR_nonDynamic5QI) {
    NonDynamic5qiDescriptor tmp = {};
    if (!tmp.decode(*qosCharacteristics.choice.nonDynamic5QI)) return false;
    m_NonDynamic5qiDescriptor =
        std::make_optional<NonDynamic5qiDescriptor>(tmp);
  } else if (
      qosCharacteristics.present == Ngap_QosCharacteristics_PR_dynamic5QI) {
    Dynamic5qiDescriptor tmp = {};
    if (!tmp.decode(*qosCharacteristics.choice.dynamic5QI)) return false;
    m_Dynamic5qiDescriptor = std::make_optional<Dynamic5qiDescriptor>(tmp);
  } else {
    return false;
  }

  return true;
}
}  // namespace ngap
