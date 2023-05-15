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

#ifndef _NETWORK_SLICING_INDICATION_H_
#define _NETWORK_SLICING_INDICATION_H_

#include "Type1NasIeFormatTv.hpp"

constexpr uint8_t kNetworkSlicingIndicationLength = 1;
constexpr auto kNetworkSlicingIndicationIeName = "Network Slicing Indication";

namespace nas {

class NetworkSlicingIndication : Type1NasIeFormatTv {
 public:
  NetworkSlicingIndication();
  NetworkSlicingIndication(uint8_t iei);
  NetworkSlicingIndication(const uint8_t iei, bool dcni, bool nssci);
  NetworkSlicingIndication(bool dcni, bool nssci);
  ~NetworkSlicingIndication();

  static std::string GetIeName() { return kNetworkSlicingIndicationIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void SetValue();
  void GetValue();
  void SetDcni(bool value);
  bool GetDcni() const;

  void SetNssci(bool value);
  bool GetNssci() const;

 private:
  bool dcni_;
  bool nssci_;
};
}  // namespace nas

#endif
