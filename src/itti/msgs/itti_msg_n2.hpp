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

#ifndef _ITTI_MSG_N2_H_
#define _ITTI_MSG_N2_H_

#include "HandoverNotifyMsg.hpp"
#include "HandoverRequestAck.hpp"
#include "HandoverRequiredMsg.hpp"
#include "InitialUeMessage.hpp"
#include "NgReset.hpp"
#include "NgSetupRequest.hpp"
#include "UeContextReleaseComplete.hpp"
#include "UeContextReleaseRequest.hpp"
#include "UeRadioCapabilityInfoIndication.hpp"
#include "UplinkNasTransport.hpp"
#include "UplinkRanStatusTransfer.hpp"
#include "itti_msg.hpp"
#include "sctp_server.hpp"
#include "utils.hpp"

using namespace oai::ngap;
using namespace sctp;

typedef struct pdu_session_info_s {
  bstring n2sm;
  bool is_n2sm_available;
} pdu_session_info_t;

class itti_msg_n2 : public itti_msg {
 public:
  itti_msg_n2(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {}
  itti_msg_n2(const itti_msg_n2& i) : itti_msg(i) {
    assoc_id = i.assoc_id;
    stream   = i.stream;
  }
  virtual ~itti_msg_n2() {}

  sctp_assoc_id_t assoc_id;
  sctp_stream_id_t stream;
};

class itti_new_sctp_association : public itti_msg_n2 {
 public:
  itti_new_sctp_association(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NEW_SCTP_ASSOCIATION, origin, destination) {}
  virtual ~itti_new_sctp_association(){};
};

class itti_ng_setup_request : public itti_msg_n2 {
 public:
  itti_ng_setup_request(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NG_SETUP_REQ, origin, destination) {
    ngSetupReq = nullptr;
  }
  itti_ng_setup_request(const itti_ng_setup_request& i) : itti_msg_n2(i) {
    ngSetupReq = i.ngSetupReq;
  }
  virtual ~itti_ng_setup_request() { delete ngSetupReq; }

 public:
  NgSetupRequestMsg* ngSetupReq;
};

class itti_ng_reset : public itti_msg_n2 {
 public:
  itti_ng_reset(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NG_RESET, origin, destination) {
    ngReset = nullptr;
  }
  itti_ng_reset(const itti_ng_reset& i) : itti_msg_n2(i) {
    ngReset = i.ngReset;
  }
  virtual ~itti_ng_reset() { delete ngReset; }

