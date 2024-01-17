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

#ifndef _SERVICE_AREA_LIST_H_
#define _SERVICE_AREA_LIST_H_

#include <vector>

#include "Type4NasIe.hpp"
#include "Struct.hpp"

constexpr uint8_t kServiceAreaListMinimumLength        = 6;
constexpr uint8_t kServiceAreaListMaximumLength        = 114;
constexpr uint8_t kServiceAreaListMaximumSupportedTAIs = 16;
constexpr auto kServiceAreaListIeName                  = "Service Area List";

namespace nas {

class ServiceAreaList : public Type4NasIe {
 public:
  ServiceAreaList();
  ServiceAreaList(bool iei);
  ServiceAreaList(const std::vector<service_area_list_ie_t>& list);

  int Encode(uint8_t* buf, int len);
  // TODO: int Decode(uint8_t* buf, int len);

  static std::string GetIeName() { return kServiceAreaListIeName; }

 private:
  std::vector<service_area_list_ie_t> ie_list_;

 private:
  int encode_00_type(service_area_list_ie_t item, uint8_t* buf, int len);
  int encode_01_type(service_area_list_ie_t item, uint8_t* buf, int len);
  int encode_10_type(service_area_list_ie_t item, uint8_t* buf, int len);
  int encode_11_type(service_area_list_ie_t item, uint8_t* buf, int len);
};

}  // namespace nas

#endif
