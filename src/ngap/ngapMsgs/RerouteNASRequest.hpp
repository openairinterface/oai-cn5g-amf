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

#include "AmfSetId.hpp"
#include "AllowedNssai.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_RerouteNASRequest.h"
}

namespace ngap {

class RerouteNASRequest : public NgapMessage {
 public:
  RerouteNASRequest();
  virtual ~RerouteNASRequest();

  void initialize();
  bool decode(Ngap_NGAP_PDU_t* ngap_msg_pdu) override;

  void setAmfUeNgapId(const unsigned long& id);  // 40 bits
  bool getAmfUeNgapId(unsigned long& id);

  void setRanUeNgapId(const uint32_t& id);  // 32 bits
  uint32_t getRanUeNgapId();

  void setNgapMessage(const OCTET_STRING_t& message);
  bool getNgapMessage(OCTET_STRING_t& message) const;

  bool setAMFSetID(const uint16_t& amf_set_id);
  void getAMFSetID(std::string& amf_set_id);

  void setAllowedNssai(const std::vector<S_Nssai>& list);
  bool getAllowedNssai(std::vector<S_Nssai>& list);

 private:
  Ngap_RerouteNASRequest_t* rerouteNASRequestIEs;

  RanUeNgapId ranUeNgapId;                   // Mandatory
  std::optional<AmfUeNgapId> amfUeNgapId;    // Optional
  OCTET_STRING_t ngapMessage;                // Mandatory
  AmfSetId amfSetID;                         // Mandatory
  std::optional<AllowedNSSAI> allowedNssai;  // Optional
  // SourceToTarget-AMFInformationReroute //Optional
};

}  // namespace ngap
#endif
