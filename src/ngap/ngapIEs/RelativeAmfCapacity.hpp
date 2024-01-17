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

#ifndef _RELATIVE_AMF_CAPACITY_H_
#define _RELATIVE_AMF_CAPACITY_H_

extern "C" {
#include "Ngap_RelativeAMFCapacity.h"
}

namespace ngap {

class RelativeAmfCapacity {
 public:
  RelativeAmfCapacity();
  virtual ~RelativeAmfCapacity();

  void setValue(uint8_t amf_capacity);
  uint8_t getValue() const;

  bool encode(Ngap_RelativeAMFCapacity_t&) const;
  bool decode(Ngap_RelativeAMFCapacity_t);

 private:
  uint8_t amf_capacity_;
};
}  // namespace ngap

#endif
