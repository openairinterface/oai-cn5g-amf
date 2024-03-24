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

#include "ServedGuamiItem.hpp"

namespace ngap {

//------------------------------------------------------------------------------
ServedGuamiItem::ServedGuamiItem() {
  m_BackupAmfName = std::nullopt;
}

//------------------------------------------------------------------------------
ServedGuamiItem::~ServedGuamiItem() {}

//------------------------------------------------------------------------------
void ServedGuamiItem::setGuami(const Guami& guami) {
  m_GuamiGroup = guami;
}

//------------------------------------------------------------------------------
void ServedGuamiItem::setBackupAmfName(const AmfName& amfName) {
  m_BackupAmfName = std::optional<AmfName>(amfName);
}

//------------------------------------------------------------------------------
bool ServedGuamiItem::getBackupAmfName(AmfName& amfName) const {
  if (!m_BackupAmfName.has_value()) return false;
  amfName = m_BackupAmfName.value();
  return true;
}
//------------------------------------------------------------------------------
bool ServedGuamiItem::encode(Ngap_ServedGUAMIItem& servedGUAMIItem) const {
  if (!m_GuamiGroup.encode(servedGUAMIItem.gUAMI)) return false;
  if (m_BackupAmfName.has_value()) {
    servedGUAMIItem.backupAMFName =
        (Ngap_AMFName_t*) calloc(1, sizeof(Ngap_AMFName_t));
    if (!servedGUAMIItem.backupAMFName) return false;
    if (!m_BackupAmfName.value().encode(*servedGUAMIItem.backupAMFName))
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool ServedGuamiItem::decode(const Ngap_ServedGUAMIItem& pdu) {
  if (!m_GuamiGroup.decode(pdu.gUAMI)) return false;
  if (pdu.backupAMFName) {
    AmfName amfName = {};
    if (!amfName.decode(*pdu.backupAMFName)) return false;
    m_BackupAmfName = std::optional<AmfName>(amfName);
  }
  return true;
}

//------------------------------------------------------------------------------
void ServedGuamiItem::getGuami(Guami& guami) const {
  guami = m_GuamiGroup;
}

}  // namespace ngap
