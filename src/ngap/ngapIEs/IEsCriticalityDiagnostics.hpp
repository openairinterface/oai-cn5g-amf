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

#ifndef _IES_CRITICALITY_DIAGNOSTICS_H_
#define _IES_CRITICALITY_DIAGNOSTICS_H_

extern "C" {
#include "Ngap_CriticalityDiagnostics-IE-Item.h"
}

namespace ngap {

class IEsCriticalityDiagnostics {
 public:
  IEsCriticalityDiagnostics();
  virtual ~IEsCriticalityDiagnostics();

  void setIeCriticality(Ngap_Criticality_t);
  Ngap_Criticality_t getIeCriticality() const;

  void setIeId(Ngap_ProtocolIE_ID_t);
  Ngap_ProtocolIE_ID_t getIeId() const;

  void setTypeOfError(Ngap_TypeOfError_t);
  Ngap_TypeOfError_t getTypeOfError() const;

  void encode(Ngap_CriticalityDiagnostics_IE_Item_t&);
  void decode(const Ngap_CriticalityDiagnostics_IE_Item_t&);

 private:
  Ngap_Criticality_t criticality;      // Mandatory
  Ngap_ProtocolIE_ID_t protocolIE_ID;  // Mandatory
  Ngap_TypeOfError_t typeOfError;      // Mandatory
};
}  // namespace ngap

#endif
