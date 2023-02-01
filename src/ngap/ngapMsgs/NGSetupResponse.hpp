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

#include "AMFName.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "PLMNSupportList.hpp"
#include "RelativeAMFCapacity.hpp"
#include "ServedGUAMIList.hpp"
#include "NgapMessage.hpp"

extern "C" {
#include "Ngap_NGSetupResponse.h"
}

namespace ngap {

class NGSetupResponseMsg : public NgapMessage {
 public:
  NGSetupResponseMsg();
  virtual ~NGSetupResponseMsg();

  void initialize();

  void setAMFName(const std::string& name);
  bool getAMFName(std::string& name) const;

  void setGUAMIList(std::vector<struct GuamiItem_s> list);
  bool getGUAMIList(std::vector<struct GuamiItem_s>& list);

  void setRelativeAmfCapacity(long capacity);
  long getRelativeAmfCapacity();

  void setPlmnSupportList(std::vector<PlmnSliceSupport_t> list);
  bool getPlmnSupportList(std::vector<PlmnSliceSupport_t>& list);

  bool decodeFromPdu(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGSetupResponse_t* ngSetupResponsIEs;

  AmfName amfName;                          // Mandatory
  ServedGUAMIList servedGUAMIList;          // Mandatory
  RelativeAMFCapacity relativeAmfCapacity;  // Mandatory
  PLMNSupportList plmnSupportList;          // Mandatory
  // CriticalityDiagnostics //Optional, TODO
  // UE Retention Information //Optional, TODO
};

}  // namespace ngap
#endif
