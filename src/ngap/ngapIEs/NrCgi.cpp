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

#include "NrCgi.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NrCgi::NrCgi() {}

//------------------------------------------------------------------------------
NrCgi::~NrCgi() {}

//------------------------------------------------------------------------------
void NrCgi::set(const PlmnId& plmnId, const NrCellIdentity& nrCellIdentity) {
  m_PlmnId         = plmnId;
  m_NrCellIdentity = nrCellIdentity;
}

//------------------------------------------------------------------------------
void NrCgi::set(
    const std::string& mcc, const std::string& mnc,
    const unsigned long& nrCellIdentity) {
  m_PlmnId.set(mcc, mnc);
  m_NrCellIdentity.set(nrCellIdentity);
}

//------------------------------------------------------------------------------
void NrCgi::set(const struct NrCgi_s& cig) {
  m_PlmnId.set(cig.mcc, cig.mnc);
  m_NrCellIdentity.set(cig.nrCellId);
}

//------------------------------------------------------------------------------
void NrCgi::get(struct NrCgi_s& cig) const {
  m_PlmnId.getMcc(cig.mcc);
  m_PlmnId.getMnc(cig.mnc);
  cig.nrCellId = m_NrCellIdentity.get();
}

//------------------------------------------------------------------------------
bool NrCgi::encode(Ngap_NR_CGI_t& nrCgi) const {
  if (!m_PlmnId.encode(nrCgi.pLMNIdentity)) return false;
  if (!m_NrCellIdentity.encode(nrCgi.nRCellIdentity)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool NrCgi::decode(const Ngap_NR_CGI_t& nrCgi) {
  if (!m_PlmnId.decode(nrCgi.pLMNIdentity)) return false;
  if (!m_NrCellIdentity.decode(nrCgi.nRCellIdentity)) return false;
  return true;
}

//------------------------------------------------------------------------------
void NrCgi::get(PlmnId& plmnId, NrCellIdentity& nrCellIdentity) const {
  plmnId         = m_PlmnId;
  nrCellIdentity = m_NrCellIdentity;
}
}  // namespace ngap
