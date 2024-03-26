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

#include "UeAssociatedLogicalNgConnectionItem.hpp"

#include "logger.hpp"
#include "utils.hpp"

namespace ngap {

UeAssociatedLogicalNgConnectionItem::UeAssociatedLogicalNgConnectionItem() {
  m_AmfUeNgapId = std::nullopt;
  m_RanUeNgapId = std::nullopt;
}

//------------------------------------------------------------------------------
bool UeAssociatedLogicalNgConnectionItem::setAmfUeNgapId(const uint64_t& id) {
  AmfUeNgapId tmp = {};
  if (!tmp.set(id)) return false;
  m_AmfUeNgapId = std::optional<AmfUeNgapId>(tmp);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret = m_AmfUeNgapId.value().encode(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode AMF_UE_NGAP_ID IE error");
  }
  utils::free_wrapper((void**) &ie);
  return true;
}

//------------------------------------------------------------------------------
bool UeAssociatedLogicalNgConnectionItem::getAmfUeNgapId(uint64_t& id) const {
  if (m_AmfUeNgapId.has_value()) {
    id = m_AmfUeNgapId.value().get();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void UeAssociatedLogicalNgConnectionItem::setRanUeNgapId(
    const uint32_t& ranUeNgapId) {
  m_RanUeNgapId = std::make_optional<RanUeNgapId>(ranUeNgapId);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = m_RanUeNgapId.value().encode(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::ngap().error("Encode RAN_UE_NGAP_ID IE error");
  }
  utils::free_wrapper((void**) &ie);
  return;
}

//------------------------------------------------------------------------------
bool UeAssociatedLogicalNgConnectionItem::getRanUeNgapId(
    uint32_t& ranUeNgapId) const {
  if (m_RanUeNgapId.has_value()) {
    ranUeNgapId = m_RanUeNgapId.value().get();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void UeAssociatedLogicalNgConnectionItem::get(
    UeAssociatedLogicalNgConnectionItem& item) const {
  if (m_AmfUeNgapId.has_value()) {
    item.setAmfUeNgapId(m_AmfUeNgapId.value().get());
  }
  if (m_RanUeNgapId.has_value()) {
    item.setRanUeNgapId(m_RanUeNgapId.value().get());
  }
}

//------------------------------------------------------------------------------
bool UeAssociatedLogicalNgConnectionItem::encode(
    Ngap_UE_associatedLogicalNG_connectionItem_t& item) const {
  item.aMF_UE_NGAP_ID = new Ngap_AMF_UE_NGAP_ID_t();
  m_AmfUeNgapId.value().encode(*item.aMF_UE_NGAP_ID);
  item.rAN_UE_NGAP_ID = new Ngap_RAN_UE_NGAP_ID_t();
  m_RanUeNgapId.value().encode(*item.rAN_UE_NGAP_ID);
  return true;
}

//------------------------------------------------------------------------------
bool UeAssociatedLogicalNgConnectionItem::decode(
    const Ngap_UE_associatedLogicalNG_connectionItem_t& item) {
  if (item.aMF_UE_NGAP_ID) {
    AmfUeNgapId tmp = {};
    if (!tmp.decode(*item.aMF_UE_NGAP_ID)) {
      Logger::ngap().error("Decoded NGAP AmfUeNgapId IE error");
      return false;
    }
    m_AmfUeNgapId = std::optional<AmfUeNgapId>(tmp);
  }

  if (item.rAN_UE_NGAP_ID) {
    RanUeNgapId tmp = {};
    if (!tmp.decode(*item.rAN_UE_NGAP_ID)) {
      Logger::ngap().error("Decoded NGAP RAN_UE_NGAP_ID IE error");
      return false;
    }
    m_RanUeNgapId = std::optional<RanUeNgapId>(tmp);
  }
  return true;
}

}  // namespace ngap
