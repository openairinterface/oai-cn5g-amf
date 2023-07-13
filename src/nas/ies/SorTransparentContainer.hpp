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

#ifndef _SOR_TRANSPARENT_CONTAINER_H_
#define _SOR_TRANSPARENT_CONTAINER_H_

#include "Type6NasIe.hpp"

constexpr uint8_t kSorTransparentContainerMinimumLength   = 20;
constexpr uint8_t kSorTransparentContainerIeMinimumLength = 17;
constexpr uint8_t kSorTransparentContainerIeMacLength     = 16;
constexpr auto kSorTransparentContainerIeName = "SOR Transparent Container";

namespace nas {

class SorTransparentContainer : Type6NasIe {
 public:
  SorTransparentContainer();
  SorTransparentContainer(
      uint8_t header,
      const uint8_t (&value)[kSorTransparentContainerIeMacLength]);
  ~SorTransparentContainer();

  static std::string GetIeName() { return kSorTransparentContainerIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void getValue(uint8_t (&value)[kSorTransparentContainerIeMacLength]) const;

 private:
  uint8_t header_;
  uint8_t sor_mac_i_[kSorTransparentContainerIeMacLength];
  std::optional<uint8_t> counter_;
  // Other IEs
};

}  // namespace nas

#endif
