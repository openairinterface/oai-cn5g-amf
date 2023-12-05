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

#ifndef _AMF_NAME_H_
#define _AMF_NAME_H_

#include <string>

extern "C" {
#include "Ngap_AMFName.h"
}

constexpr uint8_t AMF_NAME_SIZE_MAX = 150;

namespace ngap {

class AmfName {
 public:
  AmfName();
  virtual ~AmfName();

  bool setValue(const std::string& amf_name);
  void getValue(std::string& amf_name) const;

  bool encode(Ngap_AMFName_t&) const;
  bool decode(const Ngap_AMFName_t&);

 private:
  std::string amf_name_;
};
}  // namespace ngap

#endif
