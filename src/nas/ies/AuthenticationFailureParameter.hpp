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

#ifndef _AUTHENTICATION_FAILURE_PARAMETER_H_
#define _AUTHENTICATION_FAILURE_PARAMETER_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kAuthenticationFailureParameterLength = 16;
constexpr uint8_t kAuthenticationFailureParameterContentLength =
    kAuthenticationFailureParameterLength - 2;
constexpr auto kAuthenticationFailureParameterIeName =
    "Authentication Failure Parameter";

namespace nas {

class AuthenticationFailureParameter : public Type4NasIe {
 public:
  AuthenticationFailureParameter();
  AuthenticationFailureParameter(const bstring& value);
  ~AuthenticationFailureParameter();

  static std::string GetIeName() {
    return kAuthenticationFailureParameterIeName;
  }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  // void SetValue(const uint8_t
  // (&value)[kAuthenticationFailureParameterContentLength]); void
  // GetValue(uint8_t (&value)[kAuthenticationFailureParameterContentLength])
  // const;
  void SetValue(const bstring& value);
  void GetValue(bstring& value) const;

 private:
  // uint8_t value_[kAuthenticationFailureParameterContentLength];
  bstring value_;
};

}  // namespace nas

#endif
