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

#ifndef _NG_SETUP_RESPONSE_H_
#define _NG_SETUP_RESPONSE_H_

#include "AmfName.hpp"
#include "MessageType.hpp"
#include "NgapIesStruct.hpp"
#include "NgapMessage.hpp"
#include "PlmnSupportList.hpp"
#include "RelativeAmfCapacity.hpp"
#include "ServedGuamiList.hpp"
#include "UeRetentionInformation.hpp"

extern "C" {
#include "Ngap_NGSetupResponse.h"
}

namespace ngap {

class NgSetupResponseMsg : public NgapMessage {
 public:
  NgSetupResponseMsg();
  virtual ~NgSetupResponseMsg();

  void initialize();

  bool setAmfName(const std::string& name);
  bool getAmfName(std::string& name) const;

  void setGuamiList(std::vector<struct GuamiItem_s>& list);
  bool getGuamiList(std::vector<struct GuamiItem_s>& list) const;

  void setRelativeAmfCapacity(const long& capacity);
  long getRelativeAmfCapacity() const;

  void setPlmnSupportList(const std::vector<PlmnSliceSupport_t>& list);
  bool getPlmnSupportList(std::vector<PlmnSliceSupport_t>& list) const;

  void setUeRetentionInformation(const UeRetentionInformation& value);
  void getUeRetentionInformation(
      std::optional<UeRetentionInformation>& value) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGSetupResponse_t* m_NgSetupResponsIes;

  AmfName m_AmfName;                          // Mandatory
  ServedGuamiList m_ServedGuamiList;          // Mandatory
  RelativeAmfCapacity m_RelativeAmfCapacity;  // Mandatory
  PlmnSupportList m_PlmnSupportList;          // Mandatory
  // TODO: CriticalityDiagnostics //Optional
  std::optional<UeRetentionInformation> m_UeRetentionInformation;  // Optional
  // TODO:IAB Supported (Optional, Rel 16.14.0)
  // TODO:Extended AMF Name (Optional, Rel 16.14.0)
};

}  // namespace ngap
#endif