 public:
  NgResetMsg* ngReset;
};

class itti_ng_shutdown : public itti_msg_n2 {
 public:
  itti_ng_shutdown(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NG_SHUTDOWN, origin, destination) {}
  itti_ng_shutdown(const itti_ng_shutdown& i) : itti_msg_n2(i) {}
  virtual ~itti_ng_shutdown() {}
};

class itti_initial_ue_message : public itti_msg_n2 {
 public:
  itti_initial_ue_message(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(INITIAL_UE_MSG, origin, destination) {
    initUeMsg = nullptr;
  }
  itti_initial_ue_message(const itti_initial_ue_message& i) : itti_msg_n2(i) {
    initUeMsg = i.initUeMsg;
  }
  virtual ~itti_initial_ue_message() { delete initUeMsg; }

  InitialUeMessageMsg* initUeMsg;
};

class itti_ul_nas_transport : public itti_msg_n2 {
 public:
  itti_ul_nas_transport(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(ITTI_UL_NAS_TRANSPORT, origin, destination) {
    ulNas = nullptr;
  }
  itti_ul_nas_transport(const itti_ul_nas_transport& i) : itti_msg_n2(i) {
    ulNas = i.ulNas;
  }
  virtual ~itti_ul_nas_transport() { delete ulNas; }

  UplinkNasTransportMsg* ulNas;
};

class itti_dl_nas_transport : public itti_msg_n2 {
 public:
  itti_dl_nas_transport(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(ITTI_DL_NAS_TRANSPORT, origin, destination) {
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    nas            = nullptr;
  }
  itti_dl_nas_transport(const itti_dl_nas_transport& i) : itti_msg_n2(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    nas            = bstrcpy(i.nas);
  }
  virtual ~itti_dl_nas_transport() {
    oai::utils::utils::bdestroy_wrapper(&nas);
  }

 public:
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  bstring nas;
};

class itti_initial_context_setup_request : public itti_msg_n2 {
 public:
  itti_initial_context_setup_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(INITIAL_CONTEXT_SETUP_REQUEST, origin, destination) {
    ran_ue_ngap_id = {};
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    kgnb           = nullptr;
    nas            = nullptr;
    is_sr          = false;
  }
  itti_initial_context_setup_request(
      const itti_initial_context_setup_request& i)
      : itti_msg_n2(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    kgnb           = bstrcpy(i.kgnb);
    nas            = bstrcpy(i.nas);
    is_sr          = i.is_sr;
    for (const auto& p : i.pdu_sessions) {
      pdu_session_info_t item = {};
      item.n2sm               = bstrcpy(p.second.n2sm);
      item.is_n2sm_available  = p.second.is_n2sm_available;
      uint8_t pdu_session_id  = p.first;
      pdu_sessions.insert(
          std::pair<uint8_t, pdu_session_info_t>(pdu_session_id, item));
    }
  }
  virtual ~itti_initial_context_setup_request() {
    oai::utils::utils::bdestroy_wrapper(&kgnb);
    oai::utils::utils::bdestroy_wrapper(&nas);
    for (auto& p : pdu_sessions) {
      pdu_session_info_t item = {};
      oai::utils::utils::bdestroy_wrapper(&p.second.n2sm);
    }
  }

  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  bstring kgnb;
  bstring nas;
  bool is_sr;
  std::map<uint8_t, pdu_session_info_t> pdu_sessions;
};

class itti_pdu_session_resource_setup_request : public itti_msg_n2 {
 public:
  itti_pdu_session_resource_setup_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(PDU_SESSION_RESOURCE_SETUP_REQUEST, origin, destination) {
    nas            = nullptr;
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  }
  itti_pdu_session_resource_setup_request(
      const itti_pdu_session_resource_setup_request& i)
      : itti_msg_n2(i) {
    nas            = bstrcpy(i.nas);
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    for (const auto& p : i.pdu_sessions) {
      pdu_session_info_t item = {};
      item.is_n2sm_available  = p.second.is_n2sm_available;
      if (item.is_n2sm_available) item.n2sm = bstrcpy(p.second.n2sm);
      uint8_t pdu_session_id = p.first;
      pdu_sessions.insert(
          std::pair<uint8_t, pdu_session_info_t>(pdu_session_id, item));
    }
  }
  virtual ~itti_pdu_session_resource_setup_request() {
    oai::utils::utils::bdestroy_wrapper(&nas);
    for (auto& p : pdu_sessions) {
      pdu_session_info_t item = {};
      oai::utils::utils::bdestroy_wrapper(&p.second.n2sm);
    }
  }

  bstring nas;
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  std::map<uint8_t, pdu_session_info_t> pdu_sessions;
};

class itti_pdu_session_resource_modify_request : public itti_msg_n2 {
 public:
  itti_pdu_session_resource_modify_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(PDU_SESSION_RESOURCE_MODIFY_REQUEST, origin, destination) {
    nas            = nullptr;
    n2sm           = nullptr;
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    pdu_session_id = 0;
    s_NSSAI        = {};
  }
  itti_pdu_session_resource_modify_request(
      const itti_pdu_session_resource_modify_request& i)
      : itti_msg_n2(i) {
    nas            = bstrcpy(i.nas);
    n2sm           = bstrcpy(i.n2sm);
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    pdu_session_id = i.pdu_session_id;
    s_NSSAI        = i.s_NSSAI;
  }
  virtual ~itti_pdu_session_resource_modify_request() {
    oai::utils::utils::bdestroy_wrapper(&nas);
    oai::utils::utils::bdestroy_wrapper(&n2sm);
  }

