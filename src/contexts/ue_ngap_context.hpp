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

#ifndef _UE_NGAP_CONTEXT_H_
#define _UE_NGAP_CONTEXT_H_

#include "amf.hpp"

using namespace sctp;
typedef enum {
  NGAP_UE_INVALID_STATE,
  NGAP_UE_WAITING_CSR,  // Context Setup Response(CSR)
  NGAP_UE_HANDOVER,
  NGAP_UE_CONNECTED,
  NGAP_UE_WAITING_CRR
} ng_ue_state_t;

class ue_ngap_context {
 public:
  ue_ngap_context() {
    ran_ue_ngap_id        = 0;
    amf_ue_ngap_id        = INVALID_AMF_UE_NGAP_ID;
    target_ran_ue_ngap_id = 0;

    sctp_stream_recv = {};
    sctp_stream_send = {};

    release_gnb         = {};
    release_cause       = {};
    gnb_assoc_id        = {};
    target_gnb_assoc_id = {};
    ue_context_request  = false;
    s_tmsi_5g           = {};
    s_setid             = {};
    s_pointer           = {};
    s_tmsi              = {};
    tai                 = {};
    ng_ue_state         = NGAP_UE_INVALID_STATE;
    ncc                 = 0;
    initial_ue_msg.buf  = new uint8_t[BUFFER_SIZE_1024];
    initial_ue_msg.size = 0;
    ue_radio_cap_ind    = nullptr;
  }

  virtual ~ue_ngap_context() {
    delete[] initial_ue_msg.buf;
    initial_ue_msg.buf  = nullptr;
    initial_ue_msg.size = 0;
  }

  uint32_t ran_ue_ngap_id;         // 32bits
  uint64_t amf_ue_ngap_id;         // 40bits
  uint32_t target_ran_ue_ngap_id;  // 32bits, for HO

  sctp_stream_id_t sctp_stream_recv;    // used to decide which ue in gNB
  sctp_stream_id_t sctp_stream_send;    // used to decide which ue in gNB
  sctp_assoc_id_t gnb_assoc_id;         // to find which gnb this UE belongs to
  sctp_assoc_id_t target_gnb_assoc_id;  // for HO

  bool ue_context_request;

  uint32_t s_tmsi_5g;

  std ::string s_setid;
  std ::string s_pointer;
  std ::string s_tmsi;

  Tai_t tai;

  // State management, ue status over the air
  ng_ue_state_t ng_ue_state;
  uint8_t ncc;  // Next Hop Chaining Counter

  OCTET_STRING_t initial_ue_msg;  // for AMF re-allocation

  // Release Command Cause and source gNB ID in case of HO
  Ngap_CauseRadioNetwork_t release_cause;
  uint32_t release_gnb;
  bstring ue_radio_cap_ind;
  std::map<uint8_t, OCTET_STRING_t> pdu_sessions_to_be_released;
};

#endif
