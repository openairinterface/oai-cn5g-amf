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

#ifndef _DEREGISTRATION_REQUEST_UE_TERMINATED_H_
#define _DEREGISTRATION_REQUEST_UE_TERMINATED_H_

#include "NasIeHeader.hpp"

namespace nas {

class DeregistrationRequestUeTerminated : public NasMmPlainHeader {
 public:
  DeregistrationRequestUeTerminated();
  ~DeregistrationRequestUeTerminated();

  int Encode(uint8_t* buf, int len);
  int Decode(uint8_t* buf, int len);

  void SetHeader(uint8_t security_header_type);

  void SetDeregistrationType(uint8_t dereg_type);
  void GetDeregistrationType(uint8_t& dereg_type) const;

  void SetDeregistrationType(const _5gs_deregistration_type_t& type);
  void GetDeregistrationType(_5gs_deregistration_type_t& type) const;

  void Set5gmmCause(uint8_t value);
  std::optional<_5gmmCause> Get5gmmCause() const;

  void SetT3346Value(uint8_t value);
  std::optional<GprsTimer2> GetT3346Value() const;

  void SetRejectedNssai(const std::vector<RejectedSNssai>& nssai);
  std::optional<RejectedNssai> GetRejectedNssai() const;

  // TODO:CagInformationList

 public:
  _5gsDeregistrationType ie_deregistration_type;   // Mandatory
  std::optional<_5gmmCause> ie_5gmm_cause;         // Optional
  std::optional<GprsTimer2> ie_t3346_value;        // Optional
  std::optional<RejectedNssai> ie_rejected_nssai;  // Optional
  // TODO: CagInformationList ie_cag_information_list ; //Optional
};

}  // namespace nas

#endif
