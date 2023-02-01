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
#include "PlmnId.hpp"
#include "NgapIEsStruct.hpp"

extern "C" {
#include "Ngap_NR-CGI.h"
}

namespace ngap {

class NR_CGI {
 public:
  NR_CGI();
  virtual ~NR_CGI();

  void setNR_CGI(const PlmnId&, const NRCellIdentity&);
  void getNR_CGI(PlmnId&, NRCellIdentity&);
  void setNR_CGI(
      const std::string& mcc, const std::string& mnc,
      const unsigned long& nrcellidentity);
  void setNR_CGI(const struct NrCgi_s& cig);
  void getNR_CGI(struct NrCgi_s& cig);

  bool encode2NR_CGI(Ngap_NR_CGI_t*);
  bool decodefromNR_CGI(Ngap_NR_CGI_t*);

 private:
  PlmnId plmnId;                  // Mandatory
  NRCellIdentity nRCellIdentity;  // Mandatory
};
}  // namespace ngap

#endif
