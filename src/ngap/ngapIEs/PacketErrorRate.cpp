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
  scalar_   = 0;
  exponent_ = 0;
}

//------------------------------------------------------------------------------
PacketErrorRate::~PacketErrorRate() {}

//------------------------------------------------------------------------------
void PacketErrorRate::setPacketErrorRate(long scalar, long exponent) {
  scalar_   = scalar;
  exponent_ = exponent;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::getPacketErrorRate(long& scalar, long& exponent) const {
  scalar   = scalar_;
  exponent = exponent_;

  return true;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::encode(Ngap_PacketErrorRate_t& packet_error_rate) const {
  packet_error_rate.pERScalar   = scalar_;
  packet_error_rate.pERExponent = exponent_;

  return true;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::decode(const Ngap_PacketErrorRate_t& packet_error_rate) {
  scalar_   = packet_error_rate.pERScalar;
  exponent_ = packet_error_rate.pERExponent;

  return true;
}
}  // namespace ngap
