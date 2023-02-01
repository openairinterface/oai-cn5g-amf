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

#ifndef _DOWNLINK_NAS_TRANSPORT_H_
#define _DOWNLINK_NAS_TRANSPORT_H_

#include "AllowedNssai.hpp"
#include "AMFName.hpp"
#include "IndexToRFSP.hpp"
#include "NAS-PDU.hpp"
#include "RANPagingPriority.hpp"
#include "NgapUEMessage.hpp"
#include "MobilityRestrictionList.hpp"
#include "UEAggregateMaxBitRate.hpp"

#include <optional>

namespace ngap {

class DownLinkNasTransportMsg : public NgapUEMessage {
 public:
  DownLinkNasTransportMsg();
  virtual ~DownLinkNasTransportMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setOldAmf(const std::string& name);
  bool getOldAmf(std::string& name);

  bool setRanPagingPriority(const uint32_t&);  // 1~256
  bool getRanPagingPriority(uint32_t&);

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu);

  void setMobilityRestrictionList(const MobilityRestrictionList&);
  bool getMobilityRestrictionList(MobilityRestrictionList&) const;

  void setUEAggregateMaxBitRate(const UEAggregateMaxBitRate& bit_rate);
  bool getUEAggregateMaxBitRate(UEAggregateMaxBitRate& bit_rate);

  void setIndex2Rat_FrequencySelectionPriority(const uint32_t& value);  // 1~256
  bool getIndex2Rat_FrequencySelectionPriority(uint32_t&) const;

  void setAllowedNssai(const AllowedNSSAI& allowed_nssai);
  bool getAllowedNssai(AllowedNSSAI& allowed_nssai) const;

 private:
  Ngap_DownlinkNASTransport_t* downLinkNasTransportIEs;

  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  std::optional<AmfName> oldAMF;                                   // Optional
  std::optional<RANPagingPriority> ranPagingPriority;              // Optional
  NAS_PDU nasPdu;                                                  // Mandatory
  std::optional<MobilityRestrictionList> mobilityRestrictionList;  // Optional
  std::optional<IndexToRFSP> indexToRFSP;                          // Optional
  std::optional<UEAggregateMaxBitRate> uEAggregateMaxBitRate;      // Optional
  std::optional<AllowedNSSAI> allowedNssai;                        // Optional
};

}  // namespace ngap
#endif
