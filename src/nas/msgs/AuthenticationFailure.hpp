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

#ifndef _AUTHENTICATION_FAILURE_H_
#define _AUTHENTICATION_FAILURE_H_

#include "NasIeHeader.hpp"

namespace nas {

class AuthenticationFailure : public NasMmPlainHeader {
 public:
  AuthenticationFailure();
  ~AuthenticationFailure();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetHeader(uint8_t security_header_type);

  void Set5gmmCause(uint8_t value);
  uint8_t Get5gmmCause() const;

  //  void SetAuthenticationFailureParameter(const uint8_t
  //  (&value)[kAuthenticationFailureParameterContentLength]); bool
  //  GetAuthenticationFailureParameter(uint8_t
  //  (&value)[kAuthenticationFailureParameterContentLength]) const;

  void SetAuthenticationFailureParameter(const bstring& value);
  bool GetAuthenticationFailureParameter(bstring& value) const;

 public:
  _5gmmCause ie_5gmm_cause;  // Mandatory
  std::optional<AuthenticationFailureParameter>
      ie_authentication_failure_parameter;  // Optional
};

}  // namespace nas

#endif
