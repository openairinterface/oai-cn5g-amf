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

#include "NetworkInstance.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NetworkInstance::NetworkInstance() {
  m_NetworkInstance = 0;
}

//------------------------------------------------------------------------------
NetworkInstance::NetworkInstance(const long& networkInstance) {
  m_NetworkInstance = networkInstance;
}
//------------------------------------------------------------------------------
NetworkInstance::~NetworkInstance() {}

//------------------------------------------------------------------------------
void NetworkInstance::set(const long& networkInstance) {
  m_NetworkInstance = networkInstance;
}

//------------------------------------------------------------------------------
bool NetworkInstance::get(long& networkInstance) const {
  networkInstance = m_NetworkInstance;

  return true;
}

//------------------------------------------------------------------------------
bool NetworkInstance::encode(Ngap_NetworkInstance_t& value) const {
  value = m_NetworkInstance;

  return true;
}

//------------------------------------------------------------------------------
bool NetworkInstance::decode(const Ngap_NetworkInstance_t& value) {
  m_NetworkInstance = value;

  return true;
}

}  // namespace ngap
