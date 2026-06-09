/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _NGAP_MESSAGE_CALLBACK_H_
#define _NGAP_MESSAGE_CALLBACK_H_

#include <cstdint>

#include "sctp_server.hpp"

struct Ngap_NGAP_PDU;

class itti_mw;
namespace amf_application {
class amf_n1;
class amf_app;
}  // namespace amf_application

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

extern ngap_message_decoded_callback
    messages_callback[NGAP_PROCEDURE_CODE_MAX_VALUE][NGAP_PRESENT_MAX_VALUE];

extern ngap_event_callback events_callback[][1];

#endif
