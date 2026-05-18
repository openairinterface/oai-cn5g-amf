/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef SRC_ITTI_ITTI_MSG_HPP_INCLUDED_
#define SRC_ITTI_ITTI_MSG_HPP_INCLUDED_

#include <stdint.h>

#include <string>
#include <utility>

typedef enum {
  TASK_FIRST      = 0,
  TASK_ITTI_TIMER = TASK_FIRST,
  TASK_ASYNC_SHELL_CMD,
  TASK_NGAP,
  TASK_AMF_N2,
  TASK_AMF_N1,
  TASK_AMF_APP,
  TASK_AMF_SBI,
  AMF_SERVER,
  TASK_MAX,
  TASK_NONE,
  TASK_ALL = 255
} task_id_t;

typedef enum message_priorities_e {
  MESSAGE_PRIORITY_MAX       = 100,
  MESSAGE_PRIORITY_MAX_LEAST = 85,
  MESSAGE_PRIORITY_MED_PLUS  = 70,
  MESSAGE_PRIORITY_MED       = 55,
  MESSAGE_PRIORITY_MED_LEAST = 40,
  MESSAGE_PRIORITY_MIN_PLUS  = 25,
  MESSAGE_PRIORITY_MIN       = 10,
} message_priorities_t;

typedef enum {
  ITTI_MSG_TYPE_NONE  = -1,
  ITTI_MSG_TYPE_FIRST = 0,
  ASYNC_SHELL_CMD     = ITTI_MSG_TYPE_FIRST,
  NEW_SCTP_ASSOCIATION,
  NG_SETUP_REQ,
  NG_RESET,
  NG_SHUTDOWN,
  INITIAL_UE_MSG,
  ITTI_UL_NAS_TRANSPORT,
  ITTI_DL_NAS_TRANSPORT,
  INITIAL_CONTEXT_SETUP_REQUEST,
  PDU_SESSION_RESOURCE_SETUP_REQUEST,
  PDU_SESSION_RESOURCE_MODIFY_REQUEST,
  PDU_SESSION_RESOURCE_RELEASE_COMMAND,
  PDU_SESSION_RESOURCE_SETUP_RESPONSE,
  NSMF_PDU_SESSION_RELEASE_SM_CTX,
  UE_CONTEXT_RELEASE_REQUEST,
  UE_CONTEXT_RELEASE_COMMAND,
  UE_CONTEXT_RELEASE_COMPLETE,
  UE_RADIO_CAP_IND,
  UL_NAS_DATA_IND,  // task amf_n1 message id
  DOWNLINK_NAS_TRANSFER,
  NAS_SIG_ESTAB_REQ,  // task amf_app
  N1N2_MESSAGE_TRANSFER_REQ,
  NON_UE_N2_MESSAGE_TRANSFER_REQ,
  REROUTE_NAS_REQ,
  NSMF_PDU_SESSION_CREATE_SM_CTX,
  NSMF_PDU_SESSION_UPDATE_SM_CTX,
  SBI_N1_MESSAGE_NOTIFY,
  SBI_N2_INFO_NOTIFY,
  SBI_REGISTER_NF_INSTANCE_REQUEST,
  SBI_REGISTER_NF_INSTANCE_RESPONSE,
  SBI_UPDATE_NF_INSTANCE_REQUEST,
  SBI_UPDATE_NF_INSTANCE_RESPONSE,
  SBI_DEREGISTER_NF_INSTANCE_REQUEST,
  SBI_DEREGISTER_NF_INSTANCE_RESPONSE,
  SBI_SLICE_SELECTION_SUBSCRIPTION_DATA,
  SBI_NETWORK_SLICE_SELECTION_INFORMATION,
  SBI_NETWORK_SLICE_SELECTION_DISCOVERY,
  SBI_NF_INSTANCE_DISCOVERY,
  SBI_AMF_CONFIGURATION,
  SBI_UPDATE_AMF_CONFIGURATION,
  SBI_EVENT_EXPOSURE_REQUEST,
  SBI_NOTIFICATION_DATA,
  SBI_NOTIFY_SUBSCRIBED_EVENT,
  SBI_N1_MESSAGE_NOTIFICATION,
  SBI_N1N2_MESSAGE_SUBSCRIBE,
  SBI_N1N2_MESSAGE_UNSUBSCRIBE,
  SBI_NON_UE_N2_INFO_SUBSCRIBE,
  SBI_NON_UE_N2_INFO_UNSUBSCRIBE,
  SBI_NON_UE_N2_INFO_NOTIFY,
  SBI_PDU_SESSION_RELEASE_NOTIF,
  SBI_DETERMINE_LOCATION_REQUEST,
  SBI_UE_AUTHENTICATION_REQUEST,
  SBI_UE_AUTHENTICATION_CONFIRMATION,
  SBI_REGISTER_WITH_UDM,
  SBI_REGISTER_WITH_UDM_RESPONSE,
  SBI_RETRIEVE_AM_DATA,
  SBI_RETRIEVE_AM_DATA_RESPONSE,
  SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA,
  SBI_RETRIEVE_SMF_SELECTION_SUBSCRIPTION_DATA_RESPONSE,
  SBI_PCF_DISCOVERY,
  SBI_PCF_DISCOVERY_RESPONSE,
  SBI_AM_POLICY_ASSOCIATION,
  SBI_AM_POLICY_ASSOCIATION_RESPONSE,
  SBI_AM_POLICY_ASSOCIATION_TERMINATION,
  SBI_AM_POLICY_ASSOCIATION_TERMINATION_RESPONSE,
  SBI_AM_POLICY_ASSOCIATION_UPDATE,
  SBI_AM_POLICY_ASSOCIATION_UPDATE_RESPONSE,
  SBI_AM_POLICY_ASSOCIATION_RETRIEVAL,
  SBI_AM_POLICY_ASSOCIATION_RETRIEVAL_RESPONSE,
  SBI_AM_POLICY_UPDATE_NOTIFICATION,
  SBI_AM_POLICY_UPDATE_NOTIFICATION_RESPONSE,
  SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION,
  SBI_AM_POLICY_ASSOCIATION_TERMINATION_NOTIFICATION_RESPONSE,
  SBI_NUDM_SDM_NOTIFICATION,
  SBI_SDM_SUBSCRIBE,
  SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL,
  SBI_UE_CONTEXT_IN_SMF_DATA_RETRIEVAL_RESPONSE,
  SBI_AMF_STATUS_CHANGE_SUBSCRIBE_REQUEST,
  SBI_AMF_STATUS_CHANGE_UNSUBSCRIBE_REQUEST,
  SBI_AMF_STATUS_CHANGE_SUBSCRIBE_MODIFY,
  SBI_AMF_STATUS_CHANGE_NOTIFICATION,
  SBI_PROVIDE_DOMAIN_SELECTION_INFO,
  SBI_PROVIDE_LOCATION_INFO,
  HANDOVER_REQUIRED_MSG,
  HANDOVER_REQUEST_ACK,
  HANDOVER_NOTIFY,
  UPLINK_RAN_STATUS_TRANSFER,
  DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT,
  UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT,
  DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT,
  UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT,
  PAGING,
  TIME_OUT,
  HEALTH_PING,
  TERMINATE,
  ITTI_MSG_TYPE_MAX
} itti_msg_type_t;

