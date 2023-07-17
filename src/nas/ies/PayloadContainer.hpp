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

#ifndef _PAYLOAD_CONTAINER_H_
#define _PAYLOAD_CONTAINER_H_

#include "NasIeHeader.hpp"
#include "Type6NasIe.hpp"

constexpr uint8_t kPayloadContainerMinimumLength  = 4;
constexpr uint32_t kPayloadContainerMaximumLength = 65538;
constexpr auto kPayloadContainerIeName            = "Payload Container";

namespace nas {
class PayloadContainer : Type6NasIe {
 public:
  PayloadContainer();
  PayloadContainer(uint8_t iei);
  PayloadContainer(const bstring& b);
  PayloadContainer(uint8_t iei, const bstring& b);
  PayloadContainer(const std::vector<PayloadContainerEntry>& content);
  PayloadContainer(
      const uint8_t iei, const std::vector<PayloadContainerEntry>& content);
  ~PayloadContainer();

  static std::string GetIeName() { return kPayloadContainerIeName; }

  // void setValue(uint8_t iei, uint8_t value);
  int Encode(uint8_t* buf, int len, uint8_t type);
  int Decode(uint8_t* buf, int len, bool is_iei, uint8_t type);

  void SetValue(const bstring& cnt);
  bool GetValue(bstring& cnt) const;

  void SetValue(const std::vector<PayloadContainerEntry>& content);
  bool GetValue(std::vector<PayloadContainerEntry>& content) const;

 private:
  std::optional<bstring> content;
  std::optional<std::vector<PayloadContainerEntry>> CONTENT;
};

}  // namespace nas

#endif
