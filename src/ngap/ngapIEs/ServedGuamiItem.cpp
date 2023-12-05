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
  backupAMFName = std::nullopt;
}

//------------------------------------------------------------------------------
ServedGuamiItem::~ServedGuamiItem() {}

//------------------------------------------------------------------------------
void ServedGuamiItem::setGUAMI(const GUAMI& m_guami) {
  guamiGroup = m_guami;
}

//------------------------------------------------------------------------------
void ServedGuamiItem::setBackupAMFName(const AmfName& amf_name) {
  backupAMFName = std::optional<AmfName>(amf_name);
}

//------------------------------------------------------------------------------
bool ServedGuamiItem::getBackupAMFName(AmfName& amf_name) const {
  if (!backupAMFName.has_value()) return false;
  amf_name = backupAMFName.value();
  return true;
}
//------------------------------------------------------------------------------
bool ServedGuamiItem::encode(Ngap_ServedGUAMIItem& servedGUAMIItem) {
  if (!guamiGroup.encode(servedGUAMIItem.gUAMI)) return false;
  if (backupAMFName.has_value()) {
    servedGUAMIItem.backupAMFName =
        (Ngap_AMFName_t*) calloc(1, sizeof(Ngap_AMFName_t));
    if (!servedGUAMIItem.backupAMFName) return false;
    if (!backupAMFName.value().encode(*servedGUAMIItem.backupAMFName))
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool ServedGuamiItem::decode(const Ngap_ServedGUAMIItem& pdu) {
  if (!guamiGroup.decode(pdu.gUAMI)) return false;
  if (pdu.backupAMFName) {
    AmfName amf_name = {};
    if (!amf_name.decode(*pdu.backupAMFName)) return false;
    backupAMFName = std::optional<AmfName>(amf_name);
  }
  return true;
}

//------------------------------------------------------------------------------
void ServedGuamiItem::getGUAMI(GUAMI& m_guami) {
  m_guami = guamiGroup;
}

}  // namespace ngap
