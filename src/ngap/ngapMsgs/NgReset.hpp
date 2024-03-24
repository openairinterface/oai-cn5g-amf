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

#ifndef _NG_RESET_H_
#define _NG_RESET_H_

#include "Cause.hpp"
#include "NgapIesStruct.hpp"
#include "NgapMessage.hpp"
#include "ResetType.hpp"

namespace ngap {

class NgResetMsg : public NgapMessage {
 public:
  NgResetMsg();
  virtual ~NgResetMsg();

  void initialize();

  void setCause(const Cause&);
  void getCause(Cause&) const;

  void setResetType(const ResetType&);
  bool getResetType(ResetType&) const;

  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

 private:
  Ngap_NGReset_t* m_NgResetIes;

  Cause m_Cause;          // Mandatory
  ResetType m_ResetType;  // Mandatory
};

}  // namespace ngap

#endif
