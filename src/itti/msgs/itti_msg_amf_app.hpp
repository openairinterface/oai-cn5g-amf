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

#ifndef _AMF_APP_ITTI_H_
#define _AMF_APP_ITTI_H_

#include <string>

#include "NgapIesStruct.hpp"
#include "itti_msg.hpp"
using namespace oai::ngap;
#include "bstrlib.h"

class itti_msg_amf_app : public itti_msg {
 public:
  itti_msg_amf_app(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {
    gnb_id         = 0;
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  }

  itti_msg_amf_app(const itti_msg_amf_app& i) : itti_msg(i) {
    gnb_id         = i.gnb_id;
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }

  uint32_t gnb_id;
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
};

class itti_nas_signalling_establishment_request : public itti_msg_amf_app {
 public:
  itti_nas_signalling_establishment_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(NAS_SIG_ESTAB_REQ, origin, destination) {
    rrc_cause            = 0;
    ueCtxReq             = 0;
    cgi                  = {};
    tai                  = {};
    nas_buf              = nullptr;
    is_5g_s_tmsi_present = false;
    _5g_s_tmsi           = {};
  }
  itti_nas_signalling_establishment_request(
      const itti_nas_signalling_establishment_request& i)
      : itti_msg_amf_app(i) {
    rrc_cause            = i.rrc_cause;
    ueCtxReq             = i.ueCtxReq;
    cgi                  = i.cgi;
    tai                  = i.tai;
    nas_buf              = i.nas_buf;
    is_5g_s_tmsi_present = i.is_5g_s_tmsi_present;
    _5g_s_tmsi           = i._5g_s_tmsi;
  }

  int rrc_cause;
  int ueCtxReq;
  NrCgi_t cgi;
  Tai_t tai;
  bstring nas_buf;
  bool is_5g_s_tmsi_present;
  std::string _5g_s_tmsi;
};

class itti_n1n2_message_transfer_request : public itti_msg_amf_app {
 public:
  itti_n1n2_message_transfer_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(N1N2_MESSAGE_TRANSFER_REQ, origin, destination) {
    supi             = {};
    n1sm             = nullptr;
    n2sm             = nullptr;
    nrppa_pdu        = nullptr;
    routing_id       = nullptr;
    is_n2sm_set      = false;
    is_n1sm_set      = false;
    is_nrppa_pdu_set = false;
    is_ppi_set       = false;

    n2sm_info_type = {};
    pdu_session_id = 0;
    ppi            = 0;
  }
  itti_n1n2_message_transfer_request(
      const itti_n1n2_message_transfer_request& i)
      : itti_msg_amf_app(i) {
    supi             = i.supi;
    n1sm             = i.n1sm;
    n2sm             = i.n2sm;
    nrppa_pdu        = i.nrppa_pdu;
    routing_id       = i.routing_id;
    is_n2sm_set      = i.is_n2sm_set;
    is_n1sm_set      = i.is_n1sm_set;
    is_nrppa_pdu_set = i.is_nrppa_pdu_set;
    is_ppi_set       = i.is_ppi_set;

    n2sm_info_type = i.n2sm_info_type;
    pdu_session_id = i.pdu_session_id;
    ppi            = i.ppi;
  }

  std::string supi;
  bstring n1sm;
  bstring n2sm;
  bstring nrppa_pdu;
  bstring routing_id;
  bool is_n2sm_set;
  bool is_n1sm_set;
  bool is_nrppa_pdu_set;
  uint8_t pdu_session_id;
  std::string n2sm_info_type;
  bool is_ppi_set;
  uint8_t ppi;
  // other parameters
};

class itti_non_ue_n2_message_transfer_request : public itti_msg_amf_app {
 public:
  itti_non_ue_n2_message_transfer_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(NON_UE_N2_MESSAGE_TRANSFER_REQ, origin, destination) {
    nrppa_pdu        = nullptr;
    routing_id       = nullptr;
    is_nrppa_pdu_set = false;
  }
  itti_non_ue_n2_message_transfer_request(
      const itti_non_ue_n2_message_transfer_request& i)
      : itti_msg_amf_app(i) {
    nrppa_pdu        = i.nrppa_pdu;
    routing_id       = i.routing_id;
    is_nrppa_pdu_set = i.is_nrppa_pdu_set;
  }

  bstring nrppa_pdu;
  bstring routing_id;
  bool is_nrppa_pdu_set;
  // other parameters
};

#endif
