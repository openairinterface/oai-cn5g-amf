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

#ifndef _UPLINK_DATA_STATUS_H_
#define _UPLINK_DATA_STATUS_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kUplinkDataStatusMinimumLength = 4;
constexpr uint8_t kUplinkDataStatusMaximumLength = 34;
constexpr auto kUplinkDataStatusIeName           = "Uplink Data Status";

namespace nas {

class UplinkDataStatus : public Type4NasIe {
 public:
  UplinkDataStatus();
  UplinkDataStatus(const uint16_t& value);
  ~UplinkDataStatus();

  static std::string GetIeName() { return kUplinkDataStatusIeName; }

  void SetValue(uint16_t value);
  uint16_t GetValue() const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

 private:
  uint16_t _value;
  // TODO: spare
};

}  // namespace nas

#endif
