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

#ifndef _LADN_H_
#define _LADN_H_

#include "Dnn.hpp"
#include "Type6NasIe.hpp"
#include "_5gsTrackingAreaIdList.hpp"

namespace nas {

class Ladn {
 public:
  Ladn();
  ~Ladn();

  void Set(const Dnn& value);
  void Set(const _5gsTrackingAreaIdList& value);
  uint32_t GetLength() const;

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

 private:
  Dnn dnn;
  _5gsTrackingAreaIdList ta_list;
};

}  // namespace nas

#endif
