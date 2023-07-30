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

#include "N3IWF-ID.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
N3IWF_ID::N3IWF_ID() {
  n3iwf_id_ = std::nullopt;
  present_  = Ngap_N3IWF_ID_PR_NOTHING;
}

//------------------------------------------------------------------------------
N3IWF_ID::~N3IWF_ID() {}

//------------------------------------------------------------------------------
void N3IWF_ID::setValue(const n3iwfId_t& n3iwf_id) {
  n3iwf_id_ = std::optional<n3iwfId_t>(n3iwf_id);
  present_  = Ngap_N3IWF_ID_PR_N3IWF_ID;
}

//------------------------------------------------------------------------------
bool N3IWF_ID::setValue(const uint16_t& id, const uint8_t& bit_length) {
  if (!((bit_length >= NGAP_N3IWF_ID_SIZE_MIN) &&
        (bit_length <= NGAP_N3IWF_ID_SIZE_MAX))) {
    Logger::ngap().warn("gNBID length out of range!");
    return false;
  }

  n3iwfId_t tmp  = {};
  tmp.id         = id;
  tmp.bit_length = bit_length;

  n3iwf_id_ = std::optional<n3iwfId_t>(tmp);
  present_  = Ngap_N3IWF_ID_PR_N3IWF_ID;
  return true;
}

//------------------------------------------------------------------------------
bool N3IWF_ID::get(n3iwfId_t& n3iwf_id) const {
  if (n3iwf_id_.has_value()) {
    n3iwf_id = n3iwf_id_.value();
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool N3IWF_ID::get(uint16_t& id) const {
  if (n3iwf_id_.has_value()) {
    id = n3iwf_id_.value().id;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool N3IWF_ID::encode(Ngap_N3IWF_ID_t& n3iwfid) {
  if (!n3iwf_id_.has_value()) {
    n3iwfid.present = Ngap_N3IWF_ID_PR_NOTHING;
    return true;
  }

  n3iwfid.present                     = Ngap_N3IWF_ID_PR_N3IWF_ID;
  n3iwfid.choice.n3IWF_ID.size        = 2;  // TODO: to be vefified
  n3iwfid.choice.n3IWF_ID.bits_unused = 16 - n3iwf_id_.value().bit_length;
  n3iwfid.choice.n3IWF_ID.buf = (uint8_t*) calloc(1, 2 * sizeof(uint8_t));
  if (!n3iwfid.choice.n3IWF_ID.buf) return false;
  n3iwfid.choice.n3IWF_ID.buf[1] = n3iwf_id_.value().id & 0x000000ff;
  n3iwfid.choice.n3IWF_ID.buf[0] = (n3iwf_id_.value().id & 0x0000ff00) >> 8;

  return true;
}

//------------------------------------------------------------------------------
bool N3IWF_ID::decode(Ngap_N3IWF_ID_t& n3iwfid) {
  if (n3iwfid.present != Ngap_N3IWF_ID_PR_N3IWF_ID) return false;
  if (!n3iwfid.choice.n3IWF_ID.buf) return false;

  n3iwfId_t tmp = {};
  tmp.id        = n3iwfid.choice.n3IWF_ID.buf[0] << 8;
  tmp.id |= n3iwfid.choice.n3IWF_ID.buf[1];
  tmp.bit_length = 16 - n3iwfid.choice.n3IWF_ID.bits_unused;

  n3iwf_id_ = std::optional<n3iwfId_t>(tmp);
  present_  = Ngap_N3IWF_ID_PR_N3IWF_ID;

  return true;
}

}  // namespace ngap
