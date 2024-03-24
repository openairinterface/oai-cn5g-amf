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

#include "PacketErrorRate.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PacketErrorRate::PacketErrorRate() {
  m_Scalar   = 0;
  m_Exponent = 0;
}

//------------------------------------------------------------------------------
PacketErrorRate::~PacketErrorRate() {}

//------------------------------------------------------------------------------
void PacketErrorRate::set(const long& scalar, const long& exponent) {
  m_Scalar   = scalar;
  m_Exponent = exponent;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::get(long& scalar, long& exponent) const {
  scalar   = m_Scalar;
  exponent = m_Exponent;

  return true;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::encode(Ngap_PacketErrorRate_t& per) const {
  per.pERScalar   = m_Scalar;
  per.pERExponent = m_Exponent;

  return true;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::decode(const Ngap_PacketErrorRate_t& per) {
  m_Scalar   = per.pERScalar;
  m_Exponent = per.pERExponent;

  return true;
}
}  // namespace ngap
