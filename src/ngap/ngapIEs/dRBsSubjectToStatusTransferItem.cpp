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

#include "dRBsSubjectToStatusTransferItem.hpp"
#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
dRBSubjectItem::dRBSubjectItem() {}

//------------------------------------------------------------------------------
dRBSubjectItem::~dRBSubjectItem() {}

//------------------------------------------------------------------------------
void dRBSubjectItem::setdRBSubjectItem(
    const Ngap_DRB_ID_t& dRB_ID, const dRBStatusUL& dRB_UL,
    const dRBStatusDL& dRB_DL) {
  drbID = dRB_ID;
  drbUL = dRB_UL;
  drbDL = dRB_DL;
}

//------------------------------------------------------------------------------
void dRBSubjectItem::getdRBSubjectItem(
    Ngap_DRB_ID_t& dRB_ID, dRBStatusUL& dRB_UL, dRBStatusDL& dRB_DL) {
  dRB_ID = drbID;
  dRB_UL = drbUL;
  dRB_DL = drbDL;
}

//------------------------------------------------------------------------------
bool dRBSubjectItem::decodefromdRBSubjectItem(
    Ngap_DRBsSubjectToStatusTransferItem_t* dRB_item) {
  if (dRB_item->dRB_ID) {
    drbID = dRB_item->dRB_ID;
  }
  if (!drbUL.decodedRBStatusUL(&dRB_item->dRBStatusUL)) {
    return false;
  }
  if (!drbDL.decodedRBStatusDL(&dRB_item->dRBStatusDL)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool dRBSubjectItem::encodedRBSubjectItem(
    Ngap_DRBsSubjectToStatusTransferItem_t* dRB_item) {
  dRB_item->dRB_ID = drbID;

  if (!drbUL.encodedRBStatusUL(&dRB_item->dRBStatusUL)) {
    return false;
  }

  if (!drbDL.encodedRBStatusDL(&dRB_item->dRBStatusDL)) {
    return false;
  }

  Logger::ngap().debug("Encode from dRBSubjectItem successfully");
  return true;
}
}  // namespace ngap