  bstring nas;
  bstring n2sm;
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  uint8_t pdu_session_id;
  oai::ngap::SNssai s_NSSAI;
};

class itti_pdu_session_resource_release_command : public itti_msg_n2 {
 public:
  itti_pdu_session_resource_release_command(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(PDU_SESSION_RESOURCE_RELEASE_COMMAND, origin, destination) {
    nas            = nullptr;
    n2sm           = nullptr;
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    pdu_session_id = 0;
  }
  itti_pdu_session_resource_release_command(
      const itti_pdu_session_resource_release_command& i)
      : itti_msg_n2(i) {
    nas            = bstrcpy(i.nas);
    n2sm           = bstrcpy(i.n2sm);
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    pdu_session_id = i.pdu_session_id;
  }
  virtual ~itti_pdu_session_resource_release_command() {
    oai::utils::utils::bdestroy_wrapper(&nas);
    oai::utils::utils::bdestroy_wrapper(&n2sm);
  }

  bstring nas;
  bstring n2sm;
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  uint8_t pdu_session_id;
};

class itti_ue_context_release_request : public itti_msg_n2 {
 public:
  itti_ue_context_release_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_CONTEXT_RELEASE_REQUEST, origin, destination) {
    ueCtxRel = nullptr;
  }
  itti_ue_context_release_request(const itti_ue_context_release_request& i)
      : itti_msg_n2(i) {
    ueCtxRel = i.ueCtxRel;
  }
  virtual ~itti_ue_context_release_request() { delete ueCtxRel; }

  UeContextReleaseRequestMsg* ueCtxRel;
};

class itti_ue_context_release_command : public itti_msg_n2 {
 public:
  itti_ue_context_release_command(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_CONTEXT_RELEASE_COMMAND, origin, destination) {
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    cause          = {};
  }
  itti_ue_context_release_command(const itti_ue_context_release_command& i)
      : itti_msg_n2(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    cause          = i.cause;
  }

 public:
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  oai::ngap::Cause cause;
};

class itti_ue_context_release_complete : public itti_msg_n2 {
 public:
  itti_ue_context_release_complete(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_CONTEXT_RELEASE_COMPLETE, origin, destination) {
    ueCtxRelCmpl = nullptr;
  }
  itti_ue_context_release_complete(const itti_ue_context_release_complete& i)
      : itti_msg_n2(i) {
    ueCtxRelCmpl = i.ueCtxRelCmpl;
  }
  virtual ~itti_ue_context_release_complete() { delete ueCtxRelCmpl; }

  UEContextReleaseCompleteMsg* ueCtxRelCmpl;
};

class itti_ue_radio_capability_indication : public itti_msg_n2 {
 public:
  itti_ue_radio_capability_indication(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_RADIO_CAP_IND, origin, destination) {
    ueRadioCap = nullptr;
  }
  itti_ue_radio_capability_indication(
      const itti_ue_radio_capability_indication& i)
      : itti_msg_n2(i) {
    ueRadioCap = i.ueRadioCap;
  }
  virtual ~itti_ue_radio_capability_indication() { delete ueRadioCap; }

  UeRadioCapabilityInfoIndicationMsg* ueRadioCap;
};

class itti_handover_required : public itti_msg_n2 {
 public:
  itti_handover_required(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(HANDOVER_REQUIRED_MSG, origin, destination) {
    handoverReq = nullptr;
  }
  itti_handover_required(const itti_handover_required& i) : itti_msg_n2(i) {
    handoverReq = i.handoverReq;
  }
  virtual ~itti_handover_required() { delete handoverReq; }

  HandoverRequiredMsg* handoverReq;
};

class itti_paging : public itti_msg_n2 {
 public:
  itti_paging(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(PAGING, origin, destination) {
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  }
  itti_paging(const itti_paging& i) : itti_msg_n2(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }

 public:
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
};

class itti_handover_request_ack : public itti_msg_n2 {
 public:
  itti_handover_request_ack(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(HANDOVER_REQUEST_ACK, origin, destination) {
    handoverRequestAck = nullptr;
  }
  itti_handover_request_ack(const itti_handover_request_ack& i)
      : itti_msg_n2(i) {
    handoverRequestAck = i.handoverRequestAck;
  }
  virtual ~itti_handover_request_ack() { delete handoverRequestAck; }

  HandoverRequestAck* handoverRequestAck;
};

class itti_handover_notify : public itti_msg_n2 {
 public:
  itti_handover_notify(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(HANDOVER_NOTIFY, origin, destination) {
    handoverNotify = nullptr;
  }
  itti_handover_notify(const itti_handover_notify& i) : itti_msg_n2(i) {
    handoverNotify = i.handoverNotify;
  }
  virtual ~itti_handover_notify() { delete handoverNotify; }

  HandoverNotifyMsg* handoverNotify;
};

class itti_uplink_ran_status_transfer : public itti_msg_n2 {
 public:
  itti_uplink_ran_status_transfer(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UPLINK_RAN_STATUS_TRANSFER, origin, destination) {
    uplinkRanTransfer = nullptr;
  }
  itti_uplink_ran_status_transfer(const itti_uplink_ran_status_transfer& i)
      : itti_msg_n2(i) {
    uplinkRanTransfer = i.uplinkRanTransfer;
  }
  virtual ~itti_uplink_ran_status_transfer() { delete uplinkRanTransfer; }

