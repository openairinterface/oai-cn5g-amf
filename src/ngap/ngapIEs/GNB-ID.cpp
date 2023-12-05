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

#include "GNB-ID.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GNB_ID::GNB_ID() {
  gnb_id_  = std::nullopt;
  present_ = Ngap_GNB_ID_PR_NOTHING;
}

//------------------------------------------------------------------------------
GNB_ID::~GNB_ID() {}

//------------------------------------------------------------------------------
void GNB_ID::setValue(const gNBId_t& gnb_id) {
  gnb_id_  = std::optional<gNBId_t>(gnb_id);
  present_ = Ngap_GNB_ID_PR_gNB_ID;
}

//------------------------------------------------------------------------------
bool GNB_ID::setValue(const uint32_t& id, const uint8_t& bit_length) {
  if (!((bit_length >= NGAP_GNB_ID_SIZE_MIN) &&
        (bit_length <= NGAP_GNB_ID_SIZE_MAX))) {
    Logger::ngap().warn("gNBID length out of range!");
    return false;
  }

  gNBId_t tmp    = {};
  tmp.id         = id;
  tmp.bit_length = bit_length;

  gnb_id_  = std::optional<gNBId_t>(tmp);
  present_ = Ngap_GNB_ID_PR_gNB_ID;
  return true;
}

//------------------------------------------------------------------------------
bool GNB_ID::get(gNBId_t& gnb_id) const {
  if (gnb_id_.has_value()) {
    gnb_id = gnb_id_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool GNB_ID::get(uint32_t& id) const {
  if (gnb_id_.has_value()) {
    id = gnb_id_.value().id;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool GNB_ID::encode(Ngap_GNB_ID_t& gnbid) {
  if (!gnb_id_.has_value()) {
    gnbid.present = Ngap_GNB_ID_PR_NOTHING;
    return true;
  }

  gnbid.present                   = Ngap_GNB_ID_PR_gNB_ID;
  gnbid.choice.gNB_ID.size        = 4;  // TODO: to be vefified
  gnbid.choice.gNB_ID.bits_unused = 32 - gnb_id_.value().bit_length;
  gnbid.choice.gNB_ID.buf         = (uint8_t*) calloc(1, 4 * sizeof(uint8_t));
  if (!gnbid.choice.gNB_ID.buf) return false;
  gnbid.choice.gNB_ID.buf[3] = gnb_id_.value().id & 0x000000ff;
  gnbid.choice.gNB_ID.buf[2] = (gnb_id_.value().id & 0x0000ff00) >> 8;
  gnbid.choice.gNB_ID.buf[1] = (gnb_id_.value().id & 0x00ff0000) >> 16;
  gnbid.choice.gNB_ID.buf[0] = (gnb_id_.value().id & 0xff000000) >> 24;

  return true;
}

//------------------------------------------------------------------------------
bool GNB_ID::decode(const Ngap_GNB_ID_t& gnbid) {
  if (gnbid.present != Ngap_GNB_ID_PR_gNB_ID) return false;
  if (!gnbid.choice.gNB_ID.buf) return false;

  gNBId_t tmp = {};
  tmp.id      = gnbid.choice.gNB_ID.buf[0] << 24;
  tmp.id |= gnbid.choice.gNB_ID.buf[1] << 16;
  tmp.id |= gnbid.choice.gNB_ID.buf[2] << 8;
  tmp.id |= gnbid.choice.gNB_ID.buf[3];
  tmp.bit_length = 32 - gnbid.choice.gNB_ID.bits_unused;

  gnb_id_  = std::optional<gNBId_t>(tmp);
  present_ = Ngap_GNB_ID_PR_gNB_ID;

  return true;
}

}  // namespace ngap
