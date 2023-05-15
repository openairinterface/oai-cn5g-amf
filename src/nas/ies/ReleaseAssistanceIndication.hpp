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

#ifndef _RELEASE_ASSISTANCE_INDICATION_H_
#define _RELEASE_ASSISTANCE_INDICATION_H_

#include "Type1NasIeFormatTv.hpp"

constexpr auto kReleaseAssistanceIndicationIeName =
    "Release Assistance Indication";

namespace nas {

class ReleaseAssistanceIndication : public Type1NasIeFormatTv {
 public:
  ReleaseAssistanceIndication();
  ReleaseAssistanceIndication(uint8_t value);
  ~ReleaseAssistanceIndication();

  static std::string GetIeName() { return kReleaseAssistanceIndicationIeName; }

  void Set(uint8_t value);
  uint8_t Get() const;

 private:
  // uint8_t ddx_;
};
}  // namespace nas

#endif
