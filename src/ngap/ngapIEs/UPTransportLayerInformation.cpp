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

#include "UPTransportLayerInformation.hpp"

#include "utils.hpp"

extern "C" {
#include "Ngap_GTPTunnel.h"
}

namespace ngap {

//------------------------------------------------------------------------------
UpTransportLayerInformation::UpTransportLayerInformation() {}

//------------------------------------------------------------------------------
UpTransportLayerInformation::~UpTransportLayerInformation() {}

//------------------------------------------------------------------------------
void UpTransportLayerInformation::setUpTransportLayerInformation(
    const TransportLayerAddress& m_transportLayerAddress,
    const GtpTeid& m_gtpTeid) {
  transportLayerAddress = m_transportLayerAddress;
  gtpTeid               = m_gtpTeid;
}

//------------------------------------------------------------------------------
bool UpTransportLayerInformation::getUpTransportLayerInformation(
    TransportLayerAddress& m_transportLayerAddress, GtpTeid& m_gtpTeid) const {
  m_transportLayerAddress = transportLayerAddress;
  m_gtpTeid               = gtpTeid;

  return true;
}

//------------------------------------------------------------------------------
void UpTransportLayerInformation::set(const GtpTunnel& gtpTunnel) {
  gtpTunnel_ = std::make_optional<GtpTunnel>(gtpTunnel);
}

//------------------------------------------------------------------------------
void UpTransportLayerInformation::get(
    std::optional<GtpTunnel>& gtpTunnel) const {
  gtpTunnel = gtpTunnel_;
}

//------------------------------------------------------------------------------
bool UpTransportLayerInformation::encode(
    Ngap_UPTransportLayerInformation_t& upTransportLayerInfo) {
  upTransportLayerInfo.present = Ngap_UPTransportLayerInformation_PR_gTPTunnel;
  Ngap_GTPTunnel_t* gtptunnel =
      (Ngap_GTPTunnel_t*) calloc(1, sizeof(Ngap_GTPTunnel_t));
  if (!gtptunnel) return false;
  if (!transportLayerAddress.encode(gtptunnel->transportLayerAddress)) {
    utils::free_wrapper((void**) &gtptunnel);
    return false;
  }

  if (!gtpTeid.encode(gtptunnel->gTP_TEID)) {
    utils::free_wrapper((void**) &gtptunnel);
    return false;
  }
  upTransportLayerInfo.choice.gTPTunnel = gtptunnel;
  return true;
}

//------------------------------------------------------------------------------
bool UpTransportLayerInformation::decode(
    const Ngap_UPTransportLayerInformation_t& upTransportLayerInfo) {
  if (upTransportLayerInfo.present !=
      Ngap_UPTransportLayerInformation_PR_gTPTunnel)
    return false;
  if (!upTransportLayerInfo.choice.gTPTunnel) return false;

  if (!transportLayerAddress.decode(
          upTransportLayerInfo.choice.gTPTunnel->transportLayerAddress))
    return false;
  if (!gtpTeid.decode(upTransportLayerInfo.choice.gTPTunnel->gTP_TEID))
    return false;

  return true;
}

}  // namespace ngap
