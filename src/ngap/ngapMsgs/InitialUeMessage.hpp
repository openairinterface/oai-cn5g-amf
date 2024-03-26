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

#ifndef _INITIAL_UE_MESSAGE_H_
#define _INITIAL_UE_MESSAGE_H_

#include <optional>

#include "AllowedNssai.hpp"
#include "FiveGSTmsi.hpp"
#include "NasPdu.hpp"
#include "NgapIesStruct.hpp"
#include "NgapMessage.hpp"
#include "RrcEstablishmentCause.hpp"
#include "UeContextRequest.hpp"
#include "UserLocationInformation.hpp"

namespace ngap {

class InitialUeMessageMsg : public NgapMessage {
 public:
  InitialUeMessageMsg();
  virtual ~InitialUeMessageMsg();

  void initialize();
  bool decode(Ngap_NGAP_PDU_t* ngap_msg_pdu) override;

  void setRanUeNgapId(const uint32_t& value);
  bool getRanUENgapID(uint32_t& value) const;

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu) const;

  void setUserLocationInfoNr(
      const struct NrCgi_s& cig, const struct Tai_s& tai);
  bool getUserLocationInfoNr(struct NrCgi_s& cig, struct Tai_s& tai) const;

  void setRrcEstablishmentCause(const e_Ngap_RRCEstablishmentCause& cause);
  int getRrcEstablishmentCause() const;

  // TODO: void set5GS_TMSI(string amfSetId, string amfPointer, string
  // _5g_tmsi);
  bool get5GSTmsi(std::string& _5GsTmsi) const;
  bool get5GSTmsi(
      std ::string& setid, std ::string& pointer, std ::string& tmsi) const;

  bool getAmfSetId(uint16_t&) const;
  bool getAmfSetId(std::string&) const;
  bool setAmfSetId(uint16_t);

  void setUeContextRequest(const e_Ngap_UEContextRequest& ueCtxReq);
  int getUeContextRequest() const;

  void setAllowedNssai(const AllowedNSSAI& allowedNssai);
  bool getAllowedNssai(AllowedNSSAI& allowedNssai) const;

 private:
  Ngap_InitialUEMessage_t* m_InitialUEMessageIes;

  RanUeNgapId m_RanUeNgapId;                           // Mandatory
  NasPdu m_NasPdu;                                     // Mandatory
  UserLocationInformation m_UserLocationInformation;   // Mandatory
  RrcEstablishmentCause m_RrcEstablishmentCause;       // Mandatory
  std::optional<FiveGSTmsi> m_FiveGSTmsi;              // 5G-S-TMSI (Optional)
  std::optional<AmfSetId> m_AmfSetId;                  // Optional
  std::optional<UeContextRequest> m_UeContextRequest;  // Optional
  std::optional<AllowedNSSAI> m_AllowedNssai;          // Optional
  // TODO: Source to Target AMF Information Reroute (Optional)
  // TODO: Selected PLMN Identity (Optional, Rel 16.14.0)
  // TODO: IAB Node Indication (Optional, Rel 16.14.0)
  // TODO: CE-mode-B Support Indicator (Optional, Rel 16.14.0)
  // TODO: LTE-M Indication (Optional, Rel 16.14.0)
  // TODO: EDT Session (Optional, Rel 16.14.0)
  // TODO: Authenticated Indication (Optional, Rel 16.14.0)
  // TODO: NPN Access Information (Optional, Rel 16.14.0)
};

}  // namespace ngap
#endif
