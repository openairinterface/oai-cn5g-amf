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

#include "NasIeHeader.hpp"
#include "bstrlib.h"

namespace nas {

class ServiceRequest : public NasMmPlainHeader {
 public:
  ServiceRequest();
  ~ServiceRequest();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  void GetNgKsi(uint8_t& ng_ksi) const;

  void SetServiceType(uint8_t value);
  void GetServiceType(uint8_t& value) const;

  void Set5gSTmsi(
      uint16_t amf_set_id, uint8_t amf_pointer, const std::string& tmsi);
  bool Get5gSTmsi(
      uint16_t& amf_set_id, uint8_t& amf_pointer, std::string& tmsi) const;

  void SetUplinkDataStatus(uint16_t value);
  bool GetUplinkDataStatus(uint16_t& value) const;

  void SetPduSessionStatus(uint16_t value);
  bool GetPduSessionStatus(uint16_t& value) const;
  std::optional<uint16_t> GetPduSessionStatus() const;

  void SetAllowedPduSessionStatus(uint16_t value);
  bool GetAllowedPduSessionStatus(uint16_t& value) const;
  std::optional<uint16_t> GetAllowedPduSessionStatus() const;

  void SetNasMessageContainer(const bstring& value);
  bool GetNasMessageContainer(bstring& nas) const;

 private:
  NasKeySetIdentifier ie_ng_ksi;    // Mandatory
  ServiceType ie_service_type;      // Mandatory
  _5gsMobileIdentity ie_5g_s_tmsi;  // Mandatory

  std::optional<UplinkDataStatus> ie_uplink_data_status;  // Optional
  std::optional<PduSessionStatus> ie_pdu_session_status;  // Optional
  std::optional<AllowedPduSessionStatus>
      ie_allowed_pdu_session_status;                            // Optional
  std::optional<NasMessageContainer> ie_nas_message_container;  // Optional
};

}  // namespace nas

#endif
