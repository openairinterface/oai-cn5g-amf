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

#ifndef _PDU_SESSION_RESOURCE_SETUP_REQUEST_H_
#define _PDU_SESSION_RESOURCE_SETUP_REQUEST_H_

#include <optional>

#include "NgapUeMessage.hpp"
#include "PduSessionResourceSetupListSUReq.hpp"
#include "RanPagingPriority.hpp"
#include "UeAggregateMaxBitRate.hpp"

extern "C" {
#include "Ngap_InitialContextSetupRequest.h"
}

namespace ngap {

class PduSessionResourceSetupRequestMsg : public NgapUeMessage {
 public:
  PduSessionResourceSetupRequestMsg();
  virtual ~PduSessionResourceSetupRequestMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setRanPagingPriority(const uint32_t& priority);
  bool getRanPagingPriority(uint32_t& priority) const;

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu) const;

  void setPduSessionResourceSetupRequestList(
      const std::vector<PDUSessionResourceSetupRequestItem_t>& list);
  bool getPduSessionResourceSetupRequestList(
      std::vector<PDUSessionResourceSetupRequestItem_t>& list) const;

  void setUeAggregateMaxBitRate(
      const uint64_t& bitRateDl, const uint64_t& bitRateUl);
  bool getUeAggregateMaxBitRate(uint64_t& bitRateDl, uint64_t& bitRateUl) const;

 private:
  Ngap_PDUSessionResourceSetupRequest_t* m_PduSessionResourceSetupRequestIes;

  std::optional<RanPagingPriority> m_RanPagingPriority;  // Optional
  std::optional<NasPdu> m_NasPdu;                        // Optional
  PduSessionResourceSetupListSUReq
      m_PduSessionResourceSetupRequestList;                      // Mandatory
  std::optional<UeAggregateMaxBitRate> m_UeAggregateMaxBitRate;  // Optional
};

}  // namespace ngap
#endif
