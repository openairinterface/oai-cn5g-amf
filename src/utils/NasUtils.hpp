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

#ifndef FILE_NAS_UTILS_HPP_SEEN
#define FILE_NAS_UTILS_HPP_SEEN

#include <iostream>
#include <string>
constexpr uint8_t kMccMncLength = 3;

class NasUtils {
 public:
  static int encodeMccMnc2Buffer(
      const std::string& mcc_str, const std::string& mnc_str, uint8_t* buf,
      int len);

  static int decodeMccMncFromBuffer(
      std::string& mcc_str, std::string& mnc_str, uint8_t* buf, int len);
};
#endif /* FILE_NAS_UTILS_HPP_SEEN */
