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

#include "UnavailableGuamiItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UnavailableGuamiItem::UnavailableGuamiItem() {
  timer_approach_for_guami_removal_ = std::nullopt;
  backup_amf_name_                  = std::nullopt;
}

//------------------------------------------------------------------------------
UnavailableGuamiItem::~UnavailableGuamiItem() {}

//------------------------------------------------------------------------------
void UnavailableGuamiItem::setGuami(const GUAMI& guami) {
  guami_ = guami;
}

//------------------------------------------------------------------------------
void UnavailableGuamiItem::getGuami(GUAMI& guami) const {
  guami = guami_;
}

//------------------------------------------------------------------------------
void UnavailableGuamiItem::setTimerApproachForGuamiRemoval(
    const TimerApproachForGuamiRemoval& timer) {
  timer_approach_for_guami_removal_ =
      std::make_optional<TimerApproachForGuamiRemoval>(timer);
}

//------------------------------------------------------------------------------
void UnavailableGuamiItem::getTimerApproachForGuamiRemoval(
    std::optional<TimerApproachForGuamiRemoval>& timer) const {
  timer = timer_approach_for_guami_removal_;
}

//------------------------------------------------------------------------------
void UnavailableGuamiItem::setBackupAmfName(const AmfName& name) {
  backup_amf_name_ = std::make_optional<AmfName>(name);
}

//------------------------------------------------------------------------------
void UnavailableGuamiItem::getBackupAmfName(
    std::optional<AmfName>& name) const {
  name = backup_amf_name_;
}

//------------------------------------------------------------------------------
bool UnavailableGuamiItem::encode(Ngap_UnavailableGUAMIItem& item) {
  if (!guami_.encode(item.gUAMI)) return false;
  if (timer_approach_for_guami_removal_.has_value()) {
    if (!timer_approach_for_guami_removal_.value().encode(
            *item.timerApproachForGUAMIRemoval))
      return false;
  }
  if (backup_amf_name_.has_value())
    if (!backup_amf_name_.value().encode(*item.backupAMFName)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool UnavailableGuamiItem::decode(const Ngap_UnavailableGUAMIItem& item) {
  if (!guami_.decode(item.gUAMI)) return false;

  if (item.timerApproachForGUAMIRemoval) {
    TimerApproachForGuamiRemoval tmp = {};
    if (!tmp.decode(*item.timerApproachForGUAMIRemoval)) return false;
    timer_approach_for_guami_removal_ =
        std::make_optional<TimerApproachForGuamiRemoval>(tmp);
  }

  if (item.backupAMFName) {
    AmfName tmp = {};
    if (!tmp.decode(*item.backupAMFName)) return false;
    backup_amf_name_ = std::make_optional<AmfName>(tmp);
  }
  return true;
}

}  // namespace ngap
