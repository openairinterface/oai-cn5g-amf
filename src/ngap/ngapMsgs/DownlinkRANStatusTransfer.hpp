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

#ifndef _DOWNLINK_RAN_STATUS_TRANSFER_H_
#define _DOWNLINK_RAN_STATUS_TRANSFER_H_

#include "NgapUEMessage.hpp"
#include "RanStatusTransferTransparentContainer.hpp"

extern "C" {
#include "Ngap_RANStatusTransfer-TransparentContainer.h"
}
namespace ngap {
class DownlinkRANStatusTransfer : public NgapUEMessage {
 public:
  DownlinkRANStatusTransfer();
  virtual ~DownlinkRANStatusTransfer();

  void initialize();
  void setAmfUeNgapId(const unsigned long& id) override;
  void setRanUeNgapId(const uint32_t& id) override;
  bool decode(Ngap_NGAP_PDU_t* ngapMsgPdu) override;

  void setRANStatusTransfer_TransparentContainer(
      const long& drbIDValue, const long& ulPdcpValue,
      const long& ulHfnPdcpValue, const long& dlPdcpValue,
      const long& dlHfnPdcpValue);
  void getRANStatusTransfer_TransparentContainer(
      long& drbIDValue, long& ulPdcpValue, long& ulHfnPdcpValue,
      long& dlPdcpValue, long& dlHfnPdcpValue);

 private:
  Ngap_DownlinkRANStatusTransfer_t* downlinkranstatustransferIEs;
  // AMF_UE_NGAP_ID (Mandatory)
  // RAN_UE_NGAP_ID (Mandatory)
  RanStatusTransferTransparentContainer
      ranStatusTransfer_TransparentContainer;  // Mandatory
};
}  // namespace ngap
#endif
