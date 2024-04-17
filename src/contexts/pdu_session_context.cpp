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

#include "pdu_session_context.hpp"

//------------------------------------------------------------------------------
pdu_session_context::pdu_session_context() {
  is_n2sm_available       = false;
  is_n1sm_available       = false;
  ran_ue_ngap_id          = 0;
  amf_ue_ngap_id          = INVALID_AMF_UE_NGAP_ID;
  req_type                = 0;
  pdu_session_id          = 0;
  n2sm                    = nullptr;
  n1sm                    = nullptr;
  smf_info.info_available = false;
  smf_info.addr           = {};
  smf_info.api_version    = "v1";
  smf_info.port           = DEFAULT_HTTP2_PORT;
  snssai                  = {};
  plmn                    = {};
  is_ho_accepted          = false;
  up_cnx_state            = up_cnx_state_e::UPCNX_STATE_UNKNOWN;
}

//------------------------------------------------------------------------------
pdu_session_context::~pdu_session_context() {}
