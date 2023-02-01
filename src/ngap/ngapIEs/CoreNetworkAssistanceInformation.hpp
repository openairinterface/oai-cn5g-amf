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

#ifndef _CORENETWORKASSISTANCEINFORMATION_H_
#define _CORENETWORKASSISTANCEINFORMATION_H_

#include "DefaultPagingDRX.hpp"
#include "MICOModeIndication.hpp"
#include "PeriodicRegistrationUpdateTimer.hpp"
#include "TAI.hpp"
#include "UEIdentityIndexValue.hpp"

#include <optional>

extern "C" {
#include "Ngap_CoreNetworkAssistanceInformation.h"
}

namespace ngap {

class CoreNetworkAssistanceInfo {
 public:
  CoreNetworkAssistanceInfo();
  virtual ~CoreNetworkAssistanceInfo();

  void set(
      const UEIdentityIndexValue& ue_identity_index_value,
      const DefaultPagingDRX& paging_drx,
      const PeriodicRegistrationUpdateTimer& periodic_reg_update_timer,
      const bool& mico_mode_ind, const std::vector<TAI>& tai);

  void get(
      UEIdentityIndexValue& ue_identity_index_value,
      std::optional<DefaultPagingDRX>& paging_drx,
      PeriodicRegistrationUpdateTimer& periodic_reg_update_timer,
      bool& mico_mode_ind, std::vector<TAI>& tai);

  bool encode(Ngap_CoreNetworkAssistanceInformation_t*
                  core_network_assistance_information);
  bool decode(Ngap_CoreNetworkAssistanceInformation_t*
                  core_network_assistance_information);

 private:
  UEIdentityIndexValue ueIdentityIndexValue;  // Mandatory
  std::optional<DefaultPagingDRX> pagingDRX;  // UE Specific DRX, Optional
  PeriodicRegistrationUpdateTimer periodicRegUpdateTimer;  // Mandatory
  std::optional<MICOModeIndication> micoModeInd;           // Optional
  std::vector<TAI> taiList;  // TAI List for RRC Inactive, Mandatory
  // TODO: Expected UE Behaviour (Optional)
};

}  // namespace ngap

#endif
