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

#ifndef _UE_ASSOCIATED_LOGICAL_NG_CONNECTION_ITEM_H_
#define _UE_ASSOCIATED_LOGICAL_NG_CONNECTION_ITEM_H_

#include <optional>

#include "AmfUeNgapId.hpp"
#include "RanUeNgapId.hpp"

extern "C" {
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_UE-associatedLogicalNG-connectionItem.h"
}

namespace ngap {

class UeAssociatedLogicalNgConnectionItem {
 public:
  UeAssociatedLogicalNgConnectionItem();
  virtual ~UeAssociatedLogicalNgConnectionItem(){};

  bool setAmfUeNgapId(const uint64_t& id);
  bool getAmfUeNgapId(uint64_t& id) const;
  void setRanUeNgapId(const uint32_t& id);
  bool getRanUeNgapId(uint32_t& id) const;

  void get(UeAssociatedLogicalNgConnectionItem& item) const;

  bool encode(Ngap_UE_associatedLogicalNG_connectionItem_t& item) const;
  bool decode(const Ngap_UE_associatedLogicalNG_connectionItem_t& item);

 private:
  std::optional<AmfUeNgapId> m_AmfUeNgapId;  // Optional
  std::optional<RanUeNgapId> m_RanUeNgapId;  // Optional
};

}  // namespace ngap
#endif
