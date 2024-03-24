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
  m_PagingDRX   = std::nullopt;
  m_MicoModeInd = std::nullopt;
}

//------------------------------------------------------------------------------
CoreNetworkAssistanceInformationForInactive::
    ~CoreNetworkAssistanceInformationForInactive() {}

//------------------------------------------------------------------------------
void CoreNetworkAssistanceInformationForInactive::set(
    const UeIdentityIndexValue& ueIdentityIndexValue,
    const DefaultPagingDrx& pagingDrx,
    const PeriodicRegistrationUpdateTimer& periodicRegUpdateTimer,
    bool micoModeInd, const std::vector<Tai>& tai) {
  m_UeIdentityIndexValue   = ueIdentityIndexValue;
  m_PagingDRX              = std::optional<DefaultPagingDrx>(pagingDrx);
  m_PeriodicRegUpdateTimer = periodicRegUpdateTimer;
  if (micoModeInd) {
    m_MicoModeInd = std::make_optional<MicoModeIndication>();
  }
}

//------------------------------------------------------------------------------
void CoreNetworkAssistanceInformationForInactive::get(
    UeIdentityIndexValue& ueIdentityIndexValue,
    std::optional<DefaultPagingDrx>& pagingDrx,
    PeriodicRegistrationUpdateTimer& periodicRegUpdateTimer, bool& micoModeInd,
    std::vector<Tai>& tai) const {
  ueIdentityIndexValue   = m_UeIdentityIndexValue;
  pagingDrx              = m_PagingDRX;
  periodicRegUpdateTimer = m_PeriodicRegUpdateTimer;
  if (m_MicoModeInd.has_value())
    micoModeInd = true;
  else
    micoModeInd = false;
  tai = m_TaiList;
}

//------------------------------------------------------------------------------
bool CoreNetworkAssistanceInformationForInactive::encode(
    Ngap_CoreNetworkAssistanceInformationForInactive_t&
        coreNetworkAssistanceInformation) const {
  if (!m_UeIdentityIndexValue.encode(
          coreNetworkAssistanceInformation.uEIdentityIndexValue))
    return false;

  if (!m_PeriodicRegUpdateTimer.encode(
          coreNetworkAssistanceInformation.periodicRegistrationUpdateTimer))
    return false;

  for (std::vector<Tai>::const_iterator it = std::begin(m_TaiList);
       it < std::end(m_TaiList); ++it) {
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

  if (m_PagingDRX.has_value()) {
    Ngap_PagingDRX_t* pagingDrx =
        (Ngap_PagingDRX_t*) calloc(1, sizeof(Ngap_PagingDRX_t));
    if (!pagingDrx) return false;
    if (!m_PagingDRX.value().encode(*pagingDrx)) return false;
    coreNetworkAssistanceInformation.uESpecificDRX = pagingDrx;
  }

  if (m_MicoModeInd.has_value()) {
    Ngap_MICOModeIndication_t* micomodeindication =
        (Ngap_MICOModeIndication_t*) calloc(
            1, sizeof(Ngap_MICOModeIndication_t));
    if (!micomodeindication) return false;
    if (!m_MicoModeInd.value().encode(*micomodeindication)) return false;
    coreNetworkAssistanceInformation.mICOModeIndication = micomodeindication;
  }

  return true;
}

//------------------------------------------------------------------------------
bool CoreNetworkAssistanceInformationForInactive::decode(
    const Ngap_CoreNetworkAssistanceInformationForInactive_t&
        coreNetworkAssistanceInformation) {
  if (!m_UeIdentityIndexValue.decode(
          coreNetworkAssistanceInformation.uEIdentityIndexValue))
    return false;

  if (!m_PeriodicRegUpdateTimer.decode(
          coreNetworkAssistanceInformation.periodicRegistrationUpdateTimer))
    return false;

  for (int i = 0;
       i < coreNetworkAssistanceInformation.tAIListForInactive.list.count;
       i++) {
    Tai tai_item = {};
    if (!tai_item.decode(
            coreNetworkAssistanceInformation.tAIListForInactive.list.array[i]
                ->tAI))
      return false;
    m_TaiList.push_back(tai_item);
  }

  if (coreNetworkAssistanceInformation.uESpecificDRX) {
    DefaultPagingDrx tmp = {};
    if (!tmp.decode(*(coreNetworkAssistanceInformation.uESpecificDRX)))
      return false;
    m_PagingDRX = std::optional<DefaultPagingDrx>(tmp);
  }

  if (coreNetworkAssistanceInformation.mICOModeIndication) {
    MicoModeIndication tmp = {};
    if (!tmp.decode(*coreNetworkAssistanceInformation.mICOModeIndication))
      return false;
    m_MicoModeInd = std::optional<MicoModeIndication>(tmp);
  }

  return true;
}

}  // namespace ngap
