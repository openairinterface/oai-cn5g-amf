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

#ifndef _DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT_H_
#define _DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT_H_

#include "AmfUeNgapId.hpp"
#include "MessageType.hpp"
#include "NasPdu.hpp"
#include "NgapUeMessage.hpp"
#include "RanPagingPriority.hpp"
#include "RanUeNgapId.hpp"

extern "C" {
#include "Ngap_DownlinkUEAssociatedNRPPaTransport.h"
}

namespace ngap {

class DownlinkUeAssociatedNrppaTransportMsg : public NgapUeMessage {
 public:
  DownlinkUeAssociatedNrppaTransportMsg();
  virtual ~DownlinkUeAssociatedNrppaTransportMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setRoutingId(const bstring& pdu);
  void getRoutingId(bstring& pdu) const;

  void setNrppaPdu(const bstring& pdu);
  void getNrppaPdu(bstring& pdu) const;

 private:
  Ngap_DownlinkUEAssociatedNRPPaTransport_t*
      m_DownlinkUeAssociatedNrppaTransportIes;

  // AMF_UE_NGAP_ID  //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  bstring m_RoutingId;  // Mandatory
  bstring m_NrppaPdu;   // Mandatory
};

}  // namespace ngap
#endif
