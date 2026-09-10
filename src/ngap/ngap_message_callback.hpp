/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NGAP_MESSAGE_CALLBACK_H_
#define _NGAP_MESSAGE_CALLBACK_H_

#include "amf_app.hpp"
#include "amf_n1.hpp"
#include "sctp_server.hpp"

extern "C" {
struct Ngap_NGAP_PDU;
}

using namespace sctp;
using namespace amf_application;

extern itti_mw* itti_inst;
extern amf_n1* amf_n1_inst;
extern amf_app* amf_app_inst;

typedef int (*ngap_message_decoded_callback)(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

typedef void (*ngap_event_callback)(const sctp_assoc_id_t assoc_id);

constexpr uint8_t NGAP_PROCEDURE_CODE_MAX_VALUE = 66;
constexpr uint8_t NGAP_PRESENT_MAX_VALUE        = 3;

//------------------------------------------------------------------------------
int ngap_amf_handle_ng_setup_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_ng_setup_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_ng_setup_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_initial_ue_message(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_uplink_nas_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_initial_context_setup_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_initial_context_setup_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_initial_context_setup_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_ue_radio_cap_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_ue_context_release_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_ue_context_release_command(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_ue_context_release_complete(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_pdu_session_resource_release_command(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_pdu_session_resource_release_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_pdu_session_resource_setup_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_pdu_session_resource_setup_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_pdu_session_resource_modify_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_pdu_session_resource_modify_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_error_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_configuration_update(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_configuration_update_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_configuration_update_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int amf_status_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int cell_traffic_trace(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int deactivate_trace(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int downlink_nas_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int downlink_non_UEassociated_nrppa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int downlink_ran_configuration_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int downlink_ran_status_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int downlink_ue_associated_nappa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_cancel(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_cancel_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_preparation(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_command(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_preparation_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_notification(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_request_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int handover_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int location_reporting_control(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int location_reporting_failure_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int location_report(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int nas_non_delivery_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ng_reset(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int overload_start(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int overload_stop(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int paging(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_amf_handle_path_switch_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_path_switch_request_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ngap_handle_path_switch_request_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int pdu_session_resource_modify_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int pdu_session_resource_modify_confirm(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int pdu_session_resource_notify(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int private_message(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int pws_cancel_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int pws_cancel_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int pws_failure_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int pws_restart_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ran_configuration_update(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ran_configuration_update_ack(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ran_configuration_update_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int reroute_nas_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int rrc_inactive_transition_report(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int trace_failure_indication(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int trace_start(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ue_context_modification_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ue_context_modification_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ue_context_modification_failure(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ue_radio_capability_check_request(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ue_radio_capability_check_response(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int ue_tnla_binding_release(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int uplink_non_ue_associated_nrppa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int uplink_ran_configuration_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int uplink_ran_status_transfer(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int uplink_ue_associated_nrppa_transport(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
int secondary_rat_data_usage_report(
    const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
    struct Ngap_NGAP_PDU* message_p);

//------------------------------------------------------------------------------
extern ngap_message_decoded_callback
    messages_callback[NGAP_PROCEDURE_CODE_MAX_VALUE][NGAP_PRESENT_MAX_VALUE];

//------------------------------------------------------------------------------
void ngap_sctp_shutdown(const sctp_assoc_id_t assoc_id);

//------------------------------------------------------------------------------
extern ngap_event_callback events_callback[][1];

#endif
