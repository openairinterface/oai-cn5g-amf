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

class ULNASTransport : public NasMmPlainHeader {
 public:
  ULNASTransport();
  ~ULNASTransport();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetPayloadContainerType(uint8_t value);
  uint8_t GetPayloadContainerType();

  void SetPayloadContainer(std::vector<PayloadContainerEntry> content);
  void GetPayloadContainer(std::vector<PayloadContainerEntry>& content);
  void GetPayloadContainer(bstring& content);

  void SetPduSessionIdentity2(uint8_t value);
  uint8_t GetPduSessionId();

  void SetOldPduSessionIdentity2(uint8_t value);
  bool GetOldPduSessionId(uint8_t& value);

  void SetRequestType(uint8_t value);
  bool GetRequestType(uint8_t& value);

  void SetSNssai(SNSSAI_s snssai);
  bool GetSNssai(SNSSAI_s& snssai);

  void setDNN(bstring dnn);
  bool getDnn(bstring& dnn);

  void SetAdditionalInformation(const bstring& value);

  void SetMaPduSessionInformation(uint8_t value);

  void SetReleaseAssistanceIndication(uint8_t value);

 public:
  PayloadContainerType ie_payload_container_type;  // Mandatory
  Payload_Container ie_payload_container;          // Mandatory

  std::optional<PduSessionIdentity2> ie_pdu_session_identity_2;      // Optional
  std::optional<PduSessionIdentity2> ie_old_pdu_session_identity_2;  // Optional
  std::optional<RequestType> ie_request_type;                        // Optional
  std::optional<S_NSSAI> ie_s_nssai;                                 // Optional
  std::optional<DNN> ie_dnn;                                         // Optional
  std::optional<AdditionalInformation> ie_additional_information;    // Optional
  std::optional<MaPduSessionInformation>
      ie_ma_pdu_session_information;  // Optional
  std::optional<ReleaseAssistanceIndication>
      ie_release_assistance_indication;  // Optional
};

}  // namespace nas

#endif
