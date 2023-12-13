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

#include "RrcEstablishmentCause.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RrcEstablishmentCause::RrcEstablishmentCause() {
  cause_ = -1;
}

//------------------------------------------------------------------------------
RrcEstablishmentCause::~RrcEstablishmentCause() {}

//------------------------------------------------------------------------------
void RrcEstablishmentCause::set(const e_Ngap_RRCEstablishmentCause& cause) {
  cause_ = cause;
}

//------------------------------------------------------------------------------
int RrcEstablishmentCause::get() const {
  return cause_;
}

//------------------------------------------------------------------------------
bool RrcEstablishmentCause::encode(Ngap_RRCEstablishmentCause_t& cause) const {
  cause = cause_;
  return true;
}

//------------------------------------------------------------------------------
bool RrcEstablishmentCause::decode(Ngap_RRCEstablishmentCause_t cause) {
  cause_ = cause;

  return true;
}
}  // namespace ngap
