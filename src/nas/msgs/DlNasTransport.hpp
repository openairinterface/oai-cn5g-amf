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

#ifndef _DL_NAS_TRANSPORT_H_
#define _DL_NAS_TRANSPORT_H_

#include "NasIeHeader.hpp"

namespace nas {

class DlNasTransport : public NasMmPlainHeader {
 public:
  DlNasTransport();
  ~DlNasTransport();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetPayloadContainerType(uint8_t value);

  void SetPayloadContainer(const std::vector<PayloadContainerEntry>& content);
  void SetPayloadContainer(uint8_t* buf, int len);

  void SetPduSessionId(uint8_t value);
  // TODO: Get

  void SetAdditionalInformation(const bstring& value);
  // TODO: Get

  void Set5gmmCause(uint8_t value);
  // TODO: Get

  void SetBackOffTimerValue(uint8_t unit, uint8_t value);
  // TODO: Get

 public:
  PayloadContainerType ie_payload_container_type;                  // Mandatory
  PayloadContainer ie_payload_container;                           // Mandatory
  std::optional<PduSessionIdentity2> ie_pdu_session_identity_2;    // Optional
  std::optional<AdditionalInformation> ie_additional_information;  // Optional
  std::optional<_5gmmCause> ie_5gmm_cause;                         // Optional
  std::optional<GprsTimer3> ie_back_off_timer_value;               // Optional
};

}  // namespace nas

#endif
