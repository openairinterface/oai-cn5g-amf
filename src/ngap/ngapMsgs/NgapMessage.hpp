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

#ifndef _NGAP_MESSAGE_H_
#define _NGAP_MESSAGE_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "RAN-UE-NGAP-ID.hpp"

extern "C" {
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

enum class NgapMessageType {
  UNKNOWN_NGAP_MESSAGE                       = 0,
  AMF_CONFIGURATION_UPDATE                   = 1,
  AMF_CONFIGURATION_UPDATE_ACKNOWLEDGE       = 2,
  AMF_CONFIGURATION_UPDATE_FAILURE           = 3,
  RAN_CONFIGURATION_UPDATE                   = 4,
  RAN_CONFIGURATION_UPDATE_ACKNOWLEDGE       = 5,
  RAN_CONFIGURATION_UPDATE_FAILURE           = 6,
  HANDOVER_CANCEL                            = 7,
  HANDOVER_CANCEL_ACKNOWLEDGE                = 8,
  HANDOVER_REQUIRED                          = 9,
  HANDOVER_COMMAND                           = 10,
  HANDOVER_PREPARATION_FAILURE               = 11,
  HANDOVER_REQUEST                           = 12,
  HANDOVER_REQUEST_ACKNOWLEDGE               = 13,
  HANDOVER_FAILURE                           = 14,
  INITIAL_CONTEXT_SETUP_REQUEST              = 15,
  INITIAL_CONTEXT_SETUP_RESPONSE             = 16,
  INITIAL_CONTEXT_SETUP_FAILURE              = 17,
  NG_RESET                                   = 18,
  NG_RESET_ACKNOWLEDGE                       = 19,
  NG_SETUP_REQUEST                           = 20,
  NG_SETUP_RESPONSE                          = 21,
  NG_SETUP_FAILURE                           = 22,
  PATH_SWITCH_REQUEST                        = 23,
  PATH_SWITCH_REQUEST_ACKNOWLEDGE            = 24,
  PATH_SWITCH_REQUEST_FAILURE                = 25,
  PDU_SESSION_RESOURCE_MODIFY_REQUEST        = 26,
  PDU_SESSION_RESOURCE_MODIFY_RESPONSE       = 27,
  PDU_SESSION_RESOURCE_MODIFY_INDICATION     = 28,
  PDU_SESSION_RESOURCE_MODIFY_CONFIRM        = 29,
  PDU_SESSION_RESOURCE_RELEASE_COMMAND       = 30,
  PDU_SESSION_RESOURCE_RELEASE_RESPONSE      = 31,
  PDU_SESSION_RESOURCE_SETUP_REQUEST         = 32,
  PDU_SESSION_RESOURCE_SETUP_RESPONSE        = 33,
  UE_CONTEXT_MODIFICATION_REQUEST            = 34,
  UE_CONTEXT_MODIFICATION_RESPONSE           = 35,
  UE_CONTEXT_MODIFICATION_FAILURE            = 36,
  UE_CONTEXT_RELEASE_COMMAND                 = 37,
  UE_CONTEXT_RELEASE_COMPLETE                = 38,
  WRITE_REPLACE_WARNING_REQUEST              = 39,
  WRITE_REPLACE_WARNING_RESPONSE             = 40,
  PWS_CANCEL_REQUEST                         = 41,
  PWS_CANCEL_RESPONSE                        = 42,
  UE_RADIO_CAPABILITY_CHECK_REQUEST          = 43,
  UE_RADIO_CAPABILITY_CHECK_RESPONSE         = 44,
  DOWNLINK_RAN_CONFIGURATION_TRANSFER        = 45,
  DOWNLINK_RAN_STATUS_TRANSFER               = 46,
  DOWNLINK_NAS_TRANSPORT                     = 47,
  ERROR_INDICATION                           = 48,
  UPLINK_RAN_CONFIGURATION_TRANSFER          = 49,
  UPLINK_RAN_STATUS_TRANSFER                 = 50,
  HANDOVER_NOTIFY                            = 51,
  INITIAL_UE_MESSAGE                         = 52,
  NAS_NON_DELIVERY_INDICATION                = 53,
  PAGING                                     = 54,
  PDU_SESSION_RESOURCE_NOTIFY                = 55,
  REROUTE_NAS_REQUEST                        = 56,
  UE_CONTEXT_RELEASE_REQUEST                 = 57,
  UPLINK_NAS_TRANSPORT                       = 58,
  AMF_STATUS_INDICATION                      = 59,
  PWS_RESTART_INDICATION                     = 60,
  PWS_FAILURE_INDICATION                     = 61,
  DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT     = 62,
  UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT       = 63,
  DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT = 64,
  UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT   = 65,
  TRACE_START                                = 66,
  TRACE_FAILURE_INDICATION                   = 67,
  DEACTIVATE_TRACE                           = 68,
  CELL_TRAFFIC_TRACE                         = 69,
  LOCATION_REPORTING_CONTROL                 = 70,
  LOCATION_REPORTING_FAILURE_INDICATION      = 71,
  LOCATION_REPORT                            = 72,
  UE_TNLA_BINDING_RELEASE_REQUEST            = 73,
  UE_RADIO_CAPABILITY_INFO_INDICATION        = 74,
  RRC_INACTIVE_TRANSITION_REPORT             = 75,
  OVERLOAD_START                             = 76,
  OVERLOAD_STOP                              = 77,
  SECONDARY_RAT_DATA_USAGE_REPORT            = 78,
  UPLINK_RIM_INFORMATION_TRANSFER            = 79,
  DOWNLINK_RIM_INFORMATION_TRANSFER          = 80
};

class NgapMessage {
 public:
  NgapMessage(NgapMessage const&) = delete;
  NgapMessage();
  virtual ~NgapMessage();

  void setMessageType(NgapMessageType messageType);
  virtual bool decodeFromPdu(Ngap_NGAP_PDU_t* msgPdu) = 0;
  int Encode(uint8_t* buf, int bufSize);
  void encode2NewBuffer(uint8_t*& buf, int& encoded_size);

 protected:
  Ngap_NGAP_PDU_t* ngapPdu;

 private:
  MessageType messageTypeIE;
};

}  // namespace ngap
#endif
