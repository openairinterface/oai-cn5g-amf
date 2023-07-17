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

#ifndef _TYPE6_NAS_IE_H_
#define _TYPE6_NAS_IE_H_

#include "NasIe.hpp"

namespace nas {

class Type6NasIe : public NasIe {
 public:
  Type6NasIe();
  Type6NasIe(const uint8_t& iei);
  virtual ~Type6NasIe();

  bool Validate(const int& len) const override;
  bool ValidateHeader(const int& len) const;

  void SetIei(const uint8_t& iei);
  void SetLengthIndicator(const uint16_t& li);
  void GetLengthIndicator(uint16_t& li) const;
  uint16_t GetLengthIndicator() const;
  uint16_t GetIeLength() const;
  uint8_t GetHeaderLength() const;

  int Encode(uint8_t* buf, const int& len) override;
  int Encode(
      uint8_t* buf, const int& len,
      int& len_pos);  // Use this function to encode IE lengh later
  int Decode(
      const uint8_t* const buf, const int& len, bool is_iei = false) override;

 protected:
  std::optional<uint8_t> iei_;  // IEI present in format TLV-E
  uint16_t li_;                 // length indicator, 2 bytes
};

}  // namespace nas

#endif
