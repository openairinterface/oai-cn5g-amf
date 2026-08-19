/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ngap_app.hpp"

#include "amf_config.hpp"
#include "amf_n2.hpp"
#include "logger.hpp"
#include "ngap_message_callback.hpp"
#include "ngap_utils.hpp"
#include "output_wrapper.hpp"

extern "C" {
#include "Ngap_Cause.h"
#include "Ngap_CauseProtocol.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_SuccessfulOutcome.h"
#include "Ngap_UnsuccessfulOutcome.h"
#include "constr_TYPE.h"
}

#include <vector>

using namespace sctp;
using namespace oai::config;
using namespace oai::ngap;

extern std::unique_ptr<oai::config::amf_config> amf_cfg;
extern amf_n2* amf_n2_inst;

namespace {

// Some gNB stacks encode NGAP open types with a leading APER padding
// byte. The current generated decoder may either reject the whole PDU or decode
// the padded value as a different IE ID.
// Normalize only that exact IE shape and retry decoding.
bool normalize_ngap_open_types(
    const bstring payload, std::vector<uint8_t>& out) {
  const auto* data = reinterpret_cast<const uint8_t*>(bdata(payload));
  const int len    = blength(payload);

  if (len < 12) return false;
  if (data[0] != 0x00 || data[2] != 0x40 || data[3] == 0x00) return false;

  out.assign(data, data + len);
  bool normalized = false;

  for (size_t i = 4; i + 4 < out.size(); ++i) {
    const bool is_amf_ue_ngap_id_padding =
        out[i] == 0x00 && out[i + 1] == 0x0a && out[i + 2] == 0x00 &&
        out[i + 3] == 0x03 && out[i + 4] == 0x20;
    const bool is_ran_ue_ngap_id_padding =
        out[i] == 0x00 && out[i + 1] == 0x55 && out[i + 2] == 0x00 &&
        out[i + 3] == 0x05 && out[i + 4] == 0xc0;

    if (!is_amf_ue_ngap_id_padding && !is_ran_ue_ngap_id_padding) {
      continue;
    }

    out[3] -= 1;
    out[i + 3] -= 1;
    out.erase(out.begin() + i + 4);
    normalized = true;
    --i;
  }

  if (!normalized) out.clear();
  return normalized;
}

}  // namespace

//------------------------------------------------------------------------------
ngap_app::ngap_app(const std::string& address, const uint16_t port_num)
    : ppid_(60), sctp_s_38412(address.c_str(), port_num) {
  sctp_s_38412.sctp_set_ttl(amf_cfg->sctp_ttl);
  sctp_s_38412.start_receive(this);
  Logger::ngap().info(
      "Set N2 AMF IPv4 Addr %s, port %d", address.c_str(), port_num);
}

//------------------------------------------------------------------------------
ngap_app::~ngap_app() {}

