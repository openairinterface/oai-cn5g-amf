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

#ifndef _EAP_MESSAGE_H_
#define _EAP_MESSAGE_H_

#include "Type6NasIe.hpp"

constexpr uint8_t kEapMessageMinimumLength  = 7;
constexpr uint16_t kEapMessageMaximumLength = 1503;
constexpr auto kEapMessageIeName            = "EAP Message";

namespace nas {

class EapMessage : public Type6NasIe {
 public:
  EapMessage();
  EapMessage(uint8_t iei);
  EapMessage(const bstring& eap);
  EapMessage(uint8_t iei, const bstring& eap);
  ~EapMessage();

  static std::string GetIeName() { return kEapMessageIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void SetValue(const bstring& eap);
  void GetValue(bstring& eap) const;

 private:
  bstring eap_;
};

}  // namespace nas

#endif
