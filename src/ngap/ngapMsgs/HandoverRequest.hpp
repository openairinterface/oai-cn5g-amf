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

#include "AMF-UE-NGAP-ID.hpp"
#include "Cause.hpp"
#include "UEAggregateMaxBitRate.hpp"
#include "NgapMessage.hpp"
#include "GUAMI.hpp"
#include "MobilityRestrictionList.hpp"
#include "PDUSessionResourceSetupListHOReq.hpp"
#include "S-NSSAI.hpp"
#include "SecurityKey.hpp"
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
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

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
      const PlmnId& plmnId, const AMFRegionID& aMFRegionID,
      const AMFSetID& aMFSetID, const AMFPointer& aMFPointer);
  void setGUAMI(
      const std::string& mcc, const std::string& mnc,
      const std::string& regionId, const std::string& setId,
      const std::string& pointer);
  // TODO: getGUAMI

  void setMobilityRestrictionList(const PlmnId& m_plmnId);
  // TODO: getMobilityRestrictionList

 private:
  Ngap_HandoverRequest_t* handoverRequestIEs;

  AMF_UE_NGAP_ID amfUeNgapId;                       // Mandatory
  Ngap_HandoverType_t handoverType;                 // Mandatory
  Cause cause;                                      // Mandatory
  UEAggregateMaxBitRate ueAggregateMaximumBitRate;  // Mandatory
  // Core Network Assistance Information for RRC INACTIVE (TODO://Optional)
  UESecurityCapabilities ueSecurityCapabilities;  // Mandatory
  Ngap_SecurityContext_t securityContext;         // TODO: Mandatory
  // New Security Context Indicator (TODO: Optional)
  // NASC - NAS-PDU (TODO: Optional)
  PDUSessionResourceSetupListHOReq pDUSessionResourceSetupList;  // Mandatory
  Ngap_AllowedNSSAI_t allowedNSSAI;  // TODO: Mandatory
  // Trace Activation (TODO: Optional)
  // Masked IMEISV  (TODO: Optional)
  Ngap_SourceToTarget_TransparentContainer_t
      SourceToTarget_TransparentContainer;  // TODO: Mandatory
  std::optional<MobilityRestrictionList>
      mobilityRestrictionList;  // TODO: Optional
  // Location Reporting Request Type (TODO: Optional)
  // RRC Inactive Transition Report Request (TODO: Optional)
  GUAMI guami;  // Mandatory
  // Redirection for Voice EPS Fallback // Optional
  // CN Assisted RAN Parameters Tuning (TODO: Optional)
};

}  // namespace ngap

#endif
