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

#include "UserLocationInformationN3IWF.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformationN3IWF::UserLocationInformationN3IWF() {
  transportLayerAddress = NULL;
}

//------------------------------------------------------------------------------
UserLocationInformationN3IWF::~UserLocationInformationN3IWF() {}

//------------------------------------------------------------------------------
bool UserLocationInformationN3IWF::getN3iwfTransportLayerInformation(
    TransportLayerAddress*& m_transportLayerAddress) {
  m_transportLayerAddress = transportLayerAddress;
  return true;
}
//------------------------------------------------------------------------------
void UserLocationInformationN3IWF::setN3iwfTransportLayerInformation(
    TransportLayerAddress* m_transportLayerAddress) {
  transportLayerAddress = m_transportLayerAddress;
}
//------------------------------------------------------------------------------
bool UserLocationInformationN3IWF::encode(
    Ngap_UserLocationInformationN3IWF_t* user_location_info_n3iwf) {
  if (!transportLayerAddress->encode2TransportLayerAddress(
          user_location_info_n3iwf->iPAddress)) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationN3IWF::decode(
    Ngap_UserLocationInformationN3IWF_t* user_location_info_n3iwf) {
  transportLayerAddress = new TransportLayerAddress();

  if (!transportLayerAddress->decodefromTransportLayerAddress(
          user_location_info_n3iwf->iPAddress))
    return false;
  return true;
}

}  // namespace ngap
