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

#ifndef _GNB_CONTEXT_H_
#define _GNB_CONTEXT_H_

#include <vector>

#include "3gpp_23.003.h"
#include "UeRetentionInformation.hpp"
#include "NgapIesStruct.hpp"
#include "sctp_server.hpp"

extern "C" {
#include "Ngap_PagingDRX.h"
#include "bstrlib.h"
}

using namespace sctp;
using namespace oai::ngap;

typedef enum {
  NGAP_INIT,
  NGAP_RESETING,
  NGAP_READY,
  NGAP_SHUTDOWN
} ng_gnb_state_t;

static const std::vector<std::string> ng_gnb_state_str = {
    "NGAP_INIT", "NGAP_RESETTING", "NGAP_READY", "NGAP_SHUTDOWN"};

class gnb_context {
 public:
  std::string gnb_name;
  uint32_t gnb_id;  // Global RAN Node ID

  ng_gnb_state_t ng_state;
  plmn_t plmn;
  e_Ngap_PagingDRX default_paging_drx;  // v32, v64, v128, v256
  std::vector<SupportedTaItem_t> supported_ta_list;
  std::optional<UeRetentionInformation> ue_retention_info;

  sctp_assoc_id_t sctp_assoc_id;
  sctp_stream_id_t next_sctp_stream;
  sctp_stream_id_t instreams;
  sctp_stream_id_t outstreams;
};

#endif
