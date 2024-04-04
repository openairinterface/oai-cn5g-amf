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

#ifndef _NR_CELL_IDENTITY_H_
#define _NR_CELL_IDENTITY_H_

extern "C" {
#include "Ngap_NRCellIdentity.h"
}

namespace ngap {

class NrCellIdentity {
 public:
  NrCellIdentity();
  virtual ~NrCellIdentity();

  unsigned long get() const;
  void set(const unsigned long& nrCellIdentity);

  bool encode(Ngap_NRCellIdentity_t& nrCellIdentity) const;
  bool decode(const Ngap_NRCellIdentity_t& nrCellIdentity);

 private:
  unsigned long
      m_NrCellIdentity;  // TODO: only 32 bits while NRCELL is a bitstring(36)
};
}  // namespace ngap

#endif
