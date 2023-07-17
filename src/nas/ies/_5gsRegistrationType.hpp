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

#ifndef _5GS_REGISTRATION_TYPE_H_
#define _5GS_REGISTRATION_TYPE_H_

#include "Type1NasIeFormatTv.hpp"

constexpr auto k5gsRegistrationTypeName = "5GS Registration Type";

namespace nas {

class _5gsRegistrationType : public Type1NasIeFormatTv {
 public:
  _5gsRegistrationType();
  _5gsRegistrationType(const bool& follow_on_req, const uint8_t& type);
  _5gsRegistrationType(
      const uint8_t& iei, const bool& follow_on_req, const uint8_t& type);
  virtual ~_5gsRegistrationType();

  static std::string GetIeName() { return k5gsRegistrationTypeName; }

  void setValue();
  void getValue();

  bool validateValue(const bool& follow_on_req, const uint8_t& type);

  void set(const bool& follow_on_req, const uint8_t& type, const uint8_t& iei);
  void set(const bool& follow_on_req, const uint8_t& type);

  void setFollowOnReq(bool is);
  bool isFollowOnReq();

  void setRegType(uint8_t type);
  uint8_t getRegType();

 private:
  bool follow_on_req_;
  uint8_t reg_type_ : 3;
};

}  // namespace nas

#endif
