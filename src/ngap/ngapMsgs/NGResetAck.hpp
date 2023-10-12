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

#ifndef _NG_RESET_ACK_H_
#define _NG_RESET_ACK_H_

#include <optional>

#include "NgapIEsStruct.hpp"
#include "NgapMessage.hpp"
#include "UEAssociatedLogicalNGConnectionItem.hpp"
#include "UEAssociatedLogicalNGConnectionList.hpp"

namespace ngap {

class NGResetAckMsg : public NgapMessage {
 public:
  NGResetAckMsg();
  virtual ~NGResetAckMsg();

  void initialize();

  void setUE_associatedLogicalNG_connectionList(
      std::vector<UEAssociatedLogicalNGConnectionItem>& list);
  void getUE_associatedLogicalNG_connectionList(
      std::vector<UEAssociatedLogicalNGConnectionItem>& list);

  void addUE_associatedLogicalNG_connectionList();

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;
  // TODO: CriticalityDiagnostics

 private:
  Ngap_NGResetAcknowledge_t* ngResetAckIEs;

  std::optional<UEAssociatedLogicalNGConnectionList>
      ueAssociatedLogicalNGConnectionList;                // Optional
  Ngap_CriticalityDiagnostics_t* CriticalityDiagnostics;  // TODO: Optional
};

}  // namespace ngap

#endif
