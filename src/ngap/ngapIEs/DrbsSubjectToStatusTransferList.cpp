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

#include "DrbsSubjectToStatusTransferList.hpp"

#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {
//------------------------------------------------------------------------------
DrbSubjectToStatusTransferList::DrbSubjectToStatusTransferList() {}

//------------------------------------------------------------------------------
DrbSubjectToStatusTransferList::~DrbSubjectToStatusTransferList() {}

//------------------------------------------------------------------------------
void DrbSubjectToStatusTransferList::setdRBSubjectItem(
    const std::vector<DrbSubjectToStatusTransferItem>& list) {
  itemList = list;
}

//------------------------------------------------------------------------------
void DrbSubjectToStatusTransferList::getdRBSubjectItem(
    std::vector<DrbSubjectToStatusTransferItem>& list) const {
  list = itemList;
}

//------------------------------------------------------------------------------
bool DrbSubjectToStatusTransferList::encode(
    Ngap_DRBsSubjectToStatusTransferList_t& dRBsSubjectToStatusTransferList) {
  for (auto& item : itemList) {
    Ngap_DRBsSubjectToStatusTransferItem_t* ie =
        (Ngap_DRBsSubjectToStatusTransferItem_t*) calloc(
            1, sizeof(Ngap_DRBsSubjectToStatusTransferItem_t));
    if (!ie) return false;

    if (!item.encode(*ie)) {
      Logger::ngap().error("Encode DrbSubjectToStatusTransferList IE error!");
      free_wrapper((void**) &ie);
      return false;
    }
    if (ASN_SEQUENCE_ADD(&dRBsSubjectToStatusTransferList.list, ie) != 0) {
      Logger::ngap().error(
          "ASN_SEQUENCE_ADD DrbSubjectToStatusTransferList IE error!");
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool DrbSubjectToStatusTransferList::decode(
    const Ngap_DRBsSubjectToStatusTransferList_t&
        dRBsSubjectToStatusTransferList) {
  for (int i = 0; i < dRBsSubjectToStatusTransferList.list.count; i++) {
    DrbSubjectToStatusTransferItem item = {};
    if (!item.decode(*dRBsSubjectToStatusTransferList.list.array[i])) {
      Logger::ngap().error("Decode DrbSubjectToStatusTransferList IE error!");
      return false;
    }
    itemList.push_back(item);
  }
  return true;
}
}  // namespace ngap
