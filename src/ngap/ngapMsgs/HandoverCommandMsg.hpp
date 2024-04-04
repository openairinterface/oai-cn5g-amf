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

#ifndef _HANDOVER_COMMAND_H_
#define _HANDOVER_COMMAND_H_

#include <optional>

#include "NgapUeMessage.hpp"
#include "PduSessionResourceHandoverList.hpp"
#include "PduSessionResourceToReleaseListHandoverCmd.hpp"

extern "C" {
#include "Ngap_HandoverCommand.h"
#include "Ngap_NGAP-PDU.h"
}

namespace ngap {

class HandoverCommandMsg : public NgapUeMessage {
 public:
  HandoverCommandMsg();
  virtual ~HandoverCommandMsg();

  void initialize();
  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setHandoverType(const long& type);
  // void getHandoverType(Ngap_HandoverType_t &type);

  void setNasSecurityParametersFromNgRan(
      const OCTET_STRING_t& nasSecurityParameters);
  bool getNasSecurityParametersFromNgRan(
      OCTET_STRING_t& nasSecurityParameters) const;

  void setPduSessionResourceHandoverList(
      const PduSessionResourceHandoverList& list);
  bool getPduSessionResourceHandoverList(
      PduSessionResourceHandoverList& list) const;

  void setPduSessionResourceToReleaseListHOCmd(
      const PduSessionResourceToReleaseListHandoverCmd& list);
  bool getPduSessionResourceToReleaseListHOCmd(
      PduSessionResourceToReleaseListHandoverCmd& list) const;

  void setTargetToSourceTransparentContainer(
      const OCTET_STRING_t& targetTosource);
  void getTargetToSourceTransparentContainer(
      OCTET_STRING_t& targetTosource) const;

 private:
  Ngap_HandoverCommand_t* m_HandoverCommandIes;

  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  Ngap_HandoverType_t m_HandoverType;  // Mandatory
  std::optional<Ngap_NASSecurityParametersFromNGRAN_t>
      m_NasSecurityParametersFromNgRan;  // TODO: Conditional
  std::optional<PduSessionResourceHandoverList>
      m_PduSessionResourceHandoverList;  // Optional
  std::optional<PduSessionResourceToReleaseListHandoverCmd>
      m_PduSessionResourceToReleaseListHOCmd;
  Ngap_TargetToSource_TransparentContainer_t
      m_TargetToSourceTransparentContainer;                 // TODO: Mandatory
  Ngap_CriticalityDiagnostics_t* m_CriticalityDiagnostics;  // TODO: Optional
};

}  // namespace ngap

#endif
