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

#include <optional>

#include "AllowedNssai.hpp"
#include "AmfName.hpp"
#include "IndexToRfsp.hpp"
#include "MobilityRestrictionList.hpp"
#include "NasPdu.hpp"
#include "NgapUeMessage.hpp"
#include "RanPagingPriority.hpp"
#include "UeAggregateMaxBitRate.hpp"

namespace ngap {

class DownLinkNasTransportMsg : public NgapUeMessage {
 public:
  DownLinkNasTransportMsg();
  virtual ~DownLinkNasTransportMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setOldAmf(const std::string& name);
  bool getOldAmf(std::string& name) const;

  bool setRanPagingPriority(const uint32_t&);  // 1~256
  bool getRanPagingPriority(uint32_t&) const;

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu) const;

  void setMobilityRestrictionList(const MobilityRestrictionList&);
  bool getMobilityRestrictionList(MobilityRestrictionList&) const;

  void setUeAggregateMaxBitRate(const UeAggregateMaxBitRate& bitRate);
  bool getUeAggregateMaxBitRate(UeAggregateMaxBitRate& bitRate) const;

  void setIndex2RatFrequencySelectionPriority(const uint32_t& value);  // 1~256
  bool getIndex2RatFrequencySelectionPriority(uint32_t&) const;

  void setAllowedNssai(const AllowedNSSAI& allowedNssai);
  bool getAllowedNssai(AllowedNSSAI& allowedNssai) const;

 private:
  Ngap_DownlinkNASTransport_t* m_DownLinkNasTransportIes;

  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  std::optional<AmfName> m_OldAmf;                       // Optional
  std::optional<RanPagingPriority> m_RanPagingPriority;  // Optional
  NasPdu m_NasPdu;                                       // Mandatory
  std::optional<MobilityRestrictionList> m_MobilityRestrictionList;  // Optional
  std::optional<IndexToRfsp> m_IndexToRfsp;                          // Optional
  std::optional<UeAggregateMaxBitRate> m_UeAggregateMaxBitRate;      // Optional
  std::optional<AllowedNSSAI> m_AllowedNssai;                        // Optional
  // TODO: SRVCC Operation Possible (Optional, Rel 16.14.0)
  // TODO: Enhanced Coverage Restriction (Optional, Rel 16.14.0)
  // TODO: Extended Connected Time (Optional, Rel 16.14.0)
  // TODO: UE Differentiation Information (Optional, Rel 16.14.0)
  // TODO: CE-mode-B Restricted (Optional, Rel 16.14.0)
  // TODO: UE Radio Capability (Optional, Rel 16.14.0)
  // TODO: UE Capability Info Request (Optional, Rel 16.14.0)
  // TODO: End Indication (Optional, Rel 16.14.0)
  // TODO: UE Radio Capability ID (Optional, Rel 16.14.0)
  // TODO: Masked IMEISV (Optional, Rel 16.14.0)
};

}  // namespace ngap
#endif
