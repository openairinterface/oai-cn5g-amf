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

#ifndef _FIVE_GS_TMSI_H_
#define _FIVE_GS_TMSI_H_

#include <string>

#include "AmfPointer.hpp"
#include "AmfSetId.hpp"

extern "C" {
#include "Ngap_FiveG-S-TMSI.h"
}

namespace ngap {

class FiveGSTmsi {
 public:
  FiveGSTmsi();
  ~FiveGSTmsi();

 public:
  void getTmsi(std::string& value) const;

  void get(std::string& setId, std::string& pointer, std::string& tmsi) const;
  bool set(
      const std::string& setId, const std::string& pointer,
      const std::string& tmsi);

  bool encode(Ngap_FiveG_S_TMSI_t& pdu) const;
  bool decode(const Ngap_FiveG_S_TMSI_t& pdu);

 private:
  std::string m_5gSTmsi;
  std::string m_TmsiValue;

  AmfSetId m_AmfSetId;
  AmfPointer m_AmfPointer;
};

}  // namespace ngap

#endif
