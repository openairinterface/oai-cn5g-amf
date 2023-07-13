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

#ifndef _EXTENDED_DRX_PARAMETERS_H_
#define _EXTENDED_DRX_PARAMETERS_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kExtendedDrxParametersLength = 3;
constexpr auto kExtendedDrxParametersIeName    = "Extended DRX Parameters";

namespace nas {

class ExtendedDrxParameters : public Type4NasIe {
 public:
  ExtendedDrxParameters();
  ExtendedDrxParameters(uint8_t paging_time, uint8_t value);
  ~ExtendedDrxParameters();

  static std::string GetIeName() { return kExtendedDrxParametersIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void setValue(uint8_t value);
  uint8_t getValue() const;

  void setPaging_time(uint8_t value);
  uint8_t getPaging_time() const;

 private:
  uint8_t paging_time_;
  uint8_t e_drx_value_;
};
}  // namespace nas

#endif
