/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ue_context.hpp"

#include "amf.hpp"
#include "logger.hpp"

//------------------------------------------------------------------------------
ue_context::ue_context() {
  ran_ue_ngap_id        = 0;
  amf_ue_ngap_id        = INVALID_AMF_UE_NGAP_ID;
  gnb_id                = 0;
  supi                  = {};
  tmsi                  = 0;
  rrc_estb_cause        = {};
  is_ue_context_request = false;
  cgi                   = {};
  tai                   = {};
  pdu_sessions          = {};
  nrf_uri               = std::nullopt;
  pcf_addr              = {};

  // TODO [AMF-N2-QOS]: Populate dynamic UE-AMBR from subscription / SMF request
  // Reference: Phase 2: N2 QoS Message Completeness
  //
  // Task 2.1: UE-AMBR from UDM Subscription Data
  //   - Parse ueAmbr.uplink / ueAmbr.downlink from the UDM AM Data response
  //   - Convert string-encoded bitrate (e.g. "1 Gbps") to uint64_t bps
  //   - Store in ue_ambr_dl / ue_ambr_ul, set has_ue_ambr = true
  // Task 2.2: UE-AMBR from SMF N11 Request
  //   - Parse ueAmbr from N1N2MessageTransferReqData (SMF-provided)
  //   - SMF-provided value takes priority over UDM-subscribed value
  //
  // Standards:
  //   - TS 29.503 §5.2.2.2 (UDM AM data — ueAmbr)
  //   - TS 29.571 §5.2.1 (BitRate string format)
  //   - TS 29.518 §6.3.5.2 (N1N2MessageTransferReqData — ueAmbr)
  //   - TS 23.501 §5.6.2 (SMF provides UE-AMBR to AMF)
  //
  // [QOS-MOCK] Phase 2 — Dynamic UE-AMBR seeding ([AMF-N2-QOS]). Mocks the
  // Task 2.1 / Task 2.2 TODO above:
  //   - Task 2.1 (UE-AMBR from UDM AM Data): MOCKED — not fetched from UDM.
  //   - Task 2.2 (UE-AMBR from SMF N1N2 request): MOCKED — not parsed.
  // Instead, mock values are seeded here so all N2 messages that carry the
  // UE Aggregate Maximum Bit Rate IE source a dynamic, non-hardcoded value.
  // Mock DL (2 Gbps) deliberately differs from the compile-time constant
  // (1 Gbps) so the dynamic path is observable on the wire.
  ue_ambr_dl  = 2000000000;  // 2 Gbps (mock)
  ue_ambr_ul  = 1000000000;  // 1 Gbps (mock)
  has_ue_ambr = true;
}

//------------------------------------------------------------------------------
bool ue_context::get_pdu_session_context(
    std::uint8_t session_id,
    std::shared_ptr<pdu_session_context>& context) const {
  std::shared_lock lock(m_pdu_session);
  if (pdu_sessions.count(session_id) > 0) {
    if (pdu_sessions.at(session_id) != nullptr) {
      context = pdu_sessions.at(session_id);
      return true;
    }
  }

  Logger::amf_app().warn(
      "No PDU Session Context with PDU Session ID %d", session_id);
  return false;
}

//------------------------------------------------------------------------------
void ue_context::add_pdu_session_context(
    std::uint8_t session_id,
    const std::shared_ptr<pdu_session_context>& context) {
  std::unique_lock lock(m_pdu_session);
  pdu_sessions[session_id] = context;
}

//------------------------------------------------------------------------------
void ue_context::copy_pdu_sessions(const std::shared_ptr<ue_context>& ue_ctx) {
  pdu_sessions = ue_ctx->pdu_sessions;
}

//------------------------------------------------------------------------------
bool ue_context::get_pdu_sessions_context(
    std::vector<std::shared_ptr<pdu_session_context>>& sessions_ctx) const {
  std::shared_lock lock(m_pdu_session);
  for (auto s : pdu_sessions) {
    sessions_ctx.push_back(s.second);
  }
  return true;
}

//------------------------------------------------------------------------------
bool ue_context::remove_pdu_sessions_context(uint8_t pdu_session_id) {
  std::unique_lock lock(m_pdu_session);
  if (pdu_sessions.count(pdu_session_id) > 0) {
    pdu_sessions.erase(pdu_session_id);
    Logger::amf_app().debug("PDU Session ID %d removed", pdu_session_id);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool ue_context::set_up_cnx_state(
    uint8_t pdu_session_id, const up_cnx_state_e& state) {
  std::shared_ptr<pdu_session_context> psc = {};
  if (get_pdu_session_context(pdu_session_id, psc)) {
    std::unique_lock lock(m_pdu_session);
    psc->up_cnx_state = state;
    return true;
  }
  return false;
}
