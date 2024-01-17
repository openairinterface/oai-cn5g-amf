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

#ifndef _REGISTRATION_REQUEST_H_
#define _REGISTRATION_REQUEST_H_

#include <bstrlib.h>

#include <vector>

#include "NasIeHeader.hpp"

namespace nas {

class RegistrationRequest : public NasMmPlainHeader {
 public:
  RegistrationRequest();
  ~RegistrationRequest();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetHeader(uint8_t security_header_type);
  void GetSecurityHeaderType(uint8_t security_header_type);
  bool VerifyHeader();

  void Set5gsRegistrationType(bool is_for, uint8_t type);
  bool Get5gsRegistrationType(bool& is_for, uint8_t& reg_type);

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  bool GetNgKsi(uint8_t& ng_ksi) const;

  uint8_t GetMobileIdentityType() const;
  // TODO: SetMobileIdentityType(uint8_t);

  void SetSuciSupiFormatImsi(
      const std::string& mcc, const std::string& mnc,
      const std::string& routing_ind, uint8_t protection_sch_id,
      const std::string& msin);
  void SetSuciSupiFormatImsi(
      const std::string& mcc, const std::string& mnc,
      const std::string& routing_ind, uint8_t protection_sch_id, uint8_t hnpki,
      const std::string& msin);
  bool GetSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi) const;

  void Set5gGuti();
  std::string Get5gGuti() const;

  void SetImeiImeisv();
  void Set5gSTmsi();
  void SetAdditionalGuti(
      const std::string& mcc, const std::string& mnc, uint8_t amf_region_id,
      uint8_t amf_set_id, uint8_t amf_pointer, const std::string& _5g_tmsi);
  bool GetAdditionalGuti(nas::_5G_GUTI_t& guti) const;

  void SetNonCurrentNativeNasKSI(uint8_t tsc, uint8_t key_set_id);
  bool GetNonCurrentNativeNasKSI(uint8_t& value) const;

  // TODO: 5GMM Capability as an array[]
  void Set5gmmCapability(uint8_t value);
  bool Get5gmmCapability(uint8_t& value) const;

  void SetUeSecurityCapability(uint8_t ea, uint8_t ia);
  void SetUeSecurityCapability(
      uint8_t ea, uint8_t ia, uint8_t eea, uint8_t eia);
  bool GetUeSecurityCapability(uint8_t& ea, uint8_t& ia) const;
  // TODO: use std::optional for optional fields eea,eia
  bool GetUeSecurityCapability(
      uint8_t& ea, uint8_t& ia, uint8_t& eea, uint8_t& eia) const;
  std::optional<UeSecurityCapability> GetUeSecurityCapability() const;

  void SetRequestedNssai(const std::vector<struct SNSSAI_s>& nssai);
  bool GetRequestedNssai(std::vector<struct SNSSAI_s>& nssai) const;

  void SetUeNetworkCapability(uint8_t eea, uint8_t eia);
  bool GetS1UeNetworkCapability(uint8_t& eea, uint8_t& eia) const;

  void SetUplinkDataStatus(uint16_t value);
  bool GetUplinkDataStatus(uint16_t& value) const;

  void SetLastVisitedRegisteredTai(
      const std::string& mcc, const std::string& mnc, uint32_t tac);
  // TODO: Getter

  void SetPduSessionStatus(uint16_t value);
  uint16_t GetPduSessionStatus() const;

  void SetMicoIndication(bool sprti, bool raai);
  bool GetMicoIndication(uint8_t& sprti, uint8_t& raai) const;

  void SetUeStatus(bool n1, bool s1);
  bool GetUeStatus(uint8_t& n1_mode, uint8_t& s1_mode) const;

  void SetAllowedPduSessionStatus(uint16_t value);
  uint16_t GetAllowedPduSessionStatus() const;

  void SetUeUsageSetting(bool ues_usage_setting);
  uint8_t GetUeUsageSetting() const;

  void Set5gsDrxParameters(uint8_t value);
  uint8_t Get5gsDrxParameters() const;

  void SetEpsNasMessageContainer(const bstring& value);

