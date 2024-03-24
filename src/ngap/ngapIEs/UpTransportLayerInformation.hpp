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

#ifndef _UP_TRANSPORT_LAYER_INFORMATION_H_
#define _UP_TRANSPORT_LAYER_INFORMATION_H_

#include <optional>

#include "GtpTeid.hpp"
#include "GtpTunnel.hpp"
#include "TransportLayerAddress.hpp"

extern "C" {
#include "Ngap_UPTransportLayerInformation.h"
}

namespace ngap {

class UpTransportLayerInformation {
 public:
  UpTransportLayerInformation();
  virtual ~UpTransportLayerInformation();

  void set(
      const TransportLayerAddress& transportLayerAddress,
      const GtpTeid& gtpTeid);
  bool get(
      TransportLayerAddress& transportLayerAddress, GtpTeid& gtpTeid) const;

  void set(const GtpTunnel& gtpTunnel);
  void get(std::optional<GtpTunnel>& gtpTunnel) const;

  bool encode(Ngap_UPTransportLayerInformation_t& upTransportLayerInfo) const;
  bool decode(const Ngap_UPTransportLayerInformation_t& upTransportLayerInfo);

 private:
  TransportLayerAddress m_TransportLayerAddress;  // TODO: should be removed
  GtpTeid m_GtpTeid;                              // TODO: should be removed
  std::optional<GtpTunnel> m_GtpTunnel;
};

}  // namespace ngap

#endif
