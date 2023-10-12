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

#ifndef _UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT_H_
#define _UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT_H_

#include "NgapUEMessage.hpp"

extern "C" {
#include "Ngap_UplinkUEAssociatedNRPPaTransport.h"
}

namespace ngap {

class UplinkUEAssociatedNRPPaTransportMsg : public NgapUEMessage {
 public:
  UplinkUEAssociatedNRPPaTransportMsg();
  virtual ~UplinkUEAssociatedNRPPaTransportMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setRoutingId(const OCTET_STRING_t& pdu);
  void getRoutingId(OCTET_STRING_t& pdu) const;
  OCTET_STRING_t getRoutingId() const;

  void setNRPPaPdu(const OCTET_STRING_t& pdu);
  void getNRPPaPdu(OCTET_STRING_t& pdu) const;
  OCTET_STRING_t getNRPPaPdu() const;

 private:
  Ngap_UplinkUEAssociatedNRPPaTransport_t* uplinkUEAssociatedNRPPaTransportIEs;

  // AMF_UE_NGAP_ID  //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  OCTET_STRING_t routingID;  // Mandatory
  OCTET_STRING_t nRPPaPDU;   // Mandatory
};

}  // namespace ngap
#endif
