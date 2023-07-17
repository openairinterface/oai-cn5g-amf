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

#ifndef _DNN_H_
#define _DNN_H_

#include "Type4NasIe.hpp"

constexpr uint8_t kDnnMinimumLength = 3;
constexpr uint8_t kDnnMaximumLength = 102;
constexpr auto kDnnIeName           = "DNN";

namespace nas {

class Dnn : public Type4NasIe {
 public:
  Dnn();
  Dnn(bstring dnn);
  Dnn(bool iei);
  ~Dnn();

  static std::string GetIeName() { return kDnnIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_iei);

  void SetValue(const bstring& dnn);
  void GetValue(bstring& dnn) const;

 private:
  bstring _dnn;
};

}  // namespace nas

#endif
