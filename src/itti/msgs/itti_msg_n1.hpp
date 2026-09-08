/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _ITTI_AMF_N1_H_
#define _ITTI_AMF_N1_H_

#include "bstrlib.h"
#include "itti_msg.hpp"
#include "utils.hpp"

class itti_msg_n1 : public itti_msg {
 public:
  itti_msg_n1(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {
    is_nas_signalling_estab_req = false;
    ran_ue_ngap_id              = 0;
    amf_ue_ngap_id              = INVALID_AMF_UE_NGAP_ID;
  }
  itti_msg_n1(const itti_msg_n1&) = delete;
  virtual ~itti_msg_n1()          = default;

 public:
  bool is_nas_signalling_estab_req;
  uint64_t amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id;
};

class itti_uplink_nas_data_ind : public itti_msg_n1 {
 public:
  itti_uplink_nas_data_ind(const task_id_t origin, const task_id_t destination)
      : itti_msg_n1(UL_NAS_DATA_IND, origin, destination) {
    nas_msg       = nullptr;
    mcc           = {};
    mnc           = {};
    is_guti_valid = false;
    guti          = {};
  }
  itti_uplink_nas_data_ind(const itti_uplink_nas_data_ind&) = delete;
  virtual ~itti_uplink_nas_data_ind() {
    oai::utils::utils::bdestroy_wrapper(&nas_msg);
  }

 public:
  bstring nas_msg;
  std::string mcc;
  std::string mnc;
  bool is_guti_valid;
  std::string guti;
};

class itti_downlink_nas_transfer : public itti_msg_n1 {
 public:
  itti_downlink_nas_transfer(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n1(DOWNLINK_NAS_TRANSFER, origin, destination) {
    dl_nas         = nullptr;
    n2sm           = nullptr;
    is_n2sm_set    = false;
    pdu_session_id = 0;
    n2sm_info_type = {};
  }
  itti_downlink_nas_transfer(const itti_downlink_nas_transfer&) = delete;
  virtual ~itti_downlink_nas_transfer() {
    oai::utils::utils::bdestroy_wrapper(&dl_nas);
    oai::utils::utils::bdestroy_wrapper(&n2sm);
  }

 public:
  bstring dl_nas;
  bstring n2sm;
  bool is_n2sm_set;
  uint8_t pdu_session_id;
  std::string n2sm_info_type;
};

#endif
