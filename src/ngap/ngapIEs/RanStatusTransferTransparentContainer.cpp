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

#include "RanStatusTransferTransparentContainer.hpp"

#include "logger.hpp"

namespace ngap {

//------------------------------------------------------------------------------
RanStatusTransferTransparentContainer::RanStatusTransferTransparentContainer() {
}

//------------------------------------------------------------------------------
RanStatusTransferTransparentContainer::
    ~RanStatusTransferTransparentContainer() {}

//------------------------------------------------------------------------------
void RanStatusTransferTransparentContainer::getDRBSubjectList(
    DrbSubjectToStatusTransferList& drblist) {
  drblist = drb_sub_list;
}

//------------------------------------------------------------------------------
void RanStatusTransferTransparentContainer::setDRBSubjectList(
    const DrbSubjectToStatusTransferList& drblist) {
  drb_sub_list = drblist;
}

//------------------------------------------------------------------------------
bool RanStatusTransferTransparentContainer::encode(
    Ngap_RANStatusTransfer_TransparentContainer_t&
        ranstatustransfer_transparentcontainer) {
  if (!drb_sub_list.encode(ranstatustransfer_transparentcontainer
                               .dRBsSubjectToStatusTransferList)) {
    Logger::ngap().error(
        "Encode RANStatusTransferTransparentContainer IE error!");
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool RanStatusTransferTransparentContainer::decode(
    const Ngap_RANStatusTransfer_TransparentContainer_t&
        ranstatustransfer_transparentcontainer) {
  if (!drb_sub_list.decode(ranstatustransfer_transparentcontainer
                               .dRBsSubjectToStatusTransferList)) {
    Logger::ngap().error(
        "Decode RANStatusTransferTransparentContainer IE error!");
    return false;
  }
  return true;
}
}  // namespace ngap
