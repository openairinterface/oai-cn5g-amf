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

#ifndef _INITIAL_CONTEXT_SETUP_REQUEST_H_
#define _INITIAL_CONTEXT_SETUP_REQUEST_H_

#include <optional>

#include "3gpp_23.003.h"
#include "AmfName.hpp"
#include "AllowedNssai.hpp"
#include "CoreNetworkAssistanceInformationForInactive.hpp"
#include "GUAMI.hpp"
#include "MobilityRestrictionList.hpp"
#include "NgapUEMessage.hpp"
#include "PduSessionResourceSetupListCxtReq.hpp"
#include "SecurityKey.hpp"
#include "UEAggregateMaxBitRate.hpp"
#include "UERadioCapability.hpp"
#include "UESecurityCapabilities.hpp"

extern "C" {
#include "Ngap_InitialContextSetupRequest.h"
}

namespace ngap {

class InitialContextSetupRequestMsg : public NgapUEMessage {
 public:
  InitialContextSetupRequestMsg();
  virtual ~InitialContextSetupRequestMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setOldAmf(const std::string& name);
  bool getOldAmf(std::string& name);

  void setUEAggregateMaxBitRate(
      const uint64_t& bit_rate_downlink, const uint64_t& bit_rate_uplink);
  bool getUEAggregateMaxBitRate(
      uint64_t& bit_rate_downlink, uint64_t& bit_rate_uplink);

  void setUEAggregateMaxBitRate(const UEAggregateMaxBitRate& bit_rate);
  bool getUEAggregateMaxBitRate(UEAggregateMaxBitRate& bit_rate);

  void setCoreNetworkAssistanceInfo(
      const uint16_t& ue_identity_index_value_value,
      const e_Ngap_PagingDRX& ue_specific_drx_value,
      const uint8_t& periodic_reg_update_timer_value,
      const bool& mico_mode_ind_value,
      const std::vector<Tai_t>& tai_list_for_rrc_inactive);

  bool getCoreNetworkAssistanceInfo(
      uint16_t& ue_identity_index_value_value, int& ue_specific_drx_value,
      uint8_t& periodic_reg_update_timer_value, bool& mico_mode_ind_value,
      std::vector<Tai_t>& tai_list_for_rrc_inactive);

  void setGuami(const guami_full_format_t& value);
  bool getGuami(guami_full_format_t& value);

  void setPduSessionResourceSetupRequestList(
      const std::vector<PDUSessionResourceSetupRequestItem_t>& list);
  bool getPduSessionResourceSetupRequestList(
      std::vector<PDUSessionResourceSetupRequestItem_t>& list);

  void setAllowedNssai(const std::vector<S_Nssai>& list);
  bool getAllowedNssai(std::vector<S_Nssai>& list);

  void setUESecurityCapability(
      const uint16_t& nr_encryption_algs,
      const uint16_t& nr_integrity_protection_algs,
      const uint16_t& e_utra_encryption_algs,
      const uint16_t& e_utra_integrity_protection_algs);

  bool getUESecurityCapability(
      uint16_t& nr_encryption_algs, uint16_t& nr_integrity_protection_algs,
      uint16_t& e_utra_encryption_algs,
      uint16_t& e_utra_integrity_protection_algs);

  void setSecurityKey(uint8_t* key);   // 256bits
  bool getSecurityKey(uint8_t*& key);  // 256bits

  void setMobilityRestrictionList(const PlmnId& plmn_id);
  // TODO: getMobilityRestrictionList

  void setUERadioCapability(const bstring& ue_radio_capability);
  void getUERadioCapability(bstring& ue_radio_capability);

  void setMaskedIMEISV(const std::string& imeisv);
  // bool getMaskedIMEISV();

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu);

 private:
  Ngap_InitialContextSetupRequest_t* initialContextSetupRequestIEs;

  std::optional<AmfName> oldAMF;                               // Optional
  std::optional<UEAggregateMaxBitRate> uEAggregateMaxBitRate;  // Conditional
  std::optional<CoreNetworkAssistanceInformationForInactive>
      coreNetworkAssistanceInformationForInactive;  // Optional
  GUAMI guami;                                      // Mandatory
  std::optional<PduSessionResourceSetupListCxtReq>
      pduSessionResourceSetupRequestList;         // Optional
  AllowedNSSAI allowedNssai;                      // Mandatory
  UESecurityCapabilities uESecurityCapabilities;  // Mandatory
  SecurityKey securityKey;                        // Mandatory
  // TODO: Trace Activation (Optional)
  std::optional<MobilityRestrictionList> mobilityRestrictionList;
  std::optional<UERadioCapability> ueRadioCapability;  // Optional
  // TODO: Index to RAT/Frequency Selection Priority
  std::optional<Ngap_MaskedIMEISV_t> maskedIMEISV;  // Optional
  std::optional<NasPdu> nasPdu;                     // Optional
  // TODO: Emergency Fallback Indicator
  // TODO: RRC Inactive Transition Report Request
  // TODO: UE Radio Capability for Paging
  // TODO: Redirection for Voice EPS Fallback
  // TODO: Location Reporting Request Type
  // TODO: CN Assisted RAN Parameters Tuning
  // TODO: SRVCC Operation Possible (Optional, Rel 16.14.0)
  // TODO: IAB Authorized (Optional, Rel 16.14.0)
  // TODO: Enhanced Coverage Restriction (Optional, Rel 16.14.0)
  // TODO: Extended Connected Time (Optional, Rel 16.14.0)
  // TODO: UE Differentiation Information (Optional, Rel 16.14.0)
  // TODO: NR V2X Services Authorized (Optional, Rel 16.14.0)
  // TODO: LTE V2X Services Authorized (Optional, Rel 16.14.0)
  // TODO: NR UE Sidelink Aggregate Maximum Bit Rate (Optional, Rel 16.14.0)
  // TODO: LTE UE Sidelink Aggregate Maximum Bit Rate (Optional, Rel 16.14.0)
  // TODO: PC5 QoS Parameters (Optional, Rel 16.14.0)
  // TODO: CE-mode-B Restricted (Optional, Rel 16.14.0)
  // TODO: UE User Plane CIoT Support Indicator (Optional, Rel 16.14.0)
  // TODO: RG Level Wireline Access Characteristics (Optional, Rel 16.14.0)
  // TODO: Management Based MDT PLMN List (Optional, Rel 16.14.0)
  // TODO: UE Radio Capability ID (Optional, Rel 16.14.0)
};

}  // namespace ngap
#endif
