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

#ifndef _LADN_Indication_H_
#define _LADN_Indication_H_

#include "Type6NasIe.hpp"

constexpr uint8_t kLadnIndicationMinimumLength  = 3;
constexpr uint16_t kLadnIndicationMaximumLength = 811;
constexpr auto kLadnIndicationIeName            = "LADN Indication";

namespace nas {

class LadnIndication : Type6NasIe {
 public:
  LadnIndication();
  LadnIndication(const std::vector<bstring>& ladn);
  ~LadnIndication();

  static std::string GetIeName() { return kLadnIndicationIeName; }

  // void SetValue(const std::vector<bstring>& ladn);
  void GetValue(std::vector<bstring>& ladn) const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_option);

 private:
  std::vector<bstring> LADN;
};

}  // namespace nas

#endif
