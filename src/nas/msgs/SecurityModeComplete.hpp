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

#ifndef _SECURITY_MODE_COMPLETE_H_
#define _SECURITY_MODE_COMPLETE_H_

#include "NasIeHeader.hpp"

namespace nas {

class SecurityModeComplete : public NasMmPlainHeader {
 public:
  SecurityModeComplete();
  ~SecurityModeComplete();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetImeisv(const IMEI_IMEISV_t& imeisv);
  bool GetImeisv(IMEI_IMEISV_t& imeisv) const;

  void SetNasMessageContainer(const bstring& value);
  bool GetNasMessageContainer(bstring& nas) const;

  void SetNonImeisv(const IMEI_IMEISV_t& imeisv);
  bool GetNonImeisv(IMEI_IMEISV_t& imeisv) const;

 public:
  std::optional<_5gsMobileIdentity> ie_imeisv;                  // Optional
  std::optional<NasMessageContainer> ie_nas_message_container;  // Optional
  std::optional<_5gsMobileIdentity> ie_non_imeisvpei;           // Optional
};

}  // namespace nas

#endif
