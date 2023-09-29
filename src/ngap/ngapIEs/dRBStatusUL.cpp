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

#include "dRBStatusUL.hpp"
#include "logger.hpp"

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
dRBStatusUL::dRBStatusUL() {}

//------------------------------------------------------------------------------
dRBStatusUL::~dRBStatusUL() {}

//------------------------------------------------------------------------------
void dRBStatusUL::setdRBStatusUL(const dRBStatusUL18& uL18) {
  ul18 = uL18;
}

//------------------------------------------------------------------------------
void dRBStatusUL::getdRBStatusUL(dRBStatusUL18& uL18) {
  uL18 = ul18;
}

//------------------------------------------------------------------------------
bool dRBStatusUL::encodedRBStatusUL(Ngap_DRBStatusUL_t* uL) {
  uL->present = Ngap_DRBStatusUL_PR_dRBStatusUL18;
  uL->choice.dRBStatusUL18 =
      (Ngap_DRBStatusUL18_t*) calloc(1, sizeof(Ngap_DRBStatusUL18_t));
  if (!ul18.encodeddRBStatusUL18(uL->choice.dRBStatusUL18)) {
    Logger::ngap().error("Encode DRBStatusUL18 IE error");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool dRBStatusUL::decodedRBStatusUL(Ngap_DRBStatusUL_t* uL) {
  if (!ul18.decodeddRBStatusUL18(uL->choice.dRBStatusUL18)) {
    Logger::ngap().error("Decode DRBStatusUL18 IE error");
    return false;
  }
  return true;
}
}  // namespace ngap
