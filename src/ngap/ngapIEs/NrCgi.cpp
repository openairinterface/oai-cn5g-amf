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
void NrCgi::set(
    const PlmnId& m_plmnId, const NRCellIdentity& m_nRCellIdentity) {
  plmnId         = m_plmnId;
  nRCellIdentity = m_nRCellIdentity;
}

//------------------------------------------------------------------------------
void NrCgi::set(
    const std::string& mcc, const std::string& mnc,
    const unsigned long& nrcellidentity) {
  plmnId.set(mcc, mnc);
  nRCellIdentity.setNRCellIdentity(nrcellidentity);
}

//------------------------------------------------------------------------------
void NrCgi::set(const struct NrCgi_s& cig) {
  plmnId.set(cig.mcc, cig.mnc);
  nRCellIdentity.setNRCellIdentity(cig.nrCellID);
}

//------------------------------------------------------------------------------
void NrCgi::get(struct NrCgi_s& cig) const {
  plmnId.getMcc(cig.mcc);
  plmnId.getMnc(cig.mnc);
  cig.nrCellID = nRCellIdentity.getNRCellIdentity();
}

//------------------------------------------------------------------------------
bool NrCgi::encode(Ngap_NR_CGI_t& nr_cgi) {
  if (!plmnId.encode(nr_cgi.pLMNIdentity)) return false;
  if (!nRCellIdentity.encode(nr_cgi.nRCellIdentity)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool NrCgi::decode(const Ngap_NR_CGI_t& nr_cgi) {
  if (!plmnId.decode(nr_cgi.pLMNIdentity)) return false;
  if (!nRCellIdentity.decode(nr_cgi.nRCellIdentity)) return false;
  return true;
}

//------------------------------------------------------------------------------
void NrCgi::get(PlmnId& m_plmnId, NRCellIdentity& m_nRCellIdentity) const {
  m_plmnId         = plmnId;
  m_nRCellIdentity = nRCellIdentity;
}
}  // namespace ngap
