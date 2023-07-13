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

#ifndef _NSSAI_INCLUSION_MODE_H
#define _NSSAI_INCLUSION_MODE_H

#include "Type1NasIeFormatTv.hpp"

constexpr uint8_t kNssaiInclusionModeLength = 1;
constexpr auto kNssaiInclusionModeIeName    = "NSSAI Inclusion Mode";

namespace nas {

class NssaiInclusionMode : public Type1NasIeFormatTv {
 public:
  NssaiInclusionMode();
  NssaiInclusionMode(uint8_t value);
  ~NssaiInclusionMode();

  static std::string GetIeName() { return kNssaiInclusionModeIeName; }

  void setValue(uint8_t value);
  uint8_t getValue() const;

 private:
};

}  // namespace nas

#endif
