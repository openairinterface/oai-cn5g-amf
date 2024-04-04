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

#include "GtpTeid.hpp"

namespace ngap {

//------------------------------------------------------------------------------
GtpTeid::GtpTeid() {
  m_GtpTeid = 0;
}

//------------------------------------------------------------------------------
GtpTeid::~GtpTeid() {}

//------------------------------------------------------------------------------
void GtpTeid::set(const uint32_t gtpTeid) {
  m_GtpTeid = gtpTeid;
}

//------------------------------------------------------------------------------
bool GtpTeid::get(uint32_t& gtpTeid) const {
  gtpTeid = m_GtpTeid;

  return true;
}

//------------------------------------------------------------------------------
bool GtpTeid::encode(Ngap_GTP_TEID_t& gtpTeid) const {
  gtpTeid.size = sizeof(uint32_t);
  gtpTeid.buf  = (uint8_t*) calloc(1, sizeof(uint32_t));
  if (!gtpTeid.buf) return false;

  for (int i = 0; i < gtpTeid.size; i++) {
    gtpTeid.buf[i] = (m_GtpTeid >> (gtpTeid.size - i - 1) * 8) & 0xff;
  }

  return true;
}

//------------------------------------------------------------------------------
bool GtpTeid::decode(const Ngap_GTP_TEID_t& gtpTeid) {
  if (!gtpTeid.buf) return false;

  m_GtpTeid = 0;
  for (int i = 0; i < gtpTeid.size; i++) {
    m_GtpTeid = m_GtpTeid << 8;
    m_GtpTeid |= gtpTeid.buf[i];
  }

  return true;
}

}  // namespace ngap
