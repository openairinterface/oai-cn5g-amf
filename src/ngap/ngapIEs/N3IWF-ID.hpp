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

#ifndef _N3IWF_ID_H_
#define _N3IWF_ID_H_

#include "NgapIEsStruct.hpp"
#include <optional>

extern "C" {
#include "Ngap_N3IWF-ID.h"
}

namespace ngap {

constexpr uint8_t NGAP_N3IWF_ID_SIZE_MAX = 16;
constexpr uint8_t NGAP_N3IWF_ID_SIZE_MIN = 16;

class N3IWF_ID {
 public:
  N3IWF_ID();
  virtual ~N3IWF_ID();

  bool encode(Ngap_N3IWF_ID_t&);
  bool decode(Ngap_N3IWF_ID_t&);
  void setValue(const n3iwfId_t&);
  bool setValue(const uint16_t& id, const uint8_t& bit_length);
  // long getValue() const;
  bool get(n3iwfId_t& n3iwf_id) const;
  bool get(uint16_t& id) const;

 private:
  std::optional<n3iwfId_t> n3iwf_id_;  // 16bits
  Ngap_N3IWF_ID_PR present_;
};

}  // namespace ngap

#endif
