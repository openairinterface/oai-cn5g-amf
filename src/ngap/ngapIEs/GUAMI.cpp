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

#include "GUAMI.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GUAMI::GUAMI() {}

//------------------------------------------------------------------------------
GUAMI::~GUAMI() {}

//------------------------------------------------------------------------------
void GUAMI::setGUAMI(
    const PlmnId& m_plmnId, const AmfRegionId& m_aMFRegionID,
    const AmfSetId& m_aMFSetID, const AmfPointer& m_aMFPointer) {
  plmnId      = m_plmnId;
  aMFRegionID = m_aMFRegionID;
  aMFSetID    = m_aMFSetID;
  aMFPointer  = m_aMFPointer;
}

//------------------------------------------------------------------------------
bool GUAMI::setGUAMI(
    const std::string& mcc, const std::string& mnc, const uint8_t& regionId,
    const uint16_t& setId, const uint8_t& pointer) {
  plmnId.set(mcc, mnc);
  aMFRegionID.setAMFRegionID(regionId);
  if (!aMFSetID.set(setId)) return false;
  if (!aMFPointer.set(pointer)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool GUAMI::setGUAMI(
    const std::string& mcc, const std::string& mnc, const std::string& regionId,
    const std::string& setId, const std::string& pointer) {
  plmnId.set(mcc, mnc);
  aMFRegionID.setAMFRegionID(regionId);
  if (!aMFSetID.set(setId)) return false;
  if (!aMFPointer.set(pointer)) return false;
  return true;
}

//------------------------------------------------------------------------------
bool GUAMI::encode(Ngap_GUAMI_t& guami) {
  if (!plmnId.encode(guami.pLMNIdentity)) return false;
  if (!aMFRegionID.encode(guami.aMFRegionID)) return false;
  if (!aMFSetID.encode(guami.aMFSetID)) return false;
  if (!aMFPointer.encode(guami.aMFPointer)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool GUAMI::decode(const Ngap_GUAMI_t& pdu) {
  if (!plmnId.decode(pdu.pLMNIdentity)) return false;
  if (!aMFRegionID.decode(pdu.aMFRegionID)) return false;
  if (!aMFSetID.decode(pdu.aMFSetID)) return false;
  if (!aMFPointer.decode(pdu.aMFPointer)) return false;

  return true;
}

//------------------------------------------------------------------------------
void GUAMI::getGUAMI(
    PlmnId& m_plmnId, AmfRegionId& m_aMFRegionID, AmfSetId& m_aMFSetID,
    AmfPointer& m_aMFPointer) {
  m_plmnId      = plmnId;
  m_aMFRegionID = aMFRegionID;
  m_aMFSetID    = aMFSetID;
  m_aMFPointer  = aMFPointer;
}

void GUAMI::getGUAMI(
    std::string& mcc, std::string& mnc, std::string& regionId,
    std::string& setId, std::string& pointer) {
  plmnId.getMcc(mcc);
  plmnId.getMnc(mnc);
  aMFRegionID.getAMFRegionID(regionId);
  aMFSetID.get(setId);
  aMFPointer.get(pointer);
}

void GUAMI::getGUAMI(
    std::string& mcc, std::string& mnc, uint8_t regionId, uint16_t setId,
    uint8_t pointer) {
  plmnId.getMcc(mcc);
  plmnId.getMnc(mnc);
  aMFRegionID.getAMFRegionID(regionId);
  aMFSetID.get(setId);
  aMFPointer.get(pointer);
}

}  // namespace ngap