  UplinkRanStatusTransfer* uplinkRanTransfer;
};

class itti_rereoute_nas : public itti_msg_n2 {
 public:
  itti_rereoute_nas(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(REROUTE_NAS_REQ, origin, destination) {
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    amf_set_id     = 0;
  }
  itti_rereoute_nas(const itti_rereoute_nas& i) : itti_msg_n2(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
    amf_set_id     = i.amf_set_id;
  }

  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  uint16_t amf_set_id;
};

class itti_downlink_ue_associated_nrppa_transport : public itti_msg_n2 {
 public:
  itti_downlink_ue_associated_nrppa_transport(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(
            DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT, origin, destination) {
    nrppa_pdu      = nullptr;
    routing_id     = nullptr;
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  }
  itti_downlink_ue_associated_nrppa_transport(
      const itti_downlink_ue_associated_nrppa_transport& i)
      : itti_msg_n2(i) {
    nrppa_pdu      = bstrcpy(i.nrppa_pdu);
    routing_id     = bstrcpy(i.routing_id);
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }
  virtual ~itti_downlink_ue_associated_nrppa_transport() {
    oai::utils::utils::bdestroy_wrapper(&nrppa_pdu);
    oai::utils::utils::bdestroy_wrapper(&routing_id);
  };
  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  bstring nrppa_pdu;
  bstring routing_id;
};

class itti_uplink_ue_associated_nrppa_transport : public itti_msg_n2 {
 public:
  itti_uplink_ue_associated_nrppa_transport(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT, origin, destination) {
    nrppa_pdu      = nullptr;
    routing_id     = nullptr;
    ran_ue_ngap_id = 0;
    amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
  }
  itti_uplink_ue_associated_nrppa_transport(
      const itti_uplink_ue_associated_nrppa_transport& i)
      : itti_msg_n2(i) {
    nrppa_pdu      = bstrcpy(i.nrppa_pdu);
    routing_id     = bstrcpy(i.routing_id);
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }
  virtual ~itti_uplink_ue_associated_nrppa_transport() {
    oai::utils::utils::bdestroy_wrapper(&nrppa_pdu);
    oai::utils::utils::bdestroy_wrapper(&routing_id);
  }

  uint32_t ran_ue_ngap_id;
  uint64_t amf_ue_ngap_id;
  bstring nrppa_pdu;
  bstring routing_id;
};

class itti_downlink_non_ue_associated_nrppa_transport : public itti_msg_n2 {
 public:
  itti_downlink_non_ue_associated_nrppa_transport(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(
            DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT, origin, destination) {
    nrppa_pdu  = nullptr;
    routing_id = nullptr;
  }
  itti_downlink_non_ue_associated_nrppa_transport(
      const itti_downlink_non_ue_associated_nrppa_transport& i)
      : itti_msg_n2(i) {
    nrppa_pdu  = bstrcpy(i.nrppa_pdu);
    routing_id = bstrcpy(i.routing_id);
  }
  virtual ~itti_downlink_non_ue_associated_nrppa_transport() {
    oai::utils::utils::bdestroy_wrapper(&nrppa_pdu);
    oai::utils::utils::bdestroy_wrapper(&routing_id);
  };
  bstring nrppa_pdu;
  bstring routing_id;
};

class itti_uplink_non_ue_associated_nrppa_transport : public itti_msg_n2 {
 public:
  itti_uplink_non_ue_associated_nrppa_transport(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(
            UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT, origin, destination) {
    nrppa_pdu  = nullptr;
    routing_id = nullptr;
  }
  itti_uplink_non_ue_associated_nrppa_transport(
      const itti_uplink_ue_associated_nrppa_transport& i)
      : itti_msg_n2(i) {
    nrppa_pdu  = bstrcpy(i.nrppa_pdu);
    routing_id = bstrcpy(i.routing_id);
  }
  virtual ~itti_uplink_non_ue_associated_nrppa_transport() {
    oai::utils::utils::bdestroy_wrapper(&nrppa_pdu);
    oai::utils::utils::bdestroy_wrapper(&routing_id);
  }

  bstring nrppa_pdu;
  bstring routing_id;
};

#endif
