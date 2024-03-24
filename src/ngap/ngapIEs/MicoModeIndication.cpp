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

#include "MicoModeIndication.hpp"

namespace ngap {

//------------------------------------------------------------------------------
MicoModeIndication::MicoModeIndication()
    : m_MicoModeIndication(Ngap_MICOModeIndication_true) {}

//------------------------------------------------------------------------------
MicoModeIndication::~MicoModeIndication() {}

//------------------------------------------------------------------------------
void MicoModeIndication::set(const long& micoModeIndication) {
  m_MicoModeIndication = micoModeIndication;
}

//------------------------------------------------------------------------------
void MicoModeIndication::get(long& micoModeIndication) const {
  micoModeIndication = m_MicoModeIndication;
}

//------------------------------------------------------------------------------
bool MicoModeIndication::encode(
    Ngap_MICOModeIndication_t& micoModeIndication) const {
  if (!micoModeIndication) return false;
  micoModeIndication = m_MicoModeIndication;

  return true;
}

//------------------------------------------------------------------------------
bool MicoModeIndication::decode(
    const Ngap_MICOModeIndication_t& micoModeIndication) {
  if (!micoModeIndication) return false;
  m_MicoModeIndication = micoModeIndication;

  return true;
}

}  // namespace ngap
