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

#ifndef _PLMN_SUPPORT_LIST_H_
#define _PLMN_SUPPORT_LIST_H_

#include <vector>

#include "PlmnSupportItem.hpp"

extern "C" {
#include "Ngap_PLMNSupportList.h"
}

namespace ngap {

class PlmnSupportList {
 public:
  PlmnSupportList();
  virtual ~PlmnSupportList();

  void set(const std::vector<PlmnSupportItem>& items);
  void get(std::vector<PlmnSupportItem>& items);

  void addItem(const PlmnSupportItem& item);

  bool encode(Ngap_PLMNSupportList_t&);
  bool decode(const Ngap_PLMNSupportList_t&);

 private:
  std::vector<PlmnSupportItem> list_;
};

}  // namespace ngap

#endif
