/*
 * SPDX-License-Identifier: LicenseRef-CSSL-1.0
 */

#ifndef _UE_CONTEXT_H_
#define _UE_CONTEXT_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>

#include "NgapIesStruct.hpp"
#include "pdu_session_context.hpp"
#include "sbi_helper.hpp"
#include "PolicyAssociation.h"
#include "paging_context.hpp"
#include "sctp_server.hpp"

extern "C" {
#include "Ngap_RRCEstablishmentCause.h"
}

using namespace oai::ngap;
using namespace sctp;

// Forward declarations
class nas_context;
class ue_ngap_context;

class ue_context {
 public:
  ue_context();
  virtual ~ue_context();
  ue_context(const ue_context&)            = delete;
  ue_context(ue_context&&)                 = delete;
  ue_context& operator=(const ue_context&) = delete;
  ue_context& operator=(ue_context&&)      = delete;
  bool get_pdu_session_context(
      std::uint8_t session_id,
      std::shared_ptr<pdu_session_context>& context) const;
  void add_pdu_session_context(
      std::uint8_t session_id,
      const std::shared_ptr<pdu_session_context>& context);
  void copy_pdu_sessions(const std::shared_ptr<ue_context>& ue_ctx);
  bool get_pdu_sessions_context(
      std::vector<std::shared_ptr<pdu_session_context>>& sessions_ctx) const;

  bool remove_pdu_sessions_context(uint8_t pdu_session_id);
  bool set_up_cnx_state(uint8_t pdu_session_id, const up_cnx_state_e& state);

  std::shared_ptr<nas_context> get_nas_ctx() const;
  void set_nas_ctx(const std::shared_ptr<nas_context>& nc);
  std::shared_ptr<ue_ngap_context> get_ngap_ctx() const;
  void set_ngap_ctx(const std::shared_ptr<ue_ngap_context>& unc);

  // for Paging
  void set_paging_identity(
      const std::string& setid, const std::string& pointer,
      const std::string& tmsi);
  // Returns false (and leaves the outputs untouched) if any part is unset.
  bool get_paging_identity(
      std::string& setid, std::string& pointer, std::string& tmsi) const;

  void set_last_known_tai(const Tai_t& tai_in);
  // Returns false if no TAI has ever been seeded. Callers MUST NOT fall back to
  // a default-constructed Tai_t: PlmnId::set("", "") yields MCC "000"/MNC "00"
  // and TAC 0, which encodes into a well-formed PAGING for a TAI that no gNB
  // serves - a silent non-page.
  bool get_last_known_tai(Tai_t& tai_out) const;

  void set_last_gnb_assoc_id(sctp_assoc_id_t assoc_id);
  // Returns false if no association has ever been seeded. A true return does
  // not mean the association is still up: revalidate it against the gNB context
  // store before sending.
  bool get_last_gnb_assoc_id(sctp_assoc_id_t& assoc_id) const;

  // Copies out, under a single shared lock, everything TASK_AMF_N2 needs to
  // build a PAGING PDU, so that the encode and the SCTP send run with NO lock
  // held (never hold m_paging_ across a send). Returns false and leaves every
  // output untouched if any part is unseeded.
  bool get_paging_snapshot(
      std::string& setid, std::string& pointer, std::string& tmsi, Tai_t& tai,
      sctp_assoc_id_t& assoc, std::string& missing_out) const;

  // --- Buffered downlink N1/N2 payloads ------------------------------------

  // Buffers one downlink N1/N2 payload for the duration of a paging
  // transaction. The record is MOVED in and `pending_` becomes its only owner.
  //
  // The caller MUST have written `b.expires_at` before the call: a record left
  // at the default time_point{} (the epoch) is swept out immediately. The TTL
  // is computed by the caller on purpose, so that this accessor reads no
  // configuration and calls nothing while holding m_paging_.
  //
  // Two reclaim mechanisms run first, both INSIDE the lock, so that
  // `pending_.size() <= kMaxPendingPayloads` holds at every observable point:
  //   1. a lazy TTL sweep of every record whose `expires_at` has passed;
  //   2. the hard size bound, which evicts the oldest record.
  // Every record removed by either mechanism is MOVED WHOLE into
  // `evicted_out`; this object frees nothing. The caller owns `evicted_out`
  // and its destructor is the single free point for the bstrings it carries.
  //
  // Returns false when the hard size bound had to evict a still-live record
  // (i.e. a payload was dropped because the UE has too many in flight), true
  // otherwise. `evicted_out` may be non-empty on a true return: expired
  // records are swept on every call.
  bool push_pending_payload(
      buffered_n1n2_t&& b, std::vector<buffered_n1n2_t>& evicted_out);

  // Empties `pending_` and hands every record to the caller, partitioned by
  // `expires_at`: still-deliverable records go to `live_out`, records whose
  // TTL has passed go to `expired_out`. Both vectors are appended to, never
  // cleared. After the call `pending_` is empty, so a second drain - from the
  // window-expiry path racing a response path - yields nothing and can neither
  // double-free nor steal a payload from the other.
  void take_pending_payloads(
      std::vector<buffered_n1n2_t>& live_out,
      std::vector<buffered_n1n2_t>& expired_out);

  // --- Paging transaction lifecycle ----------------------------------------

  // Compare-and-set: opens a paging transaction if and only if the UE is
  // pageable and no transaction is already in flight. On kStarted the state
  // becomes kInProgress, `guti_key` is recorded, `epoch` is bumped (for log
  // correlation) and the window timer id is reset to 0.
  //
  // kAlreadyInProgress means the caller's payload is buffered and the running
  // transaction will account for it; it is NOT an error.
  // kNotPageable means no PAGING can be built: no GUTI, no UE Paging Identity,
  // no last-known TAI or no last-serving gNB association.
  paging_start_result_e try_start_paging(const std::string& guti);

