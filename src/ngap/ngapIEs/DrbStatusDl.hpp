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

#ifndef _DRB_STATUS_DL_H_
#define _DRB_STATUS_DL_H_

#include <optional>

#include "DrbStatusDl12.hpp"
#include "DrbStatusDl18.hpp"

extern "C" {
#include "Ngap_DRBStatusDL.h"
#include "Ngap_DRBStatusDL12.h"
#include "Ngap_DRBStatusDL18.h"
}

namespace ngap {

class DrbStatusDl {
 public:
  DrbStatusDl();
  virtual ~DrbStatusDl();

  void setDRBStatusDL18(const DrbStatusDl18& dL18);
  void getDRBStatusDL18(std::optional<DrbStatusDl18>& dL18) const;

  void setDRBStatusDL12(const DrbStatusDl12& dL12);
  void getDRBStatusDL12(std::optional<DrbStatusDl12>& dL12) const;

  bool encode(Ngap_DRBStatusDL_t& dL);
  bool decode(const Ngap_DRBStatusDL_t& dL);

 private:
  std::optional<DrbStatusDl18> dl18_;
  std::optional<DrbStatusDl12> dl12_;
};

}  // namespace ngap
#endif
