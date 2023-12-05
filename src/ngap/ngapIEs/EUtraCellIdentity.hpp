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

#ifndef _EUTRA_CELL_IDENTITY_H_
#define _EUTRA_CELL_IDENTITY_H_
#include <cstdint>

constexpr uint32_t kEUTRACellIdentityMaxValue = 268435455;  // 2^28-1

extern "C" {
#include "Ngap_EUTRACellIdentity.h"
}

namespace ngap {

class EUtraCellIdentity {
 public:
  EUtraCellIdentity();
  virtual ~EUtraCellIdentity();

  uint32_t get() const;
  bool set(const uint32_t& m_eutracellidentity);

  bool encode(Ngap_EUTRACellIdentity_t& eUTRACellIdentity);
  bool decode(const Ngap_EUTRACellIdentity_t& eUTRACellIdentity);

 private:
  uint32_t id_;
};
}  // namespace ngap

#endif
