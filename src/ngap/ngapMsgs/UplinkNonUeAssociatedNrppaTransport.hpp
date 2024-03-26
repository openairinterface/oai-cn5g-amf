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

#ifndef _UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT_H_
#define _UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT_H_

#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_UplinkNonUEAssociatedNRPPaTransport.h"
}

namespace ngap {

class UplinkNonUeAssociatedNrppaTransportMsg : public NgapMessage {
 public:
  UplinkNonUeAssociatedNrppaTransportMsg();
  virtual ~UplinkNonUeAssociatedNrppaTransportMsg();

  void initialize();

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setRoutingId(const OCTET_STRING_t& pdu);
  void getRoutingId(OCTET_STRING_t& pdu) const;
  OCTET_STRING_t getRoutingId() const;

  void setNrppaPdu(const OCTET_STRING_t& pdu);
  void getNrppaPdu(OCTET_STRING_t& pdu) const;
  OCTET_STRING_t getNrppaPdu() const;

 private:
  Ngap_UplinkNonUEAssociatedNRPPaTransport_t*
      m_UplinkNonUeAssociatedNrppaTransportIes;

  OCTET_STRING_t m_RoutingId;  // Mandatory
  OCTET_STRING_t m_NrppaPdu;   // Mandatory
};

}  // namespace ngap
#endif
