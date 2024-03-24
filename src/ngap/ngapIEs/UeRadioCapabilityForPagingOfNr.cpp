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

#include "UeRadioCapabilityForPagingOfNr.hpp"

#include "amf_conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UeRadioCapabilityForPagingOfNr::UeRadioCapabilityForPagingOfNr() {}

//------------------------------------------------------------------------------
UeRadioCapabilityForPagingOfNr::~UeRadioCapabilityForPagingOfNr() {}

//------------------------------------------------------------------------------
bool UeRadioCapabilityForPagingOfNr::encode(
    Ngap_UERadioCapabilityForPagingOfNR_t& ueRadioCapabilityForPagingOfNr)
    const {
  return amf_conv::octet_string_copy(
      ueRadioCapabilityForPagingOfNr, m_UeRadioCapability);
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityForPagingOfNr::decode(
    const Ngap_UERadioCapabilityForPagingOfNR_t&
        ueRadioCapabilityForPagingOfNr) {
  return amf_conv::octet_string_copy(
      m_UeRadioCapability, ueRadioCapabilityForPagingOfNr);
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityForPagingOfNr::set(const OCTET_STRING_t& capability) {
  return amf_conv::octet_string_copy(m_UeRadioCapability, capability);
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityForPagingOfNr::get(OCTET_STRING_t& capability) const {
  return amf_conv::octet_string_copy(capability, m_UeRadioCapability);
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityForPagingOfNr::set(const bstring& capability) {
  return amf_conv::bstring_2_octet_string(capability, m_UeRadioCapability);
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityForPagingOfNr::get(bstring& capability) const {
  return amf_conv::octet_string_2_bstring(m_UeRadioCapability, capability);
}

}  // namespace ngap
