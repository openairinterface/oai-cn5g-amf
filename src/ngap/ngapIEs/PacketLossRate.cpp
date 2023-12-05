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

#include "PacketLossRate.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PacketLossRate::PacketLossRate() {
  packet_loss_rate_ = 0;
}

//------------------------------------------------------------------------------
PacketLossRate::~PacketLossRate() {}

//------------------------------------------------------------------------------
void PacketLossRate::setPacketLossRate(long value) {
  packet_loss_rate_ = value;
}

//------------------------------------------------------------------------------
bool PacketLossRate::getPacketLossRate(long& value) const {
  value = packet_loss_rate_;

  return true;
}

//------------------------------------------------------------------------------
bool PacketLossRate::encode(Ngap_PacketLossRate_t& packetLossRate) const {
  packetLossRate = packet_loss_rate_;

  return true;
}

//------------------------------------------------------------------------------
bool PacketLossRate::decode(const Ngap_PacketLossRate_t& packetLossRate) {
  packet_loss_rate_ = packetLossRate;

  return true;
}
}  // namespace ngap
