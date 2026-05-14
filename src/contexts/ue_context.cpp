/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ue_context.hpp"

#include <nlohmann/json.hpp>

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

//------------------------------------------------------------------------------
nlohmann::json ue_context::to_json() const {
  nlohmann::json j;
  j["ran_ue_ngap_id"]              = ran_ue_ngap_id;
  j["amf_ue_ngap_id"]              = amf_ue_ngap_id;
  j["gnb_id"]                      = gnb_id;
  j["supi"]                        = supi;
  j["tmsi"]                        = tmsi;
  j["rrc_estb_cause"]              = rrc_estb_cause;
  j["is_ue_context_request"]       = is_ue_context_request;
  j["amf_3gpp_access_location"]    = amf_3gpp_access_location;
  j["policy_association_location"] = policy_association_location;

  j["cgi"]["mcc"]        = cgi.mcc;
  j["cgi"]["mnc"]        = cgi.mnc;
  j["cgi"]["nr_cell_id"] = cgi.nrCellId;

  j["tai"]["mcc"] = tai.mcc;
  j["tai"]["mnc"] = tai.mnc;
  j["tai"]["tac"] = static_cast<uint32_t>(tai.tac);

  if (nrf_uri.has_value()) {
    j["nrf_uri"] = nrf_uri.value();
  } else {
    j["nrf_uri"] = nullptr;
  }

  j["pcf_addr"] = pcf_addr.to_json();

  nlohmann::json pdu_array = nlohmann::json::array();
  {
    std::shared_lock<std::shared_mutex> lock(m_pdu_session);
    for (const auto& [id, psc] : pdu_sessions) {
      pdu_array.push_back(psc->to_json());
    }
  }
  j["pdu_sessions"] = pdu_array;

  return j;
}

//------------------------------------------------------------------------------
void ue_context::from_json(const nlohmann::json& j) {
  if (j.contains("ran_ue_ngap_id"))
    ran_ue_ngap_id = j["ran_ue_ngap_id"].get<uint32_t>();
  if (j.contains("amf_ue_ngap_id"))
    amf_ue_ngap_id = j["amf_ue_ngap_id"].get<uint64_t>();
  if (j.contains("gnb_id")) gnb_id = j["gnb_id"].get<uint32_t>();
  if (j.contains("supi")) supi = j["supi"].get<std::string>();
  if (j.contains("tmsi")) tmsi = j["tmsi"].get<uint32_t>();
  if (j.contains("rrc_estb_cause"))
    rrc_estb_cause = j["rrc_estb_cause"].get<uint8_t>();
  if (j.contains("is_ue_context_request"))
    is_ue_context_request = j["is_ue_context_request"].get<bool>();
  if (j.contains("amf_3gpp_access_location"))
    amf_3gpp_access_location = j["amf_3gpp_access_location"].get<std::string>();
  if (j.contains("policy_association_location"))
    policy_association_location =
        j["policy_association_location"].get<std::string>();
  if (j.contains("nrf_uri")) {
    if (!j["nrf_uri"].is_null()) {
      nrf_uri = j["nrf_uri"].get<std::string>();
    } else {
      nrf_uri = std::nullopt;
    }
  }
  if (j.contains("pcf_addr")) {
    auto pcf_copy = const_cast<nlohmann::json&>(j)["pcf_addr"];
    pcf_addr.from_json(pcf_copy);
  }

  if (j.contains("cgi")) {
    if (j["cgi"].contains("mcc")) cgi.mcc = j["cgi"]["mcc"].get<std::string>();
    if (j["cgi"].contains("mnc")) cgi.mnc = j["cgi"]["mnc"].get<std::string>();
    if (j["cgi"].contains("nr_cell_id"))
      cgi.nrCellId = j["cgi"]["nr_cell_id"].get<uint64_t>();
  }

  if (j.contains("tai")) {
    if (j["tai"].contains("mcc")) tai.mcc = j["tai"]["mcc"].get<std::string>();
    if (j["tai"].contains("mnc")) tai.mnc = j["tai"]["mnc"].get<std::string>();
    if (j["tai"].contains("tac"))
      tai.tac = j["tai"]["tac"].get<uint32_t>() & 0x00FFFFFF;
  }

  if (j.contains("pdu_sessions") && j["pdu_sessions"].is_array()) {
    for (const auto& psc_json : j["pdu_sessions"]) {
      auto psc = std::make_shared<pdu_session_context>();
      psc->from_json(psc_json);
      add_pdu_session_context(psc->pdu_session_id, psc);
    }
  }
}
