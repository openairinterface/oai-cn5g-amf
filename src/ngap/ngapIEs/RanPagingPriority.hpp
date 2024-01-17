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

#ifndef _RAN_PAGING_PRIORITY_H_
#define _RAN_PAGING_PRIORITY_H_

#include <cstdint>

constexpr uint32_t kRANPagingPriorityMaxValue = 256;

extern "C" {
#include "Ngap_RANPagingPriority.h"
}

namespace ngap {

class RanPagingPriority {
 public:
  RanPagingPriority();
  virtual ~RanPagingPriority();

  bool set(const uint32_t&);
  uint32_t get() const;

  bool encode(Ngap_RANPagingPriority_t&);
  bool decode(Ngap_RANPagingPriority_t);

 private:
  uint32_t ran_paging_priority_;
};

}  // namespace ngap

#endif
