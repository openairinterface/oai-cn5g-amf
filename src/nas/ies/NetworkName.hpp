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

#ifndef _NETWORK_NAME_H_
#define _NETWORK_NAME_H_

#include <stdint.h>

#include <string>

#include "bstrlib.h"

constexpr uint8_t kNetworkNameMinimumLength = 3;
constexpr auto kNetworkNameIeName           = "Network Name";

namespace nas {

class NetworkName {
 public:
  NetworkName();
  NetworkName(const uint8_t& iei);
  ~NetworkName();

  static std::string GetIeName() { return kNetworkNameIeName; }

  void setIEI(const uint8_t& iei);
  void setCodingScheme(const uint8_t& value);
  // TODO: getCodingScheme
  void setAddCI(const uint8_t& value);
  // TODO: getAddCI
  void setNumberOfSpareBits(const uint8_t& value);
  // TODO: getNumberOfSpareBits

  void setTextString(const std::string& str);
  void setTextString(const bstring& str);

  int Encode(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option = true);

 private:
  uint8_t iei_;
  uint16_t length;
  uint8_t coding_scheme;         // octet 3
  uint8_t add_CI;                // octet 3
  uint8_t number_of_spare_bits;  // octet 3
  bstring text_string;
};
}  // namespace nas

#endif
