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
#include "NgapIEsStruct.hpp"
#include "NgapMessage.hpp"
#include "PlmnSupportList.hpp"
#include "RelativeAmfCapacity.hpp"
#include "ServedGuamiList.hpp"

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

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGSetupResponse_t* ngSetupResponsIEs;

  AmfName amfName;                          // Mandatory
  ServedGuamiList servedGUAMIList;          // Mandatory
  RelativeAmfCapacity relativeAmfCapacity;  // Mandatory
  PlmnSupportList plmnSupportList;          // Mandatory
  // TODO: CriticalityDiagnostics //Optional
  // TODO: UE Retention Information //Optional
  // TODO:IAB Supported (Optional, Rel 16.14.0)
  // TODO:Extended AMF Name (Optional, Rel 16.14.0)
};

}  // namespace ngap
#endif
