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

#ifndef _UPLINK_NAS_TRANSPORT_H_
#define _UPLINK_NAS_TRANSPORT_H_

#include "NasPdu.hpp"
#include "NgapUeMessage.hpp"
#include "UserLocationInformation.hpp"

extern "C" {
#include "Ngap_UplinkNASTransport.h"
}

namespace ngap {

class UplinkNasTransportMsg : public NgapUeMessage {
 public:
  UplinkNasTransportMsg();
  virtual ~UplinkNasTransportMsg();

  void initialize();

  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setNasPdu(const bstring& pdu);
  bool getNasPdu(bstring& pdu) const;

  void setUserLocationInfoNr(const NrCgi_t& cig, const Tai_t& tai);
  bool getUserLocationInfoNr(NrCgi_t& cig, Tai_t& tai) const;

 private:
  Ngap_UplinkNASTransport_t* m_UplinkNasTransportIes;
  // AMF_UE_NGAP_ID //Mandatory
  // RAN_UE_NGAP_ID //Mandatory
  NasPdu m_NasPdu;                                    // Mandatory
  UserLocationInformation m_UserLocationInformation;  // Mandatory
  // TODO: W-AGF Identity Information (Optional, Rel 16.14.0)
  // TODO: TNGF Identity Information (Optional, Rel 16.14.0)
  // TODO: TWIF Identity Information (Optional, Rel 16.14.0)
};

}  // namespace ngap
#endif
