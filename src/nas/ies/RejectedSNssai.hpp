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

#ifndef _REJECTED_SNSSAI_H_
#define _REJECTED_SNSSAI_H_

#include <stdint.h>

#include <optional>

namespace nas {

class RejectedSNssai {
 public:
  RejectedSNssai();
  RejectedSNssai(const uint8_t& cause, const uint8_t& sst, const uint32_t& sd);
  ~RejectedSNssai();

  uint8_t getLength();

  void setSST(const uint8_t& sst);
  uint8_t getSST();
  void getSST(uint8_t& sst);

  void setSd(const uint32_t& sd);
  bool getSd(uint32_t& sd);
  void getSd(std::optional<uint32_t>& sd);

  void setCause(const uint8_t& cause);
  uint8_t getCause();
  void getCause(uint8_t& cause);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

 private:
  uint8_t length_;
  uint8_t cause_;
  uint8_t sst_;
  std::optional<uint32_t> sd_;
};

}  // namespace nas

#endif
