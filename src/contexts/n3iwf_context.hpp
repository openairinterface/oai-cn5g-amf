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

#ifndef _N3IWF_CONTEXT_H_
#define _N3IWF_CONTEXT_H_

#include <vector>

#include "3gpp_23.003.h"
#include "NgapIEsStruct.hpp"
#include "sctp_server.hpp"

extern "C" {
#include "Ngap_PagingDRX.h"
#include "bstrlib.h"
}

using namespace sctp;
using namespace ngap;

typedef enum {
  NGAP_N3IWF_INIT,
  NGAP_N3IWF_RESETING,
  NGAP_N3IWF_READY,
  NGAP_N3IWF_SHUTDOWN
} ng_n3iwf_state_t;

static const std::vector<std::string> ng_n3iwf_state_str = {
    "NGAP_N3IWF_INIT", "NGAP_N3IWF_RESETTING", "NGAP_N3IWF_READY",
    "NGAP_N3IWF_SHUTDOWN"};

class n3iwf_context {
 public:
  ng_n3iwf_state_t ng_state;

  std::string n3iwf_name;
  uint16_t n3iwf_id;  // Global RAN Node ID
  plmn_t plmn;
  e_Ngap_PagingDRX default_paging_drx;  // v32, v64, v128, v256
  std::vector<SupportedTaItem_t> s_ta_list;
  bstring ue_radio_cap_ind;

  sctp_assoc_id_t sctp_assoc_id;
  sctp_stream_id_t next_sctp_stream;
  sctp_stream_id_t instreams;
  sctp_stream_id_t outstreams;
};

#endif
