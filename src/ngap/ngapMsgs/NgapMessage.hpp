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

#include "AmfUeNgapId.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "RanUeNgapId.hpp"

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
  UE_CONTEXT_SUSPEND_REQUEST                 = 45,
  UE_CONTEXT_SUSPEND_RESPONSE                = 46,
  UE_CONTEXT_SUSPEND_FAILURE                 = 47,
  UE_CONTEXT_RESUME_REQUEST                  = 48,
  UE_CONTEXT_RESUME_RESPONSE                 = 49,
  UE_CONTEXT_RESUME_FAILURE                  = 50,
  UE_RADIO_CAPABILITY_ID_MAPPING_REQUEST     = 51,
  UE_RADIO_CAPABILITY_ID_MAPPING_RESPONSE    = 52,
  DOWNLINK_RAN_CONFIGURATION_TRANSFER        = 53,
  DOWNLINK_RAN_STATUS_TRANSFER               = 54,
  DOWNLINK_NAS_TRANSPORT                     = 55,
  ERROR_INDICATION                           = 56,
  UPLINK_RAN_CONFIGURATION_TRANSFER          = 57,
  UPLINK_RAN_STATUS_TRANSFER                 = 58,
  HANDOVER_NOTIFY                            = 59,
  INITIAL_UE_MESSAGE                         = 60,
  NAS_NON_DELIVERY_INDICATION                = 61,
  PAGING                                     = 62,
  PDU_SESSION_RESOURCE_NOTIFY                = 63,
  REROUTE_NAS_REQUEST                        = 64,
  UE_CONTEXT_RELEASE_REQUEST                 = 65,
  UPLINK_NAS_TRANSPORT                       = 66,
  AMF_STATUS_INDICATION                      = 67,
  PWS_RESTART_INDICATION                     = 68,
  PWS_FAILURE_INDICATION                     = 69,
  DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT     = 70,
  UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT       = 71,
  DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT = 72,
  UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT   = 73,
  TRACE_START                                = 74,
  TRACE_FAILURE_INDICATION                   = 75,
  DEACTIVATE_TRACE                           = 76,
  CELL_TRAFFIC_TRACE                         = 77,
  LOCATION_REPORTING_CONTROL                 = 78,
  LOCATION_REPORTING_FAILURE_INDICATION      = 79,
  LOCATION_REPORT                            = 80,
  UE_TNLA_BINDING_RELEASE_REQUEST            = 81,
  UE_RADIO_CAPABILITY_INFO_INDICATION        = 82,
  RRC_INACTIVE_TRANSITION_REPORT             = 83,
  OVERLOAD_START                             = 84,
  OVERLOAD_STOP                              = 85,
  SECONDARY_RAT_DATA_USAGE_REPORT            = 86,
  UPLINK_RIM_INFORMATION_TRANSFER            = 87,
  DOWNLINK_RIM_INFORMATION_TRANSFER          = 88,
  RETRIEVE_UE_INFORMATION                    = 89,
  UE_INFORMATION_TRANSFER                    = 90,
  RAN_CP_RELOCATION_INDICATION               = 91,
  CONNECTION_ESTABLISHMENT_INDICATION        = 92,
  AMF_CP_RELOCATION_INDICATION               = 93,
  HANDOVER_SUCCESS                           = 94,
  UPLINK_RAN_EARLY_STATUS_TRANSFER           = 95,
  DOWNLINK_RAN_EARLY_STATUS_TRANSFER         = 96
};

class NgapMessage {
 public:
  NgapMessage(NgapMessage const&) = delete;
  NgapMessage();
  virtual ~NgapMessage();

  void setMessageType(NgapMessageType messageType);
  virtual bool decode(Ngap_NGAP_PDU_t* msgPdu) = 0;
  int Encode(uint8_t* buf, int bufSize);
  void encode2NewBuffer(uint8_t*& buf, int& encoded_size);

 protected:
  Ngap_NGAP_PDU_t* ngapPdu;

 private:
  MessageType messageTypeIE;
};

}  // namespace ngap
#endif
