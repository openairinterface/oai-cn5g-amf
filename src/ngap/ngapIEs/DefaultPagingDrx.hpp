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

#ifndef _DEFAULT_PAGING_DRX_H_
#define _DEFAULT_PAGING_DRX_H_

extern "C" {
#include "Ngap_PagingDRX.h"
}

namespace ngap {

class DefaultPagingDrx {
 public:
  DefaultPagingDrx();
  virtual ~DefaultPagingDrx();

  void setValue(const e_Ngap_PagingDRX& paging_drx);
  e_Ngap_PagingDRX getValue() const;

  bool encode(Ngap_PagingDRX_t&) const;
  bool decode(const Ngap_PagingDRX_t&);

 private:
  e_Ngap_PagingDRX paging_drx_;
};

}  // namespace ngap

#endif
