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

#ifndef _5GS_DEREGISTRATION_TYPE_H_
#define _5GS_DEREGISTRATION_TYPE_H_

#include "Type1NasIeFormatTv.hpp"

namespace nas {
typedef struct _5gs_deregistration_type_s {
  uint8_t iei : 4;
  uint8_t switch_off : 1;
  uint8_t re_registration_required : 1;
  uint8_t access_type : 2;
} _5gs_deregistration_type_t;

constexpr auto k5gsDeregistrationTypeIeName = "5GS De-registration Type";

class _5gsDeregistrationType : public Type1NasIeFormatTv {
 public:
  _5gsDeregistrationType();
  _5gsDeregistrationType(uint8_t iei);
  _5gsDeregistrationType(uint8_t iei, uint8_t value);
  _5gsDeregistrationType(_5gs_deregistration_type_t type);
  ~_5gsDeregistrationType();

  static std::string GetIeName() { return k5gsDeregistrationTypeIeName; }

  // int Decode(uint8_t* buf, int len);
  // int Encode(uint8_t* buf, int len);

  void set(_5gs_deregistration_type_t type);
  void get(_5gs_deregistration_type_t& type) const;

  void set(uint8_t value);
  void get(uint8_t& value) const;

  void set(uint8_t iei, uint8_t value);
  void get(uint8_t& iei, uint8_t value) const;

 private:
  void setValue();
  void getValue();

  union {
    struct {
      uint8_t iei : 4;
      uint8_t switch_off : 1;
      uint8_t re_registration_required : 1;
      uint8_t access_type : 2;
    } bf;
    uint8_t b;
  } u1;
};

}  // namespace nas

#endif
