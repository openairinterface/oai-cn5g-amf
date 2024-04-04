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

#ifndef _PDU_SESSION_RESOURCE_SETUP_ITEM_HO_REQ_H_
#define _PDU_SESSION_RESOURCE_SETUP_ITEM_HO_REQ_H_

#include "PduSessionResourceItem.hpp"
#include "PduSessionResourceSetupRequestTransfer.hpp"
#include "SNssai.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupItemHOReq.h"
}

namespace ngap {

class PduSessionResourceSetupItemHoReq : public PduSessionResourceItem {
 public:
  PduSessionResourceSetupItemHoReq();
  virtual ~PduSessionResourceSetupItemHoReq();

  void set(
      const PduSessionId& pduSessionId, const SNssai& sNssai,
      const OCTET_STRING_t& handoverRequestTransfer);
  void get(
      PduSessionId& pduSessionId, SNssai& sNssai,
      OCTET_STRING_t& handoverRequestTransfer) const;

  bool encode(Ngap_PDUSessionResourceSetupItemHOReq_t&
                  pduSessionResourceSetupItemHOReq) const;
  bool decode(const Ngap_PDUSessionResourceSetupItemHOReq_t&
                  pduSessionResourceSetupItemHOReq);

 private:
  // PduSessionId //Mandatory;
  SNssai m_SNssai;  // Mandatory
  // Handover Request Transfer - Mandatory
};

}  // namespace ngap

#endif
