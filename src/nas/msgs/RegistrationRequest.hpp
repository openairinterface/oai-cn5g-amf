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

#include "NasIeHeader.hpp"

#include <bstrlib.h>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;
namespace nas {

class RegistrationRequest : public NasMmPlainHeader {
 public:
  RegistrationRequest();
  ~RegistrationRequest();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetHeader(uint8_t security_header_type);
  void GetSecurityHeaderType(uint8_t security_header_type);
  bool verifyHeader();

  void set5gsRegistrationType(bool is_for, uint8_t type);
  bool get5gsRegistrationType(bool& is_for, uint8_t& reg_type /*3bits*/);

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  bool getngKSI(uint8_t& ng_ksi);

  uint8_t getMobileIdentityType();
  // TODO: SetMobileIdentityType(uint8_t);

  void SetSuciSupiFormatImsi(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, const string msin);
  void SetSuciSupiFormatImsi(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, uint8_t hnpki, const string msin);
  bool getSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi);

  void Set5gGuti();
  std::string Get5gGuti();

  void SetImeiImeisv();
  void Set5gSTmsi();
  void setAdditional_GUTI_SUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, uint8_t amf_region_id,
      uint8_t amf_set_id, uint8_t amf_pointer, const string _5g_tmsi);
  bool getAdditionalGuti(nas::_5G_GUTI_t& guti);

  void setNonCurrentNativeNasKSI(uint8_t tsc, uint8_t key_set_id);
  bool getNonCurrentNativeNasKSI(uint8_t& value) const;

  void set5GMMCapability(uint8_t value);
  bool get5GMMCapability(uint8_t& value);

  void setUESecurityCapability(uint8_t g_EASel, uint8_t g_IASel);
  void setUESecurityCapability(
      uint8_t g_EASel, uint8_t g_IASel, uint8_t eea, uint8_t eia);
  bool getUeSecurityCapability(uint8_t& ea, uint8_t& ia);
  // TODO: use std::optional for optional fields eea,eia
  bool getUeSecurityCapability(
      uint8_t& ea, uint8_t& ia, uint8_t& eea, uint8_t& eia);

  void setRequestedNSSAI(std::vector<struct SNSSAI_s> nssai);
  bool getRequestedNssai(std::vector<struct SNSSAI_s>& nssai);

  void setUENetworkCapability(uint8_t g_EEASel, uint8_t g_EIASel);
  bool getS1UeNetworkCapability(uint8_t& eea, uint8_t& eia);

  void setUplinkDataStatus(const uint16_t& value);
  bool getUplinkDataStatus(uint16_t& value);

  void setLastVisitedRegisteredTAI(
      const std::string& mcc, const std::string mnc, const uint32_t& tac);
  // TODO: Getter

  void setPDUSessionStatus(uint16_t value);
  uint16_t getPduSessionStatus();

  void setMICOIndication(bool sprti, bool raai);
  bool getMicoIndication(uint8_t& sprti, uint8_t& raai);

  void setUEStatus(bool n1, bool s1);
  bool getUeStatus(uint8_t& n1ModeReg, uint8_t& s1ModeReg);

  void setAllowedPDUSessionStatus(uint16_t value);
  uint16_t getAllowedPduSessionStatus();

  void setUES_Usage_Setting(bool ues_usage_setting);
  uint8_t getUEsUsageSetting();

  void Set5gsDrxParameters(uint8_t value);
  uint8_t get5GSDrxParameters();

  void setEPS_NAS_Message_Container(bstring value);

  void setLADN_Indication(std::vector<bstring> ladnValue);
  bool getLadnIndication(std::vector<bstring>& ladnValue);

  void SetPayloadContainerType(uint8_t value);
  uint8_t GetPayloadContainerType();

  void SetPayloadContainer(std::vector<PayloadContainerEntry> content);
  bool GetPayloadContainer(std::vector<PayloadContainerEntry>& content);

  void SetNetworkSlicingIndication(bool dcni, bool nssci);
  bool getNetworkSlicingIndication(uint8_t& dcni, uint8_t& nssci);

  void set_5GS_Update_Type(
      uint8_t eps_pnb_ciot, uint8_t _5gs_pnb_ciot, bool ng_ran, bool sms);
  bool get5GSUpdateType(
      uint8_t& eps_pnb_ciot, uint8_t& _5gs_pnb_ciot, bool& ng_ran_rcu,
      bool& sms_requested);

  void SetNasMessageContainer(bstring value);
  bool GetNasMessageContainer(bstring& nas);

  bool getEpsNasMessageContainer(bstring& epsNas);
  // bool getEpsNasMessageContainer(bstring& epsNas);

  void SetEpsBearerContextsStatus(uint16_t value);
  bool getEpsBearerContextStatus(uint16_t& value);

 public:
  _5GSRegistrationType ie_5gs_registration_type;  // Mandatory
  NasKeySetIdentifier ie_ngKSI;                   // Mandatory
  _5GSMobileIdentity ie_5gs_mobile_identity;      // Mandatory

  std::optional<NasKeySetIdentifier> ie_non_current_native_nas_ksi;  // Optional
  std::optional<_5GMMCapability> ie_5g_mm_capability;                // Optional
  std::optional<UESecurityCapability> ie_ue_security_capability;     // Optional
  std::optional<NSSAI> ie_requested_NSSAI;                           // Optional
  std::optional<_5GSTrackingAreaIdentity>
      ie_last_visited_registered_TAI;                              // Optional
  std::optional<UENetworkCapability> ie_s1_ue_network_capability;  // Optional
  std::optional<UplinkDataStatus> ie_uplink_data_status;           // Optional
  std::optional<PDUSessionStatus> ie_PDU_session_status;           // Optional
  std::optional<MicoIndication> ie_MICO_indication;                // Optional
  std::optional<UEStatus> ie_ue_status;                            // Optional
  std::optional<_5GSMobileIdentity> ie_additional_guti;            // Optional
  std::optional<AllowedPDUSessionStatus>
      ie_allowed_PDU_session_status;                         // Optional
  std::optional<UEUsageSetting> ie_ues_usage_setting;        // Optional
  std::optional<_5GS_DRX_Parameters> ie_5gs_drx_parameters;  // Optional
  std::optional<EPS_NAS_Message_Container>
      ie_eps_nas_message_container;                               // Optional
  std::optional<LadnIndication> ie_ladn_indication;               // Optional
  std::optional<PayloadContainerType> ie_payload_container_type;  // Optional
  std::optional<Payload_Container> ie_payload_container;          // Optional
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
};

}  // namespace nas

#endif
