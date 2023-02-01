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

#include "AMFName.hpp"
#include "AllowedNssai.hpp"
#include "CoreNetworkAssistanceInformation.hpp"
#include "GUAMI.hpp"
#include "PDUSessionResourceSetupListCxtReq.hpp"
#include "SecurityKey.hpp"
#include "UEAggregateMaxBitRate.hpp"
#include "UERadioCapability.hpp"
#include "UESecurityCapabilities.hpp"
#include "NgapUEMessage.hpp"

#include <optional>

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
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

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

  void setGuami(const Guami_t& value);
  bool getGuami(Guami_t& value);

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

  void setUERadioCapability(const bstring& ue_radio_capability);
  void getUERadioCapability(bstring& ue_radio_capability);

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu);

 private:
  Ngap_InitialContextSetupRequest_t* initialContextSetupRequestIEs;

  std::optional<AmfName> oldAMF;                               // Optional
  std::optional<UEAggregateMaxBitRate> uEAggregateMaxBitRate;  // Conditional
  std::optional<CoreNetworkAssistanceInfo>
      coreNetworkAssistanceInfo;  // Optional
  GUAMI guami;                    // Mandatory
  std::optional<PDUSessionResourceSetupListCxtReq>
      pduSessionResourceSetupRequestList;         // Optional
  AllowedNSSAI allowedNssai;                      // Mandatory
  UESecurityCapabilities uESecurityCapabilities;  // Mandatory
  SecurityKey securityKey;                        // Mandatory
  // TODO: Trace Activation (Optional)
  // TODO: Mobility Restriction List
  std::optional<UERadioCapability> ueRadioCapability;  // Optional
  // TODO: Index to RAT/Frequency Selection Priority
  // TODO: Masked IMEISV
  std::optional<NAS_PDU> nasPdu;  // Optional
  // TODO: Emergency Fallback Indicator
  // TODO: RRC Inactive Transition Report Request
  // TODO: UE Radio Capability for Paging
  // TODO: Redirection for Voice EPS Fallback
  // TODO: Location Reporting Request Type
  // TODO: CN Assisted RAN Parameters Tuning
};

}  // namespace ngap
#endif
