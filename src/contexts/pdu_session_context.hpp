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

#ifndef _PDU_SESSION_CONTEXT_H_
#define _PDU_SESSION_CONTEXT_H_

#include "3gpp_29.502.h"
#include "amf.hpp"
#include "bstrlib.h"

typedef struct smf_context_info_s {
  bool info_available;
  std::string addr;
  uint32_t port;
  std::string uri_root;
  std::string api_version;
  std::string context_location;
} smf_context_info_t;

class pdu_session_context {
 public:
  pdu_session_context();
  ~pdu_session_context();

  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  uint8_t req_type;
  uint8_t pdu_session_id;
  bstring n2sm;
  bool is_n2sm_available;
  bstring n1sm;
  bool is_n1sm_available;
  std::string dnn;
  smf_context_info_t smf_info;
  snssai_t snssai;
  plmn_t plmn;
  bool is_ho_accepted;
  up_cnx_state_e
      up_cnx_state;  // N3 tunnel status (ACTIVATED, DEACTIVATED, ACTIVATING)
};

#endif
