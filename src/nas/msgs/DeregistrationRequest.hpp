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

#ifndef _DEREGISTRATION_REQUEST_H_
#define _DEREGISTRATION_REQUEST_H_

#include "NasIeHeader.hpp"

namespace nas {

class DeregistrationRequest : public NasMmPlainHeader {
 public:
  DeregistrationRequest();
  ~DeregistrationRequest();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetHeader(uint8_t security_header_type);

  void SetDeregistrationType(uint8_t dereg_type);
  void GetDeregistrationType(uint8_t& dereg_type) const;

  void SetDeregistrationType(const _5gs_deregistration_type_t& type);
  void GetDeregistrationType(_5gs_deregistration_type_t& type) const;

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  bool GetNgKsi(uint8_t& ng_ksi) const;

  void SetMobilityIdentityType(uint8_t type);
  void GetMobilityIdentityType(uint8_t& type) const;

  void SetSuciSupiFormatImsi(
      const std::string& mcc, const std::string& mnc,
      const std::string& routing_ind, uint8_t protection_sch_id,
      const std::string& msin);
  void SetSuciSupiFormatImsi(
      const std::string& mcc, const std::string& mnc,
      const std::string& routing_ind, uint8_t protection_sch_id, uint8_t hnpki,
      const std::string& msin);
  bool GetSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi) const;

  void Set5gGuti();
  std::string Get5gGuti() const;

  void SetImeiImeisv();
  // TODO: Get

  void Set5gSTmsi();
  // TODO: Get

 public:
  _5gsDeregistrationType ie_deregistrationtype;  // Mandatory
  NasKeySetIdentifier ie_ng_ksi;                 // Mandatory
  _5gsMobileIdentity ie_5gs_mobility_id;         // Mandatory
};

}  // namespace nas

#endif
