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

#ifndef _TAI_H_
#define _TAI_H_

#include "NgapIesStruct.hpp"
#include "PlmnId.hpp"
#include "Tac.hpp"

extern "C" {
#include "Ngap_TAI.h"
}

namespace ngap {

class Tai {
 public:
  Tai();
  virtual ~Tai();

  void set(const PlmnId&, const TAC&);
  void get(PlmnId&, TAC&);

  void set(const std::string& mcc, const std::string& mnc, const uint32_t& tac);
  void get(std::string& mcc, std::string& mnc, uint32_t& tac);

  void set(const Tai_t& tai);
  void get(Tai_t& tai);

  bool encode(Ngap_TAI_t&) const;
  bool decode(const Ngap_TAI_t&);

 private:
  PlmnId m_PlmnId;  // Mandatory
  TAC m_Tac;        // Mandatory
};
}  // namespace ngap

#endif
