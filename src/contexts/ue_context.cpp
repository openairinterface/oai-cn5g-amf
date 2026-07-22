/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ue_context.hpp"

#include "amf.hpp"
#include "logger.hpp"
// Complete definitions of the nested sub-contexts are only required in this TU,
// where the out-of-line destructor instantiates the shared_ptr<...>
// destructors.
#include "nas_context.hpp"
#include "ue_ngap_context.hpp"

//------------------------------------------------------------------------------
ue_context::ue_context() {
  ran_ue_ngap_id        = 0;
  amf_ue_ngap_id        = INVALID_AMF_UE_NGAP_ID;
  gnb_id                = 0;
  supi                  = {};
  guti                  = {};
  tmsi                  = 0;
  nas_ctx               = nullptr;
  ngap_ctx              = nullptr;
  rrc_estb_cause        = {};
  is_ue_context_request = false;
  cgi                   = {};
  tai                   = {};
  pdu_sessions          = {};
  nrf_uri               = std::nullopt;
  pcf_addr              = {};
}

//------------------------------------------------------------------------------
// Defined out of line so that the shared_ptr<nas_context>/<ue_ngap_context>
// member destructors are instantiated here, where the complete types are known.
ue_context::~ue_context() {}

//------------------------------------------------------------------------------
std::shared_ptr<nas_context> ue_context::get_nas_ctx() const {
  std::shared_lock lock(m_ctx_);
  return nas_ctx;
}

//------------------------------------------------------------------------------
void ue_context::set_nas_ctx(const std::shared_ptr<nas_context>& nc) {
  std::unique_lock lock(m_ctx_);
  nas_ctx = nc;
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_ngap_context> ue_context::get_ngap_ctx() const {
  std::shared_lock lock(m_ctx_);
  return ngap_ctx;
}

//------------------------------------------------------------------------------
void ue_context::set_ngap_ctx(const std::shared_ptr<ue_ngap_context>& unc) {
  std::unique_lock lock(m_ctx_);
  ngap_ctx = unc;
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
  std::map<std::uint8_t, std::shared_ptr<pdu_session_context>> snapshot;
  {
    std::shared_lock lock(ue_ctx->m_pdu_session);
    snapshot = ue_ctx->pdu_sessions;
  }
  std::unique_lock lock(m_pdu_session);
  pdu_sessions = std::move(snapshot);
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
  std::unique_lock lock(m_pdu_session);
  if (pdu_sessions.count(pdu_session_id) > 0) {
    std::shared_ptr<pdu_session_context> psc = pdu_sessions.at(pdu_session_id);
    if (psc != nullptr) {
      psc->up_cnx_state = state;
      return true;
    }
  }
  Logger::amf_app().warn(
      "No PDU Session Context with PDU Session ID %d", pdu_session_id);
  return false;
}
