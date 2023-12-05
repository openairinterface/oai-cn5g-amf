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

#include "NgENBId.hpp"

extern "C" {
#include "conversions.h"
}

namespace ngap {

NgENB_ID::NgENB_ID() {
  present_ = Ngap_NgENB_ID_PR_NOTHING;
}

//------------------------------------------------------------------------------
NgENB_ID::~NgENB_ID() {
  present_ = Ngap_NgENB_ID_PR_NOTHING;
}

//------------------------------------------------------------------------------
void NgENB_ID::get(uint32_t& id, Ngap_NgENB_ID_PR& present) const {
  present = present_;
  switch (present_) {
    case Ngap_NgENB_ID_PR_macroNgENB_ID: {
      id = id_.macro_ngenb_id;
    } break;
    case Ngap_NgENB_ID_PR_shortMacroNgENB_ID: {
      id = id_.short_macro_ngENB_id;
    } break;
    case Ngap_NgENB_ID_PR_longMacroNgENB_ID: {
      id = id_.short_macro_ngENB_id;
    } break;
    default: {
      present = Ngap_NgENB_ID_PR_NOTHING;
    }
  }
}

//------------------------------------------------------------------------------
void NgENB_ID::set(const uint32_t& id, const Ngap_NgENB_ID_PR& present) {
  present_ = present;
  switch (present) {
    case Ngap_NgENB_ID_PR_macroNgENB_ID: {
      id_.macro_ngenb_id = id;
    } break;
    case Ngap_NgENB_ID_PR_shortMacroNgENB_ID: {
      id_.short_macro_ngENB_id = id;
    } break;
    case Ngap_NgENB_ID_PR_longMacroNgENB_ID: {
      id_.short_macro_ngENB_id = id;
    } break;
    default: {
      present_ = Ngap_NgENB_ID_PR_NOTHING;
    }
  }
}

//------------------------------------------------------------------------------
bool NgENB_ID::encode(Ngap_NgENB_ID_t& ng_enb_id) {
  ng_enb_id.present = present_;
  switch (present_) {
    case Ngap_NgENB_ID_PR_macroNgENB_ID: {
      INT24_TO_BIT_STRING(
          id_.macro_ngenb_id && 0x00ffffff, &ng_enb_id.choice.macroNgENB_ID,
          20);
    } break;
    case Ngap_NgENB_ID_PR_shortMacroNgENB_ID: {
      INT24_TO_BIT_STRING(
          id_.short_macro_ngENB_id && 0x00ffffff,
          &ng_enb_id.choice.shortMacroNgENB_ID, 18);
    } break;
    case Ngap_NgENB_ID_PR_longMacroNgENB_ID: {
      INT24_TO_BIT_STRING(
          id_.long_macro_ngENB_id && 0x00ffffff,
          &ng_enb_id.choice.longMacroNgENB_ID, 21);
    } break;
    default: {
      present_ = Ngap_NgENB_ID_PR_NOTHING;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool NgENB_ID::decode(const Ngap_NgENB_ID_t& ng_enb_id) {
  present_ = ng_enb_id.present;
  switch (present_) {
    case Ngap_NgENB_ID_PR_macroNgENB_ID: {
      BIT_STRING_TO_INT24(&ng_enb_id.choice.macroNgENB_ID, id_.macro_ngenb_id);
    } break;
    case Ngap_NgENB_ID_PR_shortMacroNgENB_ID: {
      BIT_STRING_TO_INT24(
          &ng_enb_id.choice.shortMacroNgENB_ID, id_.short_macro_ngENB_id);
    } break;
    case Ngap_NgENB_ID_PR_longMacroNgENB_ID: {
      BIT_STRING_TO_INT24(
          &ng_enb_id.choice.shortMacroNgENB_ID, id_.long_macro_ngENB_id);
    } break;
    default: {
      present_ = Ngap_NgENB_ID_PR_NOTHING;
    }
  }
  return true;
}

}  // namespace ngap
