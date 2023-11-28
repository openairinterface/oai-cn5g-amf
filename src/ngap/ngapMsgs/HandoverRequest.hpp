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

#ifndef _HANDOVER_REQUEST_H_
#define _HANDOVER_REQUEST_H_

#include "AmfUeNgapId.hpp"
#include "Cause.hpp"
#include "GUAMI.hpp"
#include "MobilityRestrictionList.hpp"
#include "NgapMessage.hpp"
#include "PduSessionResourceSetupListHoReq.hpp"
#include "S-NSSAI.hpp"
#include "SecurityKey.hpp"
#include "UEAggregateMaxBitRate.hpp"
#include "UESecurityCapabilities.hpp"

extern "C" {
#include "Ngap_AllowedNSSAI-Item.h"
#include "Ngap_HandoverRequest.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class HandoverRequest : public NgapMessage {
 public:
  HandoverRequest();
  virtual ~HandoverRequest();

  void initialize();
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setAmfUeNgapId(const unsigned long& id);  // 40 bits
  unsigned long getAmfUeNgapId();

  void setHandoverType(const long& type);
  // TODO: void getHandoverType(long& type);

  void setCause(const Ngap_Cause_PR& causePresent, const long& value);
  // TODO: getCause

  void setUEAggregateMaximumBitRate(
      const long& bitRateDownlink, const long& bitRateUplink);
  // TODO: getUEAggregateMaximumBitRate

  void setUESecurityCapabilities(
      const uint16_t& nREncryptionAlgs,
      const uint16_t& nRIntegrityProtectionAlgs,
      const uint16_t& eUTRAEncryptionAlgs,
      const uint16_t& eUTRAIntegrityProtectionAlgs);
  // TODO: getUESecurityCapabilities

  void setSecurityContext(const long& count, bstring& nh);
  // TODO: getSecurityContext

  void setPduSessionResourceSetupList(
      const std::vector<PDUSessionResourceSetupRequestItem_t>& list);
  // TODO: getPduSessionResourceSetupList

  void setSourceToTarget_TransparentContainer(
      const OCTET_STRING_t& sourceTotarget);
  // TODO: getSourceToTarget_TransparentContainer

  void setAllowedNSSAI(std::vector<S_NSSAI>& list);
  // TODO: getAllowedNSSAI

  void setGUAMI(
      const PlmnId& plmnId, const AmfRegionId& aMFRegionID,
      const AmfSetId& aMFSetID, const AmfPointer& aMFPointer);
  void setGUAMI(
      const std::string& mcc, const std::string& mnc,
      const std::string& regionId, const std::string& setId,
      const std::string& pointer);
  void setGUAMI(
      const std::string& mcc, const std::string& mnc, uint8_t regionId,
      uint16_t setId, uint8_t pointer);
  // TODO: getGUAMI

  void setMobilityRestrictionList(const PlmnId& m_plmnId);
  // TODO: getMobilityRestrictionList

 private:
  Ngap_HandoverRequest_t* handoverRequestIEs;

  AmfUeNgapId amfUeNgapId;                          // Mandatory
  Ngap_HandoverType_t handoverType;                 // Mandatory
  Cause cause;                                      // Mandatory
  UEAggregateMaxBitRate ueAggregateMaximumBitRate;  // Mandatory
  // TODO: Core Network Assistance Information for RRC INACTIVE (Optional)
  UESecurityCapabilities ueSecurityCapabilities;  // Mandatory
  Ngap_SecurityContext_t securityContext;         // Mandatory
  // TODO: New Security Context Indicator (Optional)
  // TODO: NASC - NAS-PDU (Optional)
  PduSessionResourceSetupListHoReq pDUSessionResourceSetupList;  // Mandatory
  Ngap_AllowedNSSAI_t allowedNSSAI;                              // Mandatory
  // TODO: Trace Activation (Optional)
  // TODO: Masked IMEISV  (Optional)
  Ngap_SourceToTarget_TransparentContainer_t
      SourceToTarget_TransparentContainer;  // Mandatory
  std::optional<MobilityRestrictionList> mobilityRestrictionList;
  // TODO: Location Reporting Request Type (Optional)
  // TODO: RRC Inactive Transition Report Request (Optional)
  GUAMI guami;  // Mandatory
  // TODO: Redirection for Voice EPS Fallback (Optional)
  // TODO: CN Assisted RAN Parameters Tuning (Optional)
  // TODO: SRVCC Operation Possible (Optional, Rel 16.14.0)
  // TODO: IAB Authorized (Optional, Rel 16.14.0)
  // TODO: Enhanced Coverage Restriction (Optional, Rel 16.14.0)
  // TODO: UE Differentiation Information (Optional, Rel 16.14.0)
  // TODO: NR V2X Services Authorized (Optional, Rel 16.14.0)
  // TODO: LTE V2X Services Authorized (Optional, Rel 16.14.0)
  // TODO: NR UE Sidelink Aggregate Maximum Bit Rate (Optional, Rel 16.14.0)
  // TODO: LTE UE Sidelink Aggregate Maximum Bit Rate (Optional, Rel 16.14.0)
  // TODO: PC5 QoS Parameters (Optional, Rel 16.14.0)
  // TODO: CE-mode-B Restricted (Optional, Rel 16.14.0)
  // TODO: UE User Plane CIoT Support Indicator (Optional, Rel 16.14.0)
  // TODO: Management Based MDT PLMN List (Optional, Rel 16.14.0)
  // TODO: UE Radio Capability ID (Optional, Rel 16.14.0)
};

}  // namespace ngap

#endif
