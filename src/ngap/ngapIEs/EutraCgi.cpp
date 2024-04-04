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

#include "EutraCgi.hpp"

namespace ngap {

//------------------------------------------------------------------------------
EutraCgi::EutraCgi() {}

//------------------------------------------------------------------------------
EutraCgi::~EutraCgi() {}

//------------------------------------------------------------------------------
void EutraCgi::set(
    const PlmnId& plmnId, const EutraCellIdentity& eutraCellIdentity) {
  m_PlmnId            = plmnId;
  m_eutraCellIdentity = eutraCellIdentity;
}

//------------------------------------------------------------------------------
void EutraCgi::get(PlmnId& plmnId, EutraCellIdentity& eutraCellIdentity) const {
  plmnId            = m_PlmnId;
  eutraCellIdentity = m_eutraCellIdentity;
}

//------------------------------------------------------------------------------
bool EutraCgi::encode(Ngap_EUTRA_CGI_t& eutraCgi) const {
  if (!m_PlmnId.encode(eutraCgi.pLMNIdentity)) return false;
  if (!m_eutraCellIdentity.encode(eutraCgi.eUTRACellIdentity)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool EutraCgi::decode(const Ngap_EUTRA_CGI_t& eutraCgi) {
  if (!m_PlmnId.decode(eutraCgi.pLMNIdentity)) return false;
  if (!m_eutraCellIdentity.decode(eutraCgi.eUTRACellIdentity)) return false;
  return true;
}

}  // namespace ngap
