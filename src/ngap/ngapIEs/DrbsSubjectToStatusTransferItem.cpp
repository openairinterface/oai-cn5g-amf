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

#include "DrbsSubjectToStatusTransferItem.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DrbSubjectToStatusTransferItem::DrbSubjectToStatusTransferItem() {}

//------------------------------------------------------------------------------
DrbSubjectToStatusTransferItem::~DrbSubjectToStatusTransferItem() {}

//------------------------------------------------------------------------------
void DrbSubjectToStatusTransferItem::set(
    const Ngap_DRB_ID_t& drbId, const DrbStatusUl& drbUl,
    const DrbStatusDl& drbDl) {
  m_DrbId = drbId;
  m_DrbUl = drbUl;
  m_DrbDl = drbDl;
}

//------------------------------------------------------------------------------
void DrbSubjectToStatusTransferItem::get(
    Ngap_DRB_ID_t& drbId, DrbStatusUl& drbUl, DrbStatusDl& drbDl) const {
  drbId = m_DrbId;
  drbUl = m_DrbUl;
  drbDl = m_DrbDl;
}

//------------------------------------------------------------------------------
bool DrbSubjectToStatusTransferItem::decode(
    const Ngap_DRBsSubjectToStatusTransferItem_t& drbItem) {
  if (drbItem.dRB_ID) {
    m_DrbId = drbItem.dRB_ID;
  }
  if (!m_DrbUl.decode(drbItem.dRBStatusUL)) {
    return false;
  }
  if (!m_DrbDl.decode(drbItem.dRBStatusDL)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool DrbSubjectToStatusTransferItem::encode(
    Ngap_DRBsSubjectToStatusTransferItem_t& drbItem) const {
  drbItem.dRB_ID = m_DrbId;

  if (!m_DrbUl.encode(drbItem.dRBStatusUL)) {
    return false;
  }

  if (!m_DrbDl.encode(drbItem.dRBStatusDL)) {
    return false;
  }

  Logger::ngap().debug(
      "Encode from DrbSubjectToStatusTransferItem successfully");
  return true;
}
}  // namespace ngap