//------------------------------------------------------------------------------
void ngap_app::handle_receive(
    bstring payload, sctp_assoc_id_t assoc_id, sctp_stream_id_t stream,
    sctp_stream_id_t instreams, sctp_stream_id_t outstreams) {
  Logger::ngap().debug(
      "Handling SCTP payload from SCTP Server on assoc_id (%d), stream_id "
      "(%d), instreams (%d), outstreams (%d)",
      assoc_id, stream, instreams, outstreams);

  std::vector<uint8_t> normalized_payload;
  const void* decode_data = bdata(payload);
  size_t decode_size      = blength(payload);
  if (normalize_ngap_open_types(payload, normalized_payload)) {
    Logger::ngap().warn(
        "Normalizing padded NGAP integer open type(s), payload %d -> %zu "
        "bytes",
        blength(payload), normalized_payload.size());
    decode_data = normalized_payload.data();
    decode_size = normalized_payload.size();
  }

  Ngap_NGAP_PDU_t* ngap_msg_pdu =
      (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  if (!ngap_msg_pdu) {
    Logger::ngap().error("Failed to allocate memory for NGAP PDU");
    return;
  }

  asn_dec_rval_t dec_ret = aper_decode(
      NULL, &asn_DEF_Ngap_NGAP_PDU, (void**) &ngap_msg_pdu, decode_data,
      decode_size, 0, 0);

  oai::utils::output_wrapper::print_buffer(
      "ngap_app", "NGAP", (const uint8_t*) bdata(payload), blength(payload));

  if (dec_ret.code != RC_OK) {
    // Report via Error Indication
    Logger::ngap().error(
        "Decode NGAP message failed, code %d, consumed %zu bits, payload %d "
        "bytes",
        dec_ret.code, dec_ret.consumed, blength(payload));
    if (amf_n2_inst)
      amf_n2_inst->send_ng_error_indication(
          assoc_id, stream, std::nullopt, std::nullopt, Ngap_Cause_PR_protocol,
          Ngap_CauseProtocol_transfer_syntax_error);
    ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);
    return;
  }

  Ngap_ProcedureCode_t procedure_code = 0;
  const void* message_body            = nullptr;
  switch (ngap_msg_pdu->present) {
    case Ngap_NGAP_PDU_PR_initiatingMessage:
      message_body = ngap_msg_pdu->choice.initiatingMessage;
      if (message_body)
        procedure_code = ngap_msg_pdu->choice.initiatingMessage->procedureCode;
      break;
    case Ngap_NGAP_PDU_PR_successfulOutcome:
      message_body = ngap_msg_pdu->choice.successfulOutcome;
      if (message_body)
        procedure_code = ngap_msg_pdu->choice.successfulOutcome->procedureCode;
      break;
    case Ngap_NGAP_PDU_PR_unsuccessfulOutcome:
      message_body = ngap_msg_pdu->choice.unsuccessfulOutcome;
      if (message_body)
        procedure_code =
            ngap_msg_pdu->choice.unsuccessfulOutcome->procedureCode;
      break;
    default:
      break;
  }

  if ((ngap_msg_pdu->present < Ngap_NGAP_PDU_PR_initiatingMessage) ||
      (ngap_msg_pdu->present > NGAP_PRESENT_MAX_VALUE) ||
      (message_body == nullptr)) {
    Logger::ngap().error(
        "Invalid NGAP PDU present value %d or NULL message body, dropping",
        ngap_msg_pdu->present);
    if (amf_n2_inst)
      amf_n2_inst->send_ng_error_indication(
          assoc_id, stream, std::nullopt, std::nullopt, Ngap_Cause_PR_protocol,
          Ngap_CauseProtocol_abstract_syntax_error_reject);
    ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);
    return;
  }

  if (procedure_code > (NGAP_PROCEDURE_CODE_MAX_VALUE - 1)) {
    Logger::ngap().error(
        "Invalid procedure code %ld, dropping message", procedure_code);
    if (amf_n2_inst)
      amf_n2_inst->send_ng_error_indication(
          assoc_id, stream, std::nullopt, std::nullopt, Ngap_Cause_PR_protocol,
          Ngap_CauseProtocol_abstract_syntax_error_reject);
    ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);
    return;
  }

  Logger::ngap().debug(
      "Decoded NGAP message, procedure code %ld, present %d", procedure_code,
      ngap_msg_pdu->present);
  ngap_utils::print_asn_msg(&asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);

  // If no handler available
  if (messages_callback[procedure_code][ngap_msg_pdu->present - 1] == nullptr) {
    // Unknown / unsupported procedure -> Error Indication, then drop.
    Logger::ngap().error(
        "No handler available for procedure code %ld and present %d",
        procedure_code, ngap_msg_pdu->present);
    if (amf_n2_inst)
      amf_n2_inst->send_ng_error_indication(
          assoc_id, stream, std::nullopt, std::nullopt, Ngap_Cause_PR_protocol,
          Ngap_CauseProtocol_abstract_syntax_error_reject);
    ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);
    return;
  }

  // Handle the message
  (*messages_callback[procedure_code][ngap_msg_pdu->present - 1])(
      assoc_id, stream, ngap_msg_pdu);
  // Typically, NGAP PDU will be freed in the handler, so do not free it here to
  // avoid double free
}

