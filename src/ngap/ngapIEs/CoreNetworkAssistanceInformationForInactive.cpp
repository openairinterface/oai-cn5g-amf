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

#include "CoreNetworkAssistanceInformationForInactive.hpp"

extern "C" {
#include "Ngap_TAIListForInactiveItem.h"
}

namespace ngap {

//------------------------------------------------------------------------------
CoreNetworkAssistanceInformationForInactive::
    CoreNetworkAssistanceInformationForInactive() {
  pagingDRX   = std::nullopt;
  micoModeInd = std::nullopt;
}

//------------------------------------------------------------------------------
CoreNetworkAssistanceInformationForInactive::
    ~CoreNetworkAssistanceInformationForInactive() {}

//------------------------------------------------------------------------------
void CoreNetworkAssistanceInformationForInactive::set(
    const UEIdentityIndexValue& ue_identity_index_value,
    const DefaultPagingDrx& paging_drx,
    const PeriodicRegistrationUpdateTimer& m_periodicRegUpdateTimer,
    const bool& m_micoModeInd, const std::vector<TAI>& m_tai) {
  ueIdentityIndexValue   = ue_identity_index_value;
  pagingDRX              = std::optional<DefaultPagingDrx>(paging_drx);
  periodicRegUpdateTimer = m_periodicRegUpdateTimer;
  if (m_micoModeInd) {
    micoModeInd = std::make_optional<MicoModeIndication>();
  }
}

//------------------------------------------------------------------------------
void CoreNetworkAssistanceInformationForInactive::get(
    UEIdentityIndexValue& ue_identity_index_value,
    std::optional<DefaultPagingDrx>& paging_drx,
    PeriodicRegistrationUpdateTimer& m_periodicRegUpdateTimer,
    bool& m_micoModeInd, std::vector<TAI>& m_tai) const {
  ue_identity_index_value  = ueIdentityIndexValue;
  paging_drx               = pagingDRX;
  m_periodicRegUpdateTimer = periodicRegUpdateTimer;
  if (micoModeInd.has_value())
    m_micoModeInd = true;
  else
    m_micoModeInd = false;
  m_tai = taiList;
}

//------------------------------------------------------------------------------
bool CoreNetworkAssistanceInformationForInactive::encode(
    Ngap_CoreNetworkAssistanceInformationForInactive_t&
        coreNetworkAssistanceInformation) {
  if (!ueIdentityIndexValue.encode(
          coreNetworkAssistanceInformation.uEIdentityIndexValue))
    return false;

  if (!periodicRegUpdateTimer.encode(
          coreNetworkAssistanceInformation.periodicRegistrationUpdateTimer))
    return false;

  for (std::vector<TAI>::iterator it = std::begin(taiList);
       it < std::end(taiList); ++it) {
    Ngap_TAIListForInactiveItem_t* taiListForInactiveItem =
        (Ngap_TAIListForInactiveItem_t*) calloc(
            1, sizeof(Ngap_TAIListForInactiveItem_t));
    if (!taiListForInactiveItem) return false;
    if (!it->encode(taiListForInactiveItem->tAI)) return false;
    if (ASN_SEQUENCE_ADD(
            &coreNetworkAssistanceInformation.tAIListForInactive.list,
            taiListForInactiveItem) != 0)
      return false;
  }

  if (pagingDRX.has_value()) {
    Ngap_PagingDRX_t* paging_drx =
        (Ngap_PagingDRX_t*) calloc(1, sizeof(Ngap_PagingDRX_t));
    if (!paging_drx) return false;
    if (!pagingDRX.value().encode(*paging_drx)) return false;
    coreNetworkAssistanceInformation.uESpecificDRX = paging_drx;
  }

  if (micoModeInd.has_value()) {
    Ngap_MICOModeIndication_t* micomodeindication =
        (Ngap_MICOModeIndication_t*) calloc(
            1, sizeof(Ngap_MICOModeIndication_t));
    if (!micomodeindication) return false;
    if (!micoModeInd.value().encode(*micomodeindication)) return false;
    coreNetworkAssistanceInformation.mICOModeIndication = micomodeindication;
  }

  return true;
}

//------------------------------------------------------------------------------
bool CoreNetworkAssistanceInformationForInactive::decode(
    const Ngap_CoreNetworkAssistanceInformationForInactive_t&
        coreNetworkAssistanceInformation) {
  if (!ueIdentityIndexValue.decode(
          coreNetworkAssistanceInformation.uEIdentityIndexValue))
    return false;

  if (!periodicRegUpdateTimer.decode(
          coreNetworkAssistanceInformation.periodicRegistrationUpdateTimer))
    return false;

  for (int i = 0;
       i < coreNetworkAssistanceInformation.tAIListForInactive.list.count;
       i++) {
    TAI tai_item = {};
    if (!tai_item.decode(
            coreNetworkAssistanceInformation.tAIListForInactive.list.array[i]
                ->tAI))
      return false;
    taiList.push_back(tai_item);
  }

  if (coreNetworkAssistanceInformation.uESpecificDRX) {
    DefaultPagingDrx tmp = {};
    if (!tmp.decode(*(coreNetworkAssistanceInformation.uESpecificDRX)))
      return false;
    pagingDRX = std::optional<DefaultPagingDrx>(tmp);
  }

  if (coreNetworkAssistanceInformation.mICOModeIndication) {
    MicoModeIndication tmp = {};
    if (!tmp.decode(*coreNetworkAssistanceInformation.mICOModeIndication))
      return false;
    micoModeInd = std::optional<MicoModeIndication>(tmp);
  }

  return true;
}

}  // namespace ngap
