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
    const PriorityLevelARP& m_priorityLevelARP,
    const Pre_emptionCapability& m_pre_emptionCapability,
    const Pre_emptionVulnerability& m_pre_emptionVulnerability) {
  priorityLevelARP         = m_priorityLevelARP;
  pre_emptionCapability    = m_pre_emptionCapability;
  pre_emptionVulnerability = m_pre_emptionVulnerability;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::get(
    PriorityLevelARP& m_priorityLevelARP,
    Pre_emptionCapability& m_pre_emptionCapability,
    Pre_emptionVulnerability& m_pre_emptionVulnerability) const {
  m_priorityLevelARP         = priorityLevelARP;
  m_pre_emptionCapability    = pre_emptionCapability;
  m_pre_emptionVulnerability = pre_emptionVulnerability;
  return true;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::encode(
    Ngap_AllocationAndRetentionPriority_t* allocationAndRetentionPriority) {
  if (!priorityLevelARP.encode(
          &allocationAndRetentionPriority->priorityLevelARP))
    return false;
  if (!pre_emptionCapability.encode(
          &allocationAndRetentionPriority->pre_emptionCapability))
    return false;
  if (!pre_emptionVulnerability.encode(
          &allocationAndRetentionPriority->pre_emptionVulnerability))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::decode(
    Ngap_AllocationAndRetentionPriority_t* allocationAndRetentionPriority) {
  if (!priorityLevelARP.decode(
          &allocationAndRetentionPriority->priorityLevelARP))
    return false;
  if (!pre_emptionCapability.decode(
          &allocationAndRetentionPriority->pre_emptionCapability))
    return false;
  if (!pre_emptionVulnerability.decode(
          &allocationAndRetentionPriority->pre_emptionVulnerability))
    return false;

  return true;
}
}  // namespace ngap
