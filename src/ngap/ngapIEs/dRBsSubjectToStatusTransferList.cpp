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

#include "dRBsSubjectToStatusTransferList.hpp"
#include "logger.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

namespace ngap {
//------------------------------------------------------------------------------
dRBSubjectList::dRBSubjectList() {}

//------------------------------------------------------------------------------
dRBSubjectList::~dRBSubjectList() {}

//------------------------------------------------------------------------------
void dRBSubjectList::setdRBSubjectItem(
    const std::vector<dRBSubjectItem>& list) {
  itemList = list;
}

//------------------------------------------------------------------------------
void dRBSubjectList::getdRBSubjectItem(std::vector<dRBSubjectItem>& list) {
  list = itemList;
}

//------------------------------------------------------------------------------
bool dRBSubjectList::encodefromdRBSubjectlist(
    Ngap_DRBsSubjectToStatusTransferList_t& dRBsSubjectToStatusTransferList) {
  for (auto& item : itemList) {
    Ngap_DRBsSubjectToStatusTransferItem_t* ie =
        (Ngap_DRBsSubjectToStatusTransferItem_t*) calloc(
            1, sizeof(Ngap_DRBsSubjectToStatusTransferItem_t));
    if (!ie) return false;

    if (!item.encodedRBSubjectItem(ie)) {
      Logger::ngap().error("Encode dRBSubjectList IE error!");
      free_wrapper((void**) &ie);
      return false;
    }
    if (ASN_SEQUENCE_ADD(&dRBsSubjectToStatusTransferList.list, ie) != 0) {
      Logger::ngap().error("ASN_SEQUENCE_ADD dRBSubjectList IE error!");
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool dRBSubjectList::decodefromdRBSubjectlist(
    Ngap_DRBsSubjectToStatusTransferList_t& dRBsSubjectToStatusTransferList) {
  for (int i = 0; i < dRBsSubjectToStatusTransferList.list.count; i++) {
    dRBSubjectItem item = {};
    if (!item.decodefromdRBSubjectItem(
            dRBsSubjectToStatusTransferList.list.array[i])) {
      Logger::ngap().error("Decode dRBSubjectList IE error!");
      return false;
    }
    itemList.push_back(item);
  }
  return true;
}
}  // namespace ngap
