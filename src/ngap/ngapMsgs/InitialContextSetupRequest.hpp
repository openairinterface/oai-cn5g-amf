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
#include "AllowedNssai.hpp"
#include "AmfName.hpp"
#include "CoreNetworkAssistanceInformationForInactive.hpp"
#include "Guami.hpp"
#include "MobilityRestrictionList.hpp"
#include "NgapUeMessage.hpp"
#include "PduSessionResourceSetupListCxtReq.hpp"
#include "SecurityKey.hpp"
#include "UeAggregateMaxBitRate.hpp"
#include "UeRadioCapability.hpp"
#include "UeSecurityCapabilities.hpp"

extern "C" {
#include "Ngap_InitialContextSetupRequest.h"
}

namespace ngap {

class InitialContextSetupRequestMsg : public NgapUeMessage {
 public:
  InitialContextSetupRequestMsg();
  virtual ~InitialContextSetupRequestMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setOldAmf(const std::string& name);
  bool getOldAmf(std::string& name) const;

  void setUeAggregateMaxBitRate(
      const uint64_t& bitRateDl, const uint64_t& bitRateUl);
  bool getUeAggregateMaxBitRate(uint64_t& bitRateDl, uint64_t& bitRateUl) const;

  void setUeAggregateMaxBitRate(const UeAggregateMaxBitRate& bitRate);
  bool getUeAggregateMaxBitRate(UeAggregateMaxBitRate& bitRate) const;

  void setCoreNetworkAssistanceInfo(
      uint16_t ue_identity_index_value_value,
      e_Ngap_PagingDRX ue_specific_drx_value,
      uint8_t periodic_reg_update_timer_value, bool mico_mode_ind_value,
      const std::vector<Tai_t>& tai_list_for_rrc_inactive);

  bool getCoreNetworkAssistanceInfo(
      uint16_t& ue_identity_index_value_value, int& ue_specific_drx_value,
      uint8_t& periodic_reg_update_timer_value, bool& mico_mode_ind_value,
      std::vector<Tai_t>& tai_list_for_rrc_inactive) const;

  void setGuami(const guami_full_format_t& value);
  bool getGuami(guami_full_format_t& value) const;

  void setPduSessionResourceSetupRequestList(
      const std::vector<PDUSessionResourceSetupRequestItem_t>& list);
  bool getPduSessionResourceSetupRequestList(
      std::vector<PDUSessionResourceSetupRequestItem_t>& list) const;

  void setAllowedNssai(const std::vector<S_Nssai>& list);
  bool getAllowedNssai(std::vector<S_Nssai>& list) const;

  void setUeSecurityCapability(
      uint16_t nr_encryption_algs, uint16_t integrityProtectionAlgorithms,
      uint16_t eutraEncryptionAlgorithms,
      uint16_t eutraIntegrityProtectionAlgorithms);
  bool getUeSecurityCapability(
      uint16_t& nr_encryption_algs, uint16_t& integrityProtectionAlgorithms,
      uint16_t& eutraEncryptionAlgorithms,
      uint16_t& eutraIntegrityProtectionAlgorithms) const;

  void setSecurityKey(uint8_t* key);         // 256bits
  bool getSecurityKey(uint8_t*& key) const;  // 256bits

  void setMobilityRestrictionList(const PlmnId& plmn_id);
  // TODO: getMobilityRestrictionList

  void setUeRadioCapability(const bstring& ue_radio_capability);
  void getUeRadioCapability(bstring& ue_radio_capability) const;

  void setMaskedImeisv(const std::string& imeisv);
  // bool getMaskedIMEISV();

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu) const;

 private:
  Ngap_InitialContextSetupRequest_t* m_InitialContextSetupRequestIes;

  std::optional<AmfName> m_OldAmf;                               // Optional
  std::optional<UeAggregateMaxBitRate> m_UeAggregateMaxBitRate;  // Conditional
  std::optional<CoreNetworkAssistanceInformationForInactive>
      m_CoreNetworkAssistanceInformationForInactive;  // Optional
  Guami m_Guami;                                      // Mandatory
  std::optional<PduSessionResourceSetupListCxtReq>
      m_PduSessionResourceSetupRequestList;         // Optional
  AllowedNSSAI m_AllowedNssai;                      // Mandatory
  UeSecurityCapabilities m_UeSecurityCapabilities;  // Mandatory
  SecurityKey m_SecurityKey;                        // Mandatory
  // TODO: Trace Activation (Optional)
  std::optional<MobilityRestrictionList> m_MobilityRestrictionList;
  std::optional<UeRadioCapability> m_UeRadioCapability;  // Optional
  // TODO: Index to RAT/Frequency Selection Priority
  std::optional<Ngap_MaskedIMEISV_t> m_MaskedImeiSv;  // Optional
  std::optional<NasPdu> m_NasPdu;                     // Optional
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
