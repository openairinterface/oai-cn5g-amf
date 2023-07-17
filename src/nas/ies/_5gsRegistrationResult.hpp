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

#ifndef _5GS_REGISTRATION_RESULT_H_
#define _5GS_REGISTRATION_RESULT_H_

#include "Type4NasIe.hpp"

constexpr uint8_t k5gsRegistrationResultLength        = 3;
constexpr uint8_t k5gsRegistrationResultContentLength = 1;
constexpr auto k5gsRegistrationResultIeName = "5GS Registration Result";

namespace nas {

class _5gsRegistrationResult : public Type4NasIe {
 public:
  _5gsRegistrationResult();
  _5gsRegistrationResult(uint8_t iei);
  _5gsRegistrationResult(bool emergency, bool nssaa, bool sms, uint8_t value);
  _5gsRegistrationResult(
      uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value);
  ~_5gsRegistrationResult();

  static std::string GetIeName() { return k5gsRegistrationResultIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void setValue(uint8_t value);
  uint8_t getValue() const;

  void set(uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value);
  void set(bool emergency, bool nssaa, bool sms, uint8_t value);

 private:
  bool emergency_registered_;
  bool nssaa_performed_;
  bool sms_allowed_;
  uint8_t value_;
};
}  // namespace nas

#endif
