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

#ifndef _MICO_INDICATION_H
#define _MICO_INDICATION_H

#include "Type1NasIeFormatTv.hpp"

constexpr uint8_t kMicoIndicationIELength = 1;
constexpr auto kMicoIndicationIeName      = "MICO Indication";

namespace nas {

class MicoIndication : public Type1NasIeFormatTv {
 public:
  MicoIndication();
  MicoIndication(const uint8_t _iei, bool sprti, bool raai);
  MicoIndication(bool sprti, bool raai);
  ~MicoIndication();

  static std::string GetIeName() { return kMicoIndicationIeName; }

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len, bool is_option);

  void SetSprti(bool value);
  bool GetSprti() const;

  void SetRaai(bool value);
  bool GetRaai() const;

  void SetValue();

 private:
  bool sprti_;
  bool raai_;
};

}  // namespace nas

#endif
