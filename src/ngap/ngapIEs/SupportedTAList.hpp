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

#ifndef _SUPPORTED_TA_LIST_H_
#define _SUPPORTED_TA_LIST_H_

#include <vector>

#include "SupportedTaItem.hpp"

extern "C" {
#include "Ngap_SupportedTAList.h"
}

namespace ngap {

class SupportedTAList {
 public:
  SupportedTAList();
  virtual ~SupportedTAList();

  void setSupportedTaItems(const std::vector<SupportedTaItem>& items);
  void getSupportedTaItems(std::vector<SupportedTaItem>& items);

  bool encode(Ngap_SupportedTAList_t& ngSetupRequest);
  bool decode(const Ngap_SupportedTAList_t& pdu);

 private:
  std::vector<SupportedTaItem> supportedTAItems;
};
}  // namespace ngap

#endif
