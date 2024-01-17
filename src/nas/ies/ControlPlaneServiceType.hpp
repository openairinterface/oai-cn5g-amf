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

#ifndef _CONTROL_PLANE_SERVICE_TYPE_H_
#define _CONTROL_PLANE_SERVICE_TYPE_H_

#include "Type1NasIe.hpp"

constexpr auto kControlPlaneServiceTypeIeName = "Control Plane Service Type";

namespace nas {

class ControlPlaneServiceType : public Type1NasIe {
 public:
  ControlPlaneServiceType();
  ControlPlaneServiceType(uint8_t value);
  ~ControlPlaneServiceType();

  static std::string GetIeName() { return kControlPlaneServiceTypeIeName; }

  // int Encode(uint8_t* buf, int len);
  // int Decode(uint8_t* nuf, int len, bool is_iei, bool is_high);

  void SetValue(uint8_t value);
  void GetValue(uint8_t& value) const;

 private:
  void SetValue() override;
  void GetValue() override;
  uint8_t service_type_value_;
};

}  // namespace nas

#endif