//------------------------------------------------------------------------------
void ngap_app::handle_sctp_new_association(
    sctp_assoc_id_t assoc_id, sctp_stream_id_t instreams,
    sctp_stream_id_t outstreams) {
  Logger::ngap().debug(
      "Ready to handle new NGAP SCTP association request (id %d)", assoc_id);

  std::shared_ptr<gnb_context> gc = {};
  if (!assoc_id_2_gnb_context(assoc_id, gc)) {
    Logger::ngap().debug(
        "Create a new gNB context with assoc_id (%d)", assoc_id);
    gc = std::make_shared<gnb_context>();
    set_assoc_id_2_gnb_context(assoc_id, gc);
  } else {
    if (gc->ng_state == NGAP_RESETING || gc->ng_state == NGAP_SHUTDOWN) {
      Logger::ngap().warn(
          "Received a new association request on an association that is being "
          "%s, ignoring",
          ng_gnb_state_str[gc->ng_state]);
      return;
    } else {
      Logger::ngap().debug("Update gNB context with assoc id (%d)", assoc_id);
    }
  }

  // Update gNB Context
  gc->sctp_assoc_id    = assoc_id;
  gc->instreams        = instreams;
  gc->outstreams       = outstreams;
  gc->next_sctp_stream = 1;
  gc->ng_state         = NGAP_INIT;
}

//------------------------------------------------------------------------------
void ngap_app::handle_sctp_shutdown(sctp_assoc_id_t assoc_id) {
  Logger::ngap().debug(
      "Handle a SCTP Shutdown event (association id: %d)", assoc_id);

  // Handle the message
  (*events_callback[0])(assoc_id);
}

//------------------------------------------------------------------------------
uint32_t ngap_app::get_ppid() {
  return ppid_;
}

//------------------------------------------------------------------------------
bool ngap_app::is_assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id) const {
  return gnb_context_store_.exists_by_assoc(assoc_id);
}

//------------------------------------------------------------------------------
bool ngap_app::assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context>& gc) {
  auto found = gnb_context_store_.find_by_assoc(assoc_id);
  if (found == nullptr) return false;
  gc = found;
  return true;
}

//------------------------------------------------------------------------------
std::vector<sctp::sctp_assoc_id_t> ngap_app::get_all_assoc_ids() {
  return gnb_context_store_.all_assoc_ids();
}

//------------------------------------------------------------------------------
void ngap_app::set_assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context> gc) {
  gnb_context_store_.set_by_assoc(assoc_id, gc);
  return;
}

//------------------------------------------------------------------------------
bool ngap_app::is_gnb_id_2_gnb_context(const long& gnb_id) const {
  return gnb_context_store_.exists_by_gnbid(gnb_id);
}

//------------------------------------------------------------------------------
bool ngap_app::gnb_id_2_gnb_context(
    const long& gnb_id, std::shared_ptr<gnb_context>& gc) const {
  auto found = gnb_context_store_.find_by_gnbid(gnb_id);
  if (found == nullptr) return false;
  gc = found;
  return true;
}

//------------------------------------------------------------------------------
void ngap_app::set_gnb_id_2_gnb_context(
    const long& gnb_id, const std::shared_ptr<gnb_context>& gc) {
  gnb_context_store_.set_by_gnbid(gnb_id, gc);
  return;
}

//------------------------------------------------------------------------------
void ngap_app::remove_gnb_context(const long& gnb_id) {
  auto gc = gnb_context_store_.find_by_gnbid(gnb_id);
  if (gc == nullptr) return;
  gnb_context_store_.remove(gc);
}

//------------------------------------------------------------------------------
void ngap_app::remove_gnb_context(const std::shared_ptr<gnb_context>& gc) {
  gnb_context_store_.remove(gc);
}
