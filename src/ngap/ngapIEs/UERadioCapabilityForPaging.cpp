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

#include "UERadioCapabilityForPaging.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UERadioCapabilityForPaging::UERadioCapabilityForPaging() {
  ueRadioCapabilityForPagingOfNR    = std::nullopt;
  ueRadioCapabilityForPagingOfEUTRA = std::nullopt;
}

//------------------------------------------------------------------------------
UERadioCapabilityForPaging::~UERadioCapabilityForPaging() {}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPaging::encode(
    Ngap_UERadioCapabilityForPaging_t& ueRadioCapabilityForPaging) {
  if (ueRadioCapabilityForPagingOfNR.has_value()) {
    ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfNR =
        (Ngap_UERadioCapabilityForPagingOfNR_t*) calloc(
            1, sizeof(Ngap_UERadioCapabilityForPagingOfNR_t));
    if (!ueRadioCapabilityForPagingOfNR.value().encode(
            *ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfNR))
      return false;
  }
  if (ueRadioCapabilityForPagingOfEUTRA.has_value()) {
    ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfEUTRA =
        (Ngap_UERadioCapabilityForPagingOfEUTRA_t*) calloc(
            1, sizeof(Ngap_UERadioCapabilityForPagingOfEUTRA_t));
    if (!ueRadioCapabilityForPagingOfEUTRA.value().encode(
            *ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfEUTRA))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPaging::decode(
    const Ngap_UERadioCapabilityForPaging_t& ueRadioCapabilityForPaging) {
  if (ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfNR) {
    UERadioCapabilityForPagingOfNR tmp = {};
    if (!tmp.decode(*ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfNR))
      return false;
    ueRadioCapabilityForPagingOfNR =
        std::optional<UERadioCapabilityForPagingOfNR>(tmp);
  }
  if (ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfEUTRA) {
    UERadioCapabilityForPagingOfEUTRA tmp = {};
    if (!tmp.decode(
            *ueRadioCapabilityForPaging.uERadioCapabilityForPagingOfEUTRA))
      return false;
    ueRadioCapabilityForPagingOfEUTRA =
        std::optional<UERadioCapabilityForPagingOfEUTRA>(tmp);
  }

  return true;
}

//------------------------------------------------------------------------------
void UERadioCapabilityForPaging::setUERadioCapabilityForPagingOfNR(
    const OCTET_STRING_t& capability) {
  UERadioCapabilityForPagingOfNR tmp = {};
  tmp.set(capability);
  ueRadioCapabilityForPagingOfNR =
      std::optional<UERadioCapabilityForPagingOfNR>(tmp);
  ueRadioCapabilityForPagingOfEUTRA = std::nullopt;
}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPaging::getUERadioCapabilityForPagingOfNR(
    OCTET_STRING_t& capability) {
  if (!ueRadioCapabilityForPagingOfNR.has_value()) return false;
  return ueRadioCapabilityForPagingOfNR.value().get(capability);
}

//------------------------------------------------------------------------------
void UERadioCapabilityForPaging::setUERadioCapabilityForPagingOfEUTRA(
    const OCTET_STRING_t& capability) {
  UERadioCapabilityForPagingOfEUTRA tmp = {};
  tmp.set(capability);
  ueRadioCapabilityForPagingOfEUTRA =
      std::optional<UERadioCapabilityForPagingOfEUTRA>(tmp);

  ueRadioCapabilityForPagingOfNR = std::nullopt;
}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPaging::getUERadioCapabilityForPagingOfEUTRA(
    OCTET_STRING_t& capability) {
  if (!ueRadioCapabilityForPagingOfEUTRA.has_value()) return false;
  return ueRadioCapabilityForPagingOfEUTRA.value().get(capability);
}

}  // namespace ngap
