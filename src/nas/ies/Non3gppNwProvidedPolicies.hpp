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

#ifndef _NON_3GPP_NW_PROVIDED_POLICIES_H
#define _NON_3GPP_NW_PROVIDED_POLICIES_H

#include "Type1NasIeFormatTv.hpp"

constexpr uint8_t kNon3gppNwProvidedPoliciesLength = 1;
constexpr auto kNon3gppNwProvidedPoliciesIeName =
    "Non-3GPP NW Provided Policies";

namespace nas {

class Non3gppNwProvidedPolicies : public Type1NasIeFormatTv {
 public:
  Non3gppNwProvidedPolicies();
  Non3gppNwProvidedPolicies(uint8_t value);
  ~Non3gppNwProvidedPolicies();

  static std::string GetIeName() { return kNon3gppNwProvidedPoliciesIeName; }

  // int Encode(uint8_t* buf, int len);
  // int Decode(uint8_t* buf, int len, bool is_iei);

  void setValue(uint8_t value);
  uint8_t getValue() const;

 private:
};

}  // namespace nas

#endif
