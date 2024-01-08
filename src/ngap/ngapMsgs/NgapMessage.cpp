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

#include "NgapMessage.hpp"

#include "logger.hpp"
#include "output_wrapper.hpp"

extern "C" {
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

namespace ngap {

//------------------------------------------------------------------------------
NgapMessage::NgapMessage() {
  ngapPdu = nullptr;
}

//------------------------------------------------------------------------------
NgapMessage::~NgapMessage() {
  ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, ngapPdu);
  Logger::ngap().debug("Free NGAP Message PDU");
}

//------------------------------------------------------------------------------
void NgapMessage::setMessageType(NgapMessageType messageType) {
  if (!ngapPdu) ngapPdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  switch (messageType) {
    case NgapMessageType::AMF_CONFIGURATION_UPDATE: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_AMFConfigurationUpdate);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_AMFConfigurationUpdate);
    } break;
    case NgapMessageType::AMF_CONFIGURATION_UPDATE_ACKNOWLEDGE: {
      // TODO:
    } break;
    case NgapMessageType::AMF_CONFIGURATION_UPDATE_FAILURE: {
      // TODO:
    } break;
    case NgapMessageType::RAN_CONFIGURATION_UPDATE: {
      // TODO:
    } break;
    case NgapMessageType::RAN_CONFIGURATION_UPDATE_ACKNOWLEDGE: {
      // TODO:
    } break;
    case NgapMessageType::RAN_CONFIGURATION_UPDATE_FAILURE: {
      // TODO:
    } break;
    case NgapMessageType::HANDOVER_CANCEL: {
      // TODO:
    } break;
    case NgapMessageType::HANDOVER_CANCEL_ACKNOWLEDGE: {
      // TODO:
    } break;
    case NgapMessageType::HANDOVER_REQUIRED: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_HandoverPreparation);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_HandoverRequired);
    } break;
    case NgapMessageType::HANDOVER_COMMAND: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_HandoverPreparation);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_HandoverCommand);
    } break;
    case NgapMessageType::HANDOVER_PREPARATION_FAILURE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_HandoverPreparation);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_unsuccessfulOutcome);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_UnsuccessfulOutcome__value_PR_HandoverPreparationFailure);
    } break;
    case NgapMessageType::HANDOVER_REQUEST: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_HandoverResourceAllocation);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_HandoverRequest);
    } break;
    case NgapMessageType::HANDOVER_REQUEST_ACKNOWLEDGE: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_HandoverResourceAllocation);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge);
    } break;
    case NgapMessageType::HANDOVER_FAILURE: {
      // TODO:
    } break;
    case NgapMessageType::INITIAL_CONTEXT_SETUP_REQUEST: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_InitialContextSetup);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_InitialContextSetupRequest);
    } break;
    case NgapMessageType::INITIAL_CONTEXT_SETUP_RESPONSE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_InitialContextSetup);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_InitialContextSetupResponse);
    } break;
    case NgapMessageType::INITIAL_CONTEXT_SETUP_FAILURE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_InitialContextSetup);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_unsuccessfulOutcome);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setValuePresent(
          Ngap_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure);
    } break;
    case NgapMessageType::NG_RESET: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGReset);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(Ngap_InitiatingMessage__value_PR_NGReset);

    } break;
    case NgapMessageType::NG_RESET_ACKNOWLEDGE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGReset);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_NGResetAcknowledge);
    } break;
    case NgapMessageType::NG_SETUP_REQUEST: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGSetup);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_NGSetupRequest);
    } break;
    case NgapMessageType::NG_SETUP_RESPONSE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGSetup);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_NGSetupResponse);
    } break;
    case NgapMessageType::NG_SETUP_FAILURE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGSetup);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_unsuccessfulOutcome);
      messageTypeIE.setValuePresent(
          Ngap_UnsuccessfulOutcome__value_PR_NGSetupFailure);
    } break;
    case NgapMessageType::PATH_SWITCH_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::PATH_SWITCH_REQUEST_ACKNOWLEDGE: {
      // TODO:
    } break;
    case NgapMessageType::PATH_SWITCH_REQUEST_FAILURE: {
      // TODO:
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_MODIFY_REQUEST: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_PDUSessionResourceModify);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_PDUSessionResourceModifyRequest);
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_MODIFY_RESPONSE: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_PDUSessionResourceModify);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceModifyResponse);
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_MODIFY_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_MODIFY_CONFIRM: {
      // TODO:
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_RELEASE_COMMAND: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_PDUSessionResourceRelease);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand);
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_RELEASE_RESPONSE: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_PDUSessionResourceRelease);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceReleaseResponse);
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_SETUP_REQUEST: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_PDUSessionResourceSetup);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_PDUSessionResourceSetupRequest);
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_SETUP_RESPONSE: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_PDUSessionResourceSetup);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceSetupResponse);
    } break;
    case NgapMessageType::UE_CONTEXT_MODIFICATION_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_MODIFICATION_RESPONSE: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_MODIFICATION_FAILURE: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_RELEASE_COMMAND: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_UEContextRelease);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_UEContextReleaseCommand);
    } break;
    case NgapMessageType::UE_CONTEXT_RELEASE_COMPLETE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_UEContextRelease);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
      messageTypeIE.setValuePresent(
          Ngap_SuccessfulOutcome__value_PR_UEContextReleaseComplete);
    } break;
    case NgapMessageType::WRITE_REPLACE_WARNING_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::WRITE_REPLACE_WARNING_RESPONSE: {
      // TODO:
    } break;
    case NgapMessageType::PWS_CANCEL_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::PWS_CANCEL_RESPONSE: {
      // TODO:
    } break;
    case NgapMessageType::UE_RADIO_CAPABILITY_CHECK_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::UE_RADIO_CAPABILITY_CHECK_RESPONSE: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_SUSPEND_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_SUSPEND_RESPONSE: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_SUSPEND_FAILURE: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_RESUME_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_RESUME_RESPONSE: {
      // TODO:
    } break;
    case NgapMessageType::UE_CONTEXT_RESUME_FAILURE: {
      // TODO:
    } break;
    case NgapMessageType::UE_RADIO_CAPABILITY_ID_MAPPING_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::UE_RADIO_CAPABILITY_ID_MAPPING_RESPONSE: {
      // TODO:
    } break;
    case NgapMessageType::DOWNLINK_RAN_CONFIGURATION_TRANSFER: {
      // TODO:
    } break;
    case NgapMessageType::DOWNLINK_RAN_STATUS_TRANSFER: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_DownlinkRANStatusTransfer);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_DownlinkRANStatusTransfer);
    } break;
    case NgapMessageType::DOWNLINK_NAS_TRANSPORT: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_DownlinkNASTransport);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_DownlinkNASTransport);
    } break;
    case NgapMessageType::ERROR_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::UPLINK_RAN_CONFIGURATION_TRANSFER: {
      // TODO:
    } break;
    case NgapMessageType::UPLINK_RAN_STATUS_TRANSFER: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_UplinkRANStatusTransfer);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_UplinkRANStatusTransfer);
    } break;
    case NgapMessageType::HANDOVER_NOTIFY: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_HandoverNotification);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_HandoverNotify);
    } break;
    case NgapMessageType::INITIAL_UE_MESSAGE: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_InitialUEMessage);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_InitialUEMessage);
    } break;
    case NgapMessageType::NAS_NON_DELIVERY_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::PAGING: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_Paging);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(Ngap_InitiatingMessage__value_PR_Paging);
    } break;
    case NgapMessageType::PDU_SESSION_RESOURCE_NOTIFY: {
      // TODO:
    } break;
    case NgapMessageType::REROUTE_NAS_REQUEST: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_RerouteNASRequest);
      messageTypeIE.setCriticality(Ngap_Criticality_reject);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_RerouteNASRequest);

    } break;
    case NgapMessageType::UE_CONTEXT_RELEASE_REQUEST: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_UEContextReleaseRequest);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_UEContextReleaseRequest);
    } break;
    case NgapMessageType::UPLINK_NAS_TRANSPORT: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_UplinkNASTransport);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_UplinkNASTransport);
    } break;
    case NgapMessageType::AMF_STATUS_INDICATION: {
      messageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_AMFStatusIndication);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_AMFStatusIndication);
    } break;
    case NgapMessageType::PWS_RESTART_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::PWS_FAILURE_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::DOWNLINK_UE_ASSOCIATED_NRPPA_TRANSPORT: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_DownlinkUEAssociatedNRPPaTransport);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_DownlinkUEAssociatedNRPPaTransport);
    } break;
    case NgapMessageType::UPLINK_UE_ASSOCIATED_NRPPA_TRANSPORT: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_UplinkUEAssociatedNRPPaTransport);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_UplinkUEAssociatedNRPPaTransport);
    } break;

    case NgapMessageType::DOWNLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_DownlinkNonUEAssociatedNRPPaTransport);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_DownlinkNonUEAssociatedNRPPaTransport);
    } break;

    case NgapMessageType::UPLINK_NON_UE_ASSOCIATED_NRPPA_TRANSPORT: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_UplinkNonUEAssociatedNRPPaTransport);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_UplinkNonUEAssociatedNRPPaTransport);
    } break;
    case NgapMessageType::TRACE_START: {
      // TODO:
    } break;
    case NgapMessageType::TRACE_FAILURE_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::DEACTIVATE_TRACE: {
      // TODO:
    } break;
    case NgapMessageType::CELL_TRAFFIC_TRACE: {
      // TODO:
    } break;
    case NgapMessageType::LOCATION_REPORTING_CONTROL: {
      // TODO:
    } break;
    case NgapMessageType::LOCATION_REPORTING_FAILURE_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::LOCATION_REPORT: {
      // TODO:
    } break;
    case NgapMessageType::UE_TNLA_BINDING_RELEASE_REQUEST: {
      // TODO:
    } break;
    case NgapMessageType::UE_RADIO_CAPABILITY_INFO_INDICATION: {
      messageTypeIE.setProcedureCode(
          Ngap_ProcedureCode_id_UERadioCapabilityInfoIndication);
      messageTypeIE.setCriticality(Ngap_Criticality_ignore);
      messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
      messageTypeIE.setValuePresent(
          Ngap_InitiatingMessage__value_PR_UERadioCapabilityInfoIndication);
    } break;
    case NgapMessageType::RRC_INACTIVE_TRANSITION_REPORT: {
      // TODO:
    } break;
    case NgapMessageType::OVERLOAD_START: {
      // TODO:
    } break;
    case NgapMessageType::OVERLOAD_STOP: {
      // TODO:
    } break;
    case NgapMessageType::SECONDARY_RAT_DATA_USAGE_REPORT: {
      // TODO:
    } break;
    case NgapMessageType::UPLINK_RIM_INFORMATION_TRANSFER: {
      // TODO:
    } break;
    case NgapMessageType::DOWNLINK_RIM_INFORMATION_TRANSFER: {
      // TODO:
    } break;
    case NgapMessageType::RETRIEVE_UE_INFORMATION: {
      // TODO:
    } break;
    case NgapMessageType::UE_INFORMATION_TRANSFER: {
      // TODO:
    } break;
    case NgapMessageType::RAN_CP_RELOCATION_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::CONNECTION_ESTABLISHMENT_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::AMF_CP_RELOCATION_INDICATION: {
      // TODO:
    } break;
    case NgapMessageType::HANDOVER_SUCCESS: {
      // TODO:
    } break;
    case NgapMessageType::UPLINK_RAN_EARLY_STATUS_TRANSFER: {
      // TODO:
    } break;
    case NgapMessageType::DOWNLINK_RAN_EARLY_STATUS_TRANSFER: {
      // TODO:
    } break;
    default: {
    } break;
  }

  messageTypeIE.encode(*ngapPdu);
}

//------------------------------------------------------------------------------
int NgapMessage::Encode(uint8_t* buf, int bufSize) {
  output_wrapper::print_asn_msg(&asn_DEF_Ngap_NGAP_PDU, ngapPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, ngapPdu, buf, bufSize);

  int encoded_size = (er.encoded + 7) >> 3;
  Logger::ngap().debug("Encoded size (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
void NgapMessage::encode2NewBuffer(uint8_t*& buf, int& encoded_size) {
  output_wrapper::print_asn_msg(&asn_DEF_Ngap_NGAP_PDU, ngapPdu);
  encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, ngapPdu, (void**) &buf);
  Logger::ngap().debug("Encoded message size ( %d )", encoded_size);
  return;
}

}  // namespace ngap
