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

#include "TransportLayerAddress.hpp"

#include <vector>

#include "utils.hpp"

namespace ngap {

//------------------------------------------------------------------------------
TransportLayerAddress::TransportLayerAddress() {}

//------------------------------------------------------------------------------
TransportLayerAddress::~TransportLayerAddress() {}

//------------------------------------------------------------------------------
void TransportLayerAddress::set(const std::string& address) {
  // m_IpAddress = utils::fromString<long>(address);
  m_IpAddress = address;
}

//------------------------------------------------------------------------------
bool TransportLayerAddress::get(std::string& address) const {
  // address = to_string(m_IpAddress);
  address = m_IpAddress;

  return true;
}

//------------------------------------------------------------------------------
std::vector<std::string> splite(const std::string& s, const std::string& c) {
  std::string::size_type pos1, pos2;
  std::vector<std::string> v;
  pos2 = s.find(c);
  pos1 = 0;
  while (std::string::npos != pos2) {
    v.push_back(s.substr(pos1, pos2 - pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if (pos1 != s.length()) {
    v.push_back(s.substr(pos1));
  }
  return v;
}

//------------------------------------------------------------------------------
bool TransportLayerAddress::encode(
    Ngap_TransportLayerAddress_t& transportLayerAddress) const {
  transportLayerAddress.size        = sizeof(uint32_t);
  transportLayerAddress.bits_unused = 0;
  transportLayerAddress.buf = (uint8_t*) calloc(1, transportLayerAddress.size);
  if (!transportLayerAddress.buf) return false;

  std::vector<std::string> ipAddress = splite(m_IpAddress, ".");

  for (int i = 0; i < transportLayerAddress.size; i++) {
    transportLayerAddress.buf[i] = utils::fromString<int>(ipAddress[i]);
  }

  return true;
}

//------------------------------------------------------------------------------
bool TransportLayerAddress::decode(
    const Ngap_TransportLayerAddress_t& transportLayerAddress) {
  if (!transportLayerAddress.buf) return false;

  m_IpAddress = std::to_string(transportLayerAddress.buf[0]);
  for (int i = 1; i < transportLayerAddress.size; i++) {
    m_IpAddress =
        m_IpAddress + '.' + std::to_string(transportLayerAddress.buf[i]);
  }

  return true;
}

}  // namespace ngap
