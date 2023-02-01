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

#ifndef _NG_SETUP_REQUEST_H_
#define _NG_SETUP_REQUEST_H_

#include "DefaultPagingDRX.hpp"
#include "GlobalRanNodeId.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "RanNodeName.hpp"
#include "SupportedTAList.hpp"
#include "NgapMessage.hpp"

#include <optional>

namespace ngap {

class NGSetupRequestMsg : public NgapMessage {
 public:
  NGSetupRequestMsg();
  virtual ~NGSetupRequestMsg();

  void initialize();

  void setGlobalRanNodeID(
      const std::string& mcc, const std::string& mnc,
      const Ngap_GlobalRANNodeID_PR& ranNodeType, const uint32_t& ranNodeId,
      const uint8_t& ran_node_id_size);
  bool getGlobalGnbID(uint32_t& gnbId, std::string& mcc, std::string& mnc);

  void setRanNodeName(const std::string& ranNodeName);
  bool getRanNodeName(std::string& name);

  void setSupportedTAList(const std::vector<struct SupportedTaItem_s> list);
  bool getSupportedTAList(std::vector<struct SupportedTaItem_s>& list);

  void setDefaultPagingDRX(const e_Ngap_PagingDRX& value);
  int getDefaultPagingDRX();

  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGSetupRequest_t* ngSetupRequestIEs;

  GlobalRanNodeId globalRanNodeId;         // Mandatory
  std::optional<RanNodeName> ranNodeName;  // Optional
  SupportedTAList supportedTAList;         // Mandatory
  DefaultPagingDRX defaultPagingDrx;       // Mandatory
  // TODO: UE Retention Information (Optional)
};

}  // namespace ngap

#endif