typedef unsigned long message_number_t;

class itti_msg {
 public:
  itti_msg();
  itti_msg(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination);
  itti_msg(const itti_msg& i);

  itti_msg& operator=(itti_msg other) {
    std::swap(msg_num, other.msg_num);
    std::swap(origin, other.origin);
    std::swap(destination, other.destination);
    std::swap(msg_type, other.msg_type);
    return *this;
  }

  virtual ~itti_msg() = default;
  static const char* get_msg_name();

  message_number_t msg_num;
  task_id_t origin;
  task_id_t destination;
  itti_msg_type_t msg_type;
};

class itti_msg_timeout : public itti_msg {
 public:
  itti_msg_timeout(
      const task_id_t origin, const task_id_t destination, uint32_t timer_id,
      uint64_t arg1_user, std::string arg2_user)
      : itti_msg(TIME_OUT, origin, destination),
        timer_id(timer_id),
        arg1_user(arg1_user),
        arg2_user(arg2_user) {}
  itti_msg_timeout(const itti_msg_timeout& i)
      : itti_msg(i),
        timer_id(i.timer_id),
        arg1_user(i.arg1_user),
        arg2_user(i.arg2_user) {}
  virtual ~itti_msg_timeout(){};
  static const char* get_msg_name() { return "TIME_OUT"; };
  uint32_t timer_id;
  uint64_t arg1_user;
  std::string arg2_user;
};

class itti_msg_ping : public itti_msg {
 public:
  itti_msg_ping(
      const task_id_t origin, const task_id_t destination, uint32_t seq)
      : itti_msg(HEALTH_PING, origin, destination), seq(seq) {}
  itti_msg_ping(const itti_msg_ping& i) : itti_msg(i), seq(i.seq) {}
  virtual ~itti_msg_ping(){};
  static const char* get_msg_name() { return "HEALTH_PING"; };
  uint32_t seq;
};

class itti_msg_terminate : public itti_msg {
 public:
  itti_msg_terminate(const task_id_t origin, const task_id_t destination)
      : itti_msg(TERMINATE, origin, destination) {}
  itti_msg_terminate(const itti_msg_terminate& i) : itti_msg(i) {}
  virtual ~itti_msg_terminate(){};
  static const char* get_msg_name() { return "TERMINATE"; };
};

#endif /* SRC_ITTI_ITTI_MSG_HPP_INCLUDED_ */
