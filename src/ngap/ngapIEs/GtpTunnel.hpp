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

#ifndef _GTP_TUNNEL_H_
#define _GTP_TUNNEL_H_

#include "GtpTeid.hpp"
#include "TransportLayerAddress.hpp"

extern "C" {
#include "Ngap_GTPTunnel.h"
}

namespace ngap {

class GtpTunnel {
 public:
  GtpTunnel();
  virtual ~GtpTunnel();

  void set(
      const TransportLayerAddress& m_transportLayerAddress,
      const GtpTeid& m_gtpTeid);
  bool get(
      TransportLayerAddress& m_transportLayerAddress, GtpTeid& m_gtpTeid) const;

  bool encode(Ngap_GTPTunnel& upTransportLayerInfo);
  bool decode(const Ngap_GTPTunnel& upTransportLayerInfo);

 private:
  TransportLayerAddress transportLayerAddress;
  GtpTeid gtpTeid;
};

}  // namespace ngap

#endif
