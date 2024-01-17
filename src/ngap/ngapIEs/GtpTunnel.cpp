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
    const TransportLayerAddress& m_transportLayerAddress,
    const GtpTeid& m_gtpTeid) {
  transportLayerAddress = m_transportLayerAddress;
  gtpTeid               = m_gtpTeid;
}

//------------------------------------------------------------------------------
bool GtpTunnel::get(
    TransportLayerAddress& m_transportLayerAddress, GtpTeid& m_gtpTeid) const {
  m_transportLayerAddress = transportLayerAddress;
  m_gtpTeid               = gtpTeid;

  return true;
}

//------------------------------------------------------------------------------
bool GtpTunnel::encode(Ngap_GTPTunnel& gtp_tunnel) {
  if (!transportLayerAddress.encode(gtp_tunnel.transportLayerAddress)) {
    return false;
  }

  if (!gtpTeid.encode(gtp_tunnel.gTP_TEID)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool GtpTunnel::decode(const Ngap_GTPTunnel& gtp_tunnel) {
  if (!transportLayerAddress.decode(gtp_tunnel.transportLayerAddress))
    return false;
  if (!gtpTeid.decode(gtp_tunnel.gTP_TEID)) return false;

  return true;
}

}  // namespace ngap
