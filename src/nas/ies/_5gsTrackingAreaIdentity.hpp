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

#ifndef __5GS_TRACKING_AREA_IDENTITY_H_
#define __5GS_TRACKING_AREA_IDENTITY_H_

#include "Type3NasIe.hpp"

constexpr uint8_t k5gsTrackingAreaIdentityLength = 7;
constexpr auto k5gsTrackingAreaIdentityIeName    = "5GS Tracking Area Identity";

namespace nas {

class _5gsTrackingAreaIdentity : public Type3NasIe {
 public:
  _5gsTrackingAreaIdentity();
  _5gsTrackingAreaIdentity(uint8_t iei) = delete;
  _5gsTrackingAreaIdentity(
      const std::string& mcc, const std::string& mnc, const uint32_t& tac);
  ~_5gsTrackingAreaIdentity();

  static std::string GetIeName() { return k5gsTrackingAreaIdentityIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei = true);

  void SetTac(const uint32_t& value);
  uint32_t GetTac() const;
  void GetTac(uint32_t& value) const;

  void SetMcc(const std::string& mcc);
  void GetMcc(std::string& mcc) const;

  void SetMnc(const std::string& mnc);
  void GetMnc(std::string& mnc) const;

 private:
  std::string mcc_;
  std::string mnc_;
  uint32_t tac_;
};
}  // namespace nas

#endif
