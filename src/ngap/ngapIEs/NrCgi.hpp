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

#ifndef _NR_CGI_H_
#define _NR_CGI_H_

#include "NRCellIdentity.hpp"
#include "NgapIEsStruct.hpp"
#include "PlmnId.hpp"

extern "C" {
#include "Ngap_NR-CGI.h"
}

namespace ngap {

class NrCgi {
 public:
  NrCgi();
  virtual ~NrCgi();

  void set(const PlmnId&, const NRCellIdentity&);
  void get(PlmnId&, NRCellIdentity&) const;

  void set(
      const std::string& mcc, const std::string& mnc,
      const unsigned long& nrcellidentity);

  void set(const struct NrCgi_s& cig);
  void get(struct NrCgi_s& cig) const;

  bool encode(Ngap_NR_CGI_t&);
  bool decode(const Ngap_NR_CGI_t&);

 private:
  PlmnId plmnId;                  // Mandatory
  NRCellIdentity nRCellIdentity;  // Mandatory
};
}  // namespace ngap

#endif
