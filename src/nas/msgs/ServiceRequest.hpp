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

#ifndef _SERVICE_REQUEST_H_
#define _SERVICE_REQUEST_H_

#include <string>

#include "bstrlib.h"
#include "NasIeHeader.hpp"

namespace nas {

class ServiceRequest : public NasMmPlainHeader {
 public:
  ServiceRequest();
  ~ServiceRequest();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(NasMmPlainHeader* header, uint8_t* buf, int len);

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  bool getngKSI(uint8_t& ng_ksi);

  void setServiceType(uint8_t value);
  uint8_t getServiceType();

  void Set5gSTmsi(uint16_t amfSetId, uint8_t amfPointer, string tmsi);
  bool Get5gSTmsi(uint16_t& amfSetId, uint8_t& amfPointer, string& tmsi);

  void setUplink_data_status(uint16_t value);
  uint16_t getUplinkDataStatus();

  void SetPduSessionStatus(uint16_t value);
  uint16_t getPduSessionStatus();

  void setAllowed_PDU_Session_Status(uint16_t value);
  uint16_t getAllowedPduSessionStatus();

  void SetNasMessageContainer(bstring value);
  bool GetNasMessageContainer(bstring& nas);

 private:
  NasKeySetIdentifier ie_ngKSI;     // Mandatory
  ServiceType ie_service_type;      // Mandatory
  _5GSMobileIdentity ie_5g_s_tmsi;  // Mandatory

  std::optional<UplinkDataStatus> ie_uplink_data_status;  // Optional
  std::optional<PDUSessionStatus> ie_PDU_session_status;  // Optional
  std::optional<AllowedPDUSessionStatus>
      ie_allowed_PDU_session_status;                            // Optional
  std::optional<NasMessageContainer> ie_nas_message_container;  // Optional
};

}  // namespace nas

#endif
