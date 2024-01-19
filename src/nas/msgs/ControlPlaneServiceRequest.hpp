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

#ifndef _CONTROL_PLANE_SERVICE_REQUEST_H_
#define _CONTROL_PLANE_SERVICE_REQUEST_H_

#include "NasIeHeader.hpp"
#include "bstrlib.h"

namespace nas {

class ControlPlaneServiceRequest : public NasMmPlainHeader {
 public:
  ControlPlaneServiceRequest();
  ~ControlPlaneServiceRequest();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetControlPlaneServiceType(uint8_t value);
  void GetControlPlaneServiceType(uint8_t& value) const;

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  void GetNgKsi(uint8_t& ng_ksi) const;

  // TODO: CIoT small data container (Optional)
  // TODO: Payload container type (Optional)
  // TODO: Payload container (Optional)
  // TODO: PDU session ID (Optional)

  void SetPduSessionStatus(uint16_t value);
  bool GetPduSessionStatus(uint16_t& value) const;
  std::optional<uint16_t> GetPduSessionStatus() const;

  // TODO: Release assistance indication (Optional)

  void SetUplinkDataStatus(uint16_t value);
  bool GetUplinkDataStatus(uint16_t& value) const;

  void SetNasMessageContainer(const bstring& value);
  bool GetNasMessageContainer(bstring& nas) const;

  // TODO: Additional information (Optional)

 private:
  ControlPlaneServiceType ie_control_plane_service_type;  // Mandatory
  NasKeySetIdentifier ie_ng_ksi;                          // Mandatory

  // TODO: CIoT small data container (Optional)
  // TODO: Payload container type (Optional)
  // TODO: Payload container (Optional)
  // TODO: PDU session ID (Optional)
  std::optional<PduSessionStatus> ie_pdu_session_status;  // Optional
  // TODO: Release assistance indication (Optional)
  std::optional<UplinkDataStatus> ie_uplink_data_status;        // Optional
  std::optional<NasMessageContainer> ie_nas_message_container;  // Optional
  // TODO: Additional information (Optional)
};

}  // namespace nas

#endif
