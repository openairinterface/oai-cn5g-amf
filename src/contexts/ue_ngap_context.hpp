/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_NGAP_CONTEXT_H_
#define _UE_NGAP_CONTEXT_H_

#include <vector>
#include <cstdint>

#include "amf.hpp"
#include "utils.hpp"

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

    release_gnb                   = {};
    release_cause                 = {};
    gnb_assoc_id                  = {};
    target_gnb_assoc_id           = {};
    ue_context_request            = false;
    s_tmsi_5g                     = {};
    s_setid                       = {};
    s_pointer                     = {};
    s_tmsi                        = {};
    tai                           = {};
    ng_ue_state                   = NGAP_UE_INVALID_STATE;
    ncc                           = 0;
    initial_ue_msg.buf            = new uint8_t[BUFFER_SIZE_1024];
    initial_ue_msg.size           = 0;
    ue_radio_cap_ind              = nullptr;
    ue_radio_cap_for_paging_nr    = nullptr;
    ue_radio_cap_for_paging_eutra = nullptr;
  }

  virtual ~ue_ngap_context() {
    delete[] initial_ue_msg.buf;
    initial_ue_msg.buf  = nullptr;
    initial_ue_msg.size = 0;
    oai::utils::utils::bdestroy_wrapper(&ue_radio_cap_ind);
    oai::utils::utils::bdestroy_wrapper(&ue_radio_cap_for_paging_nr);
    oai::utils::utils::bdestroy_wrapper(&ue_radio_cap_for_paging_eutra);
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
  std::vector<Tai_t>
      registration_area_tai_list;  // Populated on Registration Accept

  // State management, ue status over the air
  ng_ue_state_t ng_ue_state;
  uint8_t ncc;  // Next Hop Chaining Counter

  OCTET_STRING_t initial_ue_msg;  // for AMF re-allocation

  // Release Command Cause and source gNB ID in case of HO
  Ngap_CauseRadioNetwork_t release_cause;
  uint32_t release_gnb;
  bstring ue_radio_cap_ind;
  bstring ue_radio_cap_for_paging_nr;
  bstring ue_radio_cap_for_paging_eutra;
  std::map<uint8_t, OCTET_STRING_t> pdu_sessions_to_be_released;
};

#endif
