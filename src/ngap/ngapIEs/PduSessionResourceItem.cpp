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

#include "PduSessionResourceItem.hpp"

#include "amf_conversions.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceItem::PduSessionResourceItem() {}

//------------------------------------------------------------------------------
PduSessionResourceItem::~PduSessionResourceItem() {}

//------------------------------------------------------------------------------
void PduSessionResourceItem::set(
    const PduSessionId& pduSessionId, const OCTET_STRING_t& resource) {
  m_PduSessionId = pduSessionId;
  // m_Resource = resource;
  amf_conv::octet_string_copy(m_Resource, resource);
}

//------------------------------------------------------------------------------
void PduSessionResourceItem::get(
    PduSessionId& pduSessionId, OCTET_STRING_t& resource) const {
  pduSessionId = m_PduSessionId;
  // resource = m_Resource;
  amf_conv::octet_string_copy(resource, m_Resource);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::encode(
    Ngap_PDUSessionID_t& pduSessionId, OCTET_STRING_t& resource) const {
  if (!m_PduSessionId.encode(pduSessionId)) return false;
  return amf_conv::octet_string_copy(resource, m_Resource);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::decode(
    const Ngap_PDUSessionID_t& pduSessionId, const OCTET_STRING_t& resource) {
  if (!m_PduSessionId.decode(pduSessionId)) return false;
  return amf_conv::octet_string_copy(m_Resource, resource);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::encode(
    Ngap_PDUSessionResourceSetupItemCxtRes_t& item) const {
  return encode(
      item.pDUSessionID, item.pDUSessionResourceSetupResponseTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::decode(
    const Ngap_PDUSessionResourceSetupItemCxtRes_t& item) {
  return decode(
      item.pDUSessionID, item.pDUSessionResourceSetupResponseTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::encode(
    Ngap_PDUSessionResourceItemHORqd_t& item) const {
  return encode(item.pDUSessionID, item.handoverRequiredTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::decode(
    const Ngap_PDUSessionResourceItemHORqd_t& item) {
  return decode(item.pDUSessionID, item.handoverRequiredTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::encode(
    Ngap_PDUSessionResourceHandoverItem_t& item) const {
  return encode(item.pDUSessionID, item.handoverCommandTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::decode(
    const Ngap_PDUSessionResourceHandoverItem_t& item) {
  return decode(item.pDUSessionID, item.handoverCommandTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::encode(
    Ngap_PDUSessionResourceToReleaseItemHOCmd_t& item) const {
  return encode(
      item.pDUSessionID, item.handoverPreparationUnsuccessfulTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::decode(
    const Ngap_PDUSessionResourceToReleaseItemHOCmd_t& item) {
  return decode(
      item.pDUSessionID, item.handoverPreparationUnsuccessfulTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::encode(
    Ngap_PDUSessionResourceAdmittedItem_t& item) const {
  return encode(item.pDUSessionID, item.handoverRequestAcknowledgeTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::decode(
    const Ngap_PDUSessionResourceAdmittedItem_t& item) {
  return decode(item.pDUSessionID, item.handoverRequestAcknowledgeTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::encode(
    Ngap_PDUSessionResourceFailedToSetupItemHOAck_t& item) const {
  return encode(
      item.pDUSessionID, item.handoverResourceAllocationUnsuccessfulTransfer);
}

//------------------------------------------------------------------------------
bool PduSessionResourceItem::decode(
    const Ngap_PDUSessionResourceFailedToSetupItemHOAck_t& item) {
  return decode(
      item.pDUSessionID, item.handoverResourceAllocationUnsuccessfulTransfer);
}

}  // namespace ngap
