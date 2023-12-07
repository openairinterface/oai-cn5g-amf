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

#ifndef _5GS_TRACKING_AREA_ID_LIST_H_
#define _5GS_TRACKING_AREA_ID_LIST_H_

#include <vector>

#include "Type4NasIe.hpp"
#include "Struct.hpp"

constexpr uint8_t k5gsTrackingAreaIdListMinimumLength        = 9;
constexpr uint8_t k5gsTrackingAreaIdListMaximumLength        = 114;
constexpr uint8_t k5gsTrackingAreaIdListMaximumSupportedTAIs = 16;
constexpr auto k5gsTrackingAreaIdListIeName = "5GS Tracking Area Identity List";

namespace nas {

class _5gsTrackingAreaIdList : public Type4NasIe {
 public:
  _5gsTrackingAreaIdList();
  _5gsTrackingAreaIdList(bool iei);
  _5gsTrackingAreaIdList(const std::vector<p_tai_t>& tai_list);
  int Encode(uint8_t* buf, int len);

  static std::string GetIeName() { return k5gsTrackingAreaIdListIeName; }

 private:
  std::vector<p_tai_t> m_tai_list;

 private:
  int encode_00_type(p_tai_t item, uint8_t* buf, int len);
  int encode_01_type(p_tai_t item, uint8_t* buf, int len);
  int encode_10_type(p_tai_t item, uint8_t* buf, int len);
};

}  // namespace nas

#endif
