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

#ifndef _NGAP_IES_STRUCT_H_
#define _NGAP_IES_STRUCT_H_

#include <optional>
#include <string>
#include <vector>

#include "bstrlib.h"

extern "C" {
#include <OCTET_STRING.h>

#include "Ngap_AdditionalQosFlowInformation.h"
#include "Ngap_AssociatedQosFlowItem.h"
#include "Ngap_DelayCritical.h"
#include "Ngap_NotificationControl.h"
#include "Ngap_Pre-emptionCapability.h"
#include "Ngap_Pre-emptionVulnerability.h"
#include "Ngap_ReflectiveQosAttribute.h"
}

namespace ngap {

typedef struct S_Nssai_s {
  std::string sst;
  std::string sd;

  S_Nssai_s& operator=(const S_Nssai_s& s) {
    sst = s.sst;
    sd  = s.sd;
    return *this;
  }

  bool operator==(const struct S_Nssai_s& s) const {
    if ((s.sst == this->sst) && (s.sd.compare(this->sd) == 0)) {
      return true;
    } else {
      return false;
    }
  }
  bool operator>(const struct S_Nssai_s& s) const {
    if (this->sst.compare(s.sst) > 0) return true;
    if (this->sst.compare(s.sst) == 0) {
      if (this->sd.compare(s.sd) > 0) return true;
      if (this->sd.compare(s.sd) < 0) return false;
    }
  }
} S_Nssai;

typedef struct PlmnSliceSupport_s {
  std::string mcc;
  std::string mnc;
  std::vector<S_Nssai> sliceList;
} PlmnSliceSupport_t;

typedef struct SupportedTaItem_s {
  uint32_t tac;
  std::vector<PlmnSliceSupport_t> plmnSliceSupportList;
} SupportedTaItem_t;

typedef struct GuamiItem_s {
  std::string mcc;
  std::string mnc;
  uint8_t regionId;           // 8 bits
  uint16_t amfSetId;          // 10 bits
  uint8_t amfPointer;         // 6 bits
  std::string backupAmfName;  // optional
} GuamiItem_t;

typedef struct NrCgi_s {
  std::string mcc;
  std::string mnc;
  unsigned long nrCellId;
} NrCgi_t;

typedef struct Tai_s {
  std::string mcc;
  std::string mnc;
  uint32_t tac : 24;
} Tai_t;

typedef struct {
  uint8_t pduSessionId;
  bstring nasPdu;
  S_Nssai sNssai;
  OCTET_STRING_t pduSessionResourceSetupRequestTransfer;
} PDUSessionResourceSetupRequestItem_t;

typedef struct {
  uint8_t pduSessionId;
  bstring nasPdu;
  std::optional<S_Nssai> sNssai;
  OCTET_STRING_t pduSessionResourceModifyRequestTransfer;
} PDUSessionResourceModifyRequestItem_t;

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t pduSessionResourceModifyResponseTransfer;
  OCTET_STRING_t pduSessionResourceModifyUnsuccessfulTransfer;
} PDUSessionResourceModifyResponseItem_t;

// section 9.2.1.3 PDU Session Resource Release Command (3GPP TS 38.413 V16.0.0
// (2019-12))
typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t pduSessionResourceReleaseCommandTransfer;
} PDUSessionResourceToReleaseItem_t;

// PDU Session Resource Release Item (3GPP TS 38.413 V16.0.0 (2019-12))
typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t pduSessionResourceReleaseResponseTransfer;
} PDUSessionResourceReleasedItem_t;

typedef struct {
  uint8_t pduSessionId;
} PDUSessionResourceCxtRelCplItem_t;

typedef struct {
  std::string ipAddress;
  uint32_t gtpTeid;
} GtpTunnel_t;

typedef struct {
  long _5qi;
  long* priorityLevelQos;
  long* averagingWindow;
  long* maximumDataBurstVolume;
} NonDynamic5qi_t;

typedef struct {
  long scalar;
  long exponent;
} PacketErrorRate_t;

typedef struct {
  long priorityLevelQos;
  long packetDelayBudget;
  PacketErrorRate_t packetErrorRate;
  long* _5qi;
  e_Ngap_DelayCritical* delayCritical;
  long* averagingWindow;
  long* maximumDataBurstVolume;
} Dynamic5qi_t;

typedef struct {
  NonDynamic5qi_t* nonDynamic5qi;
  Dynamic5qi_t* dynamic5qi;
} QosCharacteristics_t;

typedef struct {
  long priorityLevelArp;
  e_Ngap_Pre_emptionCapability pre_emptionCapability;
  e_Ngap_Pre_emptionVulnerability pre_emptionVulnerability;
} AllocationAndRetentionPriority_t;

typedef struct {
  long maximumFlowBitRateDl;
  long maximumFlowBitRateUl;
  long guaranteedFlowBitRateDl;
  long guaranteedFlowBitRateUl;
  e_Ngap_NotificationControl* notificationControl;
  long* maximumPacketLossRateDl;
  long* maximumPacketLossRateUl;
} GBR_QosInformation_t;

typedef struct {
  QosCharacteristics_t qosc;
  AllocationAndRetentionPriority_t arp;
  GBR_QosInformation_t* gbrQosInformation;
  e_Ngap_ReflectiveQosAttribute* reflectiveQosAttribute;
  e_Ngap_AdditionalQosFlowInformation* additionalQosFlowInformation;
} QosFlowLevelQosParameters_t;

typedef struct {
  long qosFlowId;
  QosFlowLevelQosParameters_t qflqp;
} QosFlowSetupReq_t;

typedef struct {
  long qosFlowIdentifier;
  e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication*
      qosFlowMappingIndication;
} AssociatedQosFlow_t;

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t pduSessionResourceSetupResponseTransfer;
} PDUSessionResourceSetupResponseItem_t;

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t pduSessionResourceSetupUnsuccessfulTransfer;
} PDUSessionResourceFailedToSetupItem_t;

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t pduSessionResourceReleaseCommandTransfer;
} PDUSessionResourceReleaseCommandItem_t;

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t handoverRequiredTransfer;
} PDUSessionResourceItem_t;

typedef struct {
  uint8_t pduSessionId;
  S_Nssai sNssai;
  OCTET_STRING_t pduSessionResourceSetupRequestTransfer;
} PDUSessionResourceSetupItem_t;
typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t handoverRequestAcknowledgeTransfer;
} PDUSessionResourceAdmittedItem_t;

typedef struct {
  Ngap_QosFlowIdentifier_t qosFlowIdentifier;
} QosFlowLItemWithDataForwarding_t;

typedef struct {
  long qfi;
} QosFlowToBeForwardedItem_t;

typedef struct gNBId_s {
  uint32_t id;
  uint8_t bitLength;
} gNBId_t;  // 22bits to 32bits

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t handoverCommandTransfer;
} PDUSessionResourceHandoverItem_t;

}  // namespace ngap

#endif
