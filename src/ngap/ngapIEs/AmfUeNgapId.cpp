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

#include "AmfUeNgapId.hpp"

namespace ngap {

//------------------------------------------------------------------------------
AmfUeNgapId::AmfUeNgapId() {
  id_ = 0;
}

//------------------------------------------------------------------------------
AmfUeNgapId::~AmfUeNgapId() {}

//------------------------------------------------------------------------------
bool AmfUeNgapId::set(const uint64_t& id) {
  if (id > AMF_UE_NGAP_ID_MAX_VALUE) return false;
  id_ = id;
  return true;
}

//------------------------------------------------------------------------------
uint64_t AmfUeNgapId::get() const {
  return id_;
}

//------------------------------------------------------------------------------
bool AmfUeNgapId::encode(Ngap_AMF_UE_NGAP_ID_t& amf_ue_ngap_id) const {
  amf_ue_ngap_id.size = 5;
  amf_ue_ngap_id.buf  = (uint8_t*) calloc(1, amf_ue_ngap_id.size);
  if (!amf_ue_ngap_id.buf) return false;

  for (int i = 0; i < amf_ue_ngap_id.size; i++) {
    amf_ue_ngap_id.buf[i] =
        (id_ & (0xff00000000 >> i * 8)) >> ((amf_ue_ngap_id.size - i - 1) * 8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool AmfUeNgapId::decode(Ngap_AMF_UE_NGAP_ID_t amf_ue_ngap_id) {
  if (!amf_ue_ngap_id.buf) return false;

  id_ = 0;
  for (int i = 0; i < amf_ue_ngap_id.size; i++) {
    id_ = id_ << 8;
    id_ |= amf_ue_ngap_id.buf[i];
  }

  return true;
}
}  // namespace ngap
