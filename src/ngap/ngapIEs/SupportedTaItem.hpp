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

#ifndef _SUPPORTED_TA_ITEM_H
#define _SUPPORTED_TA_ITEM_H

#include <vector>

#include "BroadcastPlmnItem.hpp"
#include "Tac.hpp"

extern "C" {
#include "Ngap_SupportedTAItem.h"
}

namespace ngap {

class SupportedTaItem {
 public:
  SupportedTaItem();
  virtual ~SupportedTaItem();

  void setTac(const TAC& m_tac);
  void getTac(TAC& m_tac);

  void setBroadcastPlmnList(const std::vector<BroadcastPlmnItem>& list);
  void getBroadcastPlmnList(std::vector<BroadcastPlmnItem>& list);

  bool encode(Ngap_SupportedTAItem_t& ta);
  bool decode(const Ngap_SupportedTAItem_t& ta);

 private:
  TAC tac;                                           // Mandatory
  std::vector<BroadcastPlmnItem> broadcastPLMNList;  // Mandatory
};

}  // namespace ngap

#endif
