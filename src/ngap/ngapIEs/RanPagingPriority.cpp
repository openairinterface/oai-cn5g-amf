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

#include "RanPagingPriority.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RanPagingPriority::RanPagingPriority() {
  ran_paging_priority_ = 0;
}

//------------------------------------------------------------------------------
RanPagingPriority::~RanPagingPriority() {}

//------------------------------------------------------------------------------
bool RanPagingPriority::set(const uint32_t& ran_paging_priority) {
  if (ran_paging_priority > kRANPagingPriorityMaxValue) return false;
  ran_paging_priority_ = ran_paging_priority;
  return true;
}

//------------------------------------------------------------------------------
uint32_t RanPagingPriority::get() const {
  return ran_paging_priority_;
}

//------------------------------------------------------------------------------
bool RanPagingPriority::encode(Ngap_RANPagingPriority_t& ran_paging_priority) {
  ran_paging_priority = ran_paging_priority_;
  return true;
}

//------------------------------------------------------------------------------
bool RanPagingPriority::decode(Ngap_RANPagingPriority_t ran_paging_priority) {
  if (ran_paging_priority > kRANPagingPriorityMaxValue) return false;
  ran_paging_priority_ = ran_paging_priority;
  return true;
}
}  // namespace ngap
