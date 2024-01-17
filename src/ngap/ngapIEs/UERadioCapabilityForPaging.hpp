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

#ifndef _UE_RADIO_CAPABILITY_FOR_PAGING_H_
#define _UE_RADIO_CAPABILITY_FOR_PAGING_H_

#include <optional>

#include "UERadioCapabilityForPagingOfEUTRA.hpp"
#include "UERadioCapabilityForPagingOfNR.hpp"

extern "C" {
#include "Ngap_UERadioCapabilityForPaging.h"
}

namespace ngap {

class UERadioCapabilityForPaging {
 public:
  UERadioCapabilityForPaging();
  virtual ~UERadioCapabilityForPaging();

  void setUERadioCapabilityForPagingOfNR(const OCTET_STRING_t& capability);
  bool getUERadioCapabilityForPagingOfNR(OCTET_STRING_t& capability);

  void setUERadioCapabilityForPagingOfEUTRA(const OCTET_STRING_t& capability);
  bool getUERadioCapabilityForPagingOfEUTRA(OCTET_STRING_t& capability);

  bool encode(Ngap_UERadioCapabilityForPaging_t& ueRadioCapabilityForPaging);
  bool decode(
      const Ngap_UERadioCapabilityForPaging_t& ueRadioCapabilityForPaging);

 private:
  std::optional<UERadioCapabilityForPagingOfNR>
      ueRadioCapabilityForPagingOfNR;  // Optional
  std::optional<UERadioCapabilityForPagingOfEUTRA>
      ueRadioCapabilityForPagingOfEUTRA;  // Optional
};

}  // namespace ngap

#endif
