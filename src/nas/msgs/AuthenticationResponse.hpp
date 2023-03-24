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

#ifndef _AUTHENTICATION_RESPONSE_H_
#define _AUTHENTICATION_RESPONSE_H_

#include "NasIeHeader.hpp"

namespace nas {

class AuthenticationResponse : public NasMmPlainHeader {
 public:
  AuthenticationResponse();
  ~AuthenticationResponse();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetHeader(uint8_t security_header_type);

  void SetAuthenticationResponseParameter(const bstring& para);
  bool GetAuthenticationResponseParameter(bstring& para) const;

  void SetEapMessage(const bstring& eap);
  bool GetEapMessage(bstring& eap) const;

 public:
  std::optional<AuthenticationResponseParameter>
      ie_authentication_response_parameter;  // Optional
  std::optional<EapMessage> ie_eap_message;  // Optional
};

}  // namespace nas

#endif
