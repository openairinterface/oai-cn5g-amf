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

#ifndef _UNAVAILABLE_GUAMI_ITEM_H
#define _UNAVAILABLE_GUAMI_ITEM_H

#include <optional>

#include "AmfName.hpp"
#include "Guami.hpp"
#include "TimerApproachForGuamiRemoval.hpp"

extern "C" {
#include "Ngap_UnavailableGUAMIItem.h"
}

namespace ngap {

class UnavailableGuamiItem {
 public:
  UnavailableGuamiItem();
  virtual ~UnavailableGuamiItem();

  void setGuami(const Guami& guami);
  void getGuami(Guami& guami) const;

  void setTimerApproachForGuamiRemoval(
      const TimerApproachForGuamiRemoval& timer);
  void getTimerApproachForGuamiRemoval(
      std::optional<TimerApproachForGuamiRemoval>& timer) const;

  void setBackupAmfName(const AmfName& name);
  void getBackupAmfName(std::optional<AmfName>& name) const;

  bool encode(Ngap_UnavailableGUAMIItem& item) const;
  bool decode(const Ngap_UnavailableGUAMIItem& item);

 private:
  Guami m_Guami;
  std::optional<TimerApproachForGuamiRemoval>
      m_TimerApproachForGuamiRemoval;      // Optional
  std::optional<AmfName> m_BackupAmfName;  // Optional
};

}  // namespace ngap

#endif
