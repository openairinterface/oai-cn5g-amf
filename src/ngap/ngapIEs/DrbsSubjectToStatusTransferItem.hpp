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

#ifndef _DRBS_SUBJECT_TO_STATUS_TRANSFERT_ITEM_H_
#define _DRBS_SUBJECT_TO_STATUS_TRANSFERT_ITEM_H_

#include "DrbStatusDl.hpp"
#include "DrbStatusUl.hpp"

extern "C" {
#include "Ngap_DRBsSubjectToStatusTransferItem.h"
}

namespace ngap {

class DrbSubjectToStatusTransferItem {
 public:
  DrbSubjectToStatusTransferItem();
  virtual ~DrbSubjectToStatusTransferItem();

  void getdRBSubjectItem(
      Ngap_DRB_ID_t& dRB_ID, DrbStatusUl& dRB_UL, DrbStatusDl& dRB_DL) const;
  void setdRBSubjectItem(
      const Ngap_DRB_ID_t& dRB_ID, const DrbStatusUl& dRB_UL,
      const DrbStatusDl& dRB_DL);

  bool encode(Ngap_DRBsSubjectToStatusTransferItem_t& dRB_item);
  bool decode(const Ngap_DRBsSubjectToStatusTransferItem_t& dRB_item);

 private:
  Ngap_DRB_ID_t drbID;  // Mandatory
  DrbStatusUl drbUL;    // Mandatory
  DrbStatusDl drbDL;    // Mandatory
  // TODO: Old Associated QoS Flow List - UL End Marker Expected
};
}  // namespace ngap
#endif
