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

#ifndef _PDU_SESSION_RESOURCE_ITEM_H_
#define _PDU_SESSION_RESOURCE_ITEM_H_

#include "PduSessionId.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceAdmittedItem.h"
#include "Ngap_PDUSessionResourceFailedToSetupItemHOAck.h"
#include "Ngap_PDUSessionResourceHandoverItem.h"
#include "Ngap_PDUSessionResourceItemHORqd.h"
#include "Ngap_PDUSessionResourceSetupItemCxtRes.h"
#include "Ngap_PDUSessionResourceToReleaseItemHOCmd.h"
}

namespace ngap {

class PduSessionResourceItem {
 public:
  PduSessionResourceItem();
  virtual ~PduSessionResourceItem();

  void set(const PduSessionId& pdu_session_id, const OCTET_STRING_t& resource);
  void get(PduSessionId& pdu_session_id, OCTET_STRING_t& resource) const;

  bool encode(
      Ngap_PDUSessionID_t& pdu_session_id, OCTET_STRING_t& resource) const;
  bool decode(
      const Ngap_PDUSessionID_t& pdu_session_id,
      const OCTET_STRING_t& resource);

  // PDU SESSION RESOURCE SETUP REQUEST
  // For PDU Session Resource Setup Request Item (in child class)

  // PDU SESSION RESOURCE SETUP RESPONSE
  // For PDU Session Resource Setup Response Item
  bool encode(Ngap_PDUSessionResourceSetupItemCxtRes_t&
                  pdu_session_resource_setup_item_cxt_res) const;
  bool decode(const Ngap_PDUSessionResourceSetupItemCxtRes_t&
                  pdu_session_resource_setup_item_cxt_res);
  // TODO: For PDU Session Resource Failed to Setup Item

  // PDU SESSION RESOURCE RELEASE COMMAND
  // TODO: For PDU Session Resource to Release Item

  // PDU SESSION RESOURCE RELEASE RESPONSE
  // TODO: For PDU Session Resource Released Item

  // PDU SESSION RESOURCE MODIFY REQUEST
  // For PDU Session Resource Modify Request Item (in child class)

  // PDU SESSION RESOURCE MODIFY RESPONSE
  // TODO: For PDU Session Resource Modify Response Item
  // TODO: For PDU Session Resource Failed to Modify Item

  // PDU SESSION RESOURCE NOTIFY
  // TODO: For PDU Session Resource Notify Item
  // TODO: For PDU Session Resource Released Item

  // PDU SESSION RESOURCE MODIFY INDICATION
  // TODO: PDU Session Resource Modify Indication Item

  // PDU SESSION RESOURCE MODIFY CONFIRM
  // TODO: PDU Session Resource Modify Confirm Item
  // TODO: PDU Session Resource Failed to Modify Item

  // INITIAL CONTEXT SETUP REQUEST
  // PDU Session Resource Setup Request Item (in child class)

  // INITIAL CONTEXT SETUP RESPONSE
  // TODO: PDU Session Resource Setup Response Item
  // TODO: PDU Session Resource Failed to Setup Item

  // INITIAL CONTEXT SETUP FAILURE
  // TODO: PDU Session Resource Failed to Setup Item

  // UE CONTEXT RELEASE REQUEST
  // TODO: PDU Session Resource Item

  // UE CONTEXT RELEASE COMPLETE
  // TODO: PDU Session Resource Item

  // HANDOVER REQUIRED
  // PDU Session Resource Item
  bool encode(Ngap_PDUSessionResourceItemHORqd_t& item) const;
  bool decode(const Ngap_PDUSessionResourceItemHORqd_t& item);

  // HANDOVER COMMAND
  // PDU Session Resource Handover Item
  bool encode(Ngap_PDUSessionResourceHandoverItem_t& item) const;
  bool decode(const Ngap_PDUSessionResourceHandoverItem_t& item);
  // PDU Session Resource to Release Item
  bool encode(Ngap_PDUSessionResourceToReleaseItemHOCmd_t& item) const;
  bool decode(const Ngap_PDUSessionResourceToReleaseItemHOCmd_t& item);

  // HANDOVER REQUEST
  // PDU Session Resource Setup Item (in child class)

  // HANDOVER REQUEST ACKNOWLEDGE
  // TODO: PDU Session Resource Admitted Item
  bool encode(Ngap_PDUSessionResourceAdmittedItem_t& item) const;
  bool decode(const Ngap_PDUSessionResourceAdmittedItem_t& item);
  // TODO: PDU Session Resource Failed to Setup Item
  bool encode(Ngap_PDUSessionResourceFailedToSetupItemHOAck_t& item) const;
  bool decode(const Ngap_PDUSessionResourceFailedToSetupItemHOAck_t& item);

  // PATH SWITCH REQUEST
  // TODO: PDU Session Resource to be Switched in Downlink Item
  // TODO: PDU Session Resource Failed to Setup Item

  // PATH SWITCH REQUEST ACKNOWLEDGE
  // TODO: PDU Session Resource Switched Item
  // TODO: PDU Session Resource Released Item

  // PATH SWITCH REQUEST FAILURE
  // TODO: PDU Session Resource Released Item

  // SECONDARY RAT DATA USAGE REPORT
  // TODO: PDU Session Resource Secondary RAT Usage Item

 private:
  PduSessionId pdu_session_id_;  // Mandatory
  OCTET_STRING_t resource_;
};

}  // namespace ngap

#endif
