/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#include "ue_context.hpp"

#include "amf.hpp"
#include "logger.hpp"
#include "nas_context.hpp"
#include "ue_ngap_context.hpp"

//------------------------------------------------------------------------------
ue_context::ue_context() {
  ran_ue_ngap_id        = 0;
  amf_ue_ngap_id        = INVALID_AMF_UE_NGAP_ID;
  gnb_id                = 0;
  supi                  = {};
  guti_                 = {};
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

  // TODO: populate from the UDM subscribedUeAmbr and set has_ue_ambr (see
  // amf_app::get_access_and_mobility_subscription_data).
  ue_ambr_dl  = 0;
  ue_ambr_ul  = 0;
  has_ue_ambr = false;
  // Paging-related state
  s_setid_               = {};
  s_pointer_             = {};
  s_tmsi_                = {};
  last_known_tai_        = {};
  has_last_known_tai_    = false;
  last_gnb_assoc_id_     = 0;
  has_last_gnb_assoc_id_ = false;
  paging_                = {};
  pending_.clear();
}

//------------------------------------------------------------------------------
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
void ue_context::set_paging_identity(
    const std::string& setid, const std::string& pointer,
    const std::string& tmsi) {
  std::unique_lock lock(m_paging_);
  s_setid_   = setid;
  s_pointer_ = pointer;
  s_tmsi_    = tmsi;
}

//------------------------------------------------------------------------------
bool ue_context::get_paging_identity(
    std::string& setid, std::string& pointer, std::string& tmsi) const {
  std::shared_lock lock(m_paging_);
  if (s_setid_.empty() || s_pointer_.empty() || s_tmsi_.empty()) return false;
  setid   = s_setid_;
  pointer = s_pointer_;
  tmsi    = s_tmsi_;
  return true;
}

//------------------------------------------------------------------------------
void ue_context::set_last_known_tai(const Tai_t& tai_in) {
  std::unique_lock lock(m_paging_);
  last_known_tai_     = tai_in;
  has_last_known_tai_ = true;
}

//------------------------------------------------------------------------------
bool ue_context::get_last_known_tai(Tai_t& tai_out) const {
  std::shared_lock lock(m_paging_);
  if (!has_last_known_tai_) return false;
  tai_out = last_known_tai_;
  return true;
}

//------------------------------------------------------------------------------
void ue_context::set_last_gnb_assoc_id(sctp_assoc_id_t assoc_id) {
  std::unique_lock lock(m_paging_);
  last_gnb_assoc_id_     = assoc_id;
  has_last_gnb_assoc_id_ = true;
}

//------------------------------------------------------------------------------
bool ue_context::get_last_gnb_assoc_id(sctp_assoc_id_t& assoc_id) const {
  std::shared_lock lock(m_paging_);
  if (!has_last_gnb_assoc_id_) return false;
  assoc_id = last_gnb_assoc_id_;
  return true;
}

//------------------------------------------------------------------------------
bool ue_context::get_paging_snapshot(
    std::string& setid, std::string& pointer, std::string& tmsi, Tai_t& tai,
    sctp_assoc_id_t& assoc, std::string& missing_out) const {
  std::shared_lock lock(m_paging_);
  missing_out.clear();
  // Validate everything before writing anything, so that a partial snapshot is
  // never handed to the encoder. Naming the unseeded inputs happens here,
  // under the same shared lock as the decision itself, so the diagnostic can
  // never disagree with the refusal it explains.
  const auto note = [&missing_out](const char* what) {
    if (!missing_out.empty()) missing_out.append(", ");
    missing_out.append(what);
  };
  if (s_setid_.empty()) note("AMF Set ID");
  if (s_pointer_.empty()) note("AMF Pointer");
  if (s_tmsi_.empty()) note("5G-TMSI");
  if (!has_last_known_tai_) note("last-known TAI");
  if (!has_last_gnb_assoc_id_) note("last-serving gNB SCTP association");
  if (!missing_out.empty()) return false;
  setid   = s_setid_;
  pointer = s_pointer_;
  tmsi    = s_tmsi_;
  tai     = last_known_tai_;
  assoc   = last_gnb_assoc_id_;
  return true;
}

//------------------------------------------------------------------------------
bool ue_context::push_pending_payload(
    buffered_n1n2_t&& b, std::vector<buffered_n1n2_t>& evicted_out) {
  // Read the clock before taking the lock: nothing but plain member access
  // happens under m_paging_.
  const std::chrono::steady_clock::time_point now =
      std::chrono::steady_clock::now();
  bool evicted_live_record = false;

  std::unique_lock lock(m_paging_);

  // 1. Lazy TTL sweep. Catches the records of a transaction whose window timer
  // was lost, and the records
  //    of a UE that answered on a different GUTI and was never re-joined. A
  //    record left at the default time_point{} sweeps here, which is the safe
  //    direction: it is freed rather than kept forever.
  for (auto it = pending_.begin(); it != pending_.end();) {
    if (it->expires_at <= now) {
      evicted_out.push_back(std::move(*it));
      it = pending_.erase(it);
    } else {
      ++it;
    }
  }

  // 2. Hard size bound. Enforced here, inside the lock, so that
  //    pending_.size() <= kMaxPendingPayloads holds at every point another
  //    thread could observe. A while loop rather than an if: it also repairs
  //    an over-full vector instead of trusting that one cannot exist.
  while (pending_.size() >= kMaxPendingPayloads) {
    evicted_out.push_back(std::move(pending_.front()));
    pending_.erase(pending_.begin());
    evicted_live_record = true;
  }

  // The vector is now the only owner of this record's two bstrings.
  pending_.push_back(std::move(b));
  return !evicted_live_record;
}

