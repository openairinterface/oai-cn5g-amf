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

#ifndef _UE_RADIO_CAPABILITY_INFO_INDICATION_H_
#define _UE_RADIO_CAPABILITY_INFO_INDICATION_H_

#include "NgapUeMessage.hpp"
#include "UeRadioCapability.hpp"
#include "UeRadioCapabilityForPaging.hpp"

extern "C" {
#include "Ngap_UERadioCapabilityInfoIndication.h"
}

namespace ngap {

class UeRadioCapabilityInfoIndicationMsg : public NgapUeMessage {
 public:
  UeRadioCapabilityInfoIndicationMsg();
  virtual ~UeRadioCapabilityInfoIndicationMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setUeRadioCapability(const OCTET_STRING_t& capability);
  void getUeRadioCapability(OCTET_STRING_t& capability) const;

  void setUeRadioCapabilityForPaging(
      const OCTET_STRING_t& ueRadioCapabilityForPagingOfNr,
      const OCTET_STRING_t& ueRadioCapabilityForPagingOfEutra);
  bool getUeRadioCapabilityForPaging(
      OCTET_STRING_t& ueRadioCapabilityForPagingOfNr,
      OCTET_STRING_t& ueRadioCapabilityForPagingOfEutra) const;

 private:
  Ngap_UERadioCapabilityInfoIndication_t* m_UeRadioCapabilityInfoIndicationIes;
  // AMF_UE_NGAP_ID //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  UeRadioCapability m_UeRadioCapability;  // Mandatory
  std::optional<UeRadioCapabilityForPaging>
      m_UeRadioCapabilityForPaging;  // Optional
  // TODO: UE Radio Capability – E-UTRA Format (Optional, Rel 16.14.0)
};

}  // namespace ngap
#endif
