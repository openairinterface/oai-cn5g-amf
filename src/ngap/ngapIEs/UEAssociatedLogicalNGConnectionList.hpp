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

#ifndef _UE_ASSOCIATED_LOGICAL_NG_CONNECTION_LIST_H_
#define _UE_ASSOCIATED_LOGICAL_NG_CONNECTION_LIST_H_

#include <vector>

#include "UEAssociatedLogicalNGConnectionItem.hpp"

extern "C" {
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_UE-associatedLogicalNG-connectionList.h"
}

namespace ngap {

class UEAssociatedLogicalNGConnectionList {
 public:
  UEAssociatedLogicalNGConnectionList();
  virtual ~UEAssociatedLogicalNGConnectionList();

  void set(std::vector<UEAssociatedLogicalNGConnectionItem>& list);
  void get(std::vector<UEAssociatedLogicalNGConnectionItem>& list);

  void addItem(UEAssociatedLogicalNGConnectionItem& item);

  bool encode(Ngap_UE_associatedLogicalNG_connectionList_t&
                  ue_associatedLogicalNG_connectionList);
  bool decode(const Ngap_UE_associatedLogicalNG_connectionList_t&
                  ue_associatedLogicalNG_connectionList);

 private:
  std::vector<UEAssociatedLogicalNGConnectionItem> list_;
};

}  // namespace ngap
#endif
