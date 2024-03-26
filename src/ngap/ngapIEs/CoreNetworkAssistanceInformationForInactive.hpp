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
#include "Tai.hpp"
#include "UeIdentityIndexValue.hpp"

extern "C" {
#include "Ngap_CoreNetworkAssistanceInformationForInactive.h"
}

namespace ngap {

class CoreNetworkAssistanceInformationForInactive {
 public:
  CoreNetworkAssistanceInformationForInactive();
  virtual ~CoreNetworkAssistanceInformationForInactive();

  void set(
      const UeIdentityIndexValue& ueIdentityIndexValue,
      const DefaultPagingDrx& pagingDrx,
      const PeriodicRegistrationUpdateTimer& periodicRegistrationUpdateTimer,
      bool micoModeIndication, const std::vector<Tai>& tai);

  void get(
      UeIdentityIndexValue& ueIdentityIndexValue,
      std::optional<DefaultPagingDrx>& pagingDrx,
      PeriodicRegistrationUpdateTimer& periodicRegistrationUpdateTimer,
      bool& micoModeIndication, std::vector<Tai>& tai) const;

  bool encode(Ngap_CoreNetworkAssistanceInformationForInactive_t&
                  coreNetworkAssistanceInformation) const;
  bool decode(const Ngap_CoreNetworkAssistanceInformationForInactive_t&
                  coreNetworkAssistanceInformation);

 private:
  UeIdentityIndexValue m_UeIdentityIndexValue;  // Mandatory
  std::optional<DefaultPagingDrx> m_PagingDRX;  // UE Specific DRX, Optional
  PeriodicRegistrationUpdateTimer m_PeriodicRegUpdateTimer;  // Mandatory
  std::optional<MicoModeIndication> m_MicoModeInd;           // Optional
  std::vector<Tai> m_TaiList;  // Tai List for RRC Inactive, Mandatory
  // TODO: Expected UE Behaviour (Optional)
  // TODO: Paging eDRX Information (Optional)
  // TODO: Extended UE Identity Index Value (Optional)
  // TODO:UE Radio Capability for Paging (Optional)
  // TODO:MICO All PLMN (Optional)
  // TODO:Hashed UE Identity Index Value (Optional)
};

}  // namespace ngap

#endif
