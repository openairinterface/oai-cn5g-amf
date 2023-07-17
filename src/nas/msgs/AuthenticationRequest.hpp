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

#ifndef _AUTHENTICATION_REQUEST_H_
#define _AUTHENTICATION_REQUEST_H_

#include "NasIeHeader.hpp"

namespace nas {

class AuthenticationRequest : public NasMmPlainHeader {
 public:
  AuthenticationRequest();
  ~AuthenticationRequest();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetHeader(uint8_t security_header_type);

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  // TODO: Get

  void SetEapMessage(const bstring& eap);
  // TODO: Get

  void SetAbba(uint8_t length, uint8_t* value);
  // TODO: Get

  void SetAuthenticationParameterRand(
      uint8_t value[kAuthenticationParameterRandValueLength]);
  // TODO: Get

  void SetAuthenticationParameterAutn(
      uint8_t value[kAuthenticationParameterAutnValueLength]);
  // TODO: Get

 public:
  NasKeySetIdentifier ie_ng_ksi;  // Mandatory
  // Spare half octet (will be processed together with NgKSI)
  Abba ie_abba;  // Mandatory

  std::optional<AuthenticationParameterRand>
      ie_authentication_parameter_rand;  // Optional
  std::optional<AuthenticationParameterAutn>
      ie_authentication_parameter_autn;      // Optional
  std::optional<EapMessage> ie_eap_message;  // Optional
};

}  // namespace nas

#endif
