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

#include "EUtraCgi.hpp"

namespace ngap {

//------------------------------------------------------------------------------
EUtraCgi::EUtraCgi() {}

//------------------------------------------------------------------------------
EUtraCgi::~EUtraCgi() {}

//------------------------------------------------------------------------------
void EUtraCgi::set(
    const PlmnId& plmn_id, const EUtraCellIdentity& eUTRA_cell_identity) {
  plmn_id_             = plmn_id;
  eUTRA_cell_identity_ = eUTRA_cell_identity;
}

//------------------------------------------------------------------------------
void EUtraCgi::get(
    PlmnId& plmn_id, EUtraCellIdentity& eUTRA_cell_identity) const {
  plmn_id             = plmn_id_;
  eUTRA_cell_identity = eUTRA_cell_identity_;
}

//------------------------------------------------------------------------------
bool EUtraCgi::encode(Ngap_EUTRA_CGI_t& eutra_cgi) {
  if (!plmn_id_.encode(eutra_cgi.pLMNIdentity)) return false;
  if (!eUTRA_cell_identity_.encode(eutra_cgi.eUTRACellIdentity)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool EUtraCgi::decode(const Ngap_EUTRA_CGI_t& eutra_cgi) {
  if (!plmn_id_.decode(eutra_cgi.pLMNIdentity)) return false;
  if (!eUTRA_cell_identity_.decode(eutra_cgi.eUTRACellIdentity)) return false;
  return true;
}

}  // namespace ngap