//------------------------------------------------------------------------------
void ue_context::take_pending_payloads(
    std::vector<buffered_n1n2_t>& live_out,
    std::vector<buffered_n1n2_t>& expired_out) {
  const std::chrono::steady_clock::time_point now =
      std::chrono::steady_clock::now();

  std::vector<buffered_n1n2_t> taken;
  {
    std::unique_lock lock(m_paging_);
    taken = std::move(pending_);
    pending_.clear();
  }

  // Partition with no lock held. `taken` owns every record until each one is
  // moved out; if anything below threw, `taken` would still free them all.
  for (auto& record : taken) {
    if (record.expires_at <= now) {
      expired_out.push_back(std::move(record));
    } else {
      live_out.push_back(std::move(record));
    }
  }
}

//------------------------------------------------------------------------------
bool ue_context::take_pending_pdu_session_resource_setup(
    uint8_t pdu_session_id, buffered_n1n2_t& setup_out) {
  const std::chrono::steady_clock::time_point now =
      std::chrono::steady_clock::now();

  std::unique_lock lock(m_paging_);

  // Only while the UE is being paged: once the paging response has started,
  // complete_paging_if_any() owns what is left in the buffer
  if (paging_.state != paging_state_e::kInProgress) return false;

  for (auto it = pending_.begin(); it != pending_.end(); ++it) {
    const bool is_for_this_pdu_session = (it->pdu_session_id == pdu_session_id);
    const bool is_resource_setup_only =
        it->is_n2sm_set and !it->is_n1sm_set and
        (it->n2sm_info_type == "PDU_RES_SETUP_REQ");
    const bool is_not_expired = (it->expires_at > now);

    if (is_for_this_pdu_session and is_resource_setup_only and is_not_expired) {
      // Moved, not copied: the caller now owns the payload and frees it after
      // this lock is released
      setup_out = std::move(*it);
      pending_.erase(it);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
paging_start_result_e ue_context::try_start_paging(const std::string& guti) {
  std::unique_lock lock(m_paging_);

  if (guti.empty() || s_setid_.empty() || s_pointer_.empty() ||
      s_tmsi_.empty() || !has_last_known_tai_ || !has_last_gnb_assoc_id_) {
    return paging_start_result_e::kNotPageable;
  }

  if (paging_.state == paging_state_e::kInProgress) {
    return paging_start_result_e::kAlreadyInProgress;
  }

  paging_.state    = paging_state_e::kInProgress;
  paging_.guti_key = guti;
  // The window timer of this transaction has not been armed yet. Zeroing it
  // here means a leftover id can never be mistaken for it.
  paging_.window_timer_id = 0;
  ++paging_.epoch;
  return paging_start_result_e::kStarted;
}

//------------------------------------------------------------------------------
void ue_context::set_paging_window_timer(timer_id_t timer_id) {
  std::unique_lock lock(m_paging_);
  // Only the transaction in flight, and only one that has no window timer
  // yet, may adopt this id.
  if ((paging_.state != paging_state_e::kInProgress) ||
      (paging_.window_timer_id != 0))
    return;
  paging_.window_timer_id = timer_id;
}

//------------------------------------------------------------------------------
timer_id_t ue_context::get_paging_window_timer() const {
  std::shared_lock lock(m_paging_);
  return paging_.window_timer_id;
}

//------------------------------------------------------------------------------
bool ue_context::clear_paging_state(timer_id_t expected_or_zero) {
  std::unique_lock lock(m_paging_);
  if ((expected_or_zero != 0) &&
      (paging_.window_timer_id != expected_or_zero)) {
    return false;
  }
  paging_.state           = paging_state_e::kIdle;
  paging_.window_timer_id = 0;
  return true;
}

//------------------------------------------------------------------------------
bool ue_context::begin_paging_response(
    std::string& guti_key_out, uint32_t& epoch_out, timer_id_t& timer_out) {
  std::unique_lock lock(m_paging_);
  if (paging_.state != paging_state_e::kInProgress) return false;
  paging_.state = paging_state_e::kResponded;
  guti_key_out  = paging_.guti_key;
  epoch_out     = paging_.epoch;
  timer_out     = paging_.window_timer_id;
  // Disown the window timer before releasing the lock
  paging_.window_timer_id = 0;
  return true;
}

//------------------------------------------------------------------------------
void ue_context::set_guti(const std::string& guti_in) {
  std::unique_lock lock(m_paging_);
  guti_ = guti_in;
}

//------------------------------------------------------------------------------
std::string ue_context::get_guti() const {
  std::shared_lock lock(m_paging_);
  return guti_;
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
