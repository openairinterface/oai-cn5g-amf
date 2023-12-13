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

#include "DefaultPagingDrx.hpp"

namespace ngap {

//------------------------------------------------------------------------------
DefaultPagingDrx::DefaultPagingDrx() {
  paging_drx_ = Ngap_PagingDRX_v32;
}

//------------------------------------------------------------------------------
DefaultPagingDrx::~DefaultPagingDrx() {}

//------------------------------------------------------------------------------
void DefaultPagingDrx::setValue(const e_Ngap_PagingDRX& paging_drx) {
  paging_drx_ = paging_drx;
}

//------------------------------------------------------------------------------
e_Ngap_PagingDRX DefaultPagingDrx::getValue() const {
  return paging_drx_;
}

//------------------------------------------------------------------------------
bool DefaultPagingDrx::encode(Ngap_PagingDRX_t& paging_drx) const {
  paging_drx = paging_drx_;
  return true;
}

//------------------------------------------------------------------------------
bool DefaultPagingDrx::decode(const Ngap_PagingDRX_t& paging_drx) {
  switch (paging_drx) {
    case 32: {
      paging_drx_ = Ngap_PagingDRX_v32;
    } break;
    case 64: {
      paging_drx_ = Ngap_PagingDRX_v64;
    } break;

    case 128: {
      paging_drx_ = Ngap_PagingDRX_v128;
    } break;

    case 256: {
      paging_drx_ = Ngap_PagingDRX_v256;
    } break;

    default: {
      paging_drx_ = (e_Ngap_PagingDRX) paging_drx;
    }
  }

  return true;
}

}  // namespace ngap
