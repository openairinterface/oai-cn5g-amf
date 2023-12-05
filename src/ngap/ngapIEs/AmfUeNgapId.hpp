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

#ifndef _AMF_UE_NGAP_ID_H_
#define _AMF_UE_NGAP_ID_H_
#include <cstdint>

constexpr uint64_t AMF_UE_NGAP_ID_MAX_VALUE = 1099511627775;  // 2^40 -1

extern "C" {
#include "Ngap_AMF-UE-NGAP-ID.h"
}

namespace ngap {

class AmfUeNgapId {
 public:
  AmfUeNgapId();
  virtual ~AmfUeNgapId();

  bool set(const uint64_t&);
  uint64_t get() const;

  bool encode(Ngap_AMF_UE_NGAP_ID_t&) const;
  bool decode(Ngap_AMF_UE_NGAP_ID_t);

 private:
  uint64_t id_;
};

}  // namespace ngap

#endif
