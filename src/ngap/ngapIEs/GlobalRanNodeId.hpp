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

#ifndef _GLOBAL_RAN_NODE_ID_H_
#define _GLOBAL_RAN_NODE_ID_H_

extern "C" {
#include "Ngap_GlobalRANNodeID.h"
}

#include <optional>

#include "GlobalNgENBId.hpp"
#include "GlobalgNBId.hpp"

namespace ngap {

class GlobalRanNodeId {
 public:
  GlobalRanNodeId();
  virtual ~GlobalRanNodeId();

  void set(const GlobalgNBId& global_gnb_id);
  bool get(GlobalgNBId& global_gnb_id) const;

  void set(const GlobalNgENBId& global_ng_enb_id);
  bool get(GlobalNgENBId& global_ng_enb_id) const;

  void setChoiceOfRanNodeId(const Ngap_GlobalRANNodeID_PR& id_present);
  Ngap_GlobalRANNodeID_PR getChoiceOfRanNodeId() const;

  bool encode(Ngap_GlobalRANNodeID_t&);
  bool decode(const Ngap_GlobalRANNodeID_t&);

 private:
  std::optional<GlobalgNBId> global_gnb_id_;
  std::optional<GlobalNgENBId> global_ng_enb_id_;
  // TODO: Global N3IWF ID
  Ngap_GlobalRANNodeID_PR id_present_;
};

}  // namespace ngap

#endif
