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
    : mico_mode_indication_(Ngap_MICOModeIndication_true) {}

//------------------------------------------------------------------------------
MicoModeIndication::~MicoModeIndication() {}

//------------------------------------------------------------------------------
void MicoModeIndication::set(const long& mico_mode_indication) {
  mico_mode_indication_ = mico_mode_indication;
}

//------------------------------------------------------------------------------
void MicoModeIndication::get(long& mico_mode_indication) const {
  mico_mode_indication = mico_mode_indication_;
}

//------------------------------------------------------------------------------
bool MicoModeIndication::encode(
    Ngap_MICOModeIndication_t& mico_mode_indication) const {
  if (!mico_mode_indication) return false;
  mico_mode_indication = mico_mode_indication_;

  return true;
}

//------------------------------------------------------------------------------
bool MicoModeIndication::decode(
    Ngap_MICOModeIndication_t mico_mode_indication) {
  if (!mico_mode_indication) return false;
  mico_mode_indication_ = mico_mode_indication;

  return true;
}

}  // namespace ngap