  // Records the id of the one-shot paging-response window timer. Adopted only
  // by a transaction that is in flight AND has no window timer yet: if the
  // transaction was terminated between the PAGING send and the arming, or if
  // the transaction in flight is a later one that already armed its own timer,
  // the id must NOT be adopted, because the expiry that follows would then
  // match and drain whatever transaction is running by then. An unrecorded
  // timer simply expires inert.
  void set_paging_window_timer(timer_id_t timer_id);
  timer_id_t get_paging_window_timer() const;

  // Terminates the paging transaction: state back to kIdle, window timer id
  // back to 0. Idempotent, and the only place the transaction ends.
  //
  // `expected_or_zero` is the stale-expiry guard. Pass the expiring timer's id
  // from a timeout handler: if it is not the id of the transaction currently
  // in flight, this function MUTATES NOTHING and returns false, so a timer
  // left over from a superseded transaction cannot terminate or drain the live
  // one. Pass 0 from a response path, which always wins.
  bool clear_paging_state(timer_id_t expected_or_zero);

  // Compare-and-set for the paging-RESPONSE path: kInProgress -> kResponded.
  // Returns false and mutates nothing when no transaction is in flight, which
  // is how a caller tells a real paging response from the ordinary
  // registration or service request of a UE nobody paged.
  //
  // Copies out, under the same lock, what the caller needs afterwards:
  //   * `guti_key_out`  - the 5G-GUTI the transaction was OPENED with. Use it
  //     for the logs rather than the current `guti_`: a REGISTRATION REQUEST
  //     paging response allocates a fresh 5G-GUTI before this runs, so the
  //     current value is no longer the key the page went out under.
  //   * `epoch_out`     - the transaction's sequence number, so the response
  //     log line can be matched to the page log line.
  //   * `timer_out`     - the window timer to remove, WITH NO LOCK HELD.
  // The stored window timer id is zeroed here, so an expiry that was already
  // queued when this ran finds a mismatch in clear_paging_state() and is inert.
  //
  // The state is deliberately left at kResponded, not kIdle: the buffer is
  // being drained by the caller and the transaction is not over until it has
  // been. The caller MUST finish with clear_paging_state(0).
  bool begin_paging_response(
      std::string& guti_key_out, uint32_t& epoch_out, timer_id_t& timer_out);

  // --- 5G-GUTI ------------------------------------------------------------
  // Guarded by m_paging_ because the 5G-GUTI is the join key of a paging
  // transaction: it is written on TASK_AMF_N1 (Registration Accept, uplink-NAS
  // GUTI) and read on TASK_AMF_APP (the paging trigger) and inside
  // ue_context_store under the store lock. A plain std::string member cannot
  // be read on one task while another assigns it.
  //
  // Lock order: the store lock may be held while calling these (1 -> 4); the
  // reverse never happens, because no m_paging_ accessor calls anything.
  void set_guti(const std::string& guti_in);
  std::string get_guti() const;  // empty = unset

 public:
  uint32_t ran_ue_ngap_id;  // 32bits
  uint64_t amf_ue_ngap_id;  // 40 bits
  uint32_t gnb_id;
  std::string supi;
  uint32_t tmsi;

  uint8_t rrc_estb_cause;
  bool is_ue_context_request;
  NrCgi_t cgi;
  Tai_t tai;
  // pdu session id <-> pdu_session_contex
  std::map<std::uint8_t, std::shared_ptr<pdu_session_context>> pdu_sessions;
  mutable std::shared_mutex m_pdu_session;

  std::string amf_3gpp_access_location;
  // std::optional<oai::_3gpp::model::AccessAndMobilitySubscriptionData>
  // am_data;

  std::optional<std::string> nrf_uri;

  // PCF related info
  oai::common::sbi::nf_addr_t pcf_addr;
  std::optional<oai::_3gpp::model::PolicyAssociation> policy_association;
  std::string policy_association_location;

  // UDM SDM subscription (Nudm_SDM_Subscribe, TS 29.503 §5.2.3.3.3)
  std::string udm_sdm_subscription_id;
  // Set when an SDM notification arrives while the UE is CM-IDLE so that
  // a Configuration Update Command can be sent on the next NAS connection.
  bool pending_sdm_update = false;

 private:
  std::shared_ptr<nas_context> nas_ctx{};
  std::shared_ptr<ue_ngap_context> ngap_ctx{};
  mutable std::shared_mutex m_ctx_;

  // --- Paging state --------------------------------------------------------
  // This lives on ue_context, not on ue_ngap_context, because ue_ngap_context
  // is destroyed on every CM-IDLE transition (see amf_n2.cpp:2960 and :3086)
  // while ue_context survives and stays indexed by SUPI and by GUTI.
  //
  // m_ctx_ is deliberately NOT widened to cover these members: it guards
  // nas_ctx/ngap_ctx only, and widening it would silently change what the four
  // accessors above it guarantee.
  // 5G-GUTI (empty = unset). Reached only through set_guti()/get_guti().
  std::string guti_;
  std::string s_setid_;
  std::string s_pointer_;
  std::string s_tmsi_;
  Tai_t last_known_tai_{};
  bool has_last_known_tai_           = false;
  sctp_assoc_id_t last_gnb_assoc_id_ = 0;
  bool has_last_gnb_assoc_id_        = false;
  paging_ctx_t paging_{};
  // Buffered downlink N1/N2 payloads. Kept OUTSIDE paging_ctx_t on purpose:
  // paging_ctx_t is copied out as a snapshot, and a vector of move-only owning
  // records cannot be part of a copyable snapshot.
  std::vector<buffered_n1n2_t> pending_{};
  mutable std::shared_mutex m_paging_;
};

#endif
