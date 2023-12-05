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

#include "AllocationAndRetentionPriority.hpp"

namespace ngap {

//------------------------------------------------------------------------------
AllocationAndRetentionPriority::AllocationAndRetentionPriority() {}

//------------------------------------------------------------------------------
AllocationAndRetentionPriority::~AllocationAndRetentionPriority() {}

//------------------------------------------------------------------------------
void AllocationAndRetentionPriority::set(
    const PriorityLevelARP& priorityLevelARP,
    const Pre_emptionCapability& pre_emptionCapability,
    const Pre_emptionVulnerability& pre_emptionVulnerability) {
  priorityLevelARP_         = priorityLevelARP;
  pre_emptionCapability_    = pre_emptionCapability;
  pre_emptionVulnerability_ = pre_emptionVulnerability;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::get(
    PriorityLevelARP& priorityLevelARP,
    Pre_emptionCapability& pre_emptionCapability,
    Pre_emptionVulnerability& pre_emptionVulnerability) const {
  priorityLevelARP         = priorityLevelARP_;
  pre_emptionCapability    = pre_emptionCapability_;
  pre_emptionVulnerability = pre_emptionVulnerability_;
  return true;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::encode(
    Ngap_AllocationAndRetentionPriority_t& allocationAndRetentionPriority)
    const {
  if (!priorityLevelARP_.encode(
          allocationAndRetentionPriority.priorityLevelARP))
    return false;
  if (!pre_emptionCapability_.encode(
          allocationAndRetentionPriority.pre_emptionCapability))
    return false;
  if (!pre_emptionVulnerability_.encode(
          allocationAndRetentionPriority.pre_emptionVulnerability))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::decode(
    const Ngap_AllocationAndRetentionPriority_t&
        allocationAndRetentionPriority) {
  if (!priorityLevelARP_.decode(
          allocationAndRetentionPriority.priorityLevelARP))
    return false;
  if (!pre_emptionCapability_.decode(
          allocationAndRetentionPriority.pre_emptionCapability))
    return false;
  if (!pre_emptionVulnerability_.decode(
          allocationAndRetentionPriority.pre_emptionVulnerability))
    return false;

  return true;
}
}  // namespace ngap
