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

#ifndef _5GS_UPDATE_TYPE_H
#define _5GS_UPDATE_TYPE_H

#include "Type4NasIe.hpp"

constexpr uint8_t k5gsUpdateTypeLength = 3;
constexpr auto k5gsUpdateTypeIeName    = "5GS Update Type";

namespace nas {

class _5gsUpdateType : public Type4NasIe {
 public:
  _5gsUpdateType();
  _5gsUpdateType(
      uint8_t eps_PNB_CIoT, uint8_t _5gs_PNB_CIoT, bool ng_RAN, bool sms);
  ~_5gsUpdateType();

  static std::string GetIeName() { return k5gsUpdateTypeIeName; }

  void SetEpsPnbCiot(uint8_t value);
  void Set5gsPnbCiot(uint8_t value);
  void SetNgRan(uint8_t value);
  void SetSms(uint8_t value);
  uint8_t GetEpsPnbCiot() const;
  uint8_t Get5gsPnbCiot() const;
  bool GetNgRan() const;
  bool GetSms() const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_option);

 private:
  uint8_t eps_pnb_ciot_;   // bit 4,5
  uint8_t _5gs_pnb_ciot_;  // bit 2,3
  bool ng_ran_;            // bit 1
  bool sms_;               // bit 0
};

}  // namespace nas

#endif
