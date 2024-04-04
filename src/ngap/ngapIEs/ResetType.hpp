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

#ifndef _RESET_TYPE_H_
#define _RESET_TYPE_H_

#include <optional>
#include <vector>

#include "UeAssociatedLogicalNgConnectionItem.hpp"
#include "UeAssociatedLogicalNgConnectionList.hpp"

extern "C" {
#include "Ngap_ResetType.h"
}

namespace ngap {
class ResetType {
 public:
  ResetType();
  virtual ~ResetType();

  void setResetType(const long&);
  void setResetType(
      const std::vector<UeAssociatedLogicalNgConnectionItem>& list);
  void getResetType(struct Ngap_UE_associatedLogicalNG_connectionList*&) const;

  void getResetType(long&) const;
  uint8_t getResetType() const;

  void setUeAssociatedLogicalNgConnectionList(
      const std::vector<UeAssociatedLogicalNgConnectionItem>& list);

  void getUeAssociatedLogicalNgConnectionList(
      std::vector<UeAssociatedLogicalNgConnectionItem>& list) const;
  void getUeAssociatedLogicalNgConnectionList(
      struct Ngap_UE_associatedLogicalNG_connectionList*&) const;

  bool encode(Ngap_ResetType_t& type) const;
  bool decode(const Ngap_ResetType_t& type);

 private:
  Ngap_ResetType_PR m_Present;
  std::optional<long> m_NgInterface;
  std::optional<UeAssociatedLogicalNgConnectionList> m_PartOfNgInterface;
  //	struct Ngap_ProtocolIE_SingleContainer	*choice_Extensions;
};

}  // namespace ngap

#endif
