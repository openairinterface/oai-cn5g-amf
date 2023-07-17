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

#ifndef _NAS_KEY_SET_IDENTIFIER_H
#define _NAS_KEY_SET_IDENTIFIER_H

#include "Type1NasIe.hpp"

constexpr auto kNasKeySetIdentifierName = "NAS Key Set Identifier";

namespace nas {

class NasKeySetIdentifier : public Type1NasIe {
 public:
  NasKeySetIdentifier();
  NasKeySetIdentifier(
      const uint8_t& iei, const bool& tsc, const uint8_t& key_id);
  NasKeySetIdentifier(
      const bool& tsc,
      const uint8_t& key_id);  // Default: low position
  ~NasKeySetIdentifier();

  static std::string GetIeName() { return kNasKeySetIdentifierName; }

  void Set(const bool& high_pos);
  // void Set(const bool& tsc, const uint8_t& key_id);
  // void Set(const bool& tsc, const uint8_t& key_id, const uint8_t& iei);
  void Get(bool& tsc, uint8_t& key_id);

  void SetTypeOfSecurityContext(const bool& type);
  bool GetTypeOfSecurityContext() const;

  void SetNasKeyIdentifier(const uint8_t& id);
  uint8_t GetNasKeyIdentifier() const;

 private:
  void SetValue() override;
  void GetValue() override;

  bool tsc_;
  uint8_t key_id_;
};

}  // namespace nas

#endif