  void SetLadnIndication(const std::vector<bstring>& ladn_value);
  bool GetLadnIndication(std::vector<bstring>& ladn_value) const;

  void SetPayloadContainerType(uint8_t value);
  uint8_t GetPayloadContainerType() const;

  void SetPayloadContainer(const std::vector<PayloadContainerEntry>& content);
  bool GetPayloadContainer(std::vector<PayloadContainerEntry>& content) const;

  void SetNetworkSlicingIndication(bool dcni, bool nssci);
  bool GetNetworkSlicingIndication(uint8_t& dcni, uint8_t& nssci) const;

  void Set5gsUpdateType(
      uint8_t eps_pnb_ciot, uint8_t _5gs_pnb_ciot, bool ng_ran, bool sms);
  bool Get5gsUpdateType(
      uint8_t& eps_pnb_ciot, uint8_t& _5gs_pnb_ciot, bool& ng_ran_rcu,
      bool& sms_requested) const;

  void SetNasMessageContainer(const bstring& value);
  bool GetNasMessageContainer(bstring& nas) const;

  bool GetEpsNasMessageContainer(bstring& eps_nas) const;
  // bool GetEpsNasMessageContainer(bstring& eps_nas);

  void SetEpsBearerContextsStatus(uint16_t value);
  bool GetEpsBearerContextStatus(uint16_t& value) const;

 public:
  _5gsRegistrationType ie_5gs_registration_type;  // Mandatory
  NasKeySetIdentifier ie_ng_ksi;                  // Mandatory
  _5gsMobileIdentity ie_5gs_mobile_identity;      // Mandatory

  std::optional<NasKeySetIdentifier> ie_non_current_native_nas_ksi;  // Optional
  std::optional<_5gmmCapability> ie_5g_mm_capability;                // Optional
  std::optional<UeSecurityCapability> ie_ue_security_capability;     // Optional
  std::optional<Nssai> ie_requested_nssai;                           // Optional
  std::optional<_5gsTrackingAreaIdentity>
      ie_last_visited_registered_tai;                              // Optional
  std::optional<UeNetworkCapability> ie_s1_ue_network_capability;  // Optional
  std::optional<UplinkDataStatus> ie_uplink_data_status;           // Optional
  std::optional<PduSessionStatus> ie_pdu_session_status;           // Optional
  std::optional<MicoIndication> ie_mico_indication;                // Optional
  std::optional<UeStatus> ie_ue_status;                            // Optional
  std::optional<_5gsMobileIdentity> ie_additional_guti;            // Optional
  std::optional<AllowedPduSessionStatus>
      ie_allowed_pdu_session_status;                       // Optional
  std::optional<UeUsageSetting> ie_ues_usage_setting;      // Optional
  std::optional<_5gsDrxParameters> ie_5gs_drx_parameters;  // Optional
  std::optional<EpsNasMessageContainer>
      ie_eps_nas_message_container;                               // Optional
  std::optional<LadnIndication> ie_ladn_indication;               // Optional
  std::optional<PayloadContainerType> ie_payload_container_type;  // Optional
  std::optional<PayloadContainer> ie_payload_container;           // Optional
  std::optional<NetworkSlicingIndication>
      ie_network_slicing_indication;                 // Optional
  std::optional<_5gsUpdateType> ie_5gs_update_type;  // Optional
  // TODO: Mobile station classmark 2
  // TODO: Supported codecs
  std::optional<NasMessageContainer> ie_nas_message_container;  // Optional
  std::optional<EpsBearerContextStatus>
      ie_eps_bearer_context_status;  // Optional
  // TODO: Requested extended DRX parameters
  // TODO: T3324 value
  // TODO: UE radio capability ID (Rel 16.4.1)
  // TODO: Requested mapped NSSAI (Rel 16.4.1)
  // TODO: Additional information requested (Rel 16.4.1)
  // TODO: Requested WUS assistance information (Rel 16.4.1)
  // TODO: N5GC indication (Rel 16.4.1)
  // TODO: Requested NB-N1 mode DRX parameters (Rel 16.14.0)
};

}  // namespace nas

#endif
