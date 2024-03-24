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

#ifndef _REROUTE_NAS_REQUEST_H_
#define _REROUTE_NAS_REQUEST_H_

#include "AllowedNssai.hpp"
#include "AmfSetId.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_RerouteNASRequest.h"
}

namespace ngap {

class RerouteNasRequest : public NgapMessage {
 public:
  RerouteNasRequest();
  virtual ~RerouteNasRequest();

  void initialize();
  bool decode(Ngap_NGAP_PDU_t* ngap_msg_pdu) override;

  void setAmfUeNgapId(const unsigned long& id);  // 40 bits
  bool getAmfUeNgapId(unsigned long& id) const;

  void setRanUeNgapId(const uint32_t& id);  // 32 bits
  uint32_t getRanUeNgapId() const;

  void setNgapMessage(const OCTET_STRING_t& message);
  bool getNgapMessage(OCTET_STRING_t& message) const;

  bool setAmfSetId(const uint16_t& amfSetId);
  void getAmfSetId(std::string& amfSetId) const;

  void setAllowedNssai(const std::vector<S_Nssai>& list);
  bool getAllowedNssai(std::vector<S_Nssai>& list) const;

 private:
  Ngap_RerouteNASRequest_t* m_RerouteNASRequestIes;

  RanUeNgapId m_RanUeNgapId;                   // Mandatory
  std::optional<AmfUeNgapId> m_AmfUeNgapId;    // Optional
  OCTET_STRING_t m_NgapMessage;                // Mandatory
  AmfSetId m_AmfSetId;                         // Mandatory
  std::optional<AllowedNSSAI> m_AllowedNssai;  // Optional
  // SourceToTarget-AMFInformationReroute //Optional
};

}  // namespace ngap
#endif
