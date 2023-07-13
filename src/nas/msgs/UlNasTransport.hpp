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

#ifndef _UL_NAS_TRANSPORT_H_
#define _UL_NAS_TRANSPORT_H_

#include "NasIeHeader.hpp"

namespace nas {

class UlNasTransport : public NasMmPlainHeader {
 public:
  UlNasTransport();
  ~UlNasTransport();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetPayloadContainerType(uint8_t value);
  uint8_t GetPayloadContainerType() const;

  void SetPayloadContainer(const std::vector<PayloadContainerEntry>& content);
  void GetPayloadContainer(std::vector<PayloadContainerEntry>& content) const;
  void GetPayloadContainer(bstring& content) const;

  void SetPduSessionId(uint8_t value);
  bool GetPduSessionId(uint8_t& value) const;

  void SetOldPduSessionId(uint8_t value);
  bool GetOldPduSessionId(uint8_t& value) const;

  void SetRequestType(uint8_t value);
  bool GetRequestType(uint8_t& value) const;

  void SetSNssai(const SNSSAI_s& snssai);
  bool GetSNssai(SNSSAI_s& snssai) const;

  void SetDnn(const bstring& dnn);
  bool GetDnn(bstring& dnn) const;

  void SetAdditionalInformation(const bstring& value);
  // TODO: Get

  void SetMaPduSessionInformation(uint8_t value);
  // TODO: Get

  void SetReleaseAssistanceIndication(uint8_t value);
  // TODO: Get

 public:
  PayloadContainerType ie_payload_container_type;  // Mandatory
  PayloadContainer ie_payload_container;           // Mandatory

  std::optional<PduSessionIdentity2> ie_pdu_session_id;            // Optional
  std::optional<PduSessionIdentity2> ie_old_pdu_session_id;        // Optional
  std::optional<RequestType> ie_request_type;                      // Optional
  std::optional<SNssai> ie_s_nssai;                                // Optional
  std::optional<Dnn> ie_dnn;                                       // Optional
  std::optional<AdditionalInformation> ie_additional_information;  // Optional
  std::optional<MaPduSessionInformation>
      ie_ma_pdu_session_information;  // Optional
  std::optional<ReleaseAssistanceIndication>
      ie_release_assistance_indication;  // Optional
};

}  // namespace nas

#endif
