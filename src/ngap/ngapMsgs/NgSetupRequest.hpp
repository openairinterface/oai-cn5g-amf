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

#include <optional>

#include "DefaultPagingDrx.hpp"
#include "GlobalRanNodeId.hpp"
#include "MessageType.hpp"
#include "NgapIesStruct.hpp"
#include "NgapMessage.hpp"
#include "RanNodeName.hpp"
#include "SupportedTaList.hpp"
#include "UeRetentionInformation.hpp"

namespace ngap {

class NgSetupRequestMsg : public NgapMessage {
 public:
  NgSetupRequestMsg();
  virtual ~NgSetupRequestMsg();

  void initialize();

  void setGlobalRanNodeId(
      const std::string& mcc, const std::string& mnc,
      const Ngap_GlobalRANNodeID_PR& ranNodeType, const uint32_t& ranNodeId,
      const uint8_t& ranNodeIdSize);
  bool getGlobalGnbId(
      uint32_t& gnbId, std::string& mcc, std::string& mnc) const;

  void setRanNodeName(const std::string& ranNodeName);
  bool getRanNodeName(std::string& name) const;

  void setSupportedTaList(const std::vector<struct SupportedTaItem_s>& list);
  bool getSupportedTaList(std::vector<struct SupportedTaItem_s>& list) const;

  void setDefaultPagingDrx(const e_Ngap_PagingDRX& value);
  e_Ngap_PagingDRX getDefaultPagingDrx() const;

  void setUeRetentionInformation(const UeRetentionInformation& value);
  void getUeRetentionInformation(
      std::optional<UeRetentionInformation>& value) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGSetupRequest_t* m_NgSetupRequestIes;

  GlobalRanNodeId m_GlobalRanNodeId;                               // Mandatory
  std::optional<RanNodeName> m_RanNodeName;                        // Optional
  SupportedTaList m_SupportedTaList;                               // Mandatory
  DefaultPagingDrx m_DefaultPagingDrx;                             // Mandatory
  std::optional<UeRetentionInformation> m_UeRetentionInformation;  // Optional
  // TODO: NB-IoT Default Paging DRX  (Optional, Rel 16.14.0)
  // TODO: Extended RAN Node Name (Optional, Rel 16.14.0)
};

}  // namespace ngap

#endif
