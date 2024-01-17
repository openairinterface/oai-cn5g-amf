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

#ifndef _NG_ENB_ID_H_
#define _NG_ENB_ID_H_

#include "PlmnId.hpp"

extern "C" {
#include "Ngap_NgENB-ID.h"
}

namespace ngap {

class NgENB_ID {
 public:
  NgENB_ID();
  virtual ~NgENB_ID();

  void get(uint32_t& id, Ngap_NgENB_ID_PR& present) const;
  void set(const uint32_t& id, const Ngap_NgENB_ID_PR& present);

  bool encode(Ngap_NgENB_ID_t&);
  bool decode(const Ngap_NgENB_ID_t&);

 private:
  PlmnId plmnId;  // Mandatory
  union {
    uint32_t macro_ngenb_id;
    uint32_t short_macro_ngENB_id;
    uint32_t long_macro_ngENB_id;
  } id_;  // Mandatory
  Ngap_NgENB_ID_PR present_;
};

}  // namespace ngap

#endif
