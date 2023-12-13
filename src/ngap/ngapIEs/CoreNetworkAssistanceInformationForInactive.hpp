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

#ifndef _CORE_NETWORK_ASSISTANCE_INFORMATION_FOR_INACTIVE_H_
#define _CORE_NETWORK_ASSISTANCE_INFORMATION_FOR_INACTIVE_H_

#include <optional>

#include "DefaultPagingDrx.hpp"
#include "MicoModeIndication.hpp"
#include "PeriodicRegistrationUpdateTimer.hpp"
#include "TAI.hpp"
#include "UEIdentityIndexValue.hpp"

extern "C" {
#include "Ngap_CoreNetworkAssistanceInformationForInactive.h"
}

namespace ngap {

class CoreNetworkAssistanceInformationForInactive {
 public:
  CoreNetworkAssistanceInformationForInactive();
  virtual ~CoreNetworkAssistanceInformationForInactive();

  void set(
      const UEIdentityIndexValue& ue_identity_index_value,
      const DefaultPagingDrx& paging_drx,
      const PeriodicRegistrationUpdateTimer& periodic_reg_update_timer,
      const bool& mico_mode_ind, const std::vector<TAI>& tai);

  void get(
      UEIdentityIndexValue& ue_identity_index_value,
      std::optional<DefaultPagingDrx>& paging_drx,
      PeriodicRegistrationUpdateTimer& periodic_reg_update_timer,
      bool& mico_mode_ind, std::vector<TAI>& tai) const;

  bool encode(Ngap_CoreNetworkAssistanceInformationForInactive_t&
                  core_network_assistance_information);
  bool decode(const Ngap_CoreNetworkAssistanceInformationForInactive_t&
                  core_network_assistance_information);

 private:
  UEIdentityIndexValue ueIdentityIndexValue;  // Mandatory
  std::optional<DefaultPagingDrx> pagingDRX;  // UE Specific DRX, Optional
  PeriodicRegistrationUpdateTimer periodicRegUpdateTimer;  // Mandatory
  std::optional<MicoModeIndication> micoModeInd;           // Optional
  std::vector<TAI> taiList;  // TAI List for RRC Inactive, Mandatory
  // TODO: Expected UE Behaviour (Optional)
  // TODO: Paging eDRX Information (Optional)
  // TODO: Extended UE Identity Index Value (Optional)
  // TODO:UE Radio Capability for Paging (Optional)
  // TODO:MICO All PLMN (Optional)
  // TODO:Hashed UE Identity Index Value (Optional)
};

}  // namespace ngap

#endif
