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

#ifndef _AUTHENTICATION_RESPONSE_PARAMETER_H_
#define _AUTHENTICATION_RESPONSE_PARAMETER_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kAuthenticationResponseParameterMinimumLength = 6;
constexpr uint8_t kAuthenticationResponseParameterMaximumLength = 18;
constexpr auto kAuthenticationResponseParameterIeName =
    "Authentication Response Parameter";

namespace nas {

class AuthenticationResponseParameter : public Type4NasIe {
 public:
  AuthenticationResponseParameter();
  AuthenticationResponseParameter(bstring para);
  ~AuthenticationResponseParameter();

  static std::string GetIeName() {
    return kAuthenticationResponseParameterIeName;
  }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void SetValue(const bstring& para);
  void GetValue(bstring& para) const;

 private:
  bstring res_or_res_star_;
};

}  // namespace nas

#endif
