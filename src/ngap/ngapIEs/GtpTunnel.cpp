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

#include "GtpTunnel.hpp"

#include "utils.hpp"

extern "C" {
#include "Ngap_GTPTunnel.h"
}

namespace ngap {

//------------------------------------------------------------------------------
GtpTunnel::GtpTunnel() {}

//------------------------------------------------------------------------------
GtpTunnel::~GtpTunnel() {}

//------------------------------------------------------------------------------
void GtpTunnel::set(
    const TransportLayerAddress& transportLayerAddress,
    const GtpTeid& gtpTeid) {
  m_TransportLayerAddress = transportLayerAddress;
  m_GtpTeid               = gtpTeid;
}

//------------------------------------------------------------------------------
bool GtpTunnel::get(
    TransportLayerAddress& transportLayerAddress, GtpTeid& gtpTeid) const {
  transportLayerAddress = m_TransportLayerAddress;
  gtpTeid               = m_GtpTeid;

  return true;
}

//------------------------------------------------------------------------------
bool GtpTunnel::encode(Ngap_GTPTunnel& gtpTunnel) const {
  if (!m_TransportLayerAddress.encode(gtpTunnel.transportLayerAddress)) {
    return false;
  }

  if (!m_GtpTeid.encode(gtpTunnel.gTP_TEID)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool GtpTunnel::decode(const Ngap_GTPTunnel& gtpTunnel) {
  if (!m_TransportLayerAddress.decode(gtpTunnel.transportLayerAddress))
    return false;
  if (!m_GtpTeid.decode(gtpTunnel.gTP_TEID)) return false;

  return true;
}

}  // namespace ngap
