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

#ifndef _MOBILITY_RESTRICTION_LIST_H_
#define _MOBILITY_RESTRICTION_LIST_H_

#include "PlmnId.hpp"

extern "C" {
#include "Ngap_MobilityRestrictionList.h"
}

namespace ngap {

class MobilityRestrictionList {
 public:
  MobilityRestrictionList();
  virtual ~MobilityRestrictionList();

  void setPLMN(const PlmnId& plmn);
  void getPLMN(PlmnId& plmn) const;

  bool encode(Ngap_MobilityRestrictionList_t& mobility_restriction_list);
  bool decode(const Ngap_MobilityRestrictionList_t& mobility_restriction_list);

 private:
  PlmnId servingPLMN;  // Mandatory
  // TODO: Equivalent PLMNs (optional)
  // TODO: RAT Restrictions (optional)
  // TODO: Forbidden Area Information (optional)
  // TODO: Service Area Information (optional)
  // TODO: Last E-UTRAN PLMN Identity (optional)
  // TODO: Core Network Type Restriction for Serving PLMN (optional)
  // TODO: Core Network Type Restriction for Equivalent PLMNs (optional)
};
}  // namespace ngap
#endif
